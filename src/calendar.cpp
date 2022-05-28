/* -*- C++ -*- */
/*******************************************************
		CALENDAR C
*******************************************************/

#include "../inc/global.h"
#include "../inc/calendar.h"


event *calendar::pop() {
    if (head == nullptr)
        return nullptr;
    event *ev;
    if (head == last) {
        ev = head;
        head = nullptr;
        last = nullptr;
        return ev;
    }
    ev = head;
    head = head->next;
    last->next = head;
    return ev;
}

void calendar::pushAndReorder(event *New) {
    event *temp = head;
    if (head == nullptr) {
        head = New;
        head->next = New;
        last = New;
    } else if (New->time < head->time) {
        New->next = head;
        head = New;
        last->next = head;
    } else if (last == head) {
        if (New->time < head->time) {
            head = New;
            head->next = last;
            last->next = head;
        } else {
            last = New;
            head->next = last;
            last->next = head;
        }
    } else if (last->time < New->time) {
        last->next = New;
        last = New;
        last->next = head;
    } else {
        while (temp->next->time < New->time)
            temp = temp->next;
        New->next = temp->next;
        temp->next = New;
    }
}

