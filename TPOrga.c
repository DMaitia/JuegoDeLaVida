/*
 ============================================================================

 Name        : TPOrga.c
 Autores     : Darius Maitia, Cristian González
 Description : Programa que simula el "juego de la vida"
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>  //atoi()
#include <stdbool.h>
#include <string.h>  //memset()

typedef struct matriz {
  int ancho;
  int largo;
  unsigned char *matriz;
} matriz_t;

void matrizConstructor(matriz_t *matrizStruct, int ancho, int largo) {
  matrizStruct->ancho = ancho;
  matrizStruct->largo = largo;
  matrizStruct->matriz = malloc(sizeof(unsigned char) * (ancho * largo));
  memset(matrizStruct->matriz, 0, ancho * largo);
}

void matrizDestructor(matriz_t *matrizStruct) {
  free(matrizStruct->matriz);
  matrizStruct->matriz = NULL;
}

int posicion(int fila, int columna, int ancho) {
  return (columna + fila * ancho);
}
void setCelda(matriz_t *matrizStruct, int fila, int columna, bool valor) {
  matrizStruct->matriz[posicion(fila, columna, matrizStruct->ancho)] = valor;
}

bool getCelda(matriz_t *matrizStruct, int fila, int columna) {
  return matrizStruct->matriz[posicion(fila, columna, matrizStruct->ancho)];
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

int cargarEstado(matriz_t *matriz, int estado) {
  int filas = matriz->largo;
  int columnas = matriz->ancho;
  char nombre_archivo[20];
  snprintf(nombre_archivo, 20, "estado_%d.pbm", estado);
  printf("Grabando %s\n", nombre_archivo);
  char dimensiones[10];

  FILE *fp = fopen(nombre_archivo, "w");  // genero el archivo para escribir
  snprintf(dimensiones, 10, "P1 %d %d\n", matriz->ancho, matriz->largo);
  fputs(dimensiones, fp);
  for (int i = 0; i < filas; i++) {
    for (int j = 0; j < columnas; j++) {
      char buffer[3];
      snprintf(buffer, 3, "%d ", getCelda(matriz, i, j));
      fputs(buffer, fp);
    }
    fputs("\n", fp);
  }
  printf("Listo");
  return 0;
}

// cargarMatriz
// carga la matriz a partir del archivo
void cargarMatriz(matriz_t *matrizStruct, char *archivo) {
  bool fin = false;
  int fila, columna;

  FILE *fp = fopen(archivo, "rb");
  while (fscanf(fp, "%d %d", &fila, &columna) != EOF && !fin) {
    if (!datosValidos(matrizStruct, fila, columna)) {
      fprintf(stderr, "Error de coordenadas.");
      fin = true;
    } else {
      setCelda(matrizStruct, fila, columna, true);
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
unsigned int vecinos(unsigned char *a, unsigned int columna, unsigned int fila,
                     unsigned int cantFilas, unsigned int cantCols) {

  int columnaFinal, filaFinal, columnaInicial, filaInicial, vecinos;

  filaInicial = fila - 1;
  if (filaInicial < 0) filaInicial = 0;

  columnaInicial = columna - 1;
  if (columnaInicial < 0) columnaInicial = 0;

  filaFinal = fila + 1;
  if (filaFinal > cantFilas - 1) filaFinal = cantFilas - 1;

  columnaFinal = columna + 1;
  if (columnaFinal > cantCols - 1) columnaFinal = cantCols - 1;

  vecinos = 0;
  for (int y = filaInicial; y <= filaFinal; y++) {
    for (int x = columnaInicial; x <= columnaFinal; x++) {
      if (!(x == fila && y == columna)) vecinos += a[x + cantCols * y];
    }
  }
  return vecinos;
}

void mostrarMatriz(matriz_t *matrizStruct) {
  for (int i = 0; i < matrizStruct->largo; i++) {
    for (int j = 0; j < matrizStruct->ancho; j++) {
      printf("%d ", getCelda(matrizStruct, i, j));
    }
    printf("\n");
  }
}
// matriz[columna + fila * matrizStruct->ancho] = valor;
void actualizarMatriz(matriz_t *matrizStruct) {
  int v;
  matriz_t nuevaMatriz;
  matrizConstructor(&nuevaMatriz, matrizStruct->ancho, matrizStruct->largo);
  for (int fil = 0; fil < matrizStruct->largo; fil++) {
    for (int col = 0; col < matrizStruct->ancho; col++) {
      v = vecinos(matrizStruct->matriz, col, fil, matrizStruct->largo,
                  matrizStruct->ancho);

      // Si una celda tiene menos de dos o mas de tres vecinos encendidos, su
      // siguiente estado es apagado.
      if (v < 2 || v > 3) {
        setCelda(&nuevaMatriz, fil, col, 0);
      }
      // Si una celda encendida tiene dos o tres vecinos encendidos, su
      // siguiente
      // estado es encendido.
      if (getCelda(matrizStruct, fil, col) && (v == 3 || v == 2)) {
        setCelda(&nuevaMatriz, fil, col, 1);
      }
      // Si una celda apagada tiene exactamente tres vecinos encendidos, su
      // siguiente estado es encendido.
      if (!getCelda(matrizStruct, fil, col) && v == 3) {
        setCelda(&nuevaMatriz, fil, col, 1);
      }
    }
  }
  memcpy(matrizStruct, &nuevaMatriz, sizeof(nuevaMatriz));
  // matrizDestructor(&nuevaMatriz);*/
}

int main() {
  char archivo[100] = "glider";
  matriz_t matriz;
  matrizConstructor(&matriz, 14, 10);
  cargarMatriz(&matriz, archivo);
  mostrarMatriz(&matriz);
  int v = vecinos(matriz.matriz, 4, 4, matriz.largo, matriz.ancho);
  printf("Vecinos en 4,4 : %d\n", v);
  v = vecinos(matriz.matriz, 3, 5, matriz.largo, matriz.ancho);
  printf("Vecinos 3,5: %d\n", v);
  v = vecinos(matriz.matriz, 6, 4, matriz.largo, matriz.ancho);
  printf("Vecinos 6,4: %d\n", v);
  printf("=====================\n");

  actualizarMatriz(&matriz);
  // printf("=====================\n");
  mostrarMatriz(&matriz);
  matrizDestructor(&matriz);
  return 0;
}
