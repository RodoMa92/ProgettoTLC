/***********************************************************************
		EVENT.C
***********************************************************************/

// #define debug

#include "../inc/event.h"
#include "../inc/buffer.h"
#include "../inc/calendar.h"
#include "../inc/rand.h"

extern calendar *cal;
extern double inter;
extern double length;
extern double capacities[4];
extern double probabilities[3];

void arrival::body() {
	event *ev;
	double esito;
    // Generation of next arrival
    GEN_EXP(SEED, inter, esito);
    ev = new arrival(time + esito, buf, buf_left, buf_right, buf_last);
    cal->pushAndReorder(ev);
    // Generation of packet
    packet *pack;
    if (this->pack != nullptr) {
    	pack = this->pack;
	} else {
		pack = new packet(time);
        pack->setNode(1);
	}
    // If some packet is already in the buffer, just push
    if (buf->full() || buf->status) {
        buf->push(pack);
    }
    // Otherwise let the packet pop in the service
    else {
    	if (pack->getLength() != 0) {
    		esito = pack->getLength();
		} else {
			//Genero la dimensione del pacchetto come esponenziale
	    	GEN_EXP(SEED, length, esito);
            pack->setLength(esito);
		}
	    //Variabile di supporto per il calcolo delle probabilit�
	    double coin;
	    //Recupero una VC casuale uniforme e la salvo in coin.
	    PSEUDO(SEED, coin);
		arrival23* ev2;
		ev = new service(time+esito/capacities[0], buf, buf_left, buf_right, buf_last);
		int node = coin<=probabilities[0] ? 2 : 3;
		pack->setNode(node);
		if (node == 2) {
			ev2 = new arrival23(time+esito/capacities[0], buf_left, buf, buf_right, buf_last);
		} else {
			ev2 = new arrival23(time+esito/capacities[0], buf_right, buf_left, buf, buf_last);
		}
		ev2->pack = pack;
		
		cal->pushAndReorder(ev);
		cal->pushAndReorder(ev2);
		buf->tot_packs+=1.0;
		buf->status=1;
    }
}

void service::body() {
    double esito;
    //Variabili di supporto per il calcolo delle probabilità
    double coin;
    //Recupero una VC casuale uniforme e la salvo in coin.
    PSEUDO(SEED, coin);
    packet* pack;
	pack=buf->pop();
	event* ev;
	arrival23* ev2;
	if(pack!=nullptr){
		if (!pack->getLength()) {
			//Genero la dimensione del pacchetto come esponenziale
    		GEN_EXP(SEED, length, esito);
			pack->setLength(esito);
		} else {
			esito = pack->getLength();
		}
		ev = new service(time+esito/capacities[0], buf, buf_left, buf_right, buf_last);
		int node = coin<=probabilities[0] ? 2 : 3;
		pack->setNode(node);
		if (node == 2) {
			ev2 = new arrival23(time+esito/capacities[0], buf_left, buf, buf_right, buf_last);
		} else {
			ev2 = new arrival23(time+esito/capacities[0], buf_right, buf_left, buf, buf_last);
		}
		ev2->pack = pack;
		cal->pushAndReorder(ev);
		cal->pushAndReorder(ev2);
		buf->tot_packs+=1.0;
	} else
		buf->status=0;
}

void arrival23::body() {
	event *ev;
	double esito;
    // Generation of packet or not if it exists already
    packet *pack = this->pack;
    // If some packet is already in the buffer, just push
    if (buf->full() || buf->status) {
        buf->push(pack);
    }
    // Otherwise let the packet pop in the service
    else {
        esito = pack->getLength();
        //Variabile di supporto per il calcolo delle probabilit�
        double coin;
        //Recupero una VC casuale uniforme e la salvo in coin.
        PSEUDO(SEED, coin);
        event *ev2;
        int currentNode = pack->getNode();
        ev = new service23(time + esito / capacities[currentNode - 1], buf, buf_left, buf_right, buf_last);
        int node = coin <= probabilities[currentNode-1] ? 1 : 4;
        pack->setNode(node);
        if (node == 1) {
            if (currentNode == 2) {
                ev2 = new arrival(time + esito / capacities[currentNode - 1], buf_left, buf, buf_right, buf_last);
            } else {
                ev2 = new arrival(time + esito / capacities[currentNode - 1], buf_right, buf_left, buf, buf_last);
            }
            ((arrival *) ev2)->pack = pack;
        } else {
            ev2 = new arrival4(time + esito / capacities[currentNode - 1], buf_last);
            ((arrival4 *) ev2)->pack = pack;
        }

        cal->pushAndReorder(ev);
        cal->pushAndReorder(ev2);
        buf->tot_packs += 1.0;
        buf->status = 1;
    }
}

void service23::body() {
    double esito;
    //Genero la dimensione del pacchetto come esponenziale
    GEN_EXP(SEED, length, esito);
    //Variabili di supporto per il calcolo delle probabilità
    double coin;
    //Recupero una VC casuale uniforme e la salvo in coin.
    PSEUDO(SEED, coin);
    packet* pack;
	pack=buf->pop();
	event* ev, *ev2;
	if(pack!=nullptr){
		esito = pack->getLength();
	    //Variabile di supporto per il calcolo delle probabilit�
	    double coin;
	    //Recupero una VC casuale uniforme e la salvo in coin.
	    PSEUDO(SEED, coin);
		int currentNode = pack->getNode();
		ev = new service23(time+esito/capacities[currentNode-1], buf, buf_left, buf_right, buf_last);
		int node = coin<=probabilities[currentNode-1] ? 1 : 4;
		pack->setNode(node);
		if (node == 1) {
			if (currentNode == 2) {
				ev2 = new arrival(time+esito/capacities[currentNode-1], buf_left, buf, buf_right, buf_last);
			} else {
				ev2 = new arrival(time+esito/capacities[currentNode-1], buf_right, buf_left, buf, buf_last);
			}
			((arrival*)ev2)->pack = pack;
		} else {
			ev2 = new arrival4(time+esito/capacities[currentNode-1], buf_last);
			((arrival4*)ev2)->pack = pack;
		}
		
		cal->pushAndReorder(ev);
		cal->pushAndReorder(ev2);
		buf->tot_packs+=1.0;
		}
	else
		buf->status=0;
}

void arrival4::body() {
	event* ev;
	packet *pack = this->pack;
	if (buf->full() || buf->status) {
        buf->push(pack);
    } else {
    	double esito = pack->getLength();
		ev = new service4(time+esito/capacities[3], buf);
		cal->pushAndReorder(ev);
		buf->tot_delay += time+esito/capacities[3]-pack->get_time();
#ifdef debug
        pack->getPath();
        printf("Length: %f\n", pack->getLength());
        printf("Delay: %f\n", buf->tot_delay);
#endif
		buf->tot_packs += 1.0;
		buf->status = 1;
		delete pack;
	}
}
void service4::body() {
	packet* pack;
	pack=buf->pop();
	event* ev;
	double esito;
	if (pack != nullptr) {
		esito = pack->getLength();
		ev = new service4(time+esito/capacities[3], buf);
		cal->pushAndReorder(ev);
		buf->tot_delay += time+esito/capacities[3]-pack->get_time();
#ifdef debug
        pack->getPath();
        printf("Length: %f\n", pack->getLength());
        printf("Delay: %f\n", buf->tot_delay);
#endif
		buf->tot_packs+=1.0;
		delete pack;
	} else {
		buf->status = 0;
	}
}