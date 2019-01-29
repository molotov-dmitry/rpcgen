#ifndef GENERATORSERVERHEADER_H
#define GENERATORSERVERHEADER_H

#include "generator.h"

class GeneratorServerHeader : public Generator
{
public:
    GeneratorServerHeader(const Settings& settings, bool generateClient, bool generateServer);

    virtual void generate(std::ostream& stream);

private:

    bool mGenerateClient;
    bool mGenerateServer;
};

#endif // GENERATORSERVERHEADER_H
