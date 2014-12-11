#include <stdio.h>
#include <stdlib.h>

#include <math.h>
#include <string.h>

#include "distribution.h"
#include "binomial.h"

struct BinoArgs {
	int n;
	double p;
};

static double combin_num(int n, int i)
{
	double cn = 1.0;

	if (i == 0 || n == i)
		return 1;

	if (i > n / 2)
		i = n - i;

//	printf("c(%d, %d) = ", n, i);
	for (; i > 0; --i, --n) 
		cn *= (double)n / i;

//	printf("%lf\n", cn);
	return cn;
}

static void add_segment_bino(double *distri_value, 
		      void *param, struct seginfo *seginfo)
{
	struct BinoArgs *args = (struct BinoArgs *)param;
	int n = args->n;
	double p = args->p;
	int nth = seginfo->nth;
	
	/* B() = (n,p)*p^i*(1-p)(n-i)*/
	if (nth == 0)
		distri_value[nth] = pow((1 - p), n);
	else
		distri_value[nth] = distri_value[nth - 1] + combin_num(n, nth)  
			            * pow(p, nth) * pow((1 - p), n - nth);
}

struct dist *create_bino(int n, double p)
{
	struct dist *bino = (struct dist *)malloc(sizeof(struct dist));
	bino->dist_type = "binomial";

	struct BinoArgs *args = malloc(sizeof(struct BinoArgs));
	args->n = n;
	args->p = p;

	bino->param = args;
	bino->add_segment = add_segment_bino;

	return bino;
}
