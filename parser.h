#ifndef PARSER_H
#define PARSER_H

#include <string>

#include "settings.h"

class Parser
{
public:
    Parser(Settings& settings);

    bool parseCustomMethodParameters(std::string line, std::map<std::string, std::string>& parameters);

    bool parseLine(const std::string& rawLine);

    std::string getLastError() const;

    std::list<std::string> getWarnings() const;

private:

    enum class State
    {
        GLOBAL,
        CLIENT_PARAM,
        SERVER_PARAM
    };

    Settings& mSettings;

    std::string mLastError;
    std::list<std::string> mWarnings;

    State mState;

    bool mParameterStarted;

    static bool isEmptyLine(const std::string& line);
    static bool isComment(const std::string& line);
    static bool isInclude(const std::string& line);
    static bool isMacro(const std::string& line);
    static bool isMethod(const std::string& line);
    static bool isMethodBlock(const std::string& line);

    static bool isSetting(const std::string& line, const char* name, bool needvalue = false, std::string* value = nullptr);
};


#endif // PARSER_H
