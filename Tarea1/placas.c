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

  MPI_Request send_sig_request, send_ante_request, recv_sig_request, recv_ante_request;
  MPI_Status status;

  int world_rank, world_size, source, destination;
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
  int num_filas = (i_final - i_inicial)/n; // Numero de filas en este sector

  int i,j,iter;
  
  double *V; // Matriz de potencial electrico presente
  double *Vfuturo; // Matriz de potencial electrico futuro
  double *Vsend; // Matriz que se envia al procesador central
  double *Vfinal, *Ex, *Ey; // Matriz del recuadro completo
  double *send_sig, *send_ante;
  double *recv_sig, *recv_ante;
  
  V = malloc((i_final - i_inicial)*sizeof(double));
  Vfuturo = malloc((i_final - i_inicial)*sizeof(double));
  Vsend = malloc(sizeof(double)*n*n/world_size);
  send_sig = malloc(n*sizeof(double));
  send_ante = malloc(n*sizeof(double));
  recv_sig = malloc(n*sizeof(double));
  recv_ante = malloc(n*sizeof(double));

  inicializar(V,i_inicial,i_final);
  valores_fijos(V,i_inicial,i_final);
  inicializar(Vfuturo,i_inicial,i_final);
  valores_fijos(Vfuturo,i_inicial,i_final);

  printf("Se inicializaron las matrices");

  for (iter=0;iter<N;iter++){
    for (i=1;i<num_filas-1;i++){ // Actualiza el futuro de acuerdo al presente
      for (j=1;j<n-1;j++){
	Vfuturo[n*i+j] = (0.25)*(V[n*i+j+1]+V[n*(i+1)+j]+V[n*i+j-1]+V[n*(i-1)+j]);
      }
    }
    for (i=0;i<n;i++){
      send_ante[i] = Vfuturo[n+i]; // Segunda fila
      send_sig[i] = Vfuturo[(num_filas-2)*n+i]; // Penultima fila
    }
    if (world_rank>0){ // Si no es el primero, envia y recibe del anterior
      destination = world_rank - 1;
      source = world_rank - 1;
      MPI_Isend(&send_ante, n, MPI_DOUBLE, destination, 0, MPI_COMM_WORLD, &send_ante_request);
      MPI_Irecv(&recv_ante, n, MPI_DOUBLE, source, 0, MPI_COMM_WORLD, &recv_ante_request);
    }
    if (world_rank<world_size-1){ // Si no es el ultimo, envia y recibe del siguiente
      destination = world_rank + 1;
      source = world_rank + 1;
      MPI_Isend(&send_sig, n, MPI_DOUBLE, destination, 0, MPI_COMM_WORLD, &send_sig_request);
      MPI_Irecv(&recv_sig, n, MPI_DOUBLE, source, 0, MPI_COMM_WORLD, &recv_sig_request);
    }

    MPI_Wait(&send_ante_request, &status);
    MPI_Wait(&recv_ante_request, &status);
    MPI_Wait(&send_sig_request, &status);
    MPI_Wait(&recv_sig_request, &status);

    if (world_rank>0){ // Actualiza en futuro frontera inferior
      for (i=0;i<n;i++){
	Vfuturo[i] = recv_ante[i];
      }
    }
    if (world_rank<world_size-1){ // Actualiza en futuro frontera superior
      for (i=0;i<n;i++){
	Vfuturo[(num_filas-1)*n+i] = recv_sig[i];
      }
    }

    for (i=0;i<(i_final-i_inicial);i++){ // Actualiza presente de acuerdo a futuro
      V[i] = Vfuturo[i];
    }

    valores_fijos(V,i_inicial,i_final); // Fijo los valores fijos
  }

  // Define el array que va a enviar a centralizar
  if (i_inicial==0){
    for (i=0;i<i_final-n;i++){
      Vsend[i] = V[i];
    }
  }
  if (i_inicial!=0){
    if (i_final==n*n){
      for (i=n;i<i_final;i++){
	Vsend[i-n] = V[i];
      }
    }
    if (i_final!=n*n){
      for(i=n;i<i_final-n;i++){
	Vsend[i-n] = V[i];
      }
    }
  }

  if (world_rank==0){ // Asigna para el proc0 la memoria de las matrices finales
    Vfinal = malloc(n*n*sizeof(double));
    Ex = malloc(n*n*sizeof(double));
    Ey = malloc(n*n*sizeof(double));
  }

  MPI_Gather(&Vsend, (n*n/world_size), MPI_DOUBLE, Vfinal, (n*n/world_size), MPI_DOUBLE, 0, MPI_COMM_WORLD);  // Centraliza los Vsend en Vfinal

  if (world_rank==0){ // Si es el proc. 0 imprime los valores
    for (i=0;i<n;i++){ // Calcula el campo Ex en las columnas en el borde
      Ex[n*i+0] = -(Vfinal[n*i+1] - Vfinal[n*i+0])/h;
      Ex[n*i+n-1] = -(Vfinal[n*i+n-1] - Vfinal[n*i+n-2])/h;
    }
    for (i=0;i<n;i++){ // Calcula el campo Ex en el resto de columnas
      for (j=1;j<n-1;j++){
	Ex[n*i+j] = -(Vfinal[n*i+j+1] - Vfinal[n*i+j-1])/(2*h);
      }
    }

    for (j=0;j<n;j++){ // Calcula el campo Ey en las filas en el borde
      Ey[n*0+j] = (Vfinal[n*1+j] - Vfinal[n*0+j])/h;
      Ey[n*(n-1)+j] = (Vfinal[n*(n-1)+j] - Vfinal[n*(n-2)+j])/h;
    }
    for (i=1;i<n-1;i++){ // Calcula el campo Ey en el resto de filas
      for (j=0;j<n;j++){
	Ey[n*i+j] = (Vfinal[n*(i+1)+j] - Vfinal[n*(i-1)+j])/(2*h);
      }
    }

    for (i=0;i<n;i++){ // Imprime Voltaje
      for (j=0;j<n;j++){
	printf("%f ",Vfinal[n*i+j]);
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
