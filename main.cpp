
#include <string>
#include <iostream>
#include <fstream>
#include <string.h>
#include <errno.h>

#include "parser.h"
#include "settings.h"

struct FilesHandles
{
    std::istream* fileInputPtr;
    std::ifstream fileInput;
    std::ofstream fileServerSource;
    std::ofstream fileServerHeader;
    std::ofstream fileClientSource;
    std::ofstream fileClientHeader;
};

bool openOutputFile(const char* path, std::ofstream& file)
{
    file.open(path);

    if (not file.is_open())
    {
        //TODO: error
        return false;
    }

    return true;
}

int main(int argc, char* argv[])
{
    FilesHandles filesHandles;
    filesHandles.fileInputPtr = &std::cin;

    //// Open files ============================================================

    for (int i = 1; i < argc; i += 2)
    {
        if (strcmp(argv[i], "-i") == 0)
        {
            if (i == argc - 1)
            {
                //TODO: error
            }

            filesHandles.fileInput.open(argv[i + 1]);
            if (not filesHandles.fileInput.is_open())
            {
                //TODO: error
                return 1;
            }

            filesHandles.fileInputPtr = &filesHandles.fileInput;
        }
        else if (strcmp(argv[i], "-ss") == 0)
        {
            if (i == argc - 1)
            {
                //TODO: error
            }

            if (not openOutputFile(argv[i + 1], filesHandles.fileServerSource))
            {
                return 1;
            }
        }
        else if (strcmp(argv[i], "-sh") == 0)
        {
            if (i == argc - 1)
            {
                //TODO: error
            }

            if (not openOutputFile(argv[i + 1], filesHandles.fileServerHeader))
            {
                return 1;
            }
        }
        else if (strcmp(argv[i], "-cs") == 0)
        {
            if (i == argc - 1)
            {
                //TODO: error
            }

            if (not openOutputFile(argv[i + 1], filesHandles.fileClientSource))
            {
                return 1;
            }
        }
        else if (strcmp(argv[i], "-ch") == 0)
        {
            if (i == argc - 1)
            {
                //TODO: error
            }

            if (not openOutputFile(argv[i + 1], filesHandles.fileClientHeader))
            {
                return 1;
            }
        }
    }

    //// Create settings =======================================================

    Settings settings;

    //// Create parser =========================================================

    Parser parser(settings);

    //// Parse input file ======================================================

    int lineNumber = 0;

    for(std::string line; getline(*filesHandles.fileInputPtr, line ); )
    {
        if (not parser.parseLine(line))
        {
            std::cerr << "Error in line " << lineNumber << ": " << parser.getLastError() << std::endl;
            return 2;
        }

        ++lineNumber;
    }

    //// Generate files ========================================================

    //// =======================================================================

    return 0;
}
