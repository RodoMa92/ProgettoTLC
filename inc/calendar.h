/*******************************************************
		     CALENDAR H
*******************************************************/


#ifndef _CALENDAR_H
#define _CALENDAR_H

#include "simulator.h"
#include "event.h"

class calendar{

	event* 	head;
	event*	last;

	public:

	calendar();
	~calendar();
	event* 	pop();
	void	pushAndReorder(event* New);
	};


inline	calendar::calendar(){
	head=nullptr;
	last=nullptr;
	}
inline	calendar::~calendar(){
	event* temp=head;
	last->next=nullptr;
	while(temp!=nullptr){
		temp=temp->next;
		delete head;
		head=temp;
		}
	}

#endif

