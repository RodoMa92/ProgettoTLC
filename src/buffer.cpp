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

void buffer::push(packet *pack) {
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

/**
 * Estrae un pacchetto dalla coda e lo ritorna
 * @return Il pacchetto rimosso dalla coda.
 */
packet *buffer::pop() {
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

/**
 * Ritorna la testa della coda SENZA rimuoverla dalla coda.
 * NON cancellare il puntatore con delete.
 * @return Il primo pacchetto all'interno della lista.
 */
packet *buffer::get(){
    packet *pack;
    if (head == nullptr)
        return nullptr;
    else return head;
}
	
