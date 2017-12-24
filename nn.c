#include "learn.h"

#define DEBUG 0
#define NUM_LAYERS 2

void create_network(int num_layers, int *layersizes, neuron **network) {
    int i, j, k, max_lsize;
    max_lsize = *layersizes;
    for (i = 0; i < num_layers; i++) {
        max_lsize = max(max_lsize, *(layersizes+i));
    }
    srand(time(NULL)); /* Seed the RNG */
    for (i = 0; i < num_layers; i++) {
        if (DEBUG) printf("layer %d.\n", i);
        for (j = 0; j < *(layersizes+i); j++) {
            if (DEBUG) printf(" - neuron %d.\n", j);
            neuron *n = *(network+i)+j;
            n->outputs = malloc(sizeof(*(n->outputs)) * *(layersizes+i+1));
            for (k = 0; k < *(layersizes+i+1); k++) {
                if (DEBUG) printf("    - synapse %d.\n", k);
                n->outputs[k].weight = (float) rand() / (float) RAND_MAX;
                 if (DEBUG) printf("     - %f\n", n->outputs[k].weight);
                n->outputs[k].output = *(network+i+1)+k;
            }
        }
    }
}

float sigmoid(float x) {
    // Logistic function. Bounded between 0 and 1. Non-linear. Derivative of s(x) is s(x)(1 - s(x))
    return 1.0f / (1.0f + exp(-x));
}

float sigmoid_prime(float x) {
    return x * (1.0 - x);
}

float activate(float x) {
    return sigmoid(x); /* This allows us to more easily change the activation function later. */
}

void feed_forward(int num_layers, int *layersizes, neuron **network, float *inputs) {
    int i, j, k;
    /* Clean the network (Zero its input_sum values) */
    for (i = 0; i < *layersizes; i++) {
        /* For each layer in the network... */
        for (j = 0; j < *(layersizes+i); j++) {
            /* For each neuron in the layer... */
            (*(network+i)/*Layer*/+j)/*Neuron*/->input_sum = (*(network+i)/*Layer*/+j)/*Neuron*/->bias;
            // TODO: bias also needs to be included in backprop.
        }
    }
    for (i = 0; i < *layersizes; i++) {
        ((*network)+i)->input_sum = *(inputs+i);
    }
    for (i = 0; i < num_layers; i++) {
        /* For each layer in the network... */
        for (j = 0; j < *(layersizes+i); j++) {
            /* For each neuron in the layer... */
            float output_val = activate((*(network+i)/*Layer*/+j)/*Neuron*/->input_sum);
            for (k = 0; k < *(layersizes+i+1); k++) {
                /* For each synapse of the neuron... */
                float outval = output_val * ((*(network+i)/*Layer*/+j)/*Neuron*/->outputs[k]/*Synapse*/.weight);
                (*(network+i)/*Layer*/+j)/*Neuron*/->outputs[k]/*Synapse*/.output/*Neuron*/->input_sum += outval;
            }
        }
    }
}

void backpropagate(int num_layers, int *layersizes, neuron **network, float *expected_outputs) {
    // TODO: the stuff to go here.
    // 1. For each layer, backwards...
    //    a. Calculate the gradient of the weights -> per neuron (gradient[w_i] = input[i] * deriv_sigmoid(gradient[o])).
    //    b. Calculate the gradient of the inputs  -> per neuron (gradient[x_i] = weight[i] * deriv_sigmoid(gradient[o])).
    //    c. Calculate the DELTA of the weights    -> per neuron (DELTA_wi = gradient[w_i] * Error[out] * lRate).
    //    TODO: Figure out how this changes for vectorised networks (I think it adds).
    int i;
    for (i = num_layers-1; i >= 0; i--) {
        // For each layer, working backwards (remember, the last layer is just for getting the outputs).
        // TODO: calculate the delta for each neuron/weight.
        printf("LAYERS.\n");
    }
}

int main(int argc, char **argv) {
    int *lsizes = malloc(sizeof(*lsizes) * (NUM_LAYERS + 1));
    *lsizes = 3; // Perceptron, for now.
    *(lsizes+1) = 2;
    *(lsizes+2) = 1;
    float *the_data = malloc(sizeof(*the_data) * *lsizes);
    *the_data = 1.0f;
    *(the_data+1)=2.0f;*(the_data+2)=3.0f;
    /* Allocate the network array. */
    int i;
    neuron **net = malloc(sizeof(*net) * NUM_LAYERS);
    for (i = 0; i < NUM_LAYERS+1; i++) {
        *(net+i) = malloc(sizeof(neuron) * *(lsizes+i));
    }
    /* Create the network (Randomise the synapse weights). */
    create_network(NUM_LAYERS, lsizes, net);
    for (i = 0; i < 10; i++) {
        feed_forward(NUM_LAYERS, lsizes, net, the_data);
        printf("%f\n", (*(net+NUM_LAYERS))->input_sum);
    }
    backpropagate(NUM_LAYERS, lsizes, net, &(*(net+NUM_LAYERS))->input_sum);
    return 0;
}
