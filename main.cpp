#include "RBTree.h"
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

    //web
    svr.Get("/", [](const httplib::Request &, httplib::Response &res) {
        try {
            string html = read_file("../web/web.html");
            res.set_content(html, "text/html");
        } catch (const exception& e) {
            res.status = 500;
            res.set_content("Error al cargar la página: " + string(e.what()), "text/plain");
        }
    });

    // javaa
    svr.Get("/javaa.js", [](const httplib::Request &, httplib::Response &res) {
        try {
            string js = read_file("../web/javaa.js");
            res.set_content(js, "application/javascript");
        } catch (const exception& e) {
            res.status = 500;
            res.set_content("Error al cargar el archivo: " + string(e.what()), "text/plain");
        }
    });
    // imprimir árbol inorden
    svr.Get("/print", [&](const httplib::Request &, httplib::Response &res) {
        vector<int> values;
        function<void(Node*)> inOrder = [&](Node* node) {
            if (!node) return;
            inOrder(node->left);
            values.push_back(node->val);
            inOrder(node->right);
        };
        inOrder(tree.getRoot());
        res.set_content(json(values).dump(), "application/json");
    });

    // borrar árbol
    svr.Post("/clear", [&](const httplib::Request &, httplib::Response &res) {
        tree = RBTree();
        json treeData = tree.serializeTree();
        res.set_content(treeData.dump(), "application/json");
    });

    // Insertar
    svr.Post("/insert", [&](const httplib::Request &req, httplib::Response &res) {
        try {
            int number = stoi(req.get_param_value("number"));
            tree.insert(number);
            json treeData = tree.serializeTree();
            res.set_content(treeData.dump(), "application/json");
        } catch (const exception& e) {
            res.status = 400;
            res.set_content("{\"error\":\"Número inválido\"}", "application/json");
        }
    });

    svr.Get("/search", [&](const httplib::Request &req, httplib::Response &res) {
        try {
            int number = stoi(req.get_param_value("number"));
            Node* foundNode = tree.search(number);
            json response;
            response["tree"] = tree.serializeTree();
            response["found"] = (foundNode != nullptr && foundNode->val == number);
            response["node"] = (foundNode != nullptr) ? foundNode->val : -1;
            res.set_content(response.dump(), "application/json");
        } catch (const exception& e) {
            res.status = 400;
            res.set_content("{\"error\":\"Número inválido\"}", "application/json");
        }
    });

    // Eliminar un número
    svr.Delete("/delete", [&](const httplib::Request &req, httplib::Response &res) {
        try {
            int number = stoi(req.get_param_value("number"));
            tree.remove(number);
            json treeData = tree.serializeTree();
            res.set_content(treeData.dump(), "application/json");
        } catch (const exception& e) {
            res.status = 400;
            res.set_content("{\"error\":\"Número inválido\"}", "application/json");
        }
    });

    // arbol actual
    svr.Get("/tree", [&](const httplib::Request &, httplib::Response &res) {
        json treeData = tree.serializeTree();
        res.set_content(treeData.dump(), "application/json");
    });

    cout << "Iniciando servidor en http://localhost:8082" << endl;
    if (!svr.listen("localhost", 8082)) {
        cerr << "Error: No se pudo iniciar el servidor." << endl;
        return 1;
    }
    cout << "El servidor se detuvo inesperadamente" << endl;
}