#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

# include "../webserv.hpp"
# include "Config.hpp"

class ServerConfig : public BaseConfig {
    protected: 
        std::string                 _root;
        size_t                      _client_max_body;
        bool                        _autoindex;
        std::map<int, std::string>  _error_pages;

        int                         _port;

    public:
        ServerConfig() : _root("/var/www"), _client_max_body(1000), _autoindex(false), _port(8080) {}
        ServerConfig(const ServerConfig& other) {
            this->_root = other._root;
            this->_client_max_body = other._client_max_body;
            this->_autoindex = other._autoindex;
            this->_error_pages = other._error_pages;
            this->_port = other._port;
        }
        virtual ~ServerConfig() {}

        void setRoot(std::string r) { _root = r; }
        void setMaxBody(size_t m) { _client_max_body = m; }
};

#endif