let highlightedNode = null; // Nodo resaltado

// Obtener el arbol actual
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
    highlightedNode = null;
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
        highlightedNode = result.node;
    } else {
        alert(`Número ${num} no encontrado.`);
        highlightedNode = null;
    }
    renderTree(result.tree, highlightedNode);
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

    highlightedNode = null;
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
        .size([width - 200, newHeight-100])
        .separation((a, b) => 0.5);

    treeLayout(root);

    const g = svg.append("g").attr("transform", "translate(50,50)");

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

    const nodes = g.selectAll(".node")
        .data(root.descendants())
        .enter()
        .append("g")
        .attr("class", d => `node ${d.data.color}`)
        .attr("transform", d => `translate(${d.x},${d.y})`);

    nodes.append("circle")
        .attr("r", 20)
        .attr("fill", d => {
            if (highlightedNode !== null && d.data.key === highlightedNode) {
                return d.data.color === "red" ? "red" : "black";
            }
            return d.data.color === "red" ? "#D07272DD" : "#a29e9e";
        })
        .attr("stroke", d => d.data.color === "red" ? "red" : "black")
        .attr("stroke-width", 2);

    nodes.append("text")
        .attr("dy", 5)
        .attr("fill", "white")
        .text(d => d.data.key);

    root.descendants().forEach(d => {
        const nodeX = d.x;
        const nodeY = d.y;

        if (!d.data.left) {
            g.append("rect")
                .attr("x", nodeX - 25)
                .attr("y", nodeY + 20)
                .attr("width", 10)
                .attr("height", 10)
                .attr("fill", "gray")
        }

        if (!d.data.right) {
            g.append("rect")
                .attr("x", nodeX + 25)
                .attr("y", nodeY + 20)
                .attr("width", 10)
                .attr("height", 10)
                .attr("fill", "gray")
        }
    });
}

fetchTree();