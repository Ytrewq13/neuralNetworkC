#include <stdio.h>
#include <math.h>
#include <time.h>
#include <malloc.h>
#include <stdlib.h>

struct NEURON;

typedef struct {
	struct NEURON * output;
	float weight;
} synapse;

typedef struct NEURON {
	float input_sum;
	float bias;
    synapse * outputs;
} neuron;


int max(int a, int b) {
    return (a > b) ? a : b;
}
