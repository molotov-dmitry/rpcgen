#ifndef GENERATORHEADER_H
#define GENERATORHEADER_H

#include "generator.h"

class GeneratorHeader : public Generator
{
public:
    GeneratorHeader(const Settings& settings, bool generateClient, bool generateServer);

    virtual void generate(std::ostream& stream);

private:

    bool mGenerateClient;
    bool mGenerateServer;
};

#endif // GENERATORHEADER_H
