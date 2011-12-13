#ifndef SYSTEMGENERATOR_H
#define SYSTEMGENERATOR_H

#include <geom/Planet.h>

class SystemGenerator
{
public:
    SystemGenerator();
    virtual ~SystemGenerator();

    QList<Planet> generate();
};

#endif // SYSTEMGENERATOR_H
