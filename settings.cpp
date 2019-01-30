#include "settings.h"

Settings::Settings() : mCallRpcName("call_rpc"), mNeedEnumInHeader(false), mNeedTerminator(false)
{

}

std::string Settings::rpcName() const
{
    return mRpcName;
}

void Settings::setRpcName(const std::string& rpcName)
{
    mRpcName = rpcName;
}

std::string Settings::returnType() const
{
    return mReturnType;
}

void Settings::setReturnType(const std::string& returnType)
{
    mReturnType = returnType;
}

std::string Settings::funcSend() const
{
    return mFuncSend;
}

void Settings::setFuncSend(const std::string& funcSend)
{
    mFuncSend = funcSend;
}

std::string Settings::funcRecv() const
{
    return mFuncRecv;
}

void Settings::setFuncRecv(const std::string& funcRecv)
{
    mFuncRecv = funcRecv;
}

std::string Settings::errorNoId() const
{
    return mErrorNoId;
}

void Settings::setErrorNoId(const std::string& errorNoId)
{
    mErrorNoId = errorNoId;
}

std::string Settings::errorLenIn() const
{
    return mErrorLenIn;
}

void Settings::setErrorLenIn(const std::string& errorLenIn)
{
    mErrorLenIn = errorLenIn;
}

std::string Settings::serverArgs() const
{
    return mServerArgs;
}

void Settings::setServerArgs(const std::string& serverArgs)
{
    mServerArgs = serverArgs;
}

std::string Settings::clientArgs() const
{
    return mClientArgs;
}

void Settings::setClientArgs(const std::string& clientArgs)
{
    mClientArgs = clientArgs;
}

std::string Settings::callRpcName() const
{
    return mCallRpcName;
}

void Settings::setCallRpcName(const std::string& callRpcName)
{
    mCallRpcName = callRpcName;
}

std::string Settings::beforeCall() const
{
    return mBeforeCall;
}

void Settings::setBeforeCall(const std::string& beforeCall)
{
    mBeforeCall = beforeCall;
}

std::string Settings::afterCall() const
{
    return mAfterCall;
}

void Settings::setAfterCall(const std::string& afterCall)
{
    mAfterCall = afterCall;
}

bool Settings::needEnumInHeader() const
{
    return mNeedEnumInHeader;
}

void Settings::setNeedEnumInHeader(bool needEnumInHeader)
{
    mNeedEnumInHeader = needEnumInHeader;
}

bool Settings::needTerminator() const
{
    return mNeedTerminator;
}

void Settings::setNeedTerminator(bool needTerminator)
{
    mNeedTerminator = needTerminator;
}

std::list<std::string> Settings::serverParametersKeys() const
{
    return mServerParametersKeys;
}

std::map<std::string, Args> Settings::serverParameters() const
{
    return mServerParameters;
}

bool Settings::addServerParameter(const std::string name, const std::string& type, const std::string& defaultValue)
{
    if (mServerParameters.find(name) != mServerParameters.end())
    {
        return false;
    }

    mServerParametersKeys.push_back(name);
    mServerParameters[name] = Args{type, defaultValue};

    return true;
}

std::list<std::string> Settings::clientParametersKeys() const
{
    return mClientParametersKeys;
}

std::map<std::string, Args> Settings::clientParameters() const
{
    return mClientParameters;
}

bool Settings::addClientParameter(const std::string name, const std::string& type, const std::string& defaultValue)
{
    if (mClientParameters.find(name) != mClientParameters.end())
    {
        return false;
    }

    mClientParametersKeys.push_back(name);
    mClientParameters[name] = Args{type, defaultValue};

    return true;
}

std::list<std::string> Settings::includes() const
{
    return mIncludes;
}

void Settings::addInclude(const std::string& include)
{
    mIncludes.push_back(include);
}

std::list<Data> Settings::data() const
{
    return mData;
}

void Settings::addData(const Data& data)
{
    mData.push_back(data);
}
