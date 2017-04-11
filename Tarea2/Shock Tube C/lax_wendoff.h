/*
 * lax_wendoff.h
 *
 *  Created on: Apr 9, 2017
 *      Author: felip
 */

#ifndef LAX_WENDOFF_H_
#define LAX_WENDOFF_H_

u* init(void);
void F(f* F_n, u* U_n);
double u_max(u* U);
u* lax_wendoff(double tmax, int pI);
f* f_malloc(void);
u* u_malloc(void);

#endif /* LAX_WENDOFF_H_ */
