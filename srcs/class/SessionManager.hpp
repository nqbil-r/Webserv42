#ifndef SESSION_MANAGER_HPP
# define SESSION_MANAGER_HPP

# include "../../webserv.hpp"

struct SessionData {
    std::time_t                         created_at;
    std::time_t                         last_accessed;
    std::map<std::string, std::string>  variables;
};

class SessionManager {
    private:
        std::map<std::string, SessionData> _sessions;
        int                                _session_timeout;

        std::string generateId() {
            std::stringstream ss;
            ss << "sid_" << std::time(NULL) << "_" << std::rand();
            return ss.str();
        }

    public:
        SessionManager(int timeout = 3600) : _session_timeout(timeout) {
            std::srand(std::time(NULL));
        }
        ~SessionManager() {}

        std::string createSession() {
            std::string id = generateId();
            SessionData data;
            data.created_at = std::time(NULL);
            data.last_accessed = data.created_at;
            _sessions[id] = data;
            return id;
        }

        bool isValidSession(const std::string& id) {
            std::map<std::string, SessionData>::iterator it = _sessions.find(id);
            if (it == _sessions.end())
                return false;

            if (std::time(NULL) - it->second.last_accessed > _session_timeout) {
                _sessions.erase(it);
                return false;
            }
            it->second.last_accessed = std::time(NULL); // Rafraîchit la session
            return true;
        }

        void setValue(const std::string& id, const std::string& key, const std::string& val) {
            _sessions[id].variables[key] = val;
        }

        std::string getValue(const std::string& id, const std::string& key) {
            return _sessions[id].variables[key];
        }
};

#endif