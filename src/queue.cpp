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

calendar *cal;        // events calendar
double inter;
double length;
double capacities[4];
double probabilities[3] = {.5,.1,.1};
double Trslen;
double Runlen;
int NRUNmin;
int NRUNmax;


queue::queue(int argc, char *argv[]) : simulator(argc, argv) {
    cal = new calendar();
    buf = new buffer();
    buf_node2 = new buffer();
    buf_node3 = new buffer();
    buf_node4 = new buffer();
    delay = new Sstat();
}

queue::~queue() {
    delete delay;
    delete cal;
    delete buf;
    delete buf_node2;
    delete buf_node3;
    delete buf_node4;
}

void queue::input() {
    printf("MODEL PARAMETERS:\n\n");
    printf("\n Arrivals model:\n");
    printf("1 - Poisson:>\n");
    traffic_model = read_int("", 1, 1, 1);
    load = read_double("Lambda", 1, 0.01, 1000);
    printf("\n Service model:\n");
    printf("1 - Exponential:>\n");
    service_model = read_int("", 1, 1, 1);
    length = read_double("Average packet length [bits]", 1024, 10, 10000);
    capacities[0] = read_int("Link capacity 1 [bits/s]", 1048576, 10, 2000000);
    capacities[1] = read_int("Link capacity 2 [bits/s]", 1048576, 10, 2000000);
    capacities[2] = read_int("Link capacity 3 [bits/s]", 1048576, 10, 2000000);
    capacities[3] = read_int("Link capacity 4 [bits/s]", 1048576, 10, 2000000);
    inter = 1 / load;
    printf("SIMULATION PARAMETERS:\n\n");
    Trslen = read_double("Simulation transient len [s]", 100, 0.01, 10000);
    Trslen = Trslen;
    Runlen = read_double("Simulation RUN len [s]", 100, 0.01, 10000);
    Runlen = Runlen;
    NRUNmin = read_int("Simulation number of RUNs", 5, 2, 100);
}


void queue::init() {
    input();
    event *Ev;
    Ev = new arrival(0.0, buf, buf_node2, buf_node3, buf_node4);
    cal->pushAndReorder(Ev);
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
        ev = cal->pop();
        ev->body();
        clock = ev->time;
        delete (ev);
    }
    clear_stats();
    clear_counters();
    int current_run_number = 1;
    while (current_run_number <= NRUNmin) {
        while (clock < (current_run_number * Runlen + Trslen)) {
            ev = cal->pop();
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
    fprintf(fpout, "Average service length     %5.3f\n", length);
    fprintf(fpout, "Results:\n");
    fprintf(fpout, "Crossing Time                %2.6f   +/- %.2e  p:%3.2f\n",
            delay->mean(),
            delay->confidence(.95),
            delay->confpercerr(.95));
    fprintf(fpout, "D  %2.6f   %2.6f   %.2e %2.6f\n", load, delay->mean(), delay->confidence(.95),
            length * (load) / (1 - load));
}

void queue::print_trace(int n) {
    fprintf(fptrc, "*********************************************\n");
    fprintf(fptrc, "                 TRACE RUN %d                \n", n);
    fprintf(fptrc, "*********************************************\n\n");


    fprintf(fptrc, "Crossing Time                %2.6f   +/- %.2e  p:%3.2f\n",
            delay->mean(),
            delay->confidence(.95),
            delay->confpercerr(.95));
    fflush(fptrc);

}

void queue::clear_counters() {
    buf_node4->tot_delay = 0.0;
    buf_node4->tot_packs = 0.0;
}

void queue::clear_stats() {
    delay->reset();
}

void queue::update_stats() {
    *delay += buf_node4->tot_delay / buf_node4->tot_packs;
}

int queue::isconfsatisf(double perc) {
    return delay->isconfsatisfied(10, .95);
}
