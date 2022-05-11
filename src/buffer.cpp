/***************************************************************************
			BUFFER.C
***************************************************************************/

#include "../inc/buffer.h"

buffer::buffer() {
    head = nullptr;
    last = nullptr;
    status = 0;
    tot_delay = 0.0;
    tot_packs = 0.0;
}

void buffer::insert(packet *pack) {
    if (head == nullptr) {
        head = pack;
        last = pack;
        last->next = head;
    } else {
        last->next = pack;
        last = pack;
        last->next = head;
    }
}

packet *buffer::get() {
    packet *pack;
    if (head == nullptr)
        return nullptr;
    if (last == head) {
        pack = head;
        last = nullptr;
        head = nullptr;
    } else {
        pack = head;
        head = head->next;
        last->next = head;
    }
    return pack;
}
	
