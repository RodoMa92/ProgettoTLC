/* -*- C++ -*- */
/*******************************************************
		QUEUE.C
*******************************************************/
#include "../inc/global.h"
#include <cstdio>
#include "../inc/queue.h"
#include "../inc/rand.h"
#include "../inc/buffer.h"
#include "../inc/event.h"
#include "../inc/calendar.h"
#include "../inc/easyio.h"




queue::queue(int argc, char *argv[]) : simulator(argc, argv) {
    cal = new calendar();
    buf = new buffer();
    delay = new Sstat();
}

queue::~queue() {
    delete delay;
    delete cal;
    delete buf;
}

void queue::input() {
    printf("MODEL PARAMETERS:\n\n");

    this->link_capacity = read_double("Inserisci la capacita", 100, 50, 3000);
    if (this->with_lambda) {
        printf("\n Arrivals model:\n");
        printf("1 - Poisson:>\n");
        traffic_model = read_int("", 1, 1, 1);
        load = read_double("Traffic load (Erlang)", 0.4, 0.01, 0.999);
        printf("\n Service model:\n");
        printf("1 - Exponential:>\n");
        service_model = read_int("", 1, 1, 1);
        packet_length = read_double("Average packet length [bits]", 0.4, 0.01, 100);
        inter = (packet_length / link_capacity) / load;
    }
    printf("SIMULATION PARAMETERS:\n\n");
    Trslen = read_double("Simulation transient len (s)", 100, 0.01, 10000);
    Trslen = Trslen;
    Runlen = read_double("Simulation RUN len (s)", 100, 0.01, 10000);
    Runlen = Runlen;
    NRUNmin = read_int("Simulation number of RUNs", 5, 2, 100);
}


void queue::init(bool with_lambda = false) {
    this->with_lambda = with_lambda;
    input();
    if (this->with_lambda) {
        event *Ev;
        Ev = new arrival(0.0, buf, this);
        cal->put(Ev);
    }
    buf->status = 0;
}

void queue::run() {

    extern double Trslen;
    extern double Runlen;
    extern int NRUNmin;
    extern int NRUNmax;

    double clock = 0.0;
    event *ev;
    while (clock < Trslen) {
        ev = cal->get();
        ev->body();
        clock = ev->time;
        delete (ev);
    }
    clear_stats();
    clear_counters();
    int current_run_number = 1;
    while (current_run_number <= NRUNmin) {
        while (clock < (current_run_number * Runlen + Trslen)) {
            ev = cal->get();
            ev->body();
            clock = ev->time;
            delete (ev);
        }
        update_stats();
        clear_counters();
        print_trace(current_run_number);
        current_run_number++;
    }
}


void queue::results() {
    extern double Trslen;
    extern double Runlen;
    extern int NRUNmin;
    extern int NRUNmax;

    fprintf(fpout, "*********************************************\n");
    fprintf(fpout, "           SIMULATION RESULTS                \n");
    fprintf(fpout, "*********************************************\n\n");
    fprintf(fpout, "Input parameters:\n");
    fprintf(fpout, "Transient length (s)         %5.3f\n", Trslen);
    fprintf(fpout, "Run length (s)               %5.3f\n", Runlen);
    fprintf(fpout, "Number of runs               %5d\n", NRUNmin);
    fprintf(fpout, "Traffic load                 %5.3f\n", load);
    fprintf(fpout, "Average service packet_length     %5.3f\n", packet_length);
    fprintf(fpout, "Results:\n");
    fprintf(fpout, "Average Delay                %2.6f   +/- %.2e  p:%3.2f\n",
            delay->mean(),
            delay->confidence(.95),
            delay->confpercerr(.95));
    fprintf(fpout, "D  %2.6f   %2.6f   %.2e %2.6f\n", load, delay->mean(), delay->confidence(.95),
            packet_length * (load) / (1 - load));
}

void queue::print_trace(int n) {
    fprintf(fptrc, "*********************************************\n");
    fprintf(fptrc, "                 TRACE RUN %d                \n", n);
    fprintf(fptrc, "*********************************************\n\n");


    fprintf(fptrc, "Average Delay                %2.6f   +/- %.2e  p:%3.2f\n",
            delay->mean(),
            delay->confidence(.95),
            delay->confpercerr(.95));
    fflush(fptrc);

}

void queue::clear_counters() {
    buf->tot_delay = 0.0;
    buf->tot_packs = 0.0;
}

void queue::clear_stats() {
    delay->reset();
}

void queue::update_stats() {
    *delay += buf->tot_delay / buf->tot_packs;
}

int queue::isconfsatisf(double perc) {
    return delay->isconfsatisfied(10, .95);
}

void queue::setProbabilities(double prob1) {
    this->prob1 = prob1;
    this->prob2 = 1-prob1;
}

void queue::setLinkedNodes(queue *node1, queue *node2) {
    this->has_linked_nodes = true;
    this->linked_nodes[0] = node1;
    this->linked_nodes[1] = node2;
}
