#include <httplib.h>
#include "prueba3.h"
#include <external/json/single_include/nlohmann/json.hpp>
#include <fstream>
#include <sstream>
#include <stdexcept>

using json = nlohmann::json;

// representacion json del patricia
json obtenerArbol(const patriciaTree& tree){
    json jtree;

    function<void(const patriciaNode*, json&)> recorrer = [&](const patriciaNode* nodo, json& jnodo){
        jnodo["key"]=nodo->key;
        jnodo["isEndWord"] = nodo->isEndWord;

        for(const auto& hijo : nodo->hijosNodo){
            json jHijo;
            recorrer(hijo.second.get(), jHijo);
            jnodo["children"].push_back(jHijo);
        }
    };
    json raiz;
    recorrer(tree.getRoot(), raiz);
    jtree = raiz;
    return jtree;
}

string read_file(const string& path){
    ifstream file(path);
    if(!file.is_open()) throw runtime_error("No se pudo abrir el archivo "+ path);
    stringstream buff;
    buff << file.rdbuf();
    return buff.str();
}

int main() {
    patriciaTree tree;

    httplib::Server svr;

    svr.Get("/", [](const httplib::Request &, httplib::Response &res) {
        try {
            string html = read_file("../web/web.html");
            res.set_content(html, "text/html");
        } catch (const std::exception& e) {
            res.status = 500;
            res.set_content("Error al cargar la página: " + string(e.what()), "text/plain");
        }
    });

    // insertar palabras
    svr.Post("/insert", [&](const httplib::Request& req, httplib::Response& res) {
        string palabra = req.get_param_value("word");
        tree.insert(palabra);
        res.set_content("Palabra insertada: " + palabra, "text/plain");
    });

    // buscar palabras
    svr.Get("/search", [&](const httplib::Request& req, httplib::Response& res) {
        string palabra = req.get_param_value("word");
        bool encontrada = tree.buscar(palabra);
        json resultado = {{"word", palabra}, {"found", encontrada}};
        res.set_content(resultado.dump(), "application/json");
    });

    // eliminar palabras
    svr.Delete("/delete", [&](const httplib::Request& req, httplib::Response& res) {
        string palabra = req.get_param_value("word");
        bool eliminada = tree.eliminar(palabra);
        json resultado = {{"word", palabra}, {"deleted", eliminada}};
        res.set_content(resultado.dump(), "application/json");
    });

    // rep de aarbol
    svr.Get("/tree", [&](const httplib::Request& req, httplib::Response& res) {
        json arbol = obtenerArbol(tree);
        res.set_content(arbol.dump(), "application/json");
    });

    svr.listen("localhost", 8080);
    return 0;
}