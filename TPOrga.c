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

// cargarMatriz
// carga la matriz a partir del archivo
void cargarMatriz(t_matriz *matriz, string archivo) {
  bool fin;

  FILE *fp = fopen(archivo, "rb");
  fin = false;
  while (fscanf(fp, "%d %d", fila, columna) != 0 &&
         !fin) {  // No estoy seguro si lee bien, chekear

    if (!datosValidos(matriz, fila, columna)) {
      fputs(stderr, "Error de coordenadas.");
      fin = true;
    } else {
      setCelda(matriz, fila, columna, true);
    }
  }
  fclose(fp);
}

/* a : puntero a la posicion (0,0) de la matriz
 * i : columna
 * j : fila
 * M : cantidad de filas
 * N : cantidad de columnas
 */
unsigned int vecinos(unsigned char *a, unsigned int i, unsigned int j,
                     unsigned int M, unsigned int N) {

  vecinos = 0;
  for (int x = i - 1; x <= i + 1; x++) {
    for (int y = j - 1; y <= j + 1; y++) {
      if (y == -1) y = M - 1;
      if (y == M) y = 0;
      if (x == -1) x = N - 1;
      if (x == N) x = 0;

      vecinos += a[x + y * N];
    }
  }
}
void juego(int *argv) {

  t_matriz matriz;
  int iteraciones;
  string archivo;

  iteraciones = argv[1];
  archivo = argv[4];

  matrizConstructor(&matriz, argv[2], argv[3]);

  cargarMatriz(&matriz, archivo);
  for (int i = 0; i < iteraciones; i++) {
    // vecinos
  }

  matrizDestructor(&matriz);
}

int main(void) {
  puts("!!!Hello World!!!"); /* prints !!!Hello World!!! */
  return EXIT_SUCCESS;
}
