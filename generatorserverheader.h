#ifndef GENERATORSERVERHEADER_H
#define GENERATORSERVERHEADER_H

#include "generator.h"

class GeneratorServerHeader : public Generator
{
public:
    GeneratorServerHeader(const Settings& settings);

    virtual void generate(std::ostream& stream);
};

#endif // GENERATORSERVERHEADER_H
