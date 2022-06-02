/***************************************************************************
			PACKET.H
***************************************************************************/

#ifndef PACKET_H
#define PACKET_H

#include "global.h"

class packet {
	double gen_time;
    double length;
    int node;
    int path[20000];
    int index;
    void addToPath(int node) {
        path[index++] = node;
    }
public:
	packet(double Gen_time);
	~packet(){}
	packet* next;
    double getLength() {return this->length;}
    void setLength(double length) {
    	this->length = length;
	}
	double get_time(){ return gen_time; }
	int getNode() {
		return this->node;
	}
	void setNode(int node) {
        this->node = node;
        addToPath(node);
    }
    void getPath() {
        printf("Path:");
        for(int i=0; i<index; i++) {
            printf(" %d ", path[i]);
        }
        printf("\n");
    }
};

inline packet::packet(double Gen_time) {
    index=0;
	gen_time=Gen_time;
	node = 1;
    length = 0;
	next=nullptr;
}

#endif
