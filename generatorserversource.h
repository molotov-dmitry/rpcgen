#ifndef GENERATORSERVERSOURCE_H
#define GENERATORSERVERSOURCE_H

#include "generator.h"

class GeneratorServerSource : public Generator
{
public:
    GeneratorServerSource(const Settings& settings);

    virtual void generate(std::ostream& stream);
};

#endif // GENERATORSERVERSOURCE_H
