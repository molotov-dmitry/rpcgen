#include "settings.h"

Settings::Settings() : mNeedEnumInHeader(false)
{

}

std::string Settings::rpcName() const
{
    return mRpcName;
}

void Settings::setRpcName(const std::string &rpcName)
{
    mRpcName = rpcName;
}

std::string Settings::returnType() const
{
    return mReturnType;
}

void Settings::setReturnType(const std::string &returnType)
{
    mReturnType = returnType;
}

std::string Settings::funcSend() const
{
    return mFuncSend;
}

void Settings::setFuncSend(const std::string &funcSend)
{
    mFuncSend = funcSend;
}

std::string Settings::funcRecv() const
{
    return mFuncRecv;
}

void Settings::setFuncRecv(const std::string &funcRecv)
{
    mFuncRecv = funcRecv;
}

std::string Settings::errorNoId() const
{
    return mErrorNoId;
}

void Settings::setErrorNoId(const std::string &errorNoId)
{
    mErrorNoId = errorNoId;
}

std::string Settings::errorLenIn() const
{
    return mErrorLenIn;
}

void Settings::setErrorLenIn(const std::string &errorLenIn)
{
    mErrorLenIn = errorLenIn;
}

std::string Settings::serverArgs() const
{
    return mServerArgs;
}

void Settings::setServerArgs(const std::string &serverArgs)
{
    mServerArgs = serverArgs;
}

std::string Settings::clientArgs() const
{
    return mClientArgs;
}

void Settings::setClientArgs(const std::string &clientArgs)
{
    mClientArgs = clientArgs;
}

std::string Settings::callRpcName() const
{
    return mCallRpcName;
}

void Settings::setCallRpcName(const std::string &callRpcName)
{
    mCallRpcName = callRpcName;
}

bool Settings::needEnumInHeader() const
{
    return mNeedEnumInHeader;
}

void Settings::setNeedEnumInHeader(bool needEnumInHeader)
{
    mNeedEnumInHeader = needEnumInHeader;
}

std::map<std::string, Args> Settings::serverParameters() const
{
    return mServerParameters;
}

void Settings::addServerParameter(const std::string name, const std::string &type, const std::string &defaultValue)
{
    //TODO: check if this name already exist

    mServerParameters[name] = Args{type, defaultValue};
}

std::map<std::string, Args> Settings::clientParameters() const
{
    return mClientParameters;
}

void Settings::addClientParameter(const std::string name, const std::string &type, const std::string &defaultValue)
{
    //TODO: check if this name already exist

    mClientParameters[name] = Args{type, defaultValue};
}

std::list<std::string> Settings::includes() const
{
    return mIncludes;
}

void Settings::addInclude(const std::string &include)
{
    mIncludes.push_back(include);
}

std::list<Data> Settings::data() const
{
    return mData;
}

void Settings::addData(const Data &data)
{
    mData.push_back(data);
}
