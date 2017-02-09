#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define L (double)5
#define l (double)2
#define d (double)1
#define V0 (double)100
#define h (double)0.01953
#define N (int)(2*pow((L/h),2))
#define n (int)(L/h)
#define inicio_placa1 ((int)(((L/2) - (d/2))/h))*n + ((int)(((L/2) - (l/2))/h))
#define fin_placa1 ((int)(((L/2) - (d/2))/h))*n + ((int)(((L/2) + (l/2))/h))
#define inicio_placa2 ((int)(((L/2) + (d/2))/h))*n + ((int)(((L/2) - (l/2))/h))
#define fin_placa2 ((int)(((L/2) + (d/2))/h))*n + ((int)(((L/2) + (l/2))/h))

void inicializar(double *grid, int inicial, int final); // Inicializa todo en 0
void valores_fijos(double *grid, int inicial, int final);

int main(int argc, char** argv){
  int iter, i, j, i_inicial, i_final, num_filas;

  MPI_Init(NULL,NULL);

  int world_rank, world_size;
  int source, destination;

  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  MPI_Request reqs[4];
  MPI_Status status[4];

  i_inicial = (n*n*world_rank/world_size) - n; // Se resta para incluir la fila anterior
  i_final = (n*(world_rank + 1)*n/world_size) + n; // Se suma para ir hasta la siguiente fila

  if (world_rank==0){
    i_inicial = 0;
  }
  if (world_rank==world_size-1){
    i_final = n*n;
  }
  num_filas = (i_final - i_inicial)/n; // Numero de filas en este sector

  printf("procesador %d tiene %d filas\n",world_rank,num_filas);

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

  for (iter=0;iter<10000;iter++){ // Devolver a N no 10000
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
      MPI_Isend(send_ante, n, MPI_DOUBLE, destination, 0, MPI_COMM_WORLD, &reqs[0]);
      MPI_Irecv(recv_ante, n, MPI_DOUBLE, source, 0, MPI_COMM_WORLD, &reqs[1]);
    }
    if (world_rank<world_size-1){ // Si no es el ultimo, envia y recibe del siguiente
      destination = world_rank + 1;
      source = world_rank + 1;
      MPI_Isend(send_sig, n, MPI_DOUBLE, destination, 0, MPI_COMM_WORLD, &reqs[2]);
      MPI_Irecv(recv_sig, n, MPI_DOUBLE, source, 0, MPI_COMM_WORLD, &reqs[3]);
    }

    if (world_rank==0){
      MPI_Wait(&reqs[2], &status[2]);
      MPI_Wait(&reqs[3], &status[3]);
    }
    else if (world_rank==world_size-1){
      MPI_Wait(&reqs[0], &status[0]);
      MPI_Wait(&reqs[1], &status[1]);
    }
    else{
      MPI_Wait(&reqs[0], &status[0]);
      MPI_Wait(&reqs[1], &status[1]);
      MPI_Wait(&reqs[2], &status[2]);
      MPI_Wait(&reqs[3], &status[3]);
    }

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

    // Actualiza con lo recibido
  }

  printf("Iteraciones correctas\n");

  // Define el array que va a enviar a centralizar
  if (world_rank==0){
    for (i=0;i<i_final-n;i++){
      Vsend[i] = V[i];
    }
    printf("Proc %d creo send\n",world_rank);
  }
  else if (world_rank==(world_size-1)){
    for (i=0;i<(n*n/world_size);i++){
      Vsend[i] = V[(i+n)];
    }
    printf("Proc %d creo send\n",world_rank);
  }
  else {
    for (i=0;i<(n*n/world_size);i++){
      Vsend[i] = V[(i+n)];
    }
    printf("Proc %d creo send\n",world_rank);
  }

  if (world_rank==0){ // Asigna para el proc0 la memoria de las matrices finales
    Vfinal = malloc(n*n*sizeof(double));
    Ex = malloc(n*n*sizeof(double));
    Ey = malloc(n*n*sizeof(double));
    printf("Proc %d creo finales\n",world_rank);
  }

  MPI_Gather(Vsend, (n*n/world_size), MPI_DOUBLE, Vfinal, (n*n/world_size), MPI_DOUBLE, 0, MPI_COMM_WORLD);  // Centraliza los Vsend en Vfinal

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

  MPI_Finalize();
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
	grid[i] = -V0/2.0; // Fijar placa 1
      }
    }
  }
  if (inicial < inicio_placa2){
    if (final > fin_placa2){
      for (i=inicio_placa2-inicial;i<fin_placa2-inicial;i++){
	grid[i] = V0/2.0; // Fijar placa 2
      }
    }
  }
}
