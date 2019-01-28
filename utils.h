#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <list>
#include <map>
#include <vector>
#include <stdarg.h>

std::string string_format(const char* format, ...);

bool starts_with(const std::string& str, const std::string& start, bool case_sensitive = true);
bool ends_with(const std::string& str, const std::string& end, bool case_sensitive = true);

bool split_config_line(const std::string& line, std::string& key, std::string& val);

bool in_quotes(const std::string& str);
bool remove_quotes(std::string& str);

std::string to_upper(std::string str);

std::list<std::string> split(const std::string& str, char sep, bool removeEmpty = false);
std::vector<std::string> split_vector(const std::string& str, char sep, bool removeEmpty = false);

std::string join(const std::list<std::string>& l, const char *delimiter);

bool between(const std::string& str, const char* from, const char* to, std::string& res);
std::string before(const std::string& str, const char* to);
std::string after(const std::string& str, const char* from);


void trim(std::string& str);
std::string trimmed(const std::string& str);

bool contains(const std::string& str, const char* substr);

std::list<std::string> keys(const std::map<std::string, std::string>& stringMap);

#endif // UTILS_H
