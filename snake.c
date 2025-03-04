/*
    Juego Snake en C para la terminal de Windows (cmd)
    ---------------------------------------------------
    Controles:
      - Flechas o WASD para mover la serpiente.
      - Presiona 'X' para salir en cualquier momento.
    
    Mecánica:
      - La serpiente crece al comer la comida (*).
      - El juego termina si la serpiente choca con los bordes o consigo misma.
      - Se muestra la puntuación actual.
    
    Nota: Se utiliza system("cls") para limpiar la pantalla en cada frame.
*/

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

#define WIDTH 40          // Ancho del área de juego
#define HEIGHT 20         // Alto del área de juego
#define MAX_SNAKE_SIZE 100

// Variables globales del juego
int gameOver;
int score;
int foodX, foodY;
int snakeX[MAX_SNAKE_SIZE], snakeY[MAX_SNAKE_SIZE];
int snakeLength;
int direction; // 1: Arriba, 2: Abajo, 3: Izquierda, 4: Derecha

// Función que inicializa los parámetros del juego
void Setup() {
    gameOver = 0;
    score = 0;
    snakeLength = 3;
    int startX = WIDTH / 2;
    int startY = HEIGHT / 2;
    // La serpiente se inicia con 3 segmentos en línea horizontal
    for (int i = 0; i < snakeLength; i++) {
        snakeX[i] = startX - i;
        snakeY[i] = startY;
    }
    direction = 4; // Dirección inicial: derecha
    srand(time(NULL));
    // Ubicación aleatoria de la comida (dentro de los límites sin incluir los bordes)
    foodX = rand() % (WIDTH - 2) + 1;
    foodY = rand() % (HEIGHT - 2) + 1;
}

// Función para dibujar el tablero, la serpiente, la comida y la puntuación
void Draw() {
    system("cls"); // Limpia la consola

    // Dibuja el borde superior
    for (int i = 0; i < WIDTH; i++)
        printf("#");
    printf("\n");

    // Dibuja el área de juego
    for (int y = 1; y < HEIGHT - 1; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (x == 0 || x == WIDTH - 1) {
                printf("#"); // Bordes laterales
            } else {
                int printed = 0;
                // Dibuja la serpiente
                for (int k = 0; k < snakeLength; k++) {
                    if (snakeX[k] == x && snakeY[k] == y) {
                        // Cabeza de la serpiente
                        if (k == 0)
                            printf("O");
                        else
                            printf("o");
                        printed = 1;
                        break;
                    }
                }
                // Dibuja la comida o un espacio vacío
                if (!printed) {
                    if (x == foodX && y == foodY)
                        printf("*");
                    else
                        printf(" ");
                }
            }
        }
        printf("\n");
    }

    // Dibuja el borde inferior
    for (int i = 0; i < WIDTH; i++)
        printf("#");
    printf("\n");

    // Muestra la puntuación actual
    printf("Score: %d\n", score);
}

// Función para gestionar la entrada de teclado sin bloquear el juego
void Input() {
    if (_kbhit()) {
        int key = _getch();
        // Si se presionan teclas especiales (flechas), _getch() devuelve 0 o 224 primero
        if (key == 0 || key == 224) {
            key = _getch();
            switch (key) {
                case 72: // Flecha arriba
                    if (direction != 2)
                        direction = 1;
                    break;
                case 80: // Flecha abajo
                    if (direction != 1)
                        direction = 2;
                    break;
                case 75: // Flecha izquierda
                    if (direction != 4)
                        direction = 3;
                    break;
                case 77: // Flecha derecha
                    if (direction != 3)
                        direction = 4;
                    break;
            }
        } else {
            // Controles con WASD
            switch (key) {
                case 'w':
                case 'W':
                    if (direction != 2)
                        direction = 1;
                    break;
                case 's':
                case 'S':
                    if (direction != 1)
                        direction = 2;
                    break;
                case 'a':
                case 'A':
                    if (direction != 4)
                        direction = 3;
                    break;
                case 'd':
                case 'D':
                    if (direction != 3)
                        direction = 4;
                    break;
                case 'x':
                case 'X': // Permite salir presionando 'X'
                    gameOver = 1;
                    break;
            }
        }
    }
}

// Función para actualizar la lógica del juego: movimiento, colisiones y comida
void Logic() {
    int prevX = snakeX[0];
    int prevY = snakeY[0];
    int prev2X, prev2Y;

    // Actualiza la posición de la cabeza según la dirección
    switch (direction) {
        case 1: // Arriba
            snakeY[0]--;
            break;
        case 2: // Abajo
            snakeY[0]++;
            break;
        case 3: // Izquierda
            snakeX[0]--;
            break;
        case 4: // Derecha
            snakeX[0]++;
            break;
        default:
            break;
    }

    // Actualiza el cuerpo de la serpiente: cada segmento sigue al anterior
    for (int i = 1; i < snakeLength; i++) {
        prev2X = snakeX[i];
        prev2Y = snakeY[i];
        snakeX[i] = prevX;
        snakeY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    // Verifica colisión con los bordes
    if (snakeX[0] <= 0 || snakeX[0] >= WIDTH - 1 || snakeY[0] <= 0 || snakeY[0] >= HEIGHT - 1)
        gameOver = 1;

    // Verifica colisión consigo misma
    for (int i = 1; i < snakeLength; i++) {
        if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i]) {
            gameOver = 1;
            break;
        }
    }

    // Si la serpiente come la comida
    if (snakeX[0] == foodX && snakeY[0] == foodY) {
        score += 10;
        if (snakeLength < MAX_SNAKE_SIZE) {
            snakeLength++;
            // Se añade un nuevo segmento duplicando el último
            snakeX[snakeLength - 1] = snakeX[snakeLength - 2];
            snakeY[snakeLength - 1] = snakeY[snakeLength - 2];
        }
        // Coloca nueva comida en posición aleatoria (evitando la serpiente)
        int valid = 0;
        while (!valid) {
            valid = 1;
            foodX = rand() % (WIDTH - 2) + 1;
            foodY = rand() % (HEIGHT - 2) + 1;
            for (int i = 0; i < snakeLength; i++) {
                if (snakeX[i] == foodX && snakeY[i] == foodY) {
                    valid = 0;
                    break;
                }
            }
        }
    }
}

// Función que ejecuta el juego
void Game() {
    Setup();
    while (!gameOver) {
        Draw();
        Input();
        Logic();
        Sleep(60); // Retardo de 100ms para controlar la velocidad
    }
    // Mensaje de Game Over y espera para volver al menú
    system("cls");
    printf("Juego terminado\n");
    printf("Puntuacion final: %d\n", score);
    printf("Presiona cualquier tecla para regresar al menu...\n");
    _getch();
}

// Menú inicial del juego
void Menu() {
    int option;
    do {
        system("cls");
        printf("=== Juego Snake ===\n\n");
        printf("1. Iniciar partida\n");
        printf("2. Salir\n");
        printf("Elige una opcion: ");
        scanf("%d", &option);
        switch (option) {
            case 1:
                Game();
                break;
            case 2:
                printf("Saliendo del juego...\n");
                break;
            default:
                printf("Opcion no valida. Intenta de nuevo.\n");
                Sleep(1000);
        }
    } while (option != 2);
}

int main() {
    Menu();
    return 0;
}
