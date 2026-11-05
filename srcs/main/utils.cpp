#include "../../webserv.hpp"

uint32_t ipToNetworkOrder(const std::string& ip) {
    if (ip == "0.0.0.0" || ip == "*")
        return htonl(INADDR_ANY);
    if (ip == "localhost")
        return htonl(0x7F000001); // 127.0.0.1

    std::stringstream ss(ip);
    std::string segment;
    uint32_t result = 0;

    for (int i = 0; i < 4; ++i) {
        if (!std::getline(ss, segment, '.'))
            return htonl(INADDR_ANY);
        uint32_t byte = static_cast<uint32_t>(std::atoi(segment.c_str()));
        result = (result << 8) | (byte & 0xFF);
    }
    return htonl(result);
}