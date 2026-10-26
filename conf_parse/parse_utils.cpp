#include "../webserv.hpp"

bool is_number(const std::string& s) {
    if (s.empty())
        return false;
    for (size_t i = 0; i < s.size(); ++i) {
        if (!std::isdigit(s[i]))
            return false;
    }
    return true;
}

bool is_valid_port(const std::string& s) {
    if (!is_number(s))
        return false;
    long port = std::atol(s.c_str());
    return (port >= 1 && port <= 65535);
}

bool is_valid_listen(const std::string& s) {
    size_t colon = s.find(':');
    if (colon == std::string::npos)
        return is_valid_port(s);
    std::string ip = s.substr(0, colon);
    std::string port = s.substr(colon + 1);
    return (!ip.empty() && is_valid_port(port));
}

std::string normalize_listen(const std::string& s) {
    if (s.find(':') == std::string::npos)
        return "0.0.0.0:" + s;
    return s;
}

bool extract_args(const std::vector<std::string>& tokens, size_t& i, std::vector<std::string>& args) {
    args.clear();
    ++i;
    while (i < tokens.size() && tokens[i] != ";" && tokens[i] != "{" && tokens[i] != "}") {
        args.push_back(tokens[i]);
        ++i;
    }
    if (i >= tokens.size() || tokens[i] != ";" || args.empty())
        return false;
    return true;
}