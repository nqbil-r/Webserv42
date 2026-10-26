#ifndef LOCATIONCONFIG_HPP
# define LOCATIONCONFIG_HPP

# include "../webserv.hpp"
# include "Config.hpp"

class LocationConfig : public BaseConfig {
    private:
        std::string _path;
        bool        _upload_enabled;

    public:
        LocationConfig(const BaseConfig& parent, std::string path) : BaseConfig(parent), _path(path), _upload_enabled(false) {}
        virtual ~LocationConfig() {}
        
        void printState() {
            std::cout << "Route: " << _path << " | Root hérité: " << _root << " | MaxBody hérité: " << _client_max_body << std::endl;
        }
};

#endif