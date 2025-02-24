# AED-proyectoFinal
## Repositorio Proyecto Final del curso AED : Red-Black Tree

AUTORES:
- Sofia Ku
- Flavia Honores
- Aaron Rojas

El proyecto consiste en implementar un **Patricia Tree** en C++ de manera gráfica. Este estructura de datos permite:
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
   ├── patriciaNode.h
   ├── patriciaNode.cpp
   ├── patriciaTree.h
   ├── patriciaTree.cpp
   ```

2. Corre el archivo `main.cpp`
3. Esperar a que la consola esté lista, normalmente solo debería aparecer la ruta donde tienes el archivo

---

## **Visualizar**
1. Una vez compilado,abre tu navegador web y accede a http://localhost:8080/
---
