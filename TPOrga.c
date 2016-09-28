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
  char *matriz;
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

int cargarEstado(matriz_t *matriz, int estado) {
  int filas = matriz->largo;
  int columnas = matriz->ancho;
  char nombre_archivo[20];
  snprintf(nombre_archivo, 20, "estado_%d.pbm", estado);
  printf("Grabando %s\n", nombre_archivo);
  char dimensiones[10];

  FILE *fp = fopen(nombre_archivo, "w");  // genero el archivo para escribir
  snprintf(dimensiones, 10, "P4 %d %d\n", matriz->ancho, matriz->largo);
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
      // printf("%d ", fila);
      // printf("%d\n", columna);
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

void mostrarMatriz(matriz_t *matrizStruct) {
  for (int i = 0; i < matrizStruct->ancho; i++) {
    for (int j = 0; j < matrizStruct->largo; j++) {
      printf("%d ", getCelda(matrizStruct, i, j));
    }
    printf("\n");
  }
}

int main() {
  char archivo[100] = "/home/darius/workspace2/TPOrga/src/glider";
  matriz_t matriz;
  matrizConstructor(&matriz, 10, 10);
  cargarMatriz(&matriz, archivo);
  mostrarMatriz(&matriz);
  int v = vecinos(matriz.matriz, 4, 4, matriz.ancho, matriz.largo);
  cargarEstado(&matriz, 1);
  printf("Vecinos : %d", v);
  matrizDestructor(&matriz);
  return 0;
}
/*
int main(int argc, char *argv[]) {

  matriz_t *matriz;

  int iteraciones = atoi(argv[1]);  // convert char* to int
  int largo = atoi(argv[3]);
  int ancho = atoi(argv[2]);
  // crear matriz
  matrizConstructor(matriz, ancho, largo);

  // se carga con los datos del archivo
  cargarMatriz(matriz, argv[4]);
  for (int i = 0; i < iteraciones; i++) {
    // vecinos
  }

  matrizDestructor(matriz);
  return 0;
}*/
