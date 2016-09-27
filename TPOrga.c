/*
 ============================================================================

 Name        : TPOrga.c
 Author      :
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct matriz {
  int ancho;
  int largo;
  char *matriz;
} t_matriz;

void matrizConstructor(t_matriz *matriz, int ancho, int largo) {
  matriz->ancho = ancho;
  matriz->largo = largo;
  matriz->matriz = malloc(sizeof(char) * (ancho * largo));
  memset(matriz->matriz, 0, ancho * largo);
}

void matrizDestructor(t_matriz *matriz) {
  free(matriz->matriz);
  matriz->matriz = NULL;
}

void setCelda(t_matriz *matriz, int fila, int columna, bool valor) {
  matriz->matriz[columna + fila * matriz->ancho] = valor;
}

bool getCelda(t_matriz *matriz, int fila, int columna) {
  return matriz->matriz[columna + fila * matriz->ancho];
}

// datosValidos
// para validar que la fila y columna pasadas por el archivo no queden fuera de
// rango
bool datosValidos(t_matriz *matriz, int fila, int columna) {
  if ((fila > matriz->largo - 1) || (columna > matriz->ancho - 1))
    return false;
  else
    return true;
}
/*
// cargarMatriz
// carga la matriz a partir del archivo
void cargarMatriz(t_matriz *matriz, char *archivo) {
  bool fin;
  memset(matriz->matriz, 0, matriz->ancho * matriz->largo);

  FILE *fp = fopen(archivo, "rb");
  fin = false;
  while (fscanf(fp, "%d %d", fila, columna) != 0 && !fin) {
    // No estoy seguro si lee bien, chekear

    if (!datosValidos(matriz, fila, columna)) {
      fputs(stderr, "Error de coordenadas.");
      fin = true;
    } else {
      setCelda(matriz, fila, columna, true);
    }
  }
  fclose(fp);
}*/

/* a : puntero a la posicion (0,0) de la matriz
 * i : columna
 * j : fila
 * M : cantidad de filas
 * N : cantidad de columnas
 */
unsigned int vecinos(unsigned char *a, unsigned int i, unsigned int j,
                     unsigned int M, unsigned int N) {

  int finH, finV, inicioH, inicioV, vecinos;

  inicioV = i - 1;
  if (inicioV < 0) inicioV = 0;

  inicioH = j - 1;
  if (inicioH < 0) inicioH = 0;

  finV = i + 1;
  if (finV > N - 1) finV = N - 1;

  finH = j + 1;
  if (finH > M - 1) finH = M - 1;

  vecinos = 0;
  for (int x = inicioH; x <= finH; x++) {
    for (int y = inicioV; y <= finV; y++) {
      if (!(x == j && y == i)) vecinos += a[x + N * y];
    }
  }
  return vecinos;
}
/*
void juego(int *argv) {

  t_matriz matriz;
  int iteraciones;
  char archivo[50];

  iteraciones = argv[1];
  archivo = argv[4];

  matrizConstructor(&matriz, argv[2], argv[3]);

  cargarMatriz(&matriz, &archivo);
  for (int i = 0; i < iteraciones; i++) {
    // vecinos
  }

  matrizDestructor(&matriz);
}*/
///////////////////////////////////////////TESTS////////////////

void vecinosTests() {
  unsigned char *matriz = malloc(sizeof(unsigned char) * 10);
  matriz[0] = 1;
  matriz[1] = 0;
  matriz[2] = 1;
  matriz[3] = 0;
  matriz[4] = 1;
  matriz[5] = 0;
  matriz[6] = 1;
  matriz[7] = 0;
  matriz[8] = 1;

  unsigned int M = 3;
  unsigned int N = 3;
  unsigned int resultado = vecinos(matriz, 1, 1, M, N);
  printf("Vecinos: %d", resultado);
  resultado = vecinos(matriz, 1, 2, M, N);
  printf("Vecinos: %d", resultado);

  free(matriz);
}

////////////////////////////////////////////////////////////////
int main(void) {
  vecinosTests();
  return EXIT_SUCCESS;
}
