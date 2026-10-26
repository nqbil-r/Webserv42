#include "../webserv.hpp"

int valid_conf(const std::vector<std::string>& tokens) {
    if (tokens.empty()) {
        std::cerr << "Config Error: Empty configuration file." << std::endl;
        return -1;
    }

    int state = 0;
    bool has_server = false;
    std::set<std::string> global_listens;
    std::set<std::string> current_locations;
    bool current_server_has_listen = false;

    for (size_t i = 0; i < tokens.size(); ++i) {
        const std::string& tk = tokens[i];

        if (state == 0) {
            if (tk == "server" && i + 1 < tokens.size() && tokens[i + 1] == "{") {
                state = 1;
                has_server = true;
                current_locations.clear();
                current_server_has_listen = false;
                ++i;
            } else {
                std::cerr << "Config Error: Expected 'server {' instead of '" << tk << "'" << std::endl;
                return -1;
            }
        }
        else if (state == 1 || state == 2) {
            if (tk == "}") {
                if (state == 1 && !current_server_has_listen) {
                    std::cerr << "Config Error: A 'server' block must have at least one 'listen' directive." << std::endl;
                    return -1;
                }
                state -= 1;
                continue;
            }

            if (tk == "location") {
                if (state != 1) {
                    std::cerr << "Config Error: Nested 'location' blocks are forbidden." << std::endl;
                    return -1;
                }
                if (i + 2 >= tokens.size() || tokens[i + 1] == "{" || tokens[i + 1] == ";" || tokens[i + 2] != "{") {
                    std::cerr << "Config Error: Invalid 'location' syntax." << std::endl;
                    return -1;
                }
                std::string loc_path = tokens[i + 1];
                if (!current_locations.insert(loc_path).second) {
                    std::cerr << "Config Error: Duplicate location '" << loc_path << "' in the same server block." << std::endl;
                    return -1;
                }
                state = 2;
                i += 2;
                continue;
            }

            std::string directive = tk;
            std::vector<std::string> args;
            if (!extract_args(tokens, i, args)) {
                std::cerr << "Config Error: Missing ';' or arguments for directive '" << directive << "'" << std::endl;
                return -1;
            }

            if (directive == "listen") {
                if (state != 1 || args.size() != 1 || !is_valid_listen(args[0])) {
                    std::cerr << "Config Error: Invalid 'listen' directive." << std::endl;
                    return -1;
                }
                std::string norm_listen = normalize_listen(args[0]);
                if (!global_listens.insert(norm_listen).second) {
                    std::cerr << "Config Error: Duplicate listen address/port '" << norm_listen << "'." << std::endl;
                    return -1;
                }
                current_server_has_listen = true;
            }
            else if (directive == "server_name") {
                if (state != 1) {
                    std::cerr << "Config Error: 'server_name' only allowed in server block." << std::endl;
                    return -1;
                }
            }
            else if (directive == "root") {
                if (args.size() != 1) {
                    std::cerr << "Config Error: 'root' takes exactly 1 argument." << std::endl;
                    return -1;
                }
            }
            else if (directive == "index") {
                if (args.empty())
                    return -1;
            }
            else if (directive == "autoindex") {
                if (args.size() != 1 || (args[0] != "on" && args[0] != "off")) {
                    std::cerr << "Config Error: 'autoindex' must be 'on' or 'off'." << std::endl;
                    return -1;
                }
            }
            else if (directive == "client_max_body_size") {
                if (args.size() != 1 || !is_number(args[0])) {
                    std::cerr << "Config Error: 'client_max_body_size' must be a positive number." << std::endl;
                    return -1;
                }
            }
            else if (directive == "error_page") {
                if (args.size() < 2) {
                    std::cerr << "Config Error: 'error_page' requires status code(s) and a path." << std::endl;
                    return -1;
                }
                for (size_t j = 0; j < args.size() - 1; ++j) {
                    if (!is_number(args[j]) || std::atoi(args[j].c_str()) < 300 || std::atoi(args[j].c_str()) > 599) {
                        std::cerr << "Config Error: Invalid HTTP error code '" << args[j] << "'" << std::endl;
                        return -1;
                    }
                }
            }
            else if (directive == "allow_methods") {
                for (size_t j = 0; j < args.size(); ++j) {
                    if (args[j] != "GET" && args[j] != "POST" && args[j] != "DELETE" && args[j] != "HEAD") {
                        std::cerr << "Config Error: Unsupported HTTP method '" << args[j] << "'" << std::endl;
                        return -1;
                    }
                }
            }
            else if (directive == "return") {
                if (args.size() != 2 || !is_number(args[0])) {
                    std::cerr << "Config Error: 'return' requires a status code and a URL." << std::endl;
                    return -1;
                }
            }
            else if (directive == "upload_enable") {
                if (args.size() != 1 || (args[0] != "on" && args[0] != "off")) {
                    std::cerr << "Config Error: 'upload_enable' must be 'on' or 'off'." << std::endl;
                    return -1;
                }
            }
            else if (directive == "upload_store") {
                if (args.size() != 1) {
                    std::cerr << "Config Error: 'upload_store' takes exactly 1 path." << std::endl;
                    return -1;
                }
            }
            else if (directive == "cgi_pass") {
                if (args.size() != 2 || args[0][0] != '.') {
                    std::cerr << "Config Error: 'cgi_pass' requires <.ext> and <binary_path>." << std::endl;
                    return -1;
                }
            }
            else {
                std::cerr << "Config Error: Unknown directive '" << directive << "'" << std::endl;
                return -1;
            }
        }
    }

    if (state != 0 || !has_server) {
        std::cerr << "Config Error: Unclosed braces '{' or no server block found." << std::endl;
        return -1;
    }

    return 0;
}