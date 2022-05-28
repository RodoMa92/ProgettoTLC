/***************************************************************************
			BUFFER.H
***************************************************************************/

#ifndef BUFFER_H
#define BUFFER_H

#include "packet.h"

class buffer	{

	packet* head;
	packet* last;
	public:
	int	status;

public:
	buffer();
	~buffer(){}
	void push(packet* pack);
	packet* pop();
	packet* full(){return head;}
	double tot_delay;
	double tot_packs;

    packet *get();
};

#endif
