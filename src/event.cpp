/***********************************************************************
		EVENT.C
***********************************************************************/

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

    // generation of next arrival
    double esito;
    GEN_EXP(SEED, inter, esito);
    ev = new arrival(time + esito, buf);
    cal->pushAndReorder(ev);

    // pushAndReorder the new packet in the queue
    packet *pack = new packet(time);
    // if some packet is already in the buffer, just pushAndReorder the new one
    if (buf->full() || buf->status) {
        buf->push(pack);
    }
        // otherwise let the packet pop in the service
    else {
        buf->tot_packs += 1.0;
        delete pack;
        GEN_EXP(SEED, length, esito);
        ev = new service(time + esito, buf);
        cal->pushAndReorder(ev);
        buf->status = 1;
    }



}

void service::body() {
    double esito;
    //Genero la dimensione del pacchetto come esponenziale
    GEN_EXP(SEED, length, esito);
    //Variabili di supporto per il calcolo delle probabilità
    double coin, coin2;
    //Recupero una VC casuale uniforme e la salvo in coin.
    PSEUDO(SEED, coin);
    //E scelgo da che link passa al primo giro (se 1->2 o 1->3)
    int node = (coin <= probabilities[0] ? 1 : 2);
    //Recupero un'altra VC
    PSEUDO(SEED, coin2);
    //E calcolo la probabilità di reinserimento in coda
    if (coin2 <= probabilities[node]){
        //Se questa è inferiore, allora entro nel branch di reinserimento pacchetto...
        packet *pack= buf->pop();
        event *ev;
        //Devo verificare che il pacchetto non abbia già una sua dimensione salvata, in caso sia un pacchetto
        //già rientrato precedentemente.
        if (pack != nullptr) {
            if(pack->getLength() != 0){
                //Se lo è, sostituisco la VC exp con la sua dimensione salvata.
                esito = pack->getLength();
            } else {
                //Nell'altro caso lo salvo, visto che questo pacchetto rientrerà sicuramente.
                pack->setLength(esito);
            }
            //Sommo il tempo di uscita più il tempo impiegato dai nodi selezionati per calcolare correttamente
            //il tempo di arrivo in coda.
            ev = new service(time + esito/capacities[0] + esito/capacities[node], buf);
            //Lo schedulo...
            cal->pushAndReorder(ev);
            //Imposto il tempo a quello attuale, visto che la schedulazione è basata sul tempo di ingresso dei pacchetti.
            pack->set_time(time);
            //Salvo il tempo già passato sui link per sommarlo in uscita.
            pack->add_elapsed_time((esito / capacities[0] + esito/capacities[node]));
            //Lo riaggiungo in coda...
            buf->push(pack);
            //E incremento di due pacchetti quelli processati (il nuovo già in coda e quello appena aggiunto)
            buf->tot_packs += 2.0;
        } else
            buf->status = 0;
    } else {
        //In caso contrario, il pacchetto esce dalla rete...
        packet *pack= buf->pop();
        event *ev;
        //Se il pacchetto ha una dimensione, la recupero e la sostituisco come sopra. Qua non serve
        //salvare, visto che il pacchetto esce comunque.
        if (pack != nullptr){
            if(pack->getLength() != 0){
                esito = pack->getLength();
            }
            //Sommo il tempo attuale del giro corrente...
            double dummyTime = esito/capacities[0] + esito/capacities[node] + esito/capacities[3];
            //Riaccodo un pacchetto nuovo considerando solo il primo link
            ev = new service(time + (esito/capacities[0]), buf);
            cal->pushAndReorder(ev);
            //E calcolo il delay usando il tempo attuale, più il tempo del percorso scelto attualmente,
            //Più l'eventuale già salvato nel pacchetto.
            buf->tot_delay += time+dummyTime+pack->get_elapsed_time() - pack->get_time();
            //Solo un pacchetto aggiuntivo in coda in questo caso.
            buf->tot_packs += 1.0;
            //E cancello il pacchetto uscito dalla coda.
            delete pack;
        } else
            buf->status = 0;
    }
    // printf("ingresso             %f\n", time);

}