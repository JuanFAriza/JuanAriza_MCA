// Voy a partir la grilla horizontalmente, cada procesador maneja todas las columnas, pero solo un rango de filas que va hasta la frontera mas una fila
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define L 5 // Longitud lateral del cuadro
#define l 2 // Longitud de la placa
#define d 1 // Separacion entre placas
#define V0 (float)(100) // Diferencia de potencial entre placas
#define h 0.01953 // Longitud de cada celda de la rejilla, tal que celdas=256
#define N (int)(2*pow((L/h),2)) // Numero de iteraciones
#define n (int)(L/h) // Numero de celdas por eje (256)
#define inicio_placa1 ((int)(((L/2) - (d/2))/h))*n + ((int)(((L/2) - (l/2))/h))
#define fin_placa1 ((int)(((L/2) - (d/2))/h))*n + ((int)(((L/2) + (l/2))/h))
#define inicio_placa2 ((int)(((L/2) + (d/2))/h))*n + ((int)(((L/2) - (l/2))/h))
#define fin_placa2 ((int)(((L/2) + (d/2))/h))*n + ((int)(((L/2) + (l/2))/h))


void inicializar(double *grid, int inicial, int final); // Inicializa todo en 0
void valores_fijos(double *grid, int inicial, int final);

int main(int argc, char** argv){
  MPI_Init(NULL, NULL);

  int world_rank, world_size;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  int i_inicial, i_final; // Desde que celda empieza cada procesador, y hasta donde llega

  i_inicial = -n + n*world_rank*n/world_size; // Se resta para incluir la fila anterior
  i_final = n + n*(world_rank + 1)*n/world_size; // Se suma para ir hasta la siguiente fila
  if (i_inicial < 0){
    i_inicial = 0;
  }
  if (i_final > n*n){
    i_final = n*n;
  }

  int i,j,iter;
  int j0Placa, jfPlaca, iPlaca1, iPlaca2; // Posicion de las placas

  j0Placa = (int)(((L/2) - (l/2))/h);
  jfPlaca = (n-1) - j0Placa;
  iPlaca1 = (int)(((L/2) - (d/2))/h);
  iPlaca2 = (n-1) - iPlaca1;

  double *V; // Matriz de potencial electrico presente
  double *Vfuturo; // Matriz de potencial electrico futuro
  double *Ex; // Matriz de campo electrico en x
  double *Ey; // Matriz de campo electrico en y

  V = malloc(n*n*sizeof(double));
  Vfuturo = malloc(n*n*sizeof(double));
  Ex = malloc(n*n*sizeof(double));
  Ey = malloc(n*n*sizeof(double));

  for (i=0;i<n;i++){ // Inicializa en 0 las columnas en el borde pres. y fut.
    Vfuturo[n*i+0] = V[n*i+0] = 0;
    Vfuturo[n*i+n-1] = V[n*i+n-1] = 0;
  }
  for (j=0;j<n;j++){ // Inicializa en 0 las filas en el borde pres. y fut.
    Vfuturo[n*0+j] = V[n*0+j] = 0;
    Vfuturo[n*(n-1)+j] = V[n*(n-1)+j] = 0;
  }

  for (i=1;i<n-1;i++){ // Inicializa TODO el interior en 1
    for (j=1;j<n-1;j++){
      V[n*i+j] = 1;
    }
  }

  for (j=j0Placa;j<jfPlaca+1;j++){ // Fija el voltaje en las placas
    Vfuturo[n*iPlaca1+j] = V[n*iPlaca1+j] = -V0/2;
    Vfuturo[n*iPlaca2+j] = V[n*iPlaca2+j] = V0/2;
  }

  for (iter=0;iter<N;iter++){
    for (i=1;i<n-1;i++){ // Actualiza el futuro de acuerdo al presente
      for (j=1;j<n-1;j++){
	Vfuturo[n*i+j] = (0.25)*(V[n*i+j+1]+V[n*(i+1)+j]+V[n*i+j-1]+V[n*(i-1)+j]);
      }
    }
    for (i=1;i<n-1;i++){ // Actualiza el presente al futuro
      for (j=1;j<n-1;j++){
	V[n*i+j] = Vfuturo[n*i+j];
      }
    }
    for (j=j0Placa;j<jfPlaca+1;j++){ // Fija el voltaje en las placas
      Vfuturo[n*iPlaca1+j] = V[n*iPlaca1+j] = -V0/2;
      Vfuturo[n*iPlaca2+j] = V[n*iPlaca2+j] = V0/2;
    }
  }

  for (i=0;i<n;i++){ // Calcula el campo Ex en las columnas en el borde
    Ex[n*i+0] = -(V[n*i+1] - V[n*i+0])/h;
    Ex[n*i+n-1] = -(V[n*i+n-1] - V[n*i+n-2])/h;
  }
  for (i=0;i<n;i++){ // Calcula el campo Ex en el resto de columnas
    for (j=1;j<n-1;j++){
      Ex[n*i+j] = -(V[n*i+j+1] - V[n*i+j-1])/(2*h);
    }
  }

  for (j=0;j<n;j++){ // Calcula el campo Ey en las filas en el borde
    Ey[n*0+j] = (V[n*1+j] - V[n*0+j])/h;
    Ey[n*(n-1)+j] = (V[n*(n-1)+j] - V[n*(n-2)+j])/h;
  }
  for (i=1;i<n-1;i++){ // Calcula el campo Ey en el resto de filas
    for (j=0;j<n;j++){
      Ey[n*i+j] = (V[n*(i+1)+j] - V[n*(i-1)+j])/(2*h);
    }
  }

  for (i=0;i<n;i++){ // Imprime Voltaje
    for (j=0;j<n;j++){
      printf("%f ",V[n*i+j]);
    }
    printf("\n");
  }
  for (i=0;i<n;i++){ // Imprime campo Ex
    for (j=0;j<n;j++){
      printf("%f ",Ex[n*i+j]);
    }
    printf("\n");
  }
  for (i=0;i<n;i++){ // Imprime campo Ey
    for (j=0;j<n;j++){
      printf("%f ",Ey[n*i+j]);
    }
    printf("\n");
  }
  return 0;
}

void inicializar(double *grid, int inicial, int final){
  int i;
  for (i=0;i<(final-inicial);i++){
    grid[i] = 1;
  }
}

void valores_fijos(double *grid, int inicial, int final){
  int i;
  int num_filas = (final - inicial)/n; // Numero de filas en este sector

  for (i=0;i<num_filas;i++){ // Fija en 0 las columnas
    grid[i*n+0] = grid[i*n+n-1] = 0;
  }
  
  if (inicial==0){ // Si empieza en fila 0 fija en 0 el borde superior
    for(i=0;i<n;i++){
      grid[i] = 0;
    }
  }
  if (final==n*n){ // Si termina en la ultima fila fija en 0 el borde inferior
    for(i=0;i<n;i++){
      grid[n*(num_filas-1)+i] = 0;
    }
  }

  if (inicial < inicio_placa1){
    if (final > fin_placa1){
      for (i=inicio_placa1-inicial;i<fin_placa1-inicial;i++){
	grid[i] = -V0/2; // Fijar placa 1
      }
    }
  }
  if (inicial < inicio_placa2){
    if (final > fin_placa2){
      for (i=inicio_placa2-inicial;i<fin_placa2-inicial;i++){
	grid[i] = V0/2; // Fijar placa 2
      }
    }
  }
}
