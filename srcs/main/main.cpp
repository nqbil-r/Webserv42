#include "../../webserv.hpp"
#include "../class/ServerManager.hpp"
#include "../class/ServerConfig.hpp"

int main(int ac, char **av) {
    if (ac != 2) {
        std::cerr << "Usage: " << av[0] << " [configuration file]" << std::endl;
        return 1;
    }
    const char *conf_path = av[1];
    std::string raw_conf = get_conf(conf_path);
    if (raw_conf.empty()) {
        std::cerr << "Error: Cannot open or empty configuration file: " << conf_path << std::endl;
        return 1;
    }
    std::vector<std::string> tokens = tokenize_conf(raw_conf);
    if (valid_conf(tokens) < 0) {
        return 1;
    }
    std::vector<ServerConfig> servers = parse_servers(tokens);
    print_configs(servers);

    ServerManager manager(servers);
    if (!manager.init())
        return 1;
    manager.run();
    std::cout << "Configuration setted up!" << std::endl;

    

    return 0;
}