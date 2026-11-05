#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "../../webserv.hpp"
# include "ServerConfig.hpp"

class ClientSocket {
    private:
        int                 _fd;
        const ServerConfig* _server_conf;
        std::string         _read_buffer;
        std::string         _write_buffer;

        ClientSocket(const ClientSocket& other);
        ClientSocket& operator=(const ClientSocket& other);

    public:
        ClientSocket(int fd, const ServerConfig* conf) : _fd(fd), _server_conf(conf) {}
        ~ClientSocket() { if (_fd >= 0) close(_fd); }

        int                 getFd() const { return _fd; }
        const ServerConfig* getServerConf() const { return _server_conf; }
        std::string&        getReadBuffer() { return _read_buffer; }
        std::string&        getWriteBuffer() { return _write_buffer; }

        void appendRead(const char* buf, size_t len) { _read_buffer.append(buf, len); }
        bool hasFullHeaders() const { return _read_buffer.find("\r\n\r\n") != std::string::npos; }
};

#endif