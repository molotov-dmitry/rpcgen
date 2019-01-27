#include "parser.h"

#include <string.h>
#include <vector>

#include "utils.h"

Parser::Parser(Settings& settings) : mSettings(settings), mState(State::GLOBAL)
{

}

bool Parser::parseCustomMethodParameters(std::string line, std::map<std::string, std::string>& parameters)
{
    enum class MethodParametersState
    {
        BEFORE_NAME,
        NAME,
        AFTER_NAME,
        BEFORE_VALUE,
        VALUE,
        AFTER_VALUE,

        PARSE_FINISHED
    };

    enum class ValueQuoteStyle
    {
        NONE,
        SINGLE,
        DOUBLE
    };

    MethodParametersState state = MethodParametersState::BEFORE_NAME;
    ValueQuoteStyle       quote = ValueQuoteStyle::NONE;

    trim(line);

    size_t nameStart = 0;
    size_t nameLength = 0;

    size_t valueStart = 0;
    size_t valueLength = 0;

    for (std::string::size_type i = 0; i < line.size(); ++i)
    {
        char c = line[i];

        switch (state)
        {
        case MethodParametersState::BEFORE_NAME:
        {
            if (isalpha(c))
            {
                nameStart = i;

                state = MethodParametersState::NAME;

                continue;
            }
            else if (isspace(c))
            {
                continue;
            }
            else if (c == ';')
            {
                state = MethodParametersState::PARSE_FINISHED;

                continue;
            }
            else
            {
                mLastError = string_format("Wrong character '%c'", c);
                return false;
            }
        }

        case MethodParametersState::NAME:
        {
            if (isalnum(c))
            {
                continue;
            }
            else if (isspace(c))
            {
                nameLength = i - nameStart;

                state = MethodParametersState::AFTER_NAME;

                continue;
            }
            else if (c == '=')
            {
                nameLength = i - nameStart;

                state = MethodParametersState::BEFORE_VALUE;

                continue;
            }
            else
            {
                mLastError = string_format("Wrong character '%c'", c);
                return false;
            }
        }

        case MethodParametersState::AFTER_NAME:
        {
            if (isspace(c))
            {
                continue;
            }
            else if (c == '=')
            {
                state = MethodParametersState::BEFORE_VALUE;

                continue;
            }
            else
            {
                mLastError = string_format("Wrong character '%c'", c);
                return false;
            }
        }

        case MethodParametersState::BEFORE_VALUE:
        {
            if (isspace(c))
            {
                continue;
            }
            else if (c == '"')
            {
                valueStart = i;

                quote = ValueQuoteStyle::DOUBLE;
                state = MethodParametersState::VALUE;
            }
            else if (c == '\'')
            {
                valueStart = i + 1;

                quote = ValueQuoteStyle::SINGLE;
                state = MethodParametersState::VALUE;
            }
            else
            {
                valueStart = i;

                quote = ValueQuoteStyle::NONE;
                state = MethodParametersState::VALUE;
            }

            continue;
        }

        case MethodParametersState::VALUE:
        {
            if (c == '\\')
            {
                if (i < line.size() - 1)
                {
                    ++i;

                    continue;
                }
                else
                {
                    mLastError = string_format("Wrong character '%c'", c);
                    return false;
                }
            }
            else
            {
                switch (quote)
                {
                case ValueQuoteStyle::NONE:
                {
                    if (isspace(c))
                    {
                        valueLength = i - valueStart;

                        state = MethodParametersState::AFTER_VALUE;

                        continue;
                    }
                    else if (c == ',')
                    {
                        valueLength = i - valueStart;

                        std::string name  = line.substr(nameStart,  nameLength);
                        std::string value = line.substr(valueStart, valueLength);

                        if (parameters.find(name) == parameters.end())
                        {
                            mLastError = string_format("Method custom parameter '%s' already exist", name.c_str());
                            return false;
                        }

                        parameters[name] = value;

                        state = MethodParametersState::BEFORE_NAME;

                        continue;
                    }
                    else if (c == ';')
                    {
                        valueLength = i - valueStart;

                        std::string name  = line.substr(nameStart,  nameLength);
                        std::string value = line.substr(valueStart, valueLength);

                        if (parameters.find(name) == parameters.end())
                        {
                            mLastError = string_format("Method custom parameter '%s' already exist", name.c_str());
                            return false;
                        }

                        parameters[name] = value;

                        state = MethodParametersState::PARSE_FINISHED;

                        continue;
                    }
                    else
                    {
                        continue;
                    }
                }

                case ValueQuoteStyle::SINGLE:
                {
                    if (c == '\'')
                    {
                        valueLength = i - valueStart - 1;

                        state = MethodParametersState::AFTER_VALUE;

                        continue;
                    }
                    else
                    {
                        continue;
                    }
                }

                case ValueQuoteStyle::DOUBLE:
                {
                    if (c == '"')
                    {
                        valueLength = i - valueStart;

                        state = MethodParametersState::AFTER_VALUE;

                        continue;
                    }
                    else
                    {
                        continue;
                    }
                }
                }

                break;
            }
        }

        case MethodParametersState::AFTER_VALUE:
        {
            if (isspace(c))
            {
                continue;
            }
            else if (c == ',')
            {
                std::string name  = line.substr(nameStart,  nameLength);
                std::string value = line.substr(valueStart, valueLength);

                if (parameters.find(name) == parameters.end())
                {
                    mLastError = string_format("Method custom parameter '%s' already exist", name.c_str());
                    return false;
                }

                parameters[name] = value;

                state = MethodParametersState::BEFORE_NAME;

                continue;
            }
            else if (c == ';')
            {
                std::string name  = line.substr(nameStart,  nameLength);
                std::string value = line.substr(valueStart, valueLength);

                if (parameters.find(name) == parameters.end())
                {
                    mLastError = string_format("Method custom parameter '%s' already exist", name.c_str());
                    return false;
                }

                parameters[name] = value;

                state = MethodParametersState::PARSE_FINISHED;

                continue;
            }
            else
            {
                mLastError = string_format("Wrong character '%c'", c);
                return false;
            }
        }

        case MethodParametersState::PARSE_FINISHED:
        {
            if (isspace(c))
            {
                continue;
            }
            else
            {
                //TODO: add comment support
                mLastError = string_format("Wrong character '%c'", c);
                return false;
            }
        }
        }
    }

    if (state != MethodParametersState::PARSE_FINISHED)
    {
        mLastError = string_format("Illegal characters after method custom parameters");
        return false;
    }

    return true;
}

bool Parser::parseLine(const std::string& rawLine)
{
    std::string line = trimmed(rawLine);

    switch (mState)
    {
    case State::GLOBAL:
    {
        std::string settingValue;

        //// Empty line ========================================================

        if (isEmptyLine(line))
        {
            Data macroData;
            macroData.type = DATA_EMPTY_LINE;

            mSettings.addData(macroData);
        }

        //// Comment ===========================================================

        else if (isComment(line))
        {
            Data macroData;
            macroData.type = DATA_COMMENT;
            macroData.name = line;

            mSettings.addData(macroData);
        }

        //// Include ===========================================================

        else if (isInclude(line))
        {
            std::string include;

            if (between(line, "INCLUDE", ";", include))
            {
                trim(include);

                mSettings.addInclude(include);
            }
            else
            {
                mLastError = string_format("Wrong include directive");
                return false;
            }
        }

        //// Macro =============================================================

        else if (isMacro(line))
        {
            Data macroData;
            macroData.type = DATA_MACRO;
            macroData.name = line;

            mSettings.addData(macroData);
        }
        //// Method ============================================================

        else if (isMethod(line))
        {
            Data methodData;
            methodData.type = DATA_METHOD;

            //// Name and common parameters ------------------------------------

            std::list<std::string> methodNameParams = split(before(line, "("), ' ', true);

            if (methodNameParams.size() < 1)
            {
                mLastError = string_format("Missing method name");
                return false;
            }

            methodData.name = methodNameParams.back();
            methodNameParams.pop_back();

            for (std::list<std::string>::iterator it = methodNameParams.begin(); it != methodNameParams.end(); ++it)
            {
                if (*it == "VAR_LEN_OUT")
                {
                    methodData.out.varLen = true;
                }
                else if (*it == "VAR_LEN_IN")
                {
                    methodData.in.varLen = true;
                }
                else
                {
                    mLastError = string_format("Unknown method modifier '%s'", (*it).c_str());
                    return false;
                }
            }

            //// In and out arguments ------------------------------------------

            std::string methodParamsStr;

            if (not between(line, "(", ")", methodParamsStr))
            {
                mLastError = string_format("Missing parameters");
                return false;
            }

            std::list<std::string> methodParams = split(methodParamsStr, ',', true);

            for (std::list<std::string>::iterator it = methodParams.begin(); it != methodParams.end(); ++it)
            {
                std::vector<std::string> methodParam = split_vector(*it, ' ', true);

                bool noptr = false;

                if ((methodParam.size() == 3) && (methodParam[1] == "NOPTR"))
                {
                    noptr = true;
                }
                else if (methodParam.size() != 2)
                {
                    mLastError = string_format("Too many parameter options");
                    return false;
                }
                else
                {
                    if (methodParam[0] == "OUT")
                    {
                        methodData.out.type  = methodParam.back();
                        methodData.out.noptr = noptr;
                    }
                    else if (methodParam[0] == "IN")
                    {
                        methodData.in.type  = methodParam.back();
                        methodData.in.noptr = noptr;
                    }
                    else
                    {
                        mLastError = string_format("Unknown method parameter direction '%s'", methodParam[0].c_str());
                        return false;
                    }
                }
            }

            //// Additional parameters -----------------------------------------

            std::string methodCustomParameters = after(line, ")");

            if (not parseCustomMethodParameters(methodCustomParameters, methodData.customParameters))
            {
                //Error set by parseCustomMethodparameters()
                return false;
            }

            mSettings.addData(methodData);
        }

        //// Method block ======================================================

        else if (isMethodBlock(line))
        {

        }

        //// Settings ==========================================================

        else if (isSetting(line, "NAME", true, &settingValue))
        {
            mSettings.setRpcName(settingValue);
        }
        else if (isSetting(line, "RPC_RET_TYPE", true, &settingValue))
        {
            mSettings.setReturnType(settingValue);
        }
        else if (isSetting(line, "CLNT_RPC_FUNC_SEND", true, &settingValue))
        {
            mSettings.setFuncSend(settingValue);
        }
        else if (isSetting(line, "CLNT_RPC_FUNC_RECV", true, &settingValue))
        {
            mSettings.setFuncRecv(settingValue);
        }
        else if (isSetting(line, "ERROR_NO_SUCH_ID", true, &settingValue))
        {
            mSettings.setErrorNoId(settingValue);
        }
        else if (isSetting(line, "ERROR_BAD_LEN_IN", true, &settingValue))
        {
            mSettings.setErrorLenIn(settingValue);
        }
        else if (isSetting(line, "SERVER_ARGS", true, &settingValue))
        {
            mSettings.setServerArgs(settingValue);
        }
        else if (isSetting(line, "CLIENT_ARGS", true, &settingValue))
        {
            mSettings.setClientArgs(settingValue);
        }
        else if (isSetting(line, "CALL_RPC_NAME", true, &settingValue))
        {
            mSettings.setCallRpcName(settingValue);
        }
        else if (isSetting(line, "NEED_ENUM_IN_HDR"))
        {
            mSettings.setNeedEnumInHeader(true);
        }

        //// Parameters ========================================================

        else if (line == "SERVER_PARAM")
        {
            mParameterStarted  = false;
            mState             = State::SERVER_PARAM;
        }
        else if (line == "CLIENT_PARAM")
        {
            mParameterStarted  = false;
            mState             = State::CLIENT_PARAM;
        }

        //// ===================================================================

        else
        {
            mLastError = string_format("Unknown line '%s'", line.c_str());
            return false;
        }

    }
    break;

    case State::SERVER_PARAM:
    case State::CLIENT_PARAM:
    {
        if (line.empty())
        {
            break;
        }

        if (not mParameterStarted)
        {
            if (line == "{")
            {
                mParameterStarted = true;
                break;
            }
            else
            {
                mLastError = "Missing opening bracket for CLINE_PARAM/SERVER_PARAM";
                return false;
            }
        }

        if (line == "};")
        {
            mState = State::GLOBAL;
            break;
        }

        std::string type  = before(line, " ");
        std::string value = after(line, " ");

        std::map<std::string, std::string> valueMap;

        if (not parseCustomMethodParameters(value, valueMap))
        {
            //Error set by parseCustomMethodParameters();
            return false;
        }

        std::list<std::string> valueKeys = keys(valueMap);

        if (valueKeys.size() != 1)
        {
            mLastError = "Missing name for CLINE_PARAM/SERVER_PARAM";
            return false;
        }

        std::string name = valueKeys.front();

        if (mState == State::CLIENT_PARAM)
        {
            mSettings.addClientParameter(name, type, valueMap[name]);
        }
        else
        {
            mSettings.addServerParameter(name, type, valueMap[name]);
        }

    }
    break;

    }

    return true;
}

std::string Parser::getLastError() const
{
    return mLastError;
}

bool Parser::isEmptyLine(const std::string& line)
{
    return line.size() == 0;
}

bool Parser::isComment(const std::string& line)
{
    return starts_with(line, "//");
}

bool Parser::isInclude(const std::string& line)
{
    return starts_with(line, "INCLUDE ") || starts_with(line, "INCLUDE\t");
}

bool Parser::isMacro(const std::string& line)
{
    return starts_with(line, "#");
}

bool Parser::isMethod(const std::string& line)
{
    return not isComment(line) && contains(line, "(") && not starts_with(line, "$");
}

bool Parser::isMethodBlock(const std::string& line)
{
    return contains(line, "(") && starts_with(line, "$");
}

bool Parser::isSetting(const std::string& line, const char* name, bool haveValue, std::string* value)
{
    std::string val;

    if (starts_with(line, name) && ends_with(line, ";") && between(line, name, ";", val))
    {
        std::list<std::string> values = split(val, ' ', true);

        if (haveValue)
        {
            if (values.size() == 1)
            {
                if (value)
                {
                    *value = values.front();
                }

                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return (values.size() == 0);
        }
    }
    else
    {
        return false;
    }
}
