/***********************************************************************
		EVENT.C
***********************************************************************/

#include "../inc/event.h"
#include "../inc/buffer.h"
#include "../inc/calendar.h"
#include "../inc/rand.h"

extern calendar *cal;
extern double inter;
extern double packet_length;

void arrival::body() {
    event *ev;
    if (this->with_lambda) {
        // generation of next arrival
        double esito;
        GEN_EXP(SEED, inter, esito);
        ev = new arrival(time + esito, buf, this->myqueue);
        cal->put(ev);

        // insert the new packet in the queue
        packet *pack = new packet(time, esito);
        // if some packet is already in the buffer, just insert the new one
        if (buf->full() || buf->status) {
            buf->insert(pack);
        }
            // otherwise let the packet get in the service
        else {
            buf->tot_packs += 1.0;
            delete pack;
            GEN_EXP(SEED, packet_length, esito);
            ev = new service(time + esito, buf);
            cal->put(ev);
            buf->status = 1;
        }
    }
    double flip;
    PSEUDO(SEED, flip);
}

void service::body() {
    // printf("ingresso             %f\n", time);
    packet *pack;
    pack = buf->get();
    event *ev;
    double esito;
    GEN_EXP(SEED, packet_length, esito);
    if (pack != NULL) {
        ev = new service(time + esito, buf);
        cal->put(ev);
        buf->tot_delay += time - pack->get_time();
        // printf("%3.5f    %3.5f\n", time, time-pack->get_time());
        buf->tot_packs += 1.0;
        delete pack;
    } else
        buf->status = 0;
}

