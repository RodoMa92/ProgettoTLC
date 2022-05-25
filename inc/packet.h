/***************************************************************************
			PACKET.H
***************************************************************************/

#ifndef PACKET_H
#define PACKET_H

#include "global.h"

class packet {
	double gen_time;
    double length;
	public:
	packet(double Gen_time, double length);
	~packet(){}
	packet* next;
	public:
	double get_time(){ return gen_time; }
};

inline packet::packet(double Gen_time, double length) {
	gen_time=Gen_time;
	next=nullptr;
}

#endif
