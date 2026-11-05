#ifndef LOCATIONCONFIG_HPP
# define LOCATIONCONFIG_HPP

# include "../../webserv.hpp"
# include "Config.hpp"

class LocationConfig : public BaseConfig {
    private:
        std::string                         _path;
        std::vector<std::string>            _allowed_methods;
        bool                                _upload_enabled;
        std::string                         _upload_store;
        int                                 _redirect_code;
        std::string                         _redirect_url;
        std::map<std::string, std::string>  _cgi_pass;

    public:
        LocationConfig() : BaseConfig(), _upload_enabled(false), _redirect_code(0) {
            _allowed_methods.push_back("GET");
        }

        LocationConfig(const BaseConfig& parent, const std::string& path) :
            BaseConfig(parent),
            _path(path),
            _upload_enabled(false),
            _redirect_code(0)
        {
            _allowed_methods.push_back("GET");
        }

        virtual ~LocationConfig() {}

        void setMethods(const std::vector<std::string>& m) { _allowed_methods = m; }
        void setUploadEnable(bool en) { _upload_enabled = en; }
        void setUploadStore(const std::string& store) { _upload_store = store; }
        void setRedirect(int code, const std::string& url) { _redirect_code = code; _redirect_url = url; }
        void addCgi(const std::string& ext, const std::string& bin) { _cgi_pass[ext] = bin; }

        const std::string&                          getPath() const { return _path; }
        const std::vector<std::string>&             getMethods() const { return _allowed_methods; }
        bool                                        getUploadEnable() const { return _upload_enabled; }
        const std::string&                          getUploadStore() const { return _upload_store; }
        int                                         getRedirectCode() const { return _redirect_code; }
        const std::string&                          getRedirectUrl() const { return _redirect_url; }
        const std::map<std::string, std::string>&   getCgiPass() const { return _cgi_pass; }
};

#endif