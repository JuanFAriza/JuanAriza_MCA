/*
 * main.c
 *
 *  Created on: Apr 9, 2017
 *      Author: felipe
 */
#include <stdio.h>
#include <stdlib.h>
#include "estructuras.h"
#include "lax_wendoff.h"
#include "io.h"

int I;

int main(int argc, char** argv){
        double t;
	t = atof(argv[1]);
	I=1 + 1.0/dx;
	u* U=u_malloc();
	U=lax_wendoff(t, I);
	save(U, I);

	return 0;
}
