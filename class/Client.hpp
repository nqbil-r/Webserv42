#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "../webserv.hpp"

class ClientSocket {
    private:
        int _fd;
        
        ClientSocket(const ClientSocket& other);
        ClientSocket& operator=(const ClientSocket& other);

    public:
        ClientSocket(int fd) : _fd(fd) {}
        ~ClientSocket() { if (_fd >= 0) close(_fd); }
};

#endif