#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>
#include <map>
#include <list>

struct Args
{
    std::string type;
    std::string defaultValue;
};

enum DataType
{
    DATA_EMPTY_LINE,
    DATA_COMMENT,
    DATA_MACRO,
    DATA_METHOD
};

struct DataArg
{
    std::string type;
    bool        noptr  = false;
    bool        varLen = false;
};

struct Data
{
    DataType    type = DATA_EMPTY_LINE;
    std::string name;

    DataArg     in;
    DataArg     out;

    std::map<std::string, std::string> customParameters;
};

class Settings
{
public:
    Settings();

    std::string rpcName() const;
    void setRpcName(const std::string& rpcName);

    std::string returnType() const;
    void setReturnType(const std::string& returnType);

    std::string funcSend() const;
    void setFuncSend(const std::string& funcSend);

    std::string funcRecv() const;
    void setFuncRecv(const std::string& funcRecv);

    std::string errorNoId() const;
    void setErrorNoId(const std::string& errorNoId);

    std::string errorLenIn() const;
    void setErrorLenIn(const std::string& errorLenIn);

    std::string serverArgs() const;
    void setServerArgs(const std::string& serverArgs);

    std::string clientArgs() const;
    void setClientArgs(const std::string& clientArgs);

    std::string callRpcName() const;
    void setCallRpcName(const std::string& callRpcName);

    bool needEnumInHeader() const;
    void setNeedEnumInHeader(bool needEnumInHeader);

    std::list<std::string> serverParametersKeys() const;
    std::map<std::string, Args> serverParameters() const;
    void addServerParameter(const std::string name, const std::string& type, const std::string& defaultValue);

    std::list<std::string> clientParametersKeys() const;
    std::map<std::string, Args> clientParameters() const;
    void addClientParameter(const std::string name, const std::string& type, const std::string& defaultValue);

    std::list<std::string> includes() const;
    void addInclude(const std::string& include);

    std::list<Data> data() const;
    void addData(const Data& data);

private:

    std::string mRpcName;
    std::string mReturnType;

    std::string mFuncSend;
    std::string mFuncRecv;

    std::string mErrorNoId;
    std::string mErrorLenIn;

    std::string mServerArgs;
    std::string mClientArgs;

    std::string mCallRpcName;

    bool        mNeedEnumInHeader;

    std::list<std::string> mServerParametersKeys;
    std::list<std::string> mClientParametersKeys;

    std::map<std::string, Args> mServerParameters;
    std::map<std::string, Args> mClientParameters;

    std::list<std::string> mIncludes;

    std::list<Data> mData;
};

#endif // SETTINGS_H
