/* -*- C++ -*- */
/********************************************************
		SIMULATOR.H
	Abstract class for simulation program

*******************************************************/

#ifndef _SIMULATOR_H
#define _SIMULATOR_H

#include <cstdio>
#include "queue.h"

class simulator{
protected:
    FILE *fptrc;
	FILE *fpout;
	void read_args(int argc,char* argv[]);
	virtual void input(void) = 0;	
	virtual void print_trace(int i) = 0;
    virtual void setLinkedNodes(queue* node1, queue* node2);
public:
	simulator(int argc,char* argv[]);
	virtual ~simulator(void) {;}
	virtual void init(bool with_lambda = false) = 0;
	virtual void run(void) = 0;
	virtual void results(void) = 0;
    virtual void setProbabilities(double prob1=1);
};


#endif
