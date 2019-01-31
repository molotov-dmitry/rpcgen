#ifndef GENERATORSTUB_H
#define GENERATORSTUB_H

#include "generator.h"

class GeneratorStub : public Generator
{
public:
    GeneratorStub(const Settings& settings);

    virtual void generate(std::ostream& stream);
};

#endif // GENERATORSTUB_H
