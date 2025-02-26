let highlightedNode = null; // Nodo resaltado

// Obtener el árbol actual
async function fetchTree() {
    try {
        const response = await fetch('/tree');
        const data = await response.json();
        renderTree(data, highlightedNode);
    } catch (e) {
        console.error("Error al obtener el árbol:", e);
    }
}
// Función para imprimir el árbol
async function printTree() {
    const response = await fetch('/print');
    const sortedValues = await response.json();
    const printArea = document.getElementById("print-area");
    printArea.textContent = sortedValues.join(", ");
}

// Función para borrar el árbol
async function clearTree() {
    clearPrintArea();
    const response = await fetch('/clear', { method: 'POST' });
    const treeData = await response.json();
    highlightedNode = null;
    renderTree(treeData, highlightedNode);
}

function clearPrintArea() {
    const printArea = document.getElementById("print-area");
    printArea.textContent = "";
}

// Insertar un número
async function insertNumber() {
    clearPrintArea();
    const num = document.getElementById("number-input").value;
    if (!num) return;

    const response = await fetch(`/insert?number=${num}`, { method: 'POST' });
    const treeData = await response.json();
    highlightedNode = null; // Reiniciar el nodo resaltado
    renderTree(treeData, highlightedNode);
}

// Buscar un número
async function searchNumber() {
    clearPrintArea();
    const num = document.getElementById("number-input").value;
    if (!num) return;

    const response = await fetch(`/search?number=${num}`);
    const result = await response.json();

    if (result.found) {
        alert(`Número ${num} encontrado.`);
        highlightedNode = result.node; // Resaltar el nodo encontrado
    } else {
        alert(`Número ${num} no encontrado.`);
        highlightedNode = null; // No resaltar nada si no se encuentra
    }
    renderTree(result.tree, highlightedNode); // Renderizar el árbol con el nodo resaltado
}

// Eliminar un número
async function deleteNumber() {
    clearPrintArea();
    const num = document.getElementById("number-input").value;
    if (!num) {
        alert(`Ingrese el número`);
        return;
    }

    const response = await fetch(`/delete?number=${num}`, { method: 'DELETE' });
    const treeData = await response.json();

    highlightedNode = null; // Reiniciar el nodo resaltado
    renderTree(treeData, highlightedNode);
}

// Renderizar el árbol
function renderTree(data, highlightedNode = null) {
    const svg = d3.select("#tree");
    svg.selectAll("*").remove();

    if (!data.root || data.root === "empty") {
        svg.append("text")
            .attr("x", 50)
            .attr("y", 50)
            .text("Árbol vacío");
        return;
    }

    const width = svg.node().getBoundingClientRect().width;

    const root = d3.hierarchy(data.root, d => {
        const children = [];
        if (d.left) children.push(d.left);
        if (d.right) children.push(d.right);
        return children;
    });

    const dep = root.height+1;
    const nodeSpacingY = 100;
    const newHeight = dep * nodeSpacingY + 100;
    svg.attr("height", newHeight);

    const treeLayout = d3.tree()
        .size([width - 200, newHeight-100]) // Reducir la altura del árbol
        .separation((a, b) => 0.5); // Reducir la separación entre nodos

    treeLayout(root);

    const g = svg.append("g").attr("transform", "translate(50,50)");

    // Dibujar enlaces
    g.selectAll(".link")
        .data(root.links())
        .enter()
        .append("line")
        .attr("class", "link")
        .attr("x1", d => d.source.x)
        .attr("y1", d => d.source.y)
        .attr("x2", d => d.target.x)
        .attr("y2", d => d.target.y)
        .attr("stroke", "#655151");

    // Dibujar nodos
    const nodes = g.selectAll(".node")
        .data(root.descendants())
        .enter()
        .append("g")
        .attr("class", d => `node ${d.data.color}`)
        .attr("transform", d => `translate(${d.x},${d.y})`);

    // Dibujar círculos
    nodes.append("circle")
        .attr("r", 20)
        .attr("fill", d => {
            if (highlightedNode !== null && d.data.key === highlightedNode) {
                return d.data.color === "red" ? "red" : "black"; // Relleno del borde si está resaltado
            }
            return d.data.color === "red" ? "#D07272DD" : "#a29e9e"; // Relleno rojo claro o gris
        })
        .attr("stroke", d => d.data.color === "red" ? "red" : "black") // Borde rojo o negro
        .attr("stroke-width", 2);

    // Dibujar texto
    nodes.append("text")
        .attr("dy", 5)
        .attr("fill", "white")
        .text(d => d.data.key);

    // Dibujar nodos "null" para hijos faltantes
    root.descendants().forEach(d => {
        const nodeX = d.x; // Posición X del nodo actual
        const nodeY = d.y; // Posición Y del nodo actual

        // Si no tiene hijo izquierdo, dibujar "null" a la izquierda
        if (!d.data.left) {
            g.append("rect")
                .attr("x", nodeX - 25) // Ajustar posición (izquierda)
                .attr("y", nodeY + 20)  // Ajustar posición (abajo)
                .attr("width", 10)
                .attr("height", 10)
                .attr("fill", "gray")
        }

        // Si no tiene hijo derecho, dibujar "null" a la derecha
        if (!d.data.right) {
            g.append("rect")
                .attr("x", nodeX + 25) // Ajustar posición (derecha)
                .attr("y", nodeY + 20)  // Ajustar posición (abajo)
                .attr("width", 10)
                .attr("height", 10)
                .attr("fill", "gray")
        }
    });
}

// Cargar el árbol al inicio
fetchTree();