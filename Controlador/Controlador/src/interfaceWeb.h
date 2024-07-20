#ifndef INTERFACEWEB_H
#define INTERFACEWEB_H

#include <unordered_map>

#define PAG_CONFIG_CONTROLADOR  0
#define PAG_CONFIG_HOME         1

namespace InterfaceWeb
{
    using Dictionary = std::unordered_map<const char *, const char *>;

    extern const std::unordered_map<const char *, const char *> styles;
    extern const std::unordered_map<const char *, const char *> markupPadrao;
    //const char *homeHeader[];

    void setupServer();
    void loop();
}

#endif