/*
 ============================================================================

 Name        : TPOrga.c
 Autores     : Darius Maitia, Cristian González
 Description : Programa que simula el "juego de la vida"
 EJECUCIÓN   : gcc -std=c99 TPOrga.c -o tp

 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>  //atoi()
#include <stdbool.h>
#include <string.h>  //memset()

/*extern unsigned int vecinos(unsigned char *a, unsigned int columna, unsigned int fila,
                     unsigned int cantFilas, unsigned int cantCols);*/

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

int cargarEstado(matriz_t *matriz, int estado, char* nombre_archivo_output){
  int filas = matriz->largo;
  int columnas = matriz->ancho;
  char nombre_archivo[20];
  snprintf(nombre_archivo, 20, "%s_%d.pbm", nombre_archivo_output,estado);
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
  printf("Listo\n");
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
  if (filaInicial < 0) {
    filaInicial = 0;
  }

  columnaInicial = columna - 1;
  if (columnaInicial < 0) {
    columnaInicial = 0;
  }

  filaFinal = fila + 1;
  if (filaFinal > cantFilas - 1) {
    filaFinal = cantFilas - 1;
  }

  columnaFinal = columna + 1;
  if (columnaFinal > cantCols - 1) {
    columnaFinal = cantCols - 1;
  }

  vecinos = 0;
  for (int y = filaInicial; y <= filaFinal; y++) {
    for (int x = columnaInicial; x <= columnaFinal; x++) {
      if (!(x == columna && y == fila)) {
        vecinos += a[y * cantCols + x];
      }
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


void printAyuda(){
  printf("Uso:\n");
  printf("  conway -h\n");
  printf("  conway -V\n");
  printf("  conway i M N inputfile [-o outputprefix]\n");
  printf("Opciones:\n");
  printf("  -h, --help      Imprime este mensaje.\n");
  printf("  -V, --version   De la versión del programa.\n");
  printf("  -o              Prefijo de los archivos de salida.\n");
  printf("Ejemplos:\n");
  printf("    ./conway 5 5 10 pento -o muestra\n");
  printf("Representa 5 iteraciones del Juego de la Vida en una matriz de 5x10,\n");
  printf("con un estado inicial tomado del archivo ‘‘pento’’.\n");
  printf("Los archivos de salida se llamarán muestra_n.pbm.\n");
  printf("Si no se da un prefijo para los archivos de salida, el prefijo será el nombre del archivo de entrada.\n");
}


void printVersion(){
  printf("Juego de la Vida 1.0\n");
}

// conway 10 20 20 glider -o estado
int main(int argc, char *argv[]) {
  char* comando_ayuda = "-h";
  char* comando_version = "-v";
  if (strcmp(argv[1],comando_ayuda)==0){
    printAyuda();
    return 0;
  }
  if (strcmp(argv[1],comando_version)==0){
    printVersion();
    return 0;
  }
  int iteraciones = atoi(argv[1]);
  int ancho = atoi(argv[2]);
  int largo = atoi(argv[3]);
  char archivo[100];
  strcpy(archivo, argv[4]);
  matriz_t matriz;
  matrizConstructor(&matriz, ancho, largo);
  cargarMatriz(&matriz, archivo);

  for (int i = 1; i <= iteraciones; i++) {
    if (argc == 5){
      cargarEstado(&matriz, i,argv[4]);
    }else{
      cargarEstado(&matriz, i,argv[6]);
    }
    actualizarMatriz(&matriz);
  }
  matrizDestructor(&matriz);
  return 0;
}
