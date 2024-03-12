#include <unordered_map>

#define INTERFACE_WEB_HOME
#include "interfaceWeb.h"

namespace InterfaceWeb
{
    const Dictionary markupPadrao =
        {
            /*Header padrão*/
            {"<html>", "<html style=\"width:1280px;height:1024px;position:absolute;left:0px;top:0px;\">"},
            {"<head>", "<head>"},
            {"<meta>", "<meta charset=\"UTF-8\">"},
            {"<style>", "<style>"},
            {"</style>", "</style>"},
            {"</head>", "</head>"},
            {"<body>", "<body>"},
            {"</body>", "</body>"},
            {"</html>", "</html>"}};
}