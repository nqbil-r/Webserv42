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

class BaseConfig;
class LocationConfig;
class ServerConfig;

std::string get_conf(const char *file);
std::vector<std::string> tokenize_conf(const std::string& raw_content);

bool is_number(const std::string& s);
bool is_valid_port(const std::string& s);
bool is_valid_listen(const std::string& s);
bool extract_args(const std::vector<std::string>& tokens, size_t& i, std::vector<std::string>& args);

void                        collect_args(const std::vector<std::string>& tokens, size_t& i, std::vector<std::string>& args);
void                        apply_base_directive(BaseConfig& base, const std::string& dir, const std::vector<std::string>& args);
void                        print_configs(const std::vector<ServerConfig>& servers);
LocationConfig              parse_single_location(const ServerConfig& parent, const std::vector<std::string>& tokens, size_t start_idx);

std::vector<ServerConfig>   parse_servers(const std::vector<std::string>& tokens);

std::string normalize_listen(const std::string& s);

int valid_conf(const std::vector<std::string>& tokens);

#endif