#include "ServerManager.hpp"

ServerManager::ServerManager(const std::vector<ServerConfig>& servers)
    : _servers(servers), _epoll_fd(-1) {}

ServerManager::~ServerManager() {
    for (std::map<int, ClientSocket*>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
        delete it->second; // Le destructeur de ClientSocket ferme le FD automatiquement
    }
    _clients.clear();

    for (std::map<int, ServerConfig*>::iterator it = _listen_fds.begin(); it != _listen_fds.end(); ++it) {
        close(it->first);
    }
    _listen_fds.clear();

    if (_epoll_fd >= 0)
        close(_epoll_fd);
}

void ServerManager::setNonBlocking(int fd) {
    fcntl(fd, F_SETFL, O_NONBLOCK);
}

int ServerManager::createListenSocket(const std::string& ip, int port) {
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
        return -1;

    int opt = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    setNonBlocking(fd);

    struct sockaddr_in addr;
    std::memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = ipToNetworkOrder(ip);

    if (bind(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        close(fd);
        return -1;
    }
    if (listen(fd, SOMAXCONN) < 0) {
        close(fd);
        return -1;
    }
    return fd;
}

bool ServerManager::init() {
    _epoll_fd = epoll_create(1);
    if (_epoll_fd < 0) {
        std::cerr << "Error: epoll_create failed." << std::endl;
        return false;
    }

    for (size_t i = 0; i < _servers.size(); ++i) {
        const std::vector<std::pair<std::string, int> >& listens = _servers[i].getListen();
        for (size_t j = 0; j < listens.size(); ++j) {
            std::string ip = listens[j].first;
            int port = listens[j].second;

            int lfd = createListenSocket(ip, port);
            if (lfd < 0) {
                std::cerr << "Error: Failed to bind " << ip << ":" << port << std::endl;
                return false;
            }

            struct epoll_event ev;
            std::memset(&ev, 0, sizeof(ev));
            ev.events = EPOLLIN;
            ev.data.fd = lfd;

            if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, lfd, &ev) < 0) {
                close(lfd);
                return false;
            }

            _listen_fds[lfd] = &_servers[i];
            std::cout << "[ServerManager] Listening on http://" << ip << ":" << port << std::endl;
        }
    }
    return true;
}

void ServerManager::acceptNewClient(int listen_fd) {
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    int client_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &client_len);
    if (client_fd < 0)
        return;

    setNonBlocking(client_fd);

    struct epoll_event ev;
    std::memset(&ev, 0, sizeof(ev));
    ev.events = EPOLLIN;
    ev.data.fd = client_fd;

    if (epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, client_fd, &ev) < 0) {
        close(client_fd);
        return;
    }

    _clients[client_fd] = new ClientSocket(client_fd, _listen_fds[listen_fd]);
    std::cout << "[+] New client connected (fd: " << client_fd << ")" << std::endl;
}

void ServerManager::disconnectClient(int client_fd) {
    epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, client_fd, NULL);
    if (_clients.find(client_fd) != _clients.end()) {
        delete _clients[client_fd];
        _clients.erase(client_fd);
    }
    std::cout << "[-] Client disconnected (fd: " << client_fd << ")" << std::endl;
}

void ServerManager::handleClientRead(int client_fd) {
    char buffer[4096];
    ssize_t bytes_read = recv(client_fd, buffer, sizeof(buffer), 0);

    if (bytes_read <= 0) {
        disconnectClient(client_fd);
        return;
    }

    ClientSocket* client = _clients[client_fd];
    client->appendRead(buffer, bytes_read);

    if (client->hasFullHeaders()) {
        std::cout << "[Request received on fd " << client_fd << "]\n" 
                  << client->getReadBuffer() << std::endl;
        std::string body = "<html><body><h1>Webserv is alive!</h1></body></html>";
        std::stringstream ss;
        ss << "HTTP/1.1 200 OK\r\n"
           << "Content-Type: text/html\r\n"
           << "Content-Length: " << body.size() << "\r\n"
           << "Connection: close\r\n\r\n"
           << body;
        client->getWriteBuffer() = ss.str();

        struct epoll_event ev;
        std::memset(&ev, 0, sizeof(ev));
        ev.events = EPOLLOUT;
        ev.data.fd = client_fd;
        epoll_ctl(_epoll_fd, EPOLL_CTL_MOD, client_fd, &ev);
    }
}

void ServerManager::handleClientWrite(int client_fd) {
    ClientSocket* client = _clients[client_fd];
    std::string& resp = client->getWriteBuffer();

    ssize_t bytes_sent = send(client_fd, resp.c_str(), resp.size(), 0);
    if (bytes_sent <= 0) {
        disconnectClient(client_fd);
        return;
    }

    resp.erase(0, bytes_sent);
    if (resp.empty()) {
        disconnectClient(client_fd);
    }
}

void ServerManager::run() {
    struct epoll_event events[MAX_EVENTS];

    while (true) {
        int nfds = epoll_wait(_epoll_fd, events, MAX_EVENTS, -1);
        if (nfds < 0)
            break;

        for (int i = 0; i < nfds; ++i) {
            int fd = events[i].data.fd;
            if (_listen_fds.find(fd) != _listen_fds.end()) {
                acceptNewClient(fd);
            }
            // Cas 2 : Activité sur un socket client existant
            else if (_clients.find(fd) != _clients.end()) {
                if (events[i].events & (EPOLLERR | EPOLLHUP)) {
                    disconnectClient(fd);
                } else if (events[i].events & EPOLLIN) {
                    handleClientRead(fd);
                } else if (events[i].events & EPOLLOUT) {
                    handleClientWrite(fd);
                }
            }
        }
    }
}