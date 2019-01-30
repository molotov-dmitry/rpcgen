#ifndef GENERATORCLIENTSOURCE_H
#define GENERATORCLIENTSOURCE_H

#include "generator.h"

class GeneratorClientSource : public Generator
{
public:
    GeneratorClientSource(const Settings& settings);

    virtual void generate(std::ostream& stream);

};

#endif // GENERATORCLIENTSOURCE_H
