#ifndef LOCATIONCONFIG_HPP
#define LOCATIONCONFIG_HPP

#include <cstddef>
#include <map>
#include <set>
#include <string>
#include <vector>

namespace config {

class ServerConfig;

class LocationConfig
{
public:
  explicit LocationConfig(const ServerConfig& serverConfig);

  // GETTERS
  const std::string& getPath() const;

  const std::string& getRoot() const;
  const std::string& getIndex() const;
  std::size_t getMaxBodySize() const;
  const std::map<int, std::string>& getErrorPages() const;
  const std::string& getErrorPage(int code) const;
  const std::set<std::string>& getAllowedMethods() const;

  bool isAutoIndex() const;

  bool isCgi() const;
  const std::string& getCgiPass() const;
  const std::string& getCgiExtension() const;

  bool isRedirect() const;
  const std::string& getRedirection() const;
  int getRedirectCode() const;

  // SETTERS

  void setPath(const std::string& path);

  void setRoot(const std::string& root);
  void setIndex(const std::string& index);
  void setMaxBodySize(std::size_t size);
  void setErrorPages(const std::vector<int>& codes, const std::string& path);
  void addErrorPage(int code, const std::string& path);
  void addAllowedMethod(const std::string& method);

  void setAutoIndex(bool autoindex);

  void setCgi(bool cgi);
  void setCgiPass(const std::string& cgiPass);
  void setCgiExtension(const std::string& cgiExtension);

  void setRedirect(bool redirect);
  void setRedirection(const std::string& redirectUrl);
  void setRedirectCode(int code);

private:
  std::string _path; // URL prefix (e.g. "/upload, /, /cgi-bin")
  std::string _root; // Filesystem path, allows overriding server root,
                     // e.g. /images can be somewhere else than rest
  bool _autoindex;   // true = enable directory listing
  std::set<std::string> _allowedMethods; // e.g. {"GET", "POST", "DELETE"}
  std::string _index;                    // override default index (optional)
  std::size_t
    _maxBodysize; // optional: override maxuploadsize for this location
  std::map<int, std::string> _errorPages;

  // CGI
  bool _cgiEnabled;
  std::string _cgiPass;      // path to interpreter (e.g. /usr/bin/python3)
  std::string _cgiExtension; // file extension (e.g. ".py")

  // Redirects
  bool _redirect;
  std::string _redirectUrl;
  int _redirectCode; // e.g. 301 - Moved Permanently
};

/*
AUTOINDEX — “Show me what’s inside this folder”
What it is:
When a user requests a directory URL (like /upload/), the server usually does
this:

Check if there’s an index file (like index.html) inside that folder.
If yes → serve that file.
If no → behavior depends on the autoindex setting.

If autoindex is:
✅ true → the server dynamically generates an HTML page listing the files in
that folder.
❌ false → the server returns a 403 Forbidden (or sometimes 404 Not Found).
*/

} // namespace config

#endif
