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
            if (isalnum(c) || c == '_')
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

                        if (parameters.find(name) != parameters.end())
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

                        if (parameters.find(name) != parameters.end())
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
                        valueLength = i - valueStart;

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
                        valueLength = i - valueStart + 1;

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

                if (parameters.find(name) != parameters.end())
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

                if (parameters.find(name) != parameters.end())
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
    mWarnings.clear();

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
                else if (*it == "SEND_WAIT")
                {
                    methodData.sendWait = true;
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
                std::string type;
                std::string size;

                if ((methodParam.size() == 3) && (methodParam[1] == "NOPTR"))
                {
                    noptr = true;
                }
                else if (methodParam.size() != 2)
                {
                    mLastError = string_format("Too many parameter options");
                    return false;
                }


                {
                    type = methodParam.back();

                    if (contains(type, "="))
                    {
                        std::vector<std::string> typeInfo = split_vector(type, '=', true);

                        if (typeInfo.size() == 2 && typeInfo[0] == "SIZE")
                        {
                            type = "void";
                            size = typeInfo[1];
                        }
                        else
                        {
                            mLastError = string_format("Unknown parameter options");
                            return false;
                        }
                    }

                    if (methodParam[0] == "OUT")
                    {
                        methodData.out.type  = type;
                        methodData.out.noptr = noptr;
                        methodData.out.size  = size;
                    }
                    else if (methodParam[0] == "IN")
                    {
                        methodData.in.type   = type;
                        methodData.in.noptr  = noptr;
                        methodData.in.size   = size;
                    }
                    else if (methodParam[0] == "INOUT")
                    {
                        methodData.inOut.type  = type;
                        methodData.inOut.noptr = noptr;
                        methodData.inOut.size  = size;
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

            //// Check method --------------------------------------------------

            bool methodDeclared = false;

            for (const Data& data : mSettings.data())
            {
                if (data.type != DATA_METHOD)
                {
                    continue;
                }

                if (data.name != methodData.name)
                {
                    continue;
                }

                methodDeclared = true;

                if (data == methodData)
                {
                    mWarnings.push_back(string_format("Method '%s' declared multiple times", methodData.name.c_str()));
                }
                else
                {
                    mLastError = string_format("Method '%s' declaration does not match previous declaration", methodData.name.c_str());
                    return false;
                }

                break;
            }

            //// Add method ----------------------------------------------------

            if (not methodDeclared)
            {
                mSettings.addData(methodData);
            }
        }

        //// Method block ======================================================

        else if (isMethodBlock(line))
        {
            mLastError = string_format("Method blocks is not supported");
            return false;
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
        else if (isSetting(line, "BEFORE_CALL", true, &settingValue))
        {
            mSettings.setBeforeCall(settingValue);
        }
        else if (isSetting(line, "AFTER_CALL", true, &settingValue))
        {
            mSettings.setAfterCall(settingValue);
        }
        else if (isSetting(line, "NEED_ENUM_IN_HDR"))
        {
            mSettings.setNeedEnumInHeader(true);
        }
        else if (isSetting(line, "NEED_TERMINATOR"))
        {
            mSettings.setNeedTerminator(true);
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

        std::string name;
        std::string defaultValue;

        if (contains(value, "="))
        {
            std::map<std::string, std::string> valueMap;

            if (not parseCustomMethodParameters(value, valueMap))
            {
                //Error set by parseCustomMethodParameters();
                return false;
            }

            std::list<std::string> valueKeys = keys(valueMap);

            if (valueKeys.size() != 1)
            {
                mLastError = "Missing name for CLIENT_PARAM/SERVER_PARAM";
                return false;
            }

            name = valueKeys.front();

            defaultValue = valueMap[name];
        }
        else
        {
            name = trimmed(before(value, ";"));
        }

        if (mState == State::CLIENT_PARAM)
        {
            mSettings.addClientParameter(name, type, defaultValue);
        }
        else
        {
            mSettings.addServerParameter(name, type, defaultValue);
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

std::list<std::string> Parser::getWarnings() const
{
    return mWarnings;
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
        trim(val);

        std::string quotedValue;

        if (between(val, "\"", "\"", quotedValue))
        {
            if (not haveValue)
            {
                return false;
            }

            if (value)
            {
                *value = quotedValue;
            }

            return true;

        }

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
