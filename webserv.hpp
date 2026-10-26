#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <iostream>
# include <string>
# include <map>
# include <ctime>
# include <cstdlib>
# include <fstream>
# include <sstream>
# include <vector>
# include <cctype>
# include <set>
# include <sys/socket.h>
# include <sys/epoll.h>
# include <sys/fcntl.h>
# include <unistd.h>

std::string get_conf(const char *file);
std::vector<std::string> tokenize_conf(const std::string& raw_content);

bool is_number(const std::string& s);
bool is_valid_port(const std::string& s);
bool is_valid_listen(const std::string& s);
bool extract_args(const std::vector<std::string>& tokens, size_t& i, std::vector<std::string>& args);

std::string normalize_listen(const std::string& s);

int valid_conf(const std::vector<std::string>& tokens);

#endif