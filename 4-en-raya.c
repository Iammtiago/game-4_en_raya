#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <windows.h>
#include <conio.h>
#include <math.h>

enum colores
{
    AZUL = 1,
    VERDE = 2,
    CIAN = 3,
    ROJO = 4,
    AMARILLO = 14,
    BLANCO = 15,
    DEFAULT = 11
};

typedef struct
{
    char name[20];
    char ficha;
    int color;
    int point;
    int mode;
} Player;

int COLUMNAS = 7;
int FILAS = 6;
Player players[2];

int stateCPU = 0;
// int notThisNumber = -1;
int notThisNumber[3] = {-1, -1, -1};

bool partidaTerminada = false;
int opcionTablero, opcionModoJuego, opcionModoCPU;

void changePlayer(int *p)
{
    *p = (*p == 0) ? 1 : 0;
}

int seleccionarDificultad(char cpuName[20], int opcion)
{
    // int opcion;

    if (opcion <= 0)
    {
        printf("Selecciona el modo de juego para la %s:\n", cpuName);
        Sleep(79);
        printf("1. Facil \n");
        Sleep(79);
        printf("2. medio \n");
        Sleep(79);
        printf("3. Dificil \n");
        Sleep(79);
        printf("\nOpcion: ");
        scanf("%d", &opcion);
        printf("\n\n");
        fflush(stdin);
    }

    switch (opcion)
    {

    case 1:
        return 1;
        break;
    case 2:
        return 2;
        break;
    case 3:
        return 3;
        break;
    default:
        printf("\n\t Numero Incorrecto! Dificultad Medio\n\n");
        return 2;
        break;
    }
}

int seleccionarColor(char name[50], int cpu)
{

    int opcion;

    if (cpu == 1)
    {
        printf("Selecciona un color para el %s: \n", name);
    }
    else
    {
        printf("\n%s, Selecciona un color para tu ficha:\n", name);
    }

    Sleep(79);
    printf("1. Azul \n");
    Sleep(79);
    printf("2. Verde \n");
    Sleep(79);
    printf("3. Cyan\n");
    Sleep(79);
    printf("4. Rojo\n");
    Sleep(79);
    printf("5. Amarillo\n");
    Sleep(79);
    printf("6. Blanco\n");
    Sleep(79);
    printf("\nOpcion: ");
    scanf("%d", &opcion);
    printf("\n\n");
    fflush(stdin);

    switch (opcion)
    {

    case 1:
        return AZUL;
        break;
    case 2:
        return VERDE;
        break;
    case 3:
        return CIAN;
        break;
    case 4:
        return ROJO;
        break;
    case 5:
        return AMARILLO;
        break;
    case 6:
        return BLANCO;
        break;
    default:
        printf("\n\t Numero Incorrecto! color colocado por defecto (azul claro)\n\n");
        return DEFAULT;
        break;
    }
}

void declaraColumnasFilas(void)
{

    printf("\n\tEscoja un tablero para jugar:\n");
    Sleep(79);
    printf("\n 1. 6 filas 7 columnas (tablero tradicional)");
    Sleep(79);
    printf("\n 2. 7 filas 8 columnas");
    Sleep(79);
    printf("\n 3. 9 filas 9 columnas");
    Sleep(79);
    printf("\n\nOpcion: ");
    scanf("%d", &opcionTablero);
    fflush(stdin);

    system("cls");

    fflush(stdin);
    switch (opcionTablero)
    {
    case 1:
        FILAS = 6;
        COLUMNAS = 7;
        break;
    case 2:
        FILAS = 7;
        COLUMNAS = 8;
        break;
    case 3:
        FILAS = 9;
        COLUMNAS = 9;
        break;

    default:
        printf("\n\t Numero Incorrecto! se escojio tablero tradicional por defecto \n\n");
        FILAS = 6;
        COLUMNAS = 7;
        break;
    }
}

void inicializarTablero(char tablero[FILAS][COLUMNAS])
{
    int i, j;
    for (i = 0; i < FILAS; i++)
    {
        for (j = 0; j < COLUMNAS; j++)
        {
            tablero[i][j] = ' ';
        }
    }
}

void imprimirTablero(char tablero[FILAS][COLUMNAS], int colorX, int colorO)
{
    int i, j;
    printf("\n");

    for (i = 0; i < FILAS; i++)
    {
        textcolor(BLANCO);
        for (j = 0; j < COLUMNAS; j++)
        {

            if (j == 0)
            {
                printf("\t");
            }

            printf(" ---");
        }

        printf("\n");

        for (j = 0; j < COLUMNAS; j++)
        {
            if (j == 0)
            {
                printf("\t");
            }

            if (tablero[i][j] == 'X')
            {
                printf("|");
                textcolor(colorX);
                printf(" %c ", tablero[i][j]);
            }
            else if (tablero[i][j] == 'O')
            {
                printf("|");
                textcolor(colorO);
                printf(" %c ", tablero[i][j]);
            }
            else
            {
                printf("| %c ", tablero[i][j]);
            }

            textcolor(BLANCO);
        }
        printf("|\n");

        if (i == (FILAS - 1))
        {
            for (j = 0; j < COLUMNAS; j++)
            {

                if (j == 0)
                {
                    printf("\t");
                }

                printf(" ---");
            }
        }
    }

    printf("\n\t");
    for (j = 0; j < COLUMNAS; j++)
    {
        printf("  %d ", j + 1);
    }

    textcolor(DEFAULT);
    Sleep(90);
    printf("\n\n");
}

bool colocarFicha(char tablero[FILAS][COLUMNAS], int columna, char ficha)
{
    int i;

    if (isnan(columna) || columna < 0 || columna >= COLUMNAS)
    {
        return false;
    }

    for (i = FILAS - 1; i >= 0; i--)
    {
        if (tablero[i][columna] == ' ')
        {
            tablero[i][columna] = ficha;
            return true;
        }
    }

    return false;
}

bool comprobarVictoria(char tablero[FILAS][COLUMNAS], char ficha)
{
    int i, j;

    // Comprobar horizontalmente
    for (i = 0; i < FILAS; i++)
    {
        for (j = 0; j < COLUMNAS - 3; j++)
        {
            if (tablero[i][j] == ficha && tablero[i][j + 1] == ficha &&
                tablero[i][j + 2] == ficha && tablero[i][j + 3] == ficha)
            {
                partidaTerminada = true;
                return true;
            }
        }
    }

    // Comprobar verticalmente
    for (i = 0; i < FILAS - 3; i++)
    {
        for (j = 0; j < COLUMNAS; j++)
        {
            if (tablero[i][j] == ficha && tablero[i + 1][j] == ficha &&
                tablero[i + 2][j] == ficha && tablero[i + 3][j] == ficha)
            {
                partidaTerminada = true;
                return true;
            }
        }
    }

    // Comprobar diagonal descendente
    for (i = 0; i < FILAS - 3; i++)
    {
        for (j = 0; j < COLUMNAS - 3; j++)
        {
            if (tablero[i][j] == ficha && tablero[i + 1][j + 1] == ficha &&
                tablero[i + 2][j + 2] == ficha && tablero[i + 3][j + 3] == ficha)
            {
                partidaTerminada = true;
                return true;
            }
        }
    }

    // Comprobar diagonal ascendente
    for (i = 0; i < FILAS - 3; i++)
    {
        for (j = 3; j < COLUMNAS; j++)
        {
            if (tablero[i][j] == ficha && tablero[i + 1][j - 1] == ficha &&
                tablero[i + 2][j - 2] == ficha && tablero[i + 3][j - 3] == ficha)
            {
                partidaTerminada = true;
                return true;
            }
        }
    }
    return false;
}

bool comprobarEmpate(char tablero[FILAS][COLUMNAS])
{
    int i, j;

    for (i = 0; i < FILAS; i++)
    {
        for (j = 0; j < COLUMNAS; j++)
        {
            if (tablero[i][j] == ' ')
            {
                return false;
            }
        }
    }

    partidaTerminada = true;
    return true;
}

int strategyVertical(char tablero[FILAS][COLUMNAS], char fichaOponente, int mode, int *retornar, bool *bandera, int ofensive)
{
    int i, j;
    int value = 0;
    for (j = 0; j < COLUMNAS; j++)
    {
        for (i = FILAS - 1; i >= 3; i--)
        {
            if (mode == 1)
            {
                if (tablero[i][j] == fichaOponente &&
                    tablero[i - 1][j] == fichaOponente &&
                    tablero[i - 2][j] == fichaOponente)
                {

                    value = j;
                    if (tablero[i - 3][value] == ' ')
                    {
                        stateCPU = value;
                        *retornar = value;
                        break;
                    }
                }
            }
            else if (mode == 2)
            {
                if (tablero[i][j] == fichaOponente &&
                    tablero[i - 1][j] == fichaOponente &&
                    tablero[i - 2][j] == fichaOponente)
                {
                    value = j;
                    if (tablero[i - 3][value] == ' ')
                    {
                        stateCPU = value;
                        *retornar = value;
                        break;
                    }
                }
            }
            else if (mode == 3)
            {
                value = j;

                if (tablero[i][j] == fichaOponente &&
                    tablero[i - 1][j] == fichaOponente &&
                    tablero[i - 2][j] == fichaOponente)
                {

                    value = j;
                    if (tablero[i - 3][value] == ' ')
                    {
                        stateCPU = value;
                        *retornar = value;
                        break;
                    }
                }
                else if (tablero[i][j] == fichaOponente &&
                         tablero[i - 1][j] == fichaOponente)
                {
                    value = j;
                    if (tablero[i - 2][value] == ' ')
                    {
                        stateCPU = value;
                        *bandera = true;
                    }
                }
            }
        }
    }
}

int strategyHorizontal(char tablero[FILAS][COLUMNAS], char fichaOponente, int mode, int *retornar, bool *bandera, int ofensive)
{
    int i, j;
    int value = 0;
    for (i = FILAS - 1; i >= 0; i--)
    {
        for (j = 0; j < COLUMNAS - 3; j++)
        {
            if (mode == 1)
            {
                if (tablero[i][j] == fichaOponente &&
                    tablero[i][j + 1] == fichaOponente &&
                    tablero[i][j + 2] == fichaOponente)
                {
                    value = j + 3;
                    if (tablero[i][value] == ' ' &&
                        tablero[i + 1][value] != ' ')
                    {
                        stateCPU = value;
                        *retornar = value;
                        break;
                    }
                    else if (tablero[i + 1][value] == ' ' &&
                             tablero[i + 2][value] != ' ')
                    {
                        notThisNumber[0] = value;
                    }
                }
                else if (tablero[i][j + 1] == fichaOponente &&
                         tablero[i][j + 2] == fichaOponente &&
                         tablero[i][j + 3] == fichaOponente)
                {
                    value = j;
                    if (tablero[i][value] == ' ' &&
                        tablero[i + 1][value] != ' ')
                    {
                        stateCPU = value;
                        *retornar = value;
                        break;
                    }
                    else if (tablero[i + 1][value] == ' ' &&
                             tablero[i + 2][value] != ' ')
                    {
                        notThisNumber[0] = value;
                    }
                }
            }
            else if (mode == 2)
            {
                if (tablero[i][j] == fichaOponente &&
                    tablero[i][j + 1] == fichaOponente &&
                    tablero[i][j + 2] == fichaOponente)
                {
                    value = j + 3;
                    if (tablero[i][value] == ' ' &&
                        tablero[i + 1][value] != ' ')
                    {
                        stateCPU = value;
                        *retornar = value;
                        break;
                    }
                    else if (tablero[i + 1][value] == ' ' &&
                             tablero[i + 2][value] != ' ')
                    {
                        notThisNumber[0] = value;
                    }
                }
                else if (tablero[i][j + 1] == fichaOponente &&
                         tablero[i][j + 2] == fichaOponente &&
                         tablero[i][j + 3] == fichaOponente)
                {
                    value = j;
                    if (tablero[i][value] == ' ' &&
                        tablero[i + 1][value] != ' ')
                    {
                        stateCPU = value;
                        *retornar = value;
                        break;
                    }
                    else if (tablero[i + 1][value] == ' ' &&
                             tablero[i + 2][value] != ' ')
                    {
                        notThisNumber[0] = value;
                    }
                }
                else if (tablero[i][j] == fichaOponente &&
                         tablero[i][j + 1] == fichaOponente &&
                         tablero[i][j + 3] == fichaOponente)
                {
                    value = j + 2;
                    if (tablero[i][value] == ' ' &&
                        tablero[i + 1][value] != ' ')
                    {
                        stateCPU = value;
                        *retornar = value;
                        break;
                    }
                    else if (tablero[i + 1][value] == ' ' &&
                             tablero[i + 2][value] != ' ')
                    {
                        notThisNumber[0] = value;
                    }
                }
                else if (tablero[i][j] == fichaOponente &&
                         tablero[i][j + 2] == fichaOponente &&
                         tablero[i][j + 3] == fichaOponente)
                {
                    value = j + 1;
                    if (tablero[i][value] == ' ' &&
                        tablero[i + 1][value] != ' ')
                    {

                        stateCPU = value;
                        *retornar = value;
                        break;
                    }
                    else if (tablero[i + 1][value] == ' ' &&
                             tablero[i + 2][value] != ' ')
                    {
                        notThisNumber[0] = value;
                    }
                }
                else if (tablero[i][j + 1] == fichaOponente &&
                         tablero[i][j + 2] == fichaOponente)
                {
                    int hasReturn = -1;
                    value = j;
                    if (tablero[i][value] == ' ' &&
                        tablero[i + 1][value] != ' ' &&
                        tablero[i][j + 3] != ' ')
                    {
                        stateCPU = value;
                        hasReturn = value;
                    }

                    value = j + 3;
                    if (tablero[i][value] == ' ' &&
                        tablero[i + 1][value] != ' ' &&
                        tablero[i][j] != ' ')
                    {
                        stateCPU = value;
                        hasReturn = value;
                    }

                    if (hasReturn != -1 &&
                        hasReturn != notThisNumber[0] &&
                        hasReturn != notThisNumber[1] &&
                        hasReturn != notThisNumber[2])
                    {
                        *retornar = hasReturn;
                        break;
                    }
                }
            }
            else if (mode == 3)
            {
                if (tablero[i][j] == fichaOponente &&
                    tablero[i][j + 1] == fichaOponente &&
                    tablero[i][j + 2] == fichaOponente)
                {
                    value = j + 3;
                    if (tablero[i][value] == ' ' &&
                        tablero[i + 1][value] != ' ')
                    {
                        stateCPU = value;
                        *retornar = value;
                        break;
                    }
                    else if (tablero[i + 1][value] == ' ' &&
                             tablero[i + 2][value] != ' ')
                    {
                        notThisNumber[0] = value;
                    }
                }
                else if (tablero[i][j + 1] == fichaOponente &&
                         tablero[i][j + 2] == fichaOponente &&
                         tablero[i][j + 3] == fichaOponente)
                {
                    value = j;
                    if (tablero[i][value] == ' ' &&
                        tablero[i + 1][value] != ' ')
                    {
                        stateCPU = value;
                        *retornar = value;
                        break;
                    }
                    else if (tablero[i + 1][value] == ' ' &&
                             tablero[i + 2][value] != ' ')
                    {
                        notThisNumber[0] = value;
                    }
                }
                else if (tablero[i][j] == fichaOponente &&
                         tablero[i][j + 1] == fichaOponente &&
                         tablero[i][j + 3] == fichaOponente)
                {
                    value = j + 2;
                    if (tablero[i][value] == ' ' &&
                        tablero[i + 1][value] != ' ')
                    {
                        stateCPU = value;
                        *retornar = value;
                        break;
                    }
                    else if (tablero[i + 1][value] == ' ' &&
                             tablero[i + 2][value] != ' ')
                    {
                        notThisNumber[0] = value;
                    }
                }
                else if (tablero[i][j] == fichaOponente &&
                         tablero[i][j + 2] == fichaOponente &&
                         tablero[i][j + 3] == fichaOponente)
                {
                    value = j + 1;
                    if (tablero[i][value] == ' ' &&
                        tablero[i + 1][value] != ' ')
                    {

                        stateCPU = value;
                        *retornar = value;
                        break;
                    }
                    else if (tablero[i + 1][value] == ' ' &&
                             tablero[i + 2][value] != ' ')
                    {
                        notThisNumber[0] = value;
                    }
                }

                else if (tablero[i][j + 1] == fichaOponente &&
                         tablero[i][j + 2] == fichaOponente)
                {
                    value = j + 3;
                    if (tablero[i][value] == ' ' &&
                        tablero[i + 1][value] != ' ' &&
                        tablero[i][j] == ' ' &&
                        tablero[i + 1][j] != ' ')
                    {

                        if (value != notThisNumber[0] &&
                            value != notThisNumber[1] &&
                            value != notThisNumber[2])
                        {
                            stateCPU = value;
                            *retornar = value;
                            break;
                        }
                        else if (j != notThisNumber[0] &&
                                 j != notThisNumber[1] &&
                                 j != notThisNumber[2])
                        {
                            stateCPU = j;
                            *retornar = j;
                            break;
                        }
                    }
                    else if (tablero[i][value] == ' ' &&
                             tablero[i + 1][value] != ' ' &&
                             (tablero[i][j] != ' ' &&
                              tablero[i + 1][j] == ' '))
                    {

                        *bandera = true;
                        stateCPU = value;
                    }
                    else if (tablero[i][j] == ' ' &&
                             tablero[i + 1][j] != ' ' &&
                             (tablero[i][value] != ' ' &&
                              tablero[i + 1][value] == ' '))
                    {

                        stateCPU = j;
                        *bandera = true;
                    }
                }
                else if (tablero[i][j] == fichaOponente &&
                         tablero[i][j + 2] == fichaOponente)
                {
                    value = j + 1;
                    if (tablero[i][value] == ' ' &&
                        tablero[i + 1][value] != ' ')
                    {

                        *bandera = true;
                        stateCPU = value;
                    }
                }
            }
        }
    }
}

int strategyDiagonalDescendente(char tablero[FILAS][COLUMNAS], char fichaOponente, int mode, int *retornar, bool *bandera, int ofensive)
{
    int i, j;
    int value = 0;
    for (i = FILAS - 1; i >= 3; i--)
    {
        for (j = COLUMNAS - 1; j >= 3; j--)
        {
            if (mode == 1)
            {
                if (tablero[i][j] == fichaOponente &&
                    tablero[i - 1][j - 1] == fichaOponente &&
                    tablero[i - 2][j - 2] == fichaOponente)
                {
                    value = j - 3;

                    if (tablero[i - 3][value] == ' ' &&
                        tablero[i - 2][value] != ' ')
                    {
                        stateCPU = value;
                        *retornar = value;
                        break;
                    }
                    else if (tablero[i - 1][value] == ' ')
                    {
                        *bandera = true;
                        stateCPU = value;
                    }
                    else if (tablero[i - 2][value] == ' ')
                    {
                        notThisNumber[1] = value;
                    }
                }
                else if (tablero[i - 1][j - 1] == fichaOponente &&
                         tablero[i - 2][j - 2] == fichaOponente &&
                         tablero[i - 3][j - 3] == fichaOponente)
                {
                    value = j;

                    if (tablero[i][value] == ' ' &&
                        tablero[i + 1][value] != ' ')
                    {
                        stateCPU = value;
                        *retornar = value;
                        break;
                    }
                    else if (tablero[i + 1][value] == ' ' &&
                             tablero[i + 2][value] != ' ')
                    {
                        notThisNumber[1] = value;
                    }
                }
            }
            else if (mode == 2)
            {
                if (tablero[i][j] == fichaOponente &&
                    tablero[i - 1][j - 1] == fichaOponente &&
                    tablero[i - 2][j - 2] == fichaOponente)
                {
                    value = j - 3;

                    if (tablero[i - 3][value] == ' ' &&
                        tablero[i - 2][value] != ' ')
                    {
                        stateCPU = value;
                        *retornar = value;
                        break;
                    }
                    else if (tablero[i - 1][value] == ' ')
                    {
                        *bandera = true;
                        stateCPU = value;
                    }
                    else if (tablero[i - 2][value] == ' ')
                    {
                        notThisNumber[1] = value;
                    }
                }
                else if (tablero[i - 1][j - 1] == fichaOponente &&
                         tablero[i - 2][j - 2] == fichaOponente &&
                         tablero[i - 3][j - 3] == fichaOponente)
                {
                    value = j;

                    if (tablero[i][value] == ' ' &&
                        tablero[i + 1][value] != ' ')
                    {
                        stateCPU = value;
                        *retornar = value;
                        break;
                    }
                    else if (tablero[i + 1][value] == ' ' &&
                             tablero[i + 2][value] != ' ')
                    {
                        notThisNumber[1] = value;
                    }
                }
                else if (tablero[i][j] == fichaOponente &&
                         tablero[i - 1][j - 1] == fichaOponente &&
                         tablero[i - 3][j - 3] == fichaOponente)
                {
                    value = j - 2;

                    if (tablero[i - 2][value] == ' ' &&
                        tablero[i - 1][value] != ' ')
                    {
                        stateCPU = value;
                        *retornar = value;
                        break;
                    }
                    else if (tablero[i - 1][value] == ' ' &&
                             tablero[i][value] != ' ')
                    {
                        notThisNumber[1] = value;
                    }
                }
                else if (tablero[i][j] == fichaOponente &&
                         tablero[i - 2][j - 2] == fichaOponente &&
                         tablero[i - 3][j - 3] == fichaOponente)
                {
                    value = j - 1;

                    if (tablero[i - 1][value] == ' ' &&
                        tablero[i][value] != ' ')
                    {
                        stateCPU = value;
                        *retornar = value;
                        break;
                    }
                    else if (tablero[i][value] == ' ' &&
                             tablero[i + 1][value] != ' ')
                    {
                        notThisNumber[1] = value;
                    }
                }
            }
            else if (mode == 3)
            {
                if (tablero[i][j] == fichaOponente &&
                    tablero[i - 1][j - 1] == fichaOponente &&
                    tablero[i - 2][j - 2] == fichaOponente)
                {
                    value = j - 3;

                    if (tablero[i - 3][value] == ' ' &&
                        tablero[i - 2][value] != ' ')
                    {
                        stateCPU = value;
                        *retornar = value;
                        break;
                    }
                    else if (tablero[i - 1][value] == ' ')
                    {
                        *bandera = true;
                        stateCPU = value;
                    }
                    else if (tablero[i - 2][value] == ' ')
                    {
                        notThisNumber[1] = value;
                    }
                }
                else if (tablero[i - 1][j - 1] == fichaOponente &&
                         tablero[i - 2][j - 2] == fichaOponente &&
                         tablero[i - 3][j - 3] == fichaOponente)
                {
                    value = j;

                    if (tablero[i][value] == ' ' &&
                        tablero[i + 1][value] != ' ')
                    {
                        stateCPU = value;
                        *retornar = value;
                        break;
                    }
                    else if (tablero[i + 1][value] == ' ' &&
                             tablero[i + 2][value] != ' ')
                    {
                        notThisNumber[1] = value;
                    }
                }
                else if (tablero[i][j] == fichaOponente &&
                         tablero[i - 1][j - 1] == fichaOponente &&
                         tablero[i - 3][j - 3] == fichaOponente)
                {
                    value = j - 2;

                    if (tablero[i - 2][value] == ' ' &&
                        tablero[i - 1][value] != ' ')
                    {
                        stateCPU = value;
                        *retornar = value;
                        break;
                    }
                    else if (tablero[i][value] == ' ')
                    {
                        *bandera = true;
                        stateCPU = value;
                    }
                    else if (tablero[i - 1][value] == ' ')
                    {
                        notThisNumber[1] = value;
                    }
                }
                else if (tablero[i][j] == fichaOponente &&
                         tablero[i - 2][j - 2] == fichaOponente &&
                         tablero[i - 3][j - 3] == fichaOponente)
                {
                    value = j - 1;

                    if (tablero[i - 1][value] == ' ' &&
                        tablero[i][value] != ' ')
                    {
                        stateCPU = value;
                        *retornar = value;
                        break;
                    }
                    else if (tablero[i][value] == ' ' &&
                             tablero[i + 1][value] != ' ')
                    {
                        notThisNumber[1] = value;
                    }
                }

                if (tablero[i - 1][j - 1] == fichaOponente &&
                    tablero[i - 2][j - 2] == fichaOponente)
                {
                    value = j - 3;
                    if (value != notThisNumber[0] &
                        value != notThisNumber[1] &
                        value != notThisNumber[2])
                    {
                        if (tablero[i - 3][value] == ' ' &&
                            tablero[i - 2][value] != ' ' &&
                            tablero[i][j] == ' ' &&
                            tablero[i + 1][j] != ' ')
                        {
                            stateCPU = value;
                            *bandera = true;
                        }
                        else if (tablero[i - 2][value] == ' ' &&
                                 tablero[i - 1][value] != ' ' &&
                                 tablero[i][j] == ' ' &&
                                 tablero[i + 1][j] != ' ' && ofensive != 1)
                        {
                            notThisNumber[1] = value;
                        }
                    }
                    else if (j != notThisNumber[0] &
                             j != notThisNumber[1] &
                             j != notThisNumber[2])
                    {
                        value = j;
                        if (tablero[i][value] == ' ' &&
                            tablero[i + 1][value] != ' ' &&
                            tablero[i - 3][j - 3] == ' ' &&
                            tablero[i - 2][j - 3] != ' ')
                        {
                            stateCPU = value;
                            *bandera = true;
                        }
                        else if (tablero[i + 1][value] == ' ' &&
                                 tablero[i + 2][value] != ' ' &&
                                 tablero[i - 3][j - 3] == ' ' &&
                                 tablero[i - 2][j - 3] != ' ' && ofensive != 1)
                        {
                            notThisNumber[1] = value;
                        }
                    }
                }
                else if (tablero[i][j] == fichaOponente &&
                         tablero[i - 3][j - 3] == fichaOponente)
                {
                    value = j - 1;
                    if (value != notThisNumber[0] &
                        value != notThisNumber[1] &
                        value != notThisNumber[2])
                    {
                        if (tablero[i - 1][value] == ' ' &&
                            tablero[i][value] != ' ' &&
                            tablero[i - 2][j - 2] == ' ' &&
                            tablero[i - 1][j - 2] != ' ')
                        {
                            stateCPU = value;
                            *bandera = true;
                        }
                        else if (tablero[i][value] == ' ' &&
                                 tablero[i + 1][value] != ' ' &&
                                 tablero[i - 2][j - 2] == ' ' &&
                                 tablero[i - 1][j - 2] != ' ' && ofensive != 1)
                        {
                            notThisNumber[1] = value;
                        }
                    }
                    else if (j - 2 != notThisNumber[0] &
                             j - 2 != notThisNumber[1] &
                             j - 2 != notThisNumber[2])
                    {
                        value = j - 2;
                        if (tablero[i - 2][value] == ' ' &&
                            tablero[i - 1][value] != ' ' &&
                            tablero[i - 1][j - 2] == ' ' &&
                            tablero[i][j - 1] != ' ')
                        {
                            stateCPU = value;
                            *bandera = true;
                        }
                        else if (tablero[i - 1][value] == ' ' &&
                                 tablero[i][value] != ' ' &&
                                 tablero[i - 1][j - 2] == ' ' &&
                                 tablero[i][j - 1] != ' ' && ofensive != 1)
                        {
                            notThisNumber[1] = value;
                        }
                    }
                }
                else if (tablero[i][j] == fichaOponente &&
                         tablero[i - 1][j - 1] == fichaOponente)
                {
                    value = j - 2;
                    if (value != notThisNumber[0] &
                        value != notThisNumber[1] &
                        value != notThisNumber[2])
                    {
                        if (tablero[i - 2][value] == ' ' &&
                            tablero[i - 1][value] != ' ' &&
                            tablero[i - 3][j - 3] == ' ' &&
                            tablero[i - 2][j - 3] != ' ')
                        {
                            stateCPU = value;
                            *bandera = true;
                        }
                        else if (tablero[i - 1][value] == ' ' &&
                                 tablero[i][value] != ' ' &&
                                 tablero[i - 3][j - 3] == ' ' &&
                                 tablero[i - 2][j - 3] != ' ' && ofensive != 1)
                        {
                            notThisNumber[1] = value;
                        }
                    }
                    else if (j - 3 != notThisNumber[0] &
                             j - 3 != notThisNumber[1] &
                             j - 3 != notThisNumber[2])
                    {
                        value = j - 3;
                        if (tablero[i - 3][value] == ' ' &&
                            tablero[i - 2][value] != ' ' &&
                            tablero[i - 2][j - 2] == ' ' &&
                            tablero[i - 1][j - 2] != ' ')
                        {
                            stateCPU = value;
                            *bandera = true;
                        }
                        else if (tablero[i - 2][value] == ' ' &&
                                 tablero[i - 1][value] != ' ' &&
                                 tablero[i - 2][j - 2] == ' ' &&
                                 tablero[i - 1][j - 2] != ' ' && ofensive != 1)
                        {
                            notThisNumber[1] = value;
                        }
                    }
                }
                else if (tablero[i - 2][j - 2] == fichaOponente &&
                         tablero[i - 3][j - 3] == fichaOponente)
                {
                    value = j;
                    if (value != notThisNumber[0] &
                        value != notThisNumber[1] &
                        value != notThisNumber[2])
                    {
                        if (tablero[i][value] == ' ' &&
                            tablero[i + 1][value] != ' ' &&
                            tablero[i - 1][j - 1] == ' ' &&
                            tablero[i][j - 1] != ' ')
                        {
                            stateCPU = value;
                            *bandera = true;
                        }
                        else if (tablero[i + 1][value] == ' ' &&
                                 tablero[i + 2][value] != ' ' &&
                                 tablero[i - 1][j - 1] == ' ' &&
                                 tablero[i][j - 1] != ' ' && ofensive != 1)
                        {
                            notThisNumber[1] = value;
                        }
                    }
                    else if (j - 1 != notThisNumber[0] &
                             j - 1 != notThisNumber[1] &
                             j - 1 != notThisNumber[2])
                    {
                        value = j - 1;
                        if (tablero[i - 1][value] == ' ' &&
                            tablero[i][value] != ' ' &&
                            tablero[i][j] == ' ' &&
                            tablero[i + 1][j] != ' ')
                        {
                            stateCPU = value;
                            *bandera = true;
                        }
                        else if (tablero[i][value] == ' ' &&
                                 tablero[i + 1][value] != ' ' &&
                                 tablero[i][j] == ' ' &&
                                 tablero[i + 1][j] != ' ' && ofensive != 1)
                        {
                            notThisNumber[1] = value;
                        }
                    }
                }
                else if (tablero[i][j] == fichaOponente &&
                         tablero[i - 2][j - 2] == fichaOponente)
                {
                    value = j - 3;
                    if (value != notThisNumber[0] &
                        value != notThisNumber[1] &
                        value != notThisNumber[2])
                    {
                        if (tablero[i - 3][value] == ' ' &&
                            tablero[i - 2][value] != ' ' &&
                            tablero[i - 1][j - 1] == ' ' &&
                            tablero[i][j - 1] != ' ')
                        {
                            stateCPU = value;
                            *bandera = true;
                        }
                        else if (tablero[i - 2][value] == ' ' &&
                                 tablero[i - 1][value] != ' ' &&
                                 tablero[i - 1][j - 1] == ' ' &&
                                 tablero[i][j - 1] != ' ' && ofensive != 1)
                        {
                            notThisNumber[1] = value;
                        }
                    }
                    else if ((j - 1) != notThisNumber[0] &
                             (j - 1) != notThisNumber[1] &
                             (j - 1) != notThisNumber[2])
                    {
                        value = j - 1;
                        if (tablero[i - 1][value] == ' ' &&
                            tablero[i][value] != ' ' &&
                            tablero[i - 3][j - 3] == ' ' &&
                            tablero[i - 2][j - 3] != ' ')
                        {
                            stateCPU = value;
                            *bandera = true;
                        }
                        else if (tablero[i][value] == ' ' &&
                                 tablero[i + 1][value] != ' ' &&
                                 tablero[i - 3][j - 3] == ' ' &&
                                 tablero[i - 2][j - 3] != ' ' && ofensive != 1)
                        {
                            notThisNumber[1] = value;
                        }
                    }
                }
            }
        }
    }
}

int strategyDiagonalAscendente(char tablero[FILAS][COLUMNAS], char fichaOponente, int mode, int *retornar, bool *bandera, int ofensive)
{
    int i, j;
    int value = 0;
    for (i = FILAS - 1; i >= 3; i--)
    {
        for (j = 0; j < COLUMNAS - 3; j++)
        {
            if (mode == 1)
            {
                if (tablero[i][j] == fichaOponente &&
                    tablero[i - 1][j + 1] == fichaOponente &&
                    tablero[i - 2][j + 2] == fichaOponente)
                {
                    value = j + 3;
                    if (tablero[i - 3][value] == ' ' &&
                        tablero[i - 2][value] != ' ')
                    {
                        stateCPU = value;
                        *retornar = value;
                        break;
                    }
                    else if (tablero[i - 2][value] == ' ' &&
                             tablero[i - 1][value] != ' ')
                    {
                        notThisNumber[2] = value;
                    }
                }
                else if (tablero[i - 1][j + 1] == fichaOponente &&
                         tablero[i - 2][j + 2] == fichaOponente &&
                         tablero[i - 3][j + 3] == fichaOponente)
                {
                    value = j;
                    if (tablero[i][value] == ' ' &&
                        tablero[i + 1][value] != ' ')
                    {
                        stateCPU = value;
                        *retornar = value;
                        break;
                    }
                    else if (tablero[i + 1][value] == ' ' &&
                             tablero[i + 2][value] != ' ')
                    {
                        notThisNumber[2] = value;
                    }
                }
            }
            else if (mode == 2)
            {
                if (tablero[i][j] == fichaOponente &&
                    tablero[i - 1][j + 1] == fichaOponente &&
                    tablero[i - 2][j + 2] == fichaOponente)
                {
                    value = j + 3;
                    if (tablero[i - 3][value] == ' ' &&
                        tablero[i - 2][value] != ' ')
                    {
                        stateCPU = value;
                        *retornar = value;
                        break;
                    }
                    else if (tablero[i - 2][value] == ' ' &&
                             tablero[i - 1][value] != ' ')
                    {
                        notThisNumber[2] = value;
                    }
                }
                else if (tablero[i - 1][j + 1] == fichaOponente &&
                         tablero[i - 2][j + 2] == fichaOponente &&
                         tablero[i - 3][j + 3] == fichaOponente)
                {
                    value = j;
                    if (tablero[i][value] == ' ' &&
                        tablero[i + 1][value] != ' ')
                    {
                        stateCPU = value;
                        *retornar = value;
                        break;
                    }
                    else if (tablero[i + 1][value] == ' ' &&
                             tablero[i + 2][value] != ' ')
                    {
                        notThisNumber[2] = value;
                    }
                }
                else if (tablero[i][j] == fichaOponente &&
                         tablero[i - 1][j + 1] == fichaOponente &&
                         tablero[i - 3][j + 3] == fichaOponente)
                {
                    value = j + 2;
                    if (tablero[i - 2][value] == ' ' &&
                        tablero[i - 1][value] != ' ')
                    {
                        stateCPU = value;
                        *retornar = value;
                        break;
                    }
                    else if (tablero[i - 1][value] == ' ' &&
                             tablero[i][value] != ' ')
                    {
                        notThisNumber[2] = value;
                    }
                }
                else if (tablero[i][j] == fichaOponente &&
                         tablero[i - 2][j + 2] == fichaOponente &&
                         tablero[i - 3][j + 3] == fichaOponente)
                {
                    value = j + 1;
                    if (tablero[i - 1][value] == ' ' &&
                        tablero[i][value] != ' ')
                    {
                        stateCPU = value;
                        *retornar = value;
                        break;
                    }
                    else if (tablero[i][value] == ' ' &&
                             tablero[i + 1][value] != ' ')
                    {
                        notThisNumber[2] = value;
                    }
                }
            }
            else if (mode == 3)
            {
                if (tablero[i][j] == fichaOponente &&
                    tablero[i - 1][j + 1] == fichaOponente &&
                    tablero[i - 2][j + 2] == fichaOponente)
                {
                    value = j + 3;
                    if (tablero[i - 3][value] == ' ' &&
                        tablero[i - 2][value] != ' ')
                    {
                        stateCPU = value;
                        *retornar = value;
                        break;
                    }
                    else if (tablero[i - 1][value] == ' ')
                    {
                        *bandera = true;
                        stateCPU = value;
                    }
                    else if (tablero[i - 2][value] == ' ')
                    {
                        notThisNumber[2] = value;
                    }
                }
                else if (tablero[i - 1][j + 1] == fichaOponente &&
                         tablero[i - 2][j + 2] == fichaOponente &&
                         tablero[i - 3][j + 3] == fichaOponente)
                {
                    value = j;
                    if (tablero[i][value] == ' ' &&
                        tablero[i + 1][value] != ' ')
                    {
                        stateCPU = value;
                        *retornar = value;
                        break;
                    }
                    else if (tablero[i + 1][value] == ' ' &&
                             tablero[i + 2][value] != ' ')
                    {
                        notThisNumber[2] = value;
                    }
                }
                else if (tablero[i][j] == fichaOponente &&
                         tablero[i - 1][j + 1] == fichaOponente &&
                         tablero[i - 3][j + 3] == fichaOponente)
                {
                    value = j + 2;
                    if (tablero[i - 2][value] == ' ' &&
                        tablero[i - 1][value] != ' ')
                    {
                        stateCPU = value;
                        *retornar = value;
                        break;
                    }
                    else if (tablero[i][value] == ' ')
                    {
                        *bandera = true;
                        stateCPU = value;
                    }
                    else if (tablero[i - 1][value] == ' ')
                    {
                        notThisNumber[2] = value;
                    }
                }
                else if (tablero[i][j] == fichaOponente &&
                         tablero[i - 2][j + 2] == fichaOponente &&
                         tablero[i - 3][j + 3] == fichaOponente)
                {
                    value = j + 1;
                    if (tablero[i - 1][value] == ' ' &&
                        tablero[i][value] != ' ')
                    {
                        stateCPU = value;
                        *retornar = value;
                        break;
                    }
                    else if (tablero[i][value] == ' ' &&
                             tablero[i + 1][value] != ' ')
                    {
                        notThisNumber[2] = value;
                    }
                }

                if (tablero[i - 1][j + 1] == fichaOponente &&
                    tablero[i - 2][j + 2] == fichaOponente)
                {
                    value = j + 3;
                    if (value != notThisNumber[0] &
                        value != notThisNumber[1] &
                        value != notThisNumber[2])
                    {
                        if (tablero[i - 3][value] == ' ' &&
                            tablero[i - 2][value] != ' ' &&
                            tablero[i][j] == ' ' &&
                            tablero[i + 1][j] != ' ')
                        {
                            stateCPU = value;
                            *bandera = true;
                        }
                        else if (tablero[i - 2][value] == ' ' &&
                                 tablero[i - 1][value] != ' ' &&
                                 tablero[i][j] == ' ' &&
                                 tablero[i + 1][j] != ' ' && ofensive != 1)
                        {
                            notThisNumber[2] = value;
                        }
                    }
                    else if (j != notThisNumber[0] &
                             j != notThisNumber[1] &
                             j != notThisNumber[2])
                    {
                        value = j;
                        if (tablero[i][value] == ' ' &&
                            tablero[i + 1][value] != ' ' &&
                            tablero[i - 3][j + 3] == ' ' &&
                            tablero[i - 2][j + 3] != ' ')
                        {
                            stateCPU = value;
                            *bandera = true;
                        }
                        else if (tablero[i + 1][value] == ' ' &&
                                 tablero[i + 2][value] != ' ' &&
                                 tablero[i - 3][j + 3] == ' ' &&
                                 tablero[i - 2][j + 3] != ' ' && ofensive != 1)
                        {
                            notThisNumber[2] = value;
                        }
                    }
                }
                else if (tablero[i][j] == fichaOponente &&
                         tablero[i - 3][j + 3] == fichaOponente)
                {
                    value = j + 1;
                    if (value != notThisNumber[0] &
                        value != notThisNumber[1] &
                        value != notThisNumber[2])
                    {
                        if (tablero[i - 1][value] == ' ' &&
                            tablero[i][value] != ' ' &&
                            tablero[i - 2][j + 2] == ' ' &&
                            tablero[i - 1][j + 2] != ' ')
                        {
                            stateCPU = value;
                            *bandera = true;
                        }
                        else if (tablero[i][value] == ' ' &&
                                 tablero[i + 1][value] != ' ' &&
                                 tablero[i - 2][j + 2] == ' ' &&
                                 tablero[i - 1][j + 2] != ' ' && ofensive != 1)
                        {
                            notThisNumber[2] = value;
                        }
                    }
                    else if (j + 2 != notThisNumber[0] &
                             j + 2 != notThisNumber[1] &
                             j + 2 != notThisNumber[2])
                    {
                        value = j + 2;
                        if (tablero[i - 2][value] == ' ' &&
                            tablero[i - 1][value] != ' ' &&
                            tablero[i - 1][j + 1] == ' ' &&
                            tablero[i][j + 1] != ' ')
                        {
                            stateCPU = value;
                            *bandera = true;
                        }
                        else if (tablero[i - 1][value] == ' ' &&
                                 tablero[i][value] != ' ' &&
                                 tablero[i - 1][j + 1] == ' ' &&
                                 tablero[i][j + 1] != ' ' && ofensive != 1)
                        {
                            notThisNumber[2] = value;
                        }
                    }
                }
                else if (tablero[i][j] == fichaOponente &&
                         tablero[i - 1][j + 1] == fichaOponente)
                {
                    value = j + 2;
                    if (value != notThisNumber[0] &
                        value != notThisNumber[1] &
                        value != notThisNumber[2])
                    {
                        if (tablero[i - 2][value] == ' ' &&
                            tablero[i - 1][value] != ' ' &&
                            tablero[i - 3][j + 3] == ' ' &&
                            tablero[i - 2][j + 3] != ' ')
                        {
                            stateCPU = value;
                            *bandera = true;
                        }
                        else if (tablero[i - 1][value] == ' ' &&
                                 tablero[i][value] != ' ' &&
                                 tablero[i - 3][j + 3] == ' ' &&
                                 tablero[i - 2][j + 3] != ' ' && ofensive != 1)
                        {
                            notThisNumber[2] = value;
                        }
                    }
                    else if (j + 3 != notThisNumber[0] &
                             j + 3 != notThisNumber[1] &
                             j + 3 != notThisNumber[2])
                    {
                        value = j + 3;
                        if (tablero[i - 3][value] == ' ' &&
                            tablero[i - 2][value] != ' ' &&
                            tablero[i - 2][j + 2] == ' ' &&
                            tablero[i - 1][j + 2] != ' ')
                        {
                            stateCPU = value;
                            *bandera = true;
                        }
                        else if (tablero[i - 2][value] == ' ' &&
                                 tablero[i - 1][value] != ' ' &&
                                 tablero[i - 2][j + 2] == ' ' &&
                                 tablero[i - 1][j + 2] != ' ' && ofensive != 1)
                        {
                            notThisNumber[2] = value;
                        }
                    }
                }
                else if (tablero[i - 2][j + 2] == fichaOponente &&
                         tablero[i - 3][j + 3] == fichaOponente)
                {
                    value = j;
                    if (value != notThisNumber[0] &
                        value != notThisNumber[1] &
                        value != notThisNumber[2])
                    {
                        if (tablero[i][value] == ' ' &&
                            tablero[i + 1][value] != ' ' &&
                            tablero[i - 1][j + 1] == ' ' &&
                            tablero[i][j + 1] != ' ')
                        {
                            stateCPU = value;
                            *bandera = true;
                        }
                        else if (tablero[i + 1][value] == ' ' &&
                                 tablero[i + 2][value] != ' ' &&
                                 tablero[i - 1][j + 1] == ' ' &&
                                 tablero[i][j + 1] != ' ' && ofensive != 1)
                        {
                            notThisNumber[2] = value;
                        }
                    }
                    else if (j + 1 != notThisNumber[0] &
                             j + 1 != notThisNumber[1] &
                             j + 1 != notThisNumber[2])
                    {
                        value = j + 1;
                        if (tablero[i - 1][value] == ' ' &&
                            tablero[i][value] != ' ' &&
                            tablero[i][j] == ' ' &&
                            tablero[i + 1][j] != ' ')
                        {
                            stateCPU = value;
                            *bandera = true;
                        }
                        else if (tablero[i][value] == ' ' &&
                                 tablero[i + 1][value] != ' ' &&
                                 tablero[i][j] == ' ' &&
                                 tablero[i + 1][j] != ' ' && ofensive != 1)
                        {
                            notThisNumber[2] = value;
                        }
                    }
                }
                else if (tablero[i][j] == fichaOponente &&
                         tablero[i - 2][j + 2] == fichaOponente)
                {
                    value = j + 3;
                    if (value != notThisNumber[0] &
                        value != notThisNumber[1] &
                        value != notThisNumber[2])
                    {
                        if (tablero[i - 3][value] == ' ' &&
                            tablero[i - 2][value] != ' ' &&
                            tablero[i - 1][j + 1] == ' ' &&
                            tablero[i][j + 1] != ' ')
                        {

                            stateCPU = value;
                            *bandera = true;
                        }
                        else if (tablero[i - 2][value] == ' ' &&
                                 tablero[i - 1][value] != ' ' &&
                                 tablero[i - 1][j + 1] == ' ' &&
                                 tablero[i][j + 1] != ' ' && ofensive != 1)
                        {
                            notThisNumber[2] = value;
                        }
                    }
                    else if ((j + 1) != notThisNumber[0] &
                             (j + 1) != notThisNumber[1] &
                             (j + 1) != notThisNumber[2])
                    {
                        value = j + 1;
                        if (tablero[i - 1][value] == ' ' &&
                            tablero[i][value] != ' ' &&
                            tablero[i - 3][j + 3] == ' ' &&
                            tablero[i - 2][j + 3] != ' ')
                        {
                            stateCPU = value;
                            *bandera = true;
                        }
                        else if (tablero[i][value] == ' ' &&
                                 tablero[i + 1][value] != ' ' &&
                                 tablero[i - 3][j + 3] == ' ' &&
                                 tablero[i - 2][j + 3] != ' ' && ofensive != 1)
                        {
                            notThisNumber[2] = value;
                        }
                    }
                }
            }
        }
    }
}

int defensivoCPU(char tablero[FILAS][COLUMNAS], char fichaOponente, int mode, int ofensive)
{

    int i, j;
    int retornar = -1;
    bool bandera = false;

    // Comprobar diagonal descendente
    strategyDiagonalDescendente(tablero, fichaOponente, mode, &retornar, &bandera, ofensive);

    if (retornar != -1)
    {
        return retornar;
    }

    // Comprobar diagonal ascendente /
    strategyDiagonalAscendente(tablero, fichaOponente, mode, &retornar, &bandera, ofensive);

    if (retornar != -1)
    {
        return retornar;
    }

    // Comprobar verticalmente
    strategyVertical(tablero, fichaOponente, mode, &retornar, &bandera, ofensive);
    if (retornar != -1)
    {
        return retornar;
    }
    // Comprobar horizontalmente
    strategyHorizontal(tablero, fichaOponente, mode, &retornar, &bandera, ofensive);

    if (retornar != -1)
    {
        return retornar;
    }

    // printf("\nNot THIS NUMBER H ---: %d \n", notThisNumber[0]);
    // printf("\nNot THIS NUMBER DD \\: %d \n", notThisNumber[1]);
    // printf("\nNot THIS NUMBER DA /: %d \n", notThisNumber[2]);
    // printf("\nSTATECPU: %d \n", stateCPU);

    if (bandera && ofensive != 1)
    {
        if (notThisNumber[0] != stateCPU &&
            notThisNumber[1] != stateCPU &&
            notThisNumber[2] != stateCPU)
        {
            // printf("\n notTNumber != STATECPU: %d \n", stateCPU);
            return stateCPU;
        }
    }

    return -1;
}

int movimientoRandom(char tablero[FILAS][COLUMNAS])
{

    int columnasValidas[COLUMNAS];
    int numColumnasValidas = 0;
    int j;

    for (j = 0; j < COLUMNAS; j++)
    {
        if (tablero[0][j] == ' ' && (j != notThisNumber[0] && j != notThisNumber[1] && j != notThisNumber[2]))
        {
            columnasValidas[numColumnasValidas] = j;
            numColumnasValidas++;
        }
    }

    if (numColumnasValidas > 0)
    {
        int indiceAleatorio = rand() % numColumnasValidas;
        stateCPU = columnasValidas[indiceAleatorio];
        return columnasValidas[indiceAleatorio];
    }
}

int movimientoComputador(char tablero[FILAS][COLUMNAS], char fichaOponente, int modo)
{
    int resDefensa = 0, resOfensivo = 0, resRamdon = 0;
    char ficha;

    int index;
    for (index = 0; index < 3; index++)
    {
        notThisNumber[index] = -1;
    }

    ficha = (fichaOponente == 'O') ? 'X' : 'O';

    resDefensa = defensivoCPU(tablero, fichaOponente, modo, 0);

    Sleep(700);

    if (resDefensa == -1)
    {
        resOfensivo = defensivoCPU(tablero, ficha, 3, 0);
        if (resOfensivo == -1)
        {
            resRamdon = movimientoRandom(tablero);

            if (resRamdon != notThisNumber[0] &&
                resRamdon != notThisNumber[1] &&
                resRamdon != notThisNumber[2])
            {
                return resRamdon;
            }
            else
            {
                resRamdon = movimientoRandom(tablero);
                return resRamdon;
            }
        }

        return resOfensivo;
    }

    resOfensivo = defensivoCPU(tablero, ficha, 2, 1);

    Sleep(700);

    if (resOfensivo != -1 &&
        resOfensivo != notThisNumber[0] &&
        resOfensivo != notThisNumber[1] &&
        resOfensivo != notThisNumber[2])
    {
        return resOfensivo;
    }

    return resDefensa;
}

void inicializarJugador(Player *jugador, char ficha)
{
    printf("Introduce el nombre del player (%c): ", ficha);
    fflush(stdin);
    gets(jugador->name);
    jugador->ficha = ficha;
    jugador->color = seleccionarColor(jugador->name, 0);
    jugador->point = 0;
    jugador->mode = 0;
}

void inicializarCPU(Player *jugador, char name[50], char ficha, int opcion)
{
    strcpy(jugador->name, name);
    jugador->ficha = ficha;
    jugador->color = seleccionarColor(name, 1);
    jugador->point = 0;
    jugador->mode = seleccionarDificultad(name, opcion);
}

void twoPlayers(void)
{
    int p = 0;

    int start = 0;

    system("cls");
    printf("\n");

    inicializarJugador(&players[0], 'X');
    inicializarJugador(&players[1], 'O');

    do
    {
        declaraColumnasFilas();

        char tablero[FILAS][COLUMNAS];
        inicializarTablero(tablero);

        char otherGame[3];
        int columna, input;

        printf("\n");

        while (!partidaTerminada)
        {

            imprimirTablero(tablero, players[0].color, players[1].color);
            printf("\tJugador %s (%c), elige una columna (1-%d): ", players[p].name, players[p].ficha, COLUMNAS);
            if (scanf("%d", &input) != 1)
            {

                fflush(stdin);

                columna = -1;
            }
            else
            {
                columna = (input > 9) ? -1 : (int)input - 1;
            }

            printf("\n");

            if (colocarFicha(tablero, columna, players[p].ficha))
            {
                if (comprobarVictoria(tablero, players[p].ficha))
                {
                    imprimirTablero(tablero, players[0].color, players[1].color);
                    players[p].point++;
                    printf("\n\tJugador %s gana!\n", players[p].name);
                    start = (start == 0) ? 1 : 0;
                }
                else if (comprobarEmpate(tablero))
                {
                    imprimirTablero(tablero, players[0].color, players[1].color);
                    printf("\n\tEs un empate!\n");
                }
                else
                {
                    changePlayer(&p);
                }
            }
            else
            {
                printf("\n\tMovimiento invalido. Intentalo de nuevo.\n\n");
            }
        }
        printf("\n\n");

        printf("\t\t\tPuntajes finales: \n\n");
        printf("\t\t%s: %d\n", players[0].name, players[0].point);
        printf("\t\t%s: %d", players[1].name, players[1].point);

        printf("\n\n");
        system("pause");

        printf("\n\t Otra partida? (si/no)\n");
        fflush(stdin);
        gets(otherGame);

        if (strcmp(otherGame, "si") != 0)
        {
            break;
        }

        partidaTerminada = false;
        p = start;

        system("cls");
    } while (true);
}

void playerVsCPU(void)
{

    int p = 0;
    int start = 0;

    inicializarJugador(&players[0], 'X');
    inicializarCPU(&players[1], "Computador", 'O', 0);

    do
    {

        declaraColumnasFilas();

        char tablero[FILAS][COLUMNAS];
        inicializarTablero(tablero);

        char otherGame[3];
        int columna, input;

        printf("\n");

        while (!partidaTerminada)
        {

            imprimirTablero(tablero, players[0].color, players[1].color);

            if (p == 0)
            {
                printf("\tJugador %s (%c), elige una columna (1-%d): ", players[p].name, players[p].ficha, COLUMNAS);
                if (scanf("%d", &input) != 1)
                {
                    fflush(stdin);
                    columna = -1;
                }
                else
                {
                    columna = (input > 9) ? -1 : (int)input - 1;
                }
                printf("\n");
            }
            else
            {
                columna = movimientoComputador(tablero, players[0].ficha, players[1].mode);

                printf("\t%s (%c) elige la columna: %d\n", players[p].name, players[p].ficha, columna + 1);
            }

            if (colocarFicha(tablero, columna, players[p].ficha))
            {
                if (comprobarVictoria(tablero, players[p].ficha))
                {
                    imprimirTablero(tablero, players[0].color, players[1].color);
                    players[p].point++;
                    printf("\n\tJugador %s gana!\n", players[p].name);
                    start = (start == 0) ? 1 : 0;
                }
                else if (comprobarEmpate(tablero))
                {
                    imprimirTablero(tablero, players[0].color, players[1].color);
                    printf("\n\tEs un empate!\n");
                }
                else
                {
                    changePlayer(&p);
                }
            }
            else
            {
                printf("\n\tMovimiento invalido. Intentalo de nuevo.\n\n");
            }
        }
        printf("\n\n");

        printf("\t\t\tPuntajes finales: \n\n");
        printf("\t\t%s: %d\n", players[0].name, players[0].point);
        printf("\t\t%s: %d", players[1].name, players[1].point);

        printf("\n\n");
        system("pause");

        printf("\n\t Otra partida? (si/no)\n");
        fflush(stdin);
        gets(otherGame);

        if (strcmp(otherGame, "si") != 0)
        {
            break;
        }

        partidaTerminada = false;
        p = start;

        system("cls");
    } while (true);
}

void CPUvsCPU(void)
{

    int p = 0;
    int start = 0;

    inicializarCPU(&players[0], "Computador 1", 'X', 3);
    inicializarCPU(&players[1], "Computador 2", 'O', 3);

    do
    {

        declaraColumnasFilas();

        char tablero[FILAS][COLUMNAS];
        inicializarTablero(tablero);

        char otherGame[3];
        int columna;
        int fichaOponente;

        printf("\n");

        while (!partidaTerminada)
        {

            imprimirTablero(tablero, players[0].color, players[1].color);
            printf("\n\n");
            system("pause");
            printf("\n");
            Sleep(200);

            fichaOponente = (p == 0) ? 1 : 0;

            columna = movimientoComputador(tablero, players[fichaOponente].ficha, players[p].mode);
            printf("\t%s (%c) elige la columna: %d\n", players[p].name, players[p].ficha, columna + 1);

            if (colocarFicha(tablero, columna, players[p].ficha))
            {
                if (comprobarVictoria(tablero, players[p].ficha))
                {
                    imprimirTablero(tablero, players[0].color, players[1].color);
                    players[p].point++;
                    printf("\n\tJugador %s gana!\n", players[p].name);
                    start = (start == 0) ? 1 : 0;
                }
                else if (comprobarEmpate(tablero))
                {
                    imprimirTablero(tablero, players[0].color, players[1].color);
                    printf("\n\tEs un empate!\n");
                }
                else
                {
                    changePlayer(&p);
                }
            }
            else
            {
                printf("\n\tERROR: Movimiento invalido. Intentando de nuevo...\n\n");
            }
        }
        printf("\n\n");

        printf("\t\t\tPuntajes finales: \n\n");
        printf("\t\t%s: %d\n", players[0].name, players[0].point);
        printf("\t\t%s: %d", players[1].name, players[1].point);

        printf("\n\n");
        system("pause");

        printf("\n\t Otra partida? (si/no)\n");
        fflush(stdin);
        gets(otherGame);

        if (strcmp(otherGame, "si") != 0)
        {
            break;
        }

        partidaTerminada = false;
        p = start;

        system("cls");
    } while (true);
}

int main(void)
{
    SetConsoleTitle("4 en Raya");
    system("color 0B");

    srand(time(NULL));

    char otherTypeGame[5];

    do
    {
        printf("\n\t**********************************");
        printf("\n\t************* MENU ***************");
        printf("\n\tEscoja el modo de juego:\n");
        printf("\n\t1. player vs player");
        printf("\n\t2. player vs computadora");
        printf("\n\t3. computadora vs computadora");
        printf("\n\n\tOpcion: ");
        scanf("%d", &opcionModoJuego);
        fflush(stdin);

        switch (opcionModoJuego)
        {
        case 1:
            system("cls");

            twoPlayers();
            break;

        case 2:
            system("cls");

            playerVsCPU();
            break;

        case 3:
            system("cls");

            CPUvsCPU();
            break;

        default:
            system("cls");
            printf("numero Incorrecto! se escojio el modo de juego computadora vs computadora\n\n");
            CPUvsCPU();
            break;
        }

        system("cls");

        partidaTerminada = false;
        printf("\n\n\t Desea Salir del juego? (si/no)\n");
        fflush(stdin);
        gets(otherTypeGame);

        if (strcmp(otherTypeGame, "no") != 0)
        {
            break;
        }
    } while (true);

    return 0;
}
