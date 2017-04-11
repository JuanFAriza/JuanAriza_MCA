/*
 * lax_wendoff.c
 *
 *  Created on: Apr 9, 2017
 *      Author: felip
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "estructuras.h"
#include "lax_wendoff.h"

int I;
int i;

u* lax_wendoff(double tmax, int pI){
        I=pI;
        double dt=0.001;
	double t=0;
	double umax;

	u* U_n = init();
	u* U_n_p = init();
	u* U_n_m = init();
	f* F_n = f_malloc();
	F(F_n, U_n);
	f* F_n_p = f_malloc();
	F(F_n_p, U_n_p);
	f* F_n_m = f_malloc();
	F(F_n_m, U_n_m);

	while(t<tmax){
		t=t+dt;
		printf("Tiempo= %f, dt= %f\n",t,dt);

		for(i=1;i<I-1;i++){
		  U_n_p[i].U1 = 0.5*(U_n[i+1].U1 + U_n[i].U1) - 0.5*(dt/dx)*(F_n[i+1].F1 - F_n[i].F1);
		  U_n_p[i].U2 = 0.5*(U_n[i+1].U2 + U_n[i].U2) - 0.5*(dt/dx)*(F_n[i+1].F2 - F_n[i].F2);
		  U_n_p[i].U3 = 0.5*(U_n[i+1].U3 + U_n[i].U3) - 0.5*(dt/dx)*(F_n[i+1].F3 - F_n[i].F3);
		  U_n_m[i].U1 =0.5*(U_n[i].U1 + U_n[i-1].U1) - 0.5*(dt/dx)*(F_n[i].F1 - F_n[i-1].F1);
		  U_n_m[i].U2 =0.5*(U_n[i].U2 + U_n[i-1].U2) - 0.5*(dt/dx)*(F_n[i].F2 - F_n[i-1].F2);
		  U_n_m[i].U3 =0.5*(U_n[i].U3 + U_n[i-1].U3) - 0.5*(dt/dx)*(F_n[i].F3 - F_n[i-1].F3);
		}
		F(F_n_p,U_n_p);
		F(F_n_m,U_n_m);

		for(i=1;i<I-1;i++){
		  U_n[i].U1 = U_n[i].U1 - (dt/dx)*(F_n_p[i].F1 - F_n_m[i].F1);
		  U_n[i].U2 = U_n[i].U2 - (dt/dx)*(F_n_p[i].F2 - F_n_m[i].F2);
		  U_n[i].U3 = U_n[i].U3 - (dt/dx)*(F_n_p[i].F3 - F_n_m[i].F3);
		}
		F(F_n,U_n);

		umax=u_max(U_n);
		printf("vmax: %f\n",umax);
		if (umax>1 && umax<100){
		  dt = 0.5*dx/umax;
		  if (dt>0.01){
		    dt = 0.01;
		  }
		}
	}

	return U_n;

}

u* init(void){

	u* U_0=u_malloc();

	for (i = 0; i < I/2; i++) {
		U_0[i].U1=1.0;
		U_0[i].U2=0.0;
		U_0[i].U3=1/0.4;
	}
	for (i = I/2; i < I; i++) {
		U_0[i].U1=0.125;
		U_0[i].U2=0.0;
		U_0[i].U3=0.1/0.4;
	}
	return U_0;
}

void F(f* F_n, u* U_n){
  double P;
  for (i = 0; i < I; i++) {
    P= 0.4*(U_n[i].U3-0.5*(U_n[i].U2*U_n[i].U2)/U_n[i].U1);
    F_n[i].F1= U_n[i].U2;
    F_n[i].F2= (U_n[i].U2*U_n[i].U2)/U_n[i].U1+P;
    F_n[i].F3= U_n[i].U2/U_n[i].U1*(U_n[i].U3+P);
  }
}



double u_max(u* U){
	double max=0.0;
	double vel;
	for (i = 0; i < I; i++) {
	  //vel= pow(1.4*0.4*(U[i].U3-0.5*(U[i].U2*U[i].U2)/U[i].U1)/U[i].U1,0.5) + U[i].U2/U[i].U1;
	  vel = fabs(U[i].U2/U[i].U1);
	  if(vel>max){
	    max=vel;
	  }
	}
	return max;
}

f* f_malloc(void){
  f* F;
  if(!(F = malloc(I*sizeof(f)))){
    fprintf(stderr, "Problem with data allocation\n");fflush(stdout);
    exit(0);
  }
  else return F;
}

u* u_malloc(void){
	u* U;
		if(!(U = malloc(I*sizeof(u)))){
		    fprintf(stderr, "Problem with data allocation\n");fflush(stdout);
		    exit(0);
		}
		else return U;
}
