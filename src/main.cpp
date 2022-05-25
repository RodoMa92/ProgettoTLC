/* -*- C++ -*- */
#include "../inc/global.h"
#include "../inc/queue.h"
#include "../inc/simulator.h"


int main(int argc, char *argv[]) {

    simulator *nodo1;
    simulator *nodo2;
    simulator *nodo3;
    simulator *nodo4;

    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    printf("**********************************************************\n\n");
    printf("               G/G/1 QUEUE SIMULATION PROGRAM\n\n");
    printf("**********************************************************\n\n");

    nodo1 = new queue(argc, argv);
    nodo2 = new queue(argc, argv);
    nodo3 = new queue(argc, argv);
    nodo4 = new queue(argc, argv);
    nodo1->init(true);
    nodo1->setProbabilities(0.5);
    nodo2->init();
    nodo2->setProbabilities(0.1);
    nodo3->init();
    nodo3->setProbabilities(0.9);
    nodo4->init();
    nodo4->setProbabilities();
    nodo1->run();
    nodo1->results();
    delete (nodo1);
    return EXIT_SUCCESS;
}
