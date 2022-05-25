/*******************************************************
		     G/G/1 QUEUE SIMULATOR
*******************************************************/
#ifndef _QUEUE_H
#define _QUEUE_H

#include "simulator.h"
#include "calendar.h"
#include "event.h"
#include "buffer.h"
#include "packet.h"
#include "stat.h"

class queue: public simulator{

    calendar *cal;        // events calendar
    double inter;
    double packet_length;
    double Trslen;
    double Runlen;
    int NRUNmin;
    int NRUNmax;
	virtual void input();
	buffer* buf;            // queue buffer
	int	traffic_model;
	double	load;
	int	service_model;
	// counters
	double	packets;
	double	tot_delay;
    double link_capacity, prob1, prob2;
    bool has_linked_nodes = false;
    queue* linked_nodes[2];
    bool with_lambda;
	// statistics
	Sstat*	delay;
public:
	queue(int argc,char *argv[]);
	virtual ~queue(void);
	virtual void init(bool with_lambda);
	virtual void run(void);
    virtual void setProbabilities(double prob1=1);
    virtual void setLinkedNodes(queue* node1, queue* node2);
private:
	virtual void clear_counters(void);
	virtual void clear_stats(void);
	virtual void update_stats(void);
	virtual void print_trace(int Run);
	virtual void results(void);
	virtual int isconfsatisf(double perc);
};
#endif

