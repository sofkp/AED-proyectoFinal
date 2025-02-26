#include "prueba3.h"

// Leer archivos
string read_file(const string& path) {
    ifstream file(path);
    if (!file.is_open()) throw runtime_error("No se pudo abrir el archivo " + path);
    stringstream buff;
    buff << file.rdbuf();
    return buff.str();
}

int main() {
    httplib::Server svr;
    RBTree tree;

    // Servir la página web
    svr.Get("/", [](const httplib::Request &, httplib::Response &res) {
        try {
            string html = read_file("../web/web.html");
            res.set_content(html, "text/html");
        } catch (const std::exception& e) {
            res.status = 500;
            res.set_content("Error al cargar la página: " + string(e.what()), "text/plain");
        }
    });

    // Servir el archivo JavaScript
    svr.Get("/javaa.js", [](const httplib::Request &, httplib::Response &res) {
        try {
            string js = read_file("../web/javaa.js");
            res.set_content(js, "application/javascript");
        } catch (const std::exception& e) {
            res.status = 500;
            res.set_content("Error al cargar el archivo: " + string(e.what()), "text/plain");
        }
    });
    // Endpoint para imprimir el árbol en orden
    svr.Get("/print", [&](const httplib::Request &, httplib::Response &res) {
        std::vector<int> values;
        std::function<void(Node*)> inOrder = [&](Node* node) {
            if (!node) return;
            inOrder(node->left);
            values.push_back(node->val);
            inOrder(node->right);
        };
        inOrder(tree.getRoot());
        res.set_content(json(values).dump(), "application/json");
    });

    // Endpoint para borrar el árbol
    svr.Post("/clear", [&](const httplib::Request &, httplib::Response &res) {
        tree = RBTree(); // Reiniciar el árbol
        json treeData = tree.serializeTree();
        res.set_content(treeData.dump(), "application/json");
    });

    // Insertar un número
    svr.Post("/insert", [&](const httplib::Request &req, httplib::Response &res) {
        try {
            int number = stoi(req.get_param_value("number"));
            tree.insert(number); // Insertar el número
            json treeData = tree.serializeTree(); // Obtener el árbol actualizado
            res.set_content(treeData.dump(), "application/json");
        } catch (const std::exception& e) {
            res.status = 400; // Bad Request
            res.set_content("{\"error\":\"Número inválido\"}", "application/json");
        }
    });

    // Buscar un número
    svr.Get("/search", [&](const httplib::Request &req, httplib::Response &res) {
        try {
            int number = stoi(req.get_param_value("number"));
            Node* foundNode = tree.search(number); // Buscar el número
            json response;
            response["tree"] = tree.serializeTree(); // Árbol actual
            response["found"] = (foundNode != nullptr && foundNode->val == number); // ¿Se encontró?
            response["node"] = (foundNode != nullptr) ? foundNode->val : -1; // Nodo encontrado
            res.set_content(response.dump(), "application/json");
        } catch (const std::exception& e) {
            res.status = 400; // Bad Request
            res.set_content("{\"error\":\"Número inválido\"}", "application/json");
        }
    });

    // Eliminar un número
    svr.Delete("/delete", [&](const httplib::Request &req, httplib::Response &res) {
        try {
            int number = stoi(req.get_param_value("number"));
            tree.remove(number); // Eliminar el número
            json treeData = tree.serializeTree(); // Obtener el árbol actualizado
            res.set_content(treeData.dump(), "application/json");
        } catch (const std::exception& e) {
            res.status = 400; // Bad Request
            res.set_content("{\"error\":\"Número inválido\"}", "application/json");
        }
    });

    // Obtener el árbol actual
    svr.Get("/tree", [&](const httplib::Request &, httplib::Response &res) {
        json treeData = tree.serializeTree();
        res.set_content(treeData.dump(), "application/json");
    });

    std::cout << "Iniciando servidor en http://localhost:8082" << std::endl;
    if (!svr.listen("localhost", 8082)) {
        std::cerr << "Error: No se pudo iniciar el servidor." << std::endl;
        return 1;
    }
    std::cout << "El servidor se detuvo inesperadamente" << std::endl;
}