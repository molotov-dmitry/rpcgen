#ifndef GENERATOR_H
#define GENERATOR_H

#include "settings.h"
#include "fstream"

class Generator
{
public:
    Generator(const Settings& settings);
    virtual ~Generator();

    virtual void generate(std::ostream& stream) = 0;

protected:

    const Settings& mSettings;

    std::string nullValue() const;

    static std::string autogenerateNotify();

    static std::string title(std::string str, int level = 0);
};

#endif // GENERATOR_H
