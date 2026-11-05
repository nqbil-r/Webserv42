#ifndef SERVERMANAGER_HPP
# define SERVERMANAGER_HPP

# include "../../webserv.hpp"
# include "ServerConfig.hpp"
# include "Client.hpp"

# define MAX_EVENTS 64

class ServerManager {
    private:
        std::vector<ServerConfig>       _servers;
        int                             _epoll_fd;
        std::map<int, ServerConfig*>    _listen_fds;
        std::map<int, ClientSocket*>    _clients;

        ServerManager(const ServerManager& other);
        ServerManager& operator=(const ServerManager& other);

        int     createListenSocket(const std::string& ip, int port);
        void    setNonBlocking(int fd);
        void    acceptNewClient(int listen_fd);
        void    handleClientRead(int client_fd);
        void    handleClientWrite(int client_fd);
        void    disconnectClient(int client_fd);

    public:
        ServerManager(const std::vector<ServerConfig>& servers);
        ~ServerManager();

        bool    init();
        void    run();
};

#endif