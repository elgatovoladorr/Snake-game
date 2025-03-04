# Snake Game en C 🐍

Este es un juego de Snake programado en C para ejecutarse en la terminal de Windows (cmd).

##  Archivos del Proyecto

- `snake.c` → Código fuente del juego.
- `snakeicon.ico` → Icono personalizado del ejecutable.

##  Cómo Compilar

### **Usando MinGW**
```sh
windres resource.rc -O coff -o resource.res
gcc snake.c resource.res -o snake.exe
