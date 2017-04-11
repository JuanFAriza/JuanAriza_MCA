/*
 * io.c
 *
 *  Created on: Apr 9, 2017
 *      Author: felipe
 */
#include <stdio.h>
#include <stdlib.h>
#include "estructuras.h"
#include <string.h>

int i;

void disp(u* U, int length)
{
	for (i = 0; i < length + 1; i++) {
		fprintf(stdout,"X= %f-> U1: %f, U2: %f, U3: %f\n", i*dx-dx, U[i].U1, U[i].U2, U[i].U3);
	}
}

void save(u* U, int length)
{
	   FILE *fp;
	   fp = fopen("shock.dat", "w");
	   for (i = 0; i < length ; i++) {
		   fprintf(fp,"%f %f %f\n", U[i].U1, U[i].U2, U[i].U3);
	   }
	   fclose(fp);
}
