#include <unordered_map>

#define INTERFACE_WEB_HOME
#include "interfaceWeb.h"

namespace InterfaceWeb
{
    using Dictionary = std::unordered_map<const char *, const char *>;

    const Dictionary styles = {
        /*estilo body*/
        {"body", "body {\nbackground-color: #222;\nfont-family: sans-serif;\ncolor: white;}"},
        /*estilo h1*/
        {"h1", "h1 {\ncolor: white;\nfont-weight: bold;\ntext-decoration: underline;\ntext-align: center;\npadding-top: 15px;}"},
        /*estilo .text*/
        {".text", ".text {font-size: 14;\npadding-right: 15px;\n}"},
        /*estilo space-before*/
        {".space-before", ".space-before {\nmargin-bottom: 20px;}"},
        /* Estilos de forms */
        {".form-field", ".form-field {\nmargin: 0px 0px 0px 10px;\ndisplay: inline-block;\nvertical-align: center;}"},
        /* Estilos do menu */
        /* Estilo para a barra de menu */
        {".menu", ".menu {\nlist-style-type: none;\nmargin: 0;\npadding: 0;\nbackground-color: #333;\noverflow: hidden;}"},
        /* Estilo para os itens do menu */
        {".menu li", ".menu li {\nfloat: left;}"},
        /* Estilo para os links do menu */
        {".menu li a", ".menu li a {\ndisplay: block;\ncolor: white;\ntext-align: center;\npadding: 14px 16px;\ntext-decoration: none;}"},
        /* Estilo para os links do menu quando passar o mouse sobre eles */
        {".menu li a:hover", ".menu li a:hover {background-color: #111;}"},
        /* Estilo para o submenu */
        {".submenu", ".submenu {display: none; position: absolute;background-color: #444;z-index: 1;}"},
        /* Estilo para os itens do submenu */
        {".submenu li", ".submenu li {float: none;}"},
        /* Estilo para os links do submenu */
        {".submenu li a", ".submenu li a {padding: 10px 16px;}"},
        /* Mostrar o submenu ao passar o mouse sobre o item de menu "Configurações" */
        {".menu li:hover", ".menu li:hover .submenu {display: block;}"},
        /* Estilo para os campos de status */
        {".status", ".status {margin: 10px;}"},
        {".status label", ".status label {display: inline-block;width: 100px;}"},
        {".status span", ".status span {display: inline-block;background-color: #333;color: #FFF;padding: 5px 10px;border-radius: 5px;}"},
        {".substatus", ".substatus {margin-left: 20px;}"},

        /* Estilo para a div do controlador */
        {".controller-info", ".controller-info {\nmargin: 10px;\nbackground-color: #333;\npadding: 10px;\nborder-radius: 10px; }"},
        {".controller-name", ".controller-name {font-weight: bold;}"},
        {".network-info", ".network-info {\nmargin-top: 5px; margin-left: 20px; line-height: 1.5;}"},
        /* Estilo para a div que envolve os status dos sensores */
        {".container", ".container {\nborder: 2px solid #FFF;\nborder-radius: 10px;\npadding: 10px;\nposition: relative; }"},
        {".container-header-text", ".container-header-text {margin: 0;}"},
        {".subcontainer", ".subcontainer {border: 1px solid gray;border-radius: 10px;padding: 10px;position: relative;}"},
        /* Estilo para o cabeçalho "Sensores" */
        {".container-header", ".container-header {\nposition: absolute;\ntop: -13px;\nleft: 10px;\nbackground-color: #222;\npadding: 0 10px;\nfont-size: 12px;}"},
        /*Estilo para dar um espaçamento vertical*/
        {".space-before",".space-before {margin-bottom: 20px;}"},
        /*Estilo de fundo barra de status das conexões*/
        {".barra",".barra {display: flex;align-items: center; padding: 10px;}"},
        /*Estilo para fazer o ponto verde */
        {".dotgreen",".dotgreen {width: 25px;height: 25px;border-radius: 50%;background-color: green;margin-right: 10px;}"},
        /*Estilo para fazer o ponto vermelho*/
        {".dotred",".dotred {width: 25px;height: 25px;border-radius: 50%;background-color: red;margin-right: 10px;}"},
        
        //{,},
        /* Estilo para o tooltip */
        {".tooltip", ".tooltip {\nposition: relative;\ndisplay: inline-block;}"}};

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