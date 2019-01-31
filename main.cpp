
#include <string>
#include <iostream>
#include <fstream>
#include <string.h>
#include <errno.h>

#include "utils.h"

#include "parser.h"
#include "settings.h"

#include "generatorheader.h"
#include "generatorserversource.h"
#include "generatorclientsource.h"
#include "generatorstub.h"

struct FilesHandles
{
    std::istream* fileInputPtr;
    std::ifstream fileInput;

    std::ofstream fileCommonHeader;
    std::ofstream fileServerSource;
    std::ofstream fileServerHeader;
    std::ofstream fileClientSource;
    std::ofstream fileClientHeader;

    std::ofstream fileStub;
};

bool openOutputFile(const char* path, std::ofstream& file)
{
    file.open(path);

    if (not file.is_open())
    {
        return false;
    }

    return true;
}

struct Stream
{
    const char*    flag;
    std::ofstream& stream;
    std::string    fileName;
    const char*    description;
};

void usage(Stream* streams, int count)
{
    std::cout << "RPC call methods generator" << std::endl;
    std::cout << std::endl;

    std::cout << "Usage " << "rpcgen [-options]" << std::endl;
    std::cout << std::endl;

    std::cout << "Options:" << std::endl;

    std::cout << "  " << "-i" << " file    " << "Path to " << "RPC file" << std::endl;

    for (int i = 0; i < count; ++i)
    {
        std::cout << "  " << streams[i].flag << " file    " << "Path to " << streams[i].description << std::endl;
    }


}

int main(int argc, char* argv[])
{
    FilesHandles filesHandles;
    filesHandles.fileInputPtr = &std::cin;

    enum
    {
        STREAM_HEADER_CLIENTSERVER,
        STREAM_HEADER_CLIENT,
        STREAM_HEADER_SERVER,

        STREAM_SOURCE_CLIENT,
        STREAM_SOURCE_SERVER,

        STREAM_STUB,

        STREAM_COUNT
    };

    Stream streams[STREAM_COUNT] =
    {
        {"-h", filesHandles.fileCommonHeader, std::string(), "client/server header"},
        {"-l", filesHandles.fileClientHeader, std::string(), "client header"},
        {"-m", filesHandles.fileServerHeader, std::string(), "server header"},
        {"-c", filesHandles.fileClientSource, std::string(), "client source"},
        {"-t", filesHandles.fileServerSource, std::string(), "server source"},
        {"-s", filesHandles.fileStub,         std::string(), "stub"},
    };

    const int streamCount = STREAM_COUNT;

    //// Show usage if no parameters ===========================================

    if (argc == 1)
    {
        usage(streams, streamCount);
        return 1;
    }

    //// Create settings =======================================================

    Settings settings;

    //// Open files ============================================================

    for (int i = 1; i < argc; i += 2)
    {
        if (strcmp(argv[i], "-i") == 0)
        {
            if (i == argc - 1)
            {
                std::cerr << "Missing value for argument '" << "-i" << "'." << std::endl;
                return 1;
            }

            filesHandles.fileInput.open(argv[i + 1]);
            if (not filesHandles.fileInput.is_open())
            {
                std::cerr << "Failed to open RPC file for reading." << std::endl;
                return 1;
            }

            filesHandles.fileInputPtr = &filesHandles.fileInput;
        }
        else
        {
            bool found = false;

            for (int j = 0; j < streamCount; ++j)
            {
                if (strcmp(argv[i], streams[j].flag) == 0)
                {
                    if (i == argc - 1)
                    {
                        std::cerr << "Missing value for argument '" << streams[j].flag << "'." << std::endl;
                        return 1;
                    }

                    if (strcmp(argv[i + 1], "/dev/null") != 0)
                    {
                        if (not openOutputFile(argv[i + 1], streams[j].stream))
                        {
                            std::cerr << "Failed to open " << streams[j].description << " for writing." << std::endl;
                            return 1;
                        }

                        streams[j].fileName = basename(argv[i + 1]);

                    }

                    found = true;

                    break;
                }
            }

            if (not found)
            {
                std::cerr << "Unknown option '" << argv[i] << "'." << std::endl;
            }


        }
    }

    //// Set file names ========================================================

    settings.setClientServerHeader(streams[STREAM_HEADER_CLIENTSERVER].fileName);
    settings.setClientHeader(streams[STREAM_HEADER_CLIENT].fileName);
    settings.setServerHeader(streams[STREAM_HEADER_SERVER].fileName);

    //// Create parser =========================================================

    Parser parser(settings);

    //// Parse input file ======================================================

    int lineNumber = 1;

    for (std::string line; getline(*filesHandles.fileInputPtr, line);)
    {
        if (not parser.parseLine(line))
        {
            std::cerr << "Error in line " << lineNumber << ": " << parser.getLastError() << " (" << trimmed(line) << ")" << std::endl;
            return 2;
        }

        for (std::string warning : parser.getWarnings())
        {
            std::cerr << "Warning: " << warning << std::endl;
        }

        ++lineNumber;
    }

    //// Generate files ========================================================

    //// commont header --------------------------------------------------------

    GeneratorHeader gh(settings, true, true);

    gh.generate(filesHandles.fileCommonHeader);

    //// server header ---------------------------------------------------------

    GeneratorHeader gsh(settings, false, true);

    gsh.generate(filesHandles.fileServerHeader);

    //// client header ---------------------------------------------------------

    GeneratorHeader gch(settings, true, false);

    gch.generate(filesHandles.fileClientHeader);

    //// server source ---------------------------------------------------------

    GeneratorServerSource gss(settings);

    gss.generate(filesHandles.fileServerSource);

    //// client source ---------------------------------------------------------

    GeneratorClientSource gcs(settings);

    gcs.generate(filesHandles.fileClientSource);

    //// client source ---------------------------------------------------------

    GeneratorStub gs(settings);

    gs.generate(filesHandles.fileStub);

    //// =======================================================================

    return 0;
}
