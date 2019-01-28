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
};

#endif // GENERATOR_H
