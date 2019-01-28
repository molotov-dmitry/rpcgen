#include "utils.h"

#include <memory>
#include <string.h>
#include <algorithm>

std::string string_format(const char* format, ...)
{
    va_list args, args_size;
    va_start(args, format);
    va_copy(args_size, args);

    int size = vsnprintf(nullptr, 0, format, args_size) + 1;
    if (size < 0)
    {
        return std::string();
    }

    va_end(args_size);

    std::unique_ptr<char[]> buf(new char[size]);

    vsnprintf(buf.get(), (size_t)size, format, args);

    va_end(args);

    return std::string(buf.get(), buf.get() + size - 1);
}

bool starts_with(const std::string& str, const std::string& start, bool case_sensitive)
{
    if (case_sensitive)
    {
        return (strncasecmp(str.c_str(), start.c_str(), start.length()) == 0);
    }
    else
    {
        return (strncmp(str.c_str(), start.c_str(), start.length()) == 0);
    }
}

bool ends_with(const std::string& str, const std::string& end, bool case_sensitive)
{
    if (end.length() > str.length())
    {
        return false;
    }

    std::size_t offset = str.length() - end.length();

    if (case_sensitive)
    {
        return (strcasecmp(str.substr(offset).c_str(), end.c_str()) == 0);
    }
    else
    {
        return (strcmp(str.substr(offset).c_str(), end.c_str()) == 0);
    }
}

bool split_config_line(const std::string& line, std::string& key, std::string& val)
{
    std::size_t pos = line.find('=');

    if (pos == std::string::npos)
    {
        return false;
    };

    key = line.substr(0, pos);
    val = line.substr(pos + 1);

    if (in_quotes(val))
    {
        remove_quotes(val);
    }

    return true;
}

bool in_quotes(const std::string& str)
{
    if (str.length() >= 2 && starts_with(str, "\"") && ends_with(str, "\""))
    {
        return true;
    }

    return false;
}

bool remove_quotes(std::string& str)
{
    if (in_quotes(str))
    {
        str = str.substr(1, str.length() - 2);
        return true;
    }

    return false;
}

std::list<std::string> split(const std::string& str, char sep, bool removeEmpty)
{
    std::list<std::string> result;

    std::size_t pos = 0;

    while (pos != std::string::npos)
    {
        std::size_t end = str.substr(pos).find(sep);

        result.push_back(str.substr(pos, end));

        if (end == std::string::npos)
        {
            break;
        }
        else
        {
            pos += end + 1;
        }
    }

    if (removeEmpty)
    {
        result.remove(std::string());
    }

    return result;
}

bool between(const std::string& str, const char* from, const char* to, std::string& res)
{
    std::size_t start_pos = str.find(from);

    if (start_pos != std::string::npos)
    {
        std::size_t len = str.substr(start_pos + strlen(from)).find(to);

        if (len == std::string::npos)
        {
            return false;
        }

        res = str.substr(start_pos + strlen(from), len);

        return true;
    }

    return false;
}

void trim(std::string& str)
{
    while (str.length() > 0 && isspace(str[0]))
    {
        str.erase(0, 1);
    }

    while (str.length() > 0 && isspace(str.at(str.length() - 1)))
    {
        str.erase(str.length() - 1, 1);
    }
}

std::string trimmed(const std::string& str)
{
    std::string result = str;

    trim(result);

    return result;
}

bool contains(const std::string& str, const char* substr)
{
    return str.find(substr, 0) != std::string::npos;
}

std::string before(const std::string& str, const char* to)
{
    std::size_t start_pos = str.find(to);

    if (start_pos != std::string::npos)
    {
        return str.substr(0, start_pos);
    }

    return std::string();
}

std::string after(const std::string& str, const char* from)
{
    std::size_t start_pos = str.find(from);

    if (start_pos != std::string::npos)
    {
        return str.substr(start_pos + strlen(from));
    }

    return std::string();
}

std::vector<std::string> split_vector(const std::string& str, char sep, bool removeEmpty)
{
    std::list<std::string> list = split(str, sep, removeEmpty);
    std::vector<std::string> result
    {
        std::make_move_iterator(std::begin(list)),
        std::make_move_iterator(std::end(list)) };

    return result;
}

std::list<std::string> keys(const std::map<std::string, std::string>& stringMap)
{
    std::list<std::string> result;

    for (std::map<std::string, std::string>::const_iterator it = stringMap.begin(); it != stringMap.end(); ++it)
    {
        result.push_back(it->first);
    }

    return result;
}

std::string to_upper(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c)
    {
        return std::toupper(c);
    });

    return str;
}

std::string join(const std::list<std::string>& l, const char* delimiter)
{
    std::string result;

    for (std::list<std::string>::const_iterator p = l.begin(); p != l.end(); ++p)
    {
        if (p != l.begin())
        {
            result += delimiter;
        }

        result += *p;
    }

    return result;
}

bool contains(const std::map<std::string, std::string>& stringMap, const std::string& str)
{
    return (stringMap.find(str) != stringMap.end());
}
