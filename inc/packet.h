/***************************************************************************
			PACKET.H
***************************************************************************/

#ifndef PACKET_H
#define PACKET_H

#include "global.h"

class packet {
	double gen_time;
    double elapsed_time;
    double length;
public:
	packet(double Gen_time);
	~packet(){}
	packet* next;
    void setLength(double lenght) {this -> length = lenght;}
    double getLength() {return this->length;}
	double get_time(){ return gen_time; }
    void set_time(double time){ this->gen_time =time;}
    void add_elapsed_time(double elapsed) { this->elapsed_time = elapsed;}
    double get_elapsed_time(){ return this->elapsed_time;}
};

inline packet::packet(double Gen_time) {
	gen_time=Gen_time;
    length = 0;
    elapsed_time = 0;
	next=nullptr;
}

#endif
