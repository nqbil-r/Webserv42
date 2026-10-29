#include "../webserv.hpp"
#include "../class/ServerConfig.hpp"

void collect_args(const std::vector<std::string>& tokens, size_t& i, std::vector<std::string>& args) {
    args.clear();
    ++i;
    while (tokens[i] != ";") {
        args.push_back(tokens[i]);
        ++i;
    }
}

void apply_base_directive(BaseConfig& base, const std::string& dir, const std::vector<std::string>& args) {
    if (dir == "root") {
        base.setRoot(args[0]);
    } else if (dir == "index") {
        base.setIndex(args);
    } else if (dir == "autoindex") {
        base.setAutoindex(args[0] == "on");
    } else if (dir == "client_max_body_size") {
        base.setMaxBody(static_cast<size_t>(std::atol(args[0].c_str())));
    } else if (dir == "error_page") {
        std::string path = args.back();
        for (size_t j = 0; j < args.size() - 1; ++j) {
            base.addErrorPage(std::atoi(args[j].c_str()), path);
        }
    }
}

LocationConfig parse_single_location(const ServerConfig& parent, const std::vector<std::string>& tokens, size_t start_idx) {
    std::string path = tokens[start_idx + 1];
    LocationConfig loc(parent, path);

    size_t i = start_idx + 3;
    while (tokens[i] != "}") {
        std::string dir = tokens[i];
        std::vector<std::string> args;
        collect_args(tokens, i, args);

        if (dir == "allow_methods") {
            loc.setMethods(args);
        } else if (dir == "return") {
            loc.setRedirect(std::atoi(args[0].c_str()), args[1]);
        } else if (dir == "upload_enable") {
            loc.setUploadEnable(args[0] == "on");
        } else if (dir == "upload_store") {
            loc.setUploadStore(args[0]);
        } else if (dir == "cgi_pass") {
            loc.addCgi(args[0], args[1]);
        } else {
            apply_base_directive(loc, dir, args);
        }
        ++i;
    }
    return loc;
}