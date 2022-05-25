/*******************************************************
		     EVENT . H
*******************************************************/


#ifndef _EVENT_H
#define _EVENT_H

#include "global.h"
#include "buffer.h"


class event{
public:
	event*	next;	// next event
	double 	time;	// event time
	event();
	event(double Time, queue* ref);
	event(event* Next, double Time);
	~event(){}
	virtual void body(){}
};

inline event::event(){
	next=nullptr;
	time=-1;
	}

inline event::event(event* Next, double Time, queue* ref){
	next=Next;
	time=Time;
    myqueue = ref;
	}

inline event::event(double Time, queue* ref){
	time=Time;
    myqueue = ref;
	}

class arrival: public event{

    buffer* buf;

	public:
	int source_id;
	virtual void body();
	arrival(double Time, buffer *Buf, queue *queue_ref);
};

class service: public event{

	buffer* buf;

	public:
	virtual void body();
	service(double Time, buffer* Buf): event(Time){buf=Buf;}
	};

inline arrival::arrival(double Time, buffer *Buf, queue *queue_ref) : event(Time, queue_ref){
	buf=Buf;
	}


#endif

