#include "../webserv.hpp"

std::string get_conf(const char *file) {
    std::ifstream ifs(file);
    if (!ifs.is_open())
        return ("");
    
    std::stringstream buffer;
    buffer << ifs.rdbuf();
    return buffer.str();
}

std::vector<std::string> tokenize_conf(const std::string& raw_content) {
    std::string clean_str;
    clean_str.reserve(raw_content.size());

    for (size_t i = 0; i < raw_content.size(); i++) {
        if (raw_content[i] == '#') {
            while (i < raw_content.size() && raw_content[i] != '\n') {
                ++i;
            }
        } 
        else if (raw_content[i] == '{' || raw_content[i] == '}' || raw_content[i] == ';') {
            clean_str += ' ';
            clean_str += raw_content[i];
            clean_str += ' ';
        } 
        else {
            clean_str += raw_content[i];
        }
    }
    std::vector<std::string> tokens;
    std::stringstream ss(clean_str);
    std::string word;

    while (ss >> word) {
        tokens.push_back(word);
    }

    return tokens;
}