#include "../webserv.hpp"
#include "../class/ServerConfig.hpp"

std::vector<ServerConfig> parse_servers(const std::vector<std::string>& tokens) {
    std::vector<ServerConfig> servers;
    size_t i = 0;

    while (i < tokens.size()) {
        if (tokens[i] == "server") {
            ServerConfig srv;
            std::vector<size_t> loc_indices;
            i += 2;

            while (tokens[i] != "}") {
                if (tokens[i] == "location") {
                    loc_indices.push_back(i);
                    i += 3;
                    while (tokens[i] != "}") {
                        ++i;
                    }
                    ++i;
                    continue;
                }

                std::string dir = tokens[i];
                std::vector<std::string> args;
                collect_args(tokens, i, args);

                if (dir == "listen") {
                    size_t colon = args[0].find(':');
                    if (colon == std::string::npos) {
                        srv.addListen("0.0.0.0", std::atoi(args[0].c_str()));
                    } else {
                        std::string ip = args[0].substr(0, colon);
                        int port = std::atoi(args[0].substr(colon + 1).c_str());
                        srv.addListen(ip, port);
                    }
                } else if (dir == "server_name") {
                    srv.setServerNames(args);
                } else {
                    apply_base_directive(srv, dir, args);
                }
                ++i;
            }

            for (size_t k = 0; k < loc_indices.size(); ++k) {
                srv.addLocation(parse_single_location(srv, tokens, loc_indices[k]));
            }

            servers.push_back(srv);
        }
        ++i;
    }
    return servers;
}

void print_configs(const std::vector<ServerConfig>& servers) {
    for (size_t i = 0; i < servers.size(); ++i) {
        const ServerConfig& s = servers[i];
        std::cout << "========================================\n";
        std::cout << "SERVER #" << i + 1 << "\n";
        for (size_t j = 0; j < s.getListen().size(); ++j)
            std::cout << "  Listen      : " << s.getListen()[j].first << ":" << s.getListen()[j].second << "\n";
        std::cout << "  Root (Base) : " << s.getRoot() << "\n";
        std::cout << "  MaxBody     : " << s.getMaxBody() << "\n";
        std::cout << "  Autoindex   : " << (s.getAutoindex() ? "on" : "off") << "\n";

        const std::vector<LocationConfig>& locs = s.getLocations();
        for (size_t j = 0; j < locs.size(); ++j) {
            const LocationConfig& l = locs[j];
            std::cout << "  --------------------------------------\n";
            std::cout << "  -> LOCATION : " << l.getPath() << "\n";
            std::cout << "     Root (Hérité/Override) : " << l.getRoot() << "\n";
            std::cout << "     MaxBody (Hérité)       : " << l.getMaxBody() << "\n";
            std::cout << "     Autoindex              : " << (l.getAutoindex() ? "on" : "off") << "\n";
        }
        std::cout << "========================================\n\n";
    }
}