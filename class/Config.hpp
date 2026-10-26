#ifndef CONFIG_HPP
# define CONFIG_HPP

# include "../webserv.hpp"
# include "ServerConfig.hpp"

class BaseConfig {
    protected:
        std::string                 _root;
        std::vector<std::string>    _index;
        size_t                      _client_max_body;
        bool                        _autoindex;
        std::map<int, std::string>  _error_pages;

    public:
        BaseConfig() : 
            _root("/var/www/html"), 
            _client_max_body(1048576), 
            _autoindex(false) 
        {
            _index.push_back("index.html");
        }

        BaseConfig(const BaseConfig& other) :
            _root(other._root),
            _index(other._index),
            _client_max_body(other._client_max_body),
            _autoindex(other._autoindex),
            _error_pages(other._error_pages) {}

        BaseConfig& operator=(const BaseConfig& other) {
            if (this != &other) {
                _root = other._root;
                _index = other._index;
                _client_max_body = other._client_max_body;
                _autoindex = other._autoindex;
                _error_pages = other._error_pages;
            }
            return *this;
        }

        virtual ~BaseConfig() {}

        void setRoot(const std::string& root) { _root = root; }
        void setIndex(const std::vector<std::string>& index) { _index = index; }
        void setMaxBody(size_t max) { _client_max_body = max; }
        void setAutoindex(bool ai) { _autoindex = ai; }
        void addErrorPage(int code, const std::string& path) { _error_pages[code] = path; }

        const std::string&              getRoot() const { return _root; }
        const std::vector<std::string>& getIndex() const { return _index; }
        size_t                          getMaxBody() const { return _client_max_body; }
        bool                            getAutoindex() const { return _autoindex; }
        const std::map<int, std::string>& getErrorPages() const { return _error_pages; }
};

#endif