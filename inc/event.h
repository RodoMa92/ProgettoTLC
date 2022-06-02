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
	event(double Time);
	event(event* Next, double Time);
	~event(){}
	virtual void body(){}
};

inline event::event(){
	next=nullptr;
	time=-1;
}

inline event::event(event* Next, double Time){
	next=Next;
	time=Time;
	}

inline event::event(double Time){
	time=Time;
	}

class arrival: public event{

	buffer* buf;
	buffer* buf_left;
	buffer* buf_right;
	buffer* buf_last;
	
	public:
	packet* pack;
	virtual void body();
	arrival(double Time, buffer* buf1, buffer* buf2, buffer* buf3, buffer* buf4);
};

class service: public event{

	buffer* buf;
	buffer* buf_left;
	buffer* buf_right;
	buffer* buf_last;
	public:
	virtual void body();
	service(double Time, buffer* buf1, buffer* buf2, buffer* buf3, buffer* buf4): event(Time){
		buf = buf1;
		buf_left = buf2;
		buf_right = buf3;
		buf_last = buf4;
	}
};

inline arrival::arrival(double Time, buffer* buf1, buffer* buf2, buffer* buf3, buffer* buf4): event(Time){
	buf = buf1;
	buf_left = buf2;
	buf_right = buf3;
	buf_last = buf4;
	pack = nullptr;
}
class arrival23: public event{

	buffer* buf;
	buffer* buf_left;
	buffer* buf_right;
	buffer* buf_last;
	
	public:
	packet* pack;
	virtual void body();
	arrival23(double Time, buffer* buf1, buffer* buf2, buffer* buf3, buffer* buf4);
};

class service23: public event{

	buffer* buf;
	buffer* buf_left;
	buffer* buf_right;
	buffer* buf_last;
	public:
	virtual void body();
	service23(double Time, buffer* buf1, buffer* buf2, buffer* buf3, buffer* buf4): event(Time){
		buf = buf1;
		buf_left = buf2;
		buf_right = buf3;
		buf_last = buf4;
	}
};

inline arrival23::arrival23(double Time, buffer* buf1, buffer* buf2, buffer* buf3, buffer* buf4): event(Time){
	buf = buf1;
	buf_left = buf2;
	buf_right = buf3;
	buf_last = buf4;
	pack = nullptr;
}
class arrival4: public event{

	buffer* buf;
	
	public:
	packet* pack;
	virtual void body();
	arrival4(double Time, buffer* buf1);
};

class service4: public event{

	buffer* buf;
	public:
	virtual void body();
	service4(double Time, buffer* buf1): event(Time){
		buf = buf1;
	}
};

inline arrival4::arrival4(double Time, buffer* buf1): event(Time){
	buf = buf1;
	pack = nullptr;
}
#endif

