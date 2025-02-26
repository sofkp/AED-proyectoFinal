#include <httplib.h>
#include "prueba3.h"
#include <external/json/single_include/nlohmann/json.hpp>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <functional>
using namespace std;

using json = nlohmann::json;

// Representación JSON del árbol rojo-negro
json obtenerArbol(const RBTree& tree) {
    json jtree;

    function<void(const Node*, json&)> recorrer = [&](const Node* nodo, json& jnodo) {
        if (!nodo) return;

        jnodo["key"] = nodo->val;
        jnodo["color"] = (nodo->color == RED) ? "red" : "black";

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
json obtenerPasosBusqueda(const RBTree& tree, int n) {
    json pasos;
    Node* temp = tree.getRoot();
    while (temp != NULL) {
        json paso;
        paso["current"] = temp->val;
        paso["color"] = (temp->color == RED) ? "red" : "black";
        pasos.push_back(paso);

        if (n < temp->val) temp = temp->left;
        else if (n == temp->val) break;
        else temp = temp->right;
    }
    return pasos;
}

// Leer archivos
string read_file(const string& path) {
    ifstream file(path);
    if (!file.is_open()) throw runtime_error("No se pudo abrir el archivo " + path);
    stringstream buff;
    buff << file.rdbuf();
    return buff.str();
}

int main() {
    RBTree tree;

    httplib::Server svr;

    svr.Get("/", [](const httplib::Request&, httplib::Response& res) {
        try {
            string html = read_file("../web/web.html");
            res.set_content(html, "text/html");
        } catch (const exception& e) {
            res.status = 500;
            res.set_content("Error al cargar la página: " + string(e.what()), "text/plain");
        }
    });

    // Insertar número en el árbol
    svr.Post("/insert", [&](const httplib::Request& req, httplib::Response& res) {
        if (req.has_param("number")) {
            int num = stoi(req.get_param_value("number"));
            tree.insert(num);
            res.set_content("Número insertado: " + to_string(num), "text/plain");
        } else {
            res.status = 400;
            res.set_content("Parámetro 'number' faltante", "text/plain");
        }
    });

    // Buscar número en el árbol
    svr.Get("/search", [&](const httplib::Request& req, httplib::Response& res) {
        if (req.has_param("number")) {
            int num = stoi(req.get_param_value("number"));
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
            Node* encontrado = tree.search(num);
            bool eliminado = (encontrado != nullptr);
            if (eliminado) tree.remove(num);
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
    svr.listen("localhost", 8082);
    return 0;
}