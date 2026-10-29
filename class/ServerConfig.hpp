#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

# include "../webserv.hpp"
# include "Config.hpp"
# include "LocationConfig.hpp"

class ServerConfig : public BaseConfig {
    private:
        std::vector<std::pair<std::string, int> >   _listen;
        std::vector<std::string>                    _server_names;
        std::vector<LocationConfig>                 _locations;

    public:
        ServerConfig() : BaseConfig() {}
        virtual ~ServerConfig() {}

        void addListen(const std::string& ip, int port) {
            _listen.push_back(std::make_pair(ip, port));
        }
        void setServerNames(const std::vector<std::string>& names) { _server_names = names; }
        void addLocation(const LocationConfig& loc) { _locations.push_back(loc); }

        const std::vector<std::pair<std::string, int> >& getListen() const { return _listen; }
        const std::vector<std::string>&                  getServerNames() const { return _server_names; }
        const std::vector<LocationConfig>&               getLocations() const { return _locations; }
};

#endif