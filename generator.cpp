#include "generator.h"

const std::size_t titleWidth = 80;

Generator::Generator(const Settings& settings) : mSettings(settings)
{

}

Generator::~Generator()
{

}

std::string Generator::title(std::string str, int level)
{
    str = "//// " + str;

    str.reserve(titleWidth + 1);

    if (str.size() < titleWidth)
    {
        str.append(" ");
    }

    while (str.size() < titleWidth)
    {
        str.append((level == 0) ? "=" : "-");
    }

    return str;
}
