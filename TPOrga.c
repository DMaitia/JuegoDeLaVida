/*
 ============================================================================

 Name        : TPOrga.c
 Author      :
 Version     :
 Copyright   : Your copyright notice
 Description : Programa que simulo el "juego de la vida"
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h> //memset()

typedef struct matriz {
  int ancho;
  int largo;
  char *matriz;
} matriz_t;

void matrizConstructor(matriz_t* matrizStruct, int ancho, int largo) {
  matrizStruct->ancho = ancho;
  matrizStruct->largo = largo;
  matrizStruct->matriz = malloc(sizeof(char) * (ancho * largo));
  memset(matrizStruct->matriz, 0, ancho * largo);
}

void matrizDestructor(matriz_t *matrizStruct) {
  free(matrizStruct->matriz);
  matrizStruct->matriz = NULL;
}

void setCelda(matriz_t *matrizStruct, int fila, int columna, bool valor) {
  matrizStruct->matriz[columna + fila * matrizStruct->ancho] = valor;
}

bool getCelda(matriz_t *matrizStruct, int fila, int columna) {
  return matrizStruct->matriz[columna + fila * matrizStruct->ancho];
}

// datosValidos
// para validar que la fila y columna pasadas por el archivo no queden fuera de
// rango
bool datosValidos(matriz_t *matrizStruct, int fila, int columna) {
  if ((fila > matrizStruct->largo - 1) || (columna > matrizStruct->ancho - 1))
    return false;
  else
    return true;
}

// cargarMatriz
// carga la matriz a partir del archivo
void cargarMatriz(matriz_t *matrizStruct, char* archivo) {
  bool fin = false;

  FILE *fp = fopen(archivo, "rb");
  //de donde sale fila y columna?
  //FORMATO DE SCANF
  //int fscanf ( FILE * stream, const char * format,char* destin);
  while (fscanf(fp, "%d %d", fila, columna) != 0 && !fin) {  // No estoy seguro si lee bien, chekear
    if (!datosValidos(matrizStruct, fila, columna)) {
      fprintf(stderr,"Error de coordenadas.");
      fin = true;
    } else {
      //recibe ancho y largo o largo y ancho
      setCelda(matrizStruct, matrizStruct->ancho, matrizStruct->largo, true);
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

  int vecinos = 0;
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

  matriz_t matriz;
  int iteraciones;
  char* archivo;

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
