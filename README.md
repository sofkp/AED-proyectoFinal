# AED-proyectoFinal
## Repositorio Proyecto Final del curso AED : Red-Black Tree

AUTORES:
- Sofia Ku
- Flavia Honores
- Aaron Rojas

El proyecto consiste en implementar un **Red-Black Tree** en C++ de manera gráfica. Este estructura de datos permite:
- **INSERTAR**
- **ELIMINAR**
- **BUSCAR**

Para la parte visual se utilizaron los repositorios [cpp-httplib](https://github.com/yhirose/cpp-httplib) y [json](https://github.com/nlohmann/json.git) como framework para crear un
servidor web y customizarlo. Estas se pueden visualizar en http://localhost:8080/

---

## **Pasos de configuración**

### **1. Configurar librerías externas**
1. Clona el repositorio oficial de **cpp-httplib** y **json** en la carpeta `external`:
   ```bash
   git clone https://github.com/yhirose/cpp-httplib
   ```
   ```bash
   git clone https://github.com/nlohmann/json.git
   ```
2. Volver a cargar el archivo `CMakeLists.txt`

### **2. Compilar el proyecto**
1. Asegúrarse de que la estructura del proyecto sea la siguiente:
   ```
   ProjectFolder/
   ├── external/
       ├── cpp-httplib/
           ├── archivos. de httplib...
       ├── json/
           ├── archivos. de json...
   ├── CMakeLists.txt
   ├── main.cpp
   ├── RBTree.h
   ├── RBTree.cpp
   ```

2. Corre el archivo `main.cpp`
3. Esperar a que la consola esté lista, normalmente solo debería aparecer la ruta donde tienes el archivo

---

## **Visualizar**
1. Una vez compilado,abre tu navegador web y accede a http://localhost:8080/
---


## **Fuentes consultadas para implementación**
- [Introducción a Red-Black Tree - GeeksforGeeks](https://www.geeksforgeeks.org/introduction-to-red-black-tree/)
- [Red-Black Tree - Programiz](https://www.programiz.com/dsa/red-black-tree)
- [Eliminación en Red-Black Tree - GeeksforGeeks](https://www.geeksforgeeks.org/deletion-in-red-black-tree/)
- [redBlackTree](https://github.com/yassiommi/redblacktree/blob/main/RedBlackTree.h)
- [Red-Black-Tree](https://github.com/anandarao/Red-Black-Tree)

---
### **Guía de visualización**
Para comprender mejor la estructura y operaciones del **Red-Black Tree**, puedes utilizar la siguiente herramienta interactiva:  
[Red-Black Tree Visualizer - USFCA](https://www.cs.usfca.edu/~galles/visualization/RedBlack.html)

