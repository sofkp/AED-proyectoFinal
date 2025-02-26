#include <httplib.h>
#include "prueba3.h"
#include "external/json/single_include/nlohmann/json.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>

using json = nlohmann::json;

Node* RBTree::getNullNode() const {
    return nullptr;
}

// Representación JSON del árbol rojo-negro
json obtenerArbol(const RBTree& tree) {
    json jtree;

    function<void(const Node*, json&)> recorrer = [&](const Node* nodo, json& jnodo) {
        if (!nodo || nodo == tree.getNullNode()) return; // Verificar si el nodo es nulo o un nodo centinela

        jnodo["key"] = nodo->val;
        jnodo["color"] = (nodo->color == RED) ? "red" : "black"; // Suponiendo que RBTree usa colores RED y BLACK

        json left, right;
        recorrer(nodo->left, left);
        recorrer(nodo->right, right);

        if (!left.is_null()) jnodo["left"] = left;
        if (!right.is_null()) jnodo["right"] = right;
    };

    json raiz;
    recorrer(tree.getRoot(), raiz);
    jtree["root"] = raiz;
    return jtree;
}

// Leer archivos
std::string read_file(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) throw std::runtime_error("No se pudo abrir el archivo " + path);
    std::stringstream buff;
    buff << file.rdbuf();
    return buff.str();
}

int main() {
    RBTree tree; // Instancia del árbol rojo-negro

    httplib::Server svr;

    // Servir la página HTML
    svr.Get("/", [](const httplib::Request&, httplib::Response& res) {
        try {
            std::string html = read_file("../web/web.html");
            res.set_content(html, "text/html");
        } catch (const std::exception& e) {
            res.status = 500;
            res.set_content("Error al cargar la página: " + std::string(e.what()), "text/plain");
        }
    });

    // Insertar número en el árbol
    svr.Post("/insert", [&](const httplib::Request& req, httplib::Response& res) {
        if (req.has_param("number")) {
            int num = std::stoi(req.get_param_value("number"));
            tree.insert(num);
            res.set_content("Número insertado: " + std::to_string(num), "text/plain");
        } else {
            res.status = 400;
            res.set_content("Parámetro 'number' faltante", "text/plain");
        }
    });

    // Buscar número en el árbol
    svr.Get("/search", [&](const httplib::Request& req, httplib::Response& res) {
        if (req.has_param("number")) {
            int num = std::stoi(req.get_param_value("number"));
            bool encontrado = tree.search(num);
            json resultado = {{"number", num}, {"found", encontrado}};
            res.set_content(resultado.dump(), "application/json");
        } else {
            res.status = 400;
            res.set_content("Parámetro 'number' faltante", "text/plain");
        }
    });

    // Eliminar número del árbol
    svr.Delete("/delete", [&](const httplib::Request& req, httplib::Response& res) {
        if (req.has_param("number")) {
            int num = std::stoi(req.get_param_value("number"));
            bool eliminado = tree.remove(num);
            json resultado = {{"number", num}, {"deleted", eliminado}};
            res.set_content(resultado.dump(), "application/json");
        } else {
            res.status = 400;
            res.set_content("Parámetro 'number' faltante", "text/plain");
        }
    });

    // Representación del árbol
    svr.Get("/tree", [&](const httplib::Request&, httplib::Response& res) {
        json arbol = obtenerArbol(tree);
        res.set_content(arbol.dump(), "application/json");
    });

    // Iniciar el servidor en localhost:8080
    svr.listen("localhost", 8080);
    return 0;
}
