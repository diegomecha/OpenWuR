#include <stdlib.h>
#include <stdio.h>

#include "contiki.h"
#include "net/rime/rime.h"

#include "lib/list.h"
#include "lib/memb.h"

#include "dev/button-sensor.h"
#include "dev/leds.h"

#include "net/netstack.h"

#define MAX_RETRANSMISSIONS 4
#define NUM_HISTORY_ENTRIES 4

#define EXAMPLE_PORT_BASE  GPIO_PORT_TO_BASE(GPIO_A_NUM)
#define EXAMPLE_PIN_MASK   GPIO_PIN_MASK(5)


/*---------------------------------------------------------------------------*/
PROCESS(test_runicast_process, "runicast test");
AUTOSTART_PROCESSES(&test_runicast_process);
/*---------------------------------------------------------------------------*/
int enviado = 0;
int enviado1 = 0;
int enviado2 = 0;
int recibido = 0;
int recibido2 = 0;
char *datos ="";
int cont = 0;

/* OPTIONAL: Sender history.
 * Detects duplicate callbacks at receiving nodes.
 * Duplicates appear when ack messages are lost. */
struct history_entry {
  struct history_entry *next;
  linkaddr_t addr;
  uint8_t seq;
};
LIST(history_table);
MEMB(history_mem, struct history_entry, NUM_HISTORY_ENTRIES);
/*---------------------------------------------------------------------------*/
static void
recv_runicast(struct runicast_conn *c, const linkaddr_t *from, uint8_t seqno)
{
  /* OPTIONAL: Sender history */
  struct history_entry *e = NULL;
  for(e = list_head(history_table); e != NULL; e = e->next) {
    if(linkaddr_cmp(&e->addr, from)) {
      break;
    }
  }
  if(e == NULL) {
    /* Create new history entry */
    e = memb_alloc(&history_mem);
    if(e == NULL) {
      e = list_chop(history_table); /* Remove oldest at full history */
    }
    linkaddr_copy(&e->addr, from);
    e->seq = seqno;
    list_push(history_table, e);
  } else {
    /* Detect duplicate callback */
    if(e->seq == seqno) {

      uint16_t i;
    uint16_t len = packetbuf_datalen();
    char cadena[]="";
    for(i = 0; i < len; i++) {
      cadena[i]=((char *)packetbuf_dataptr())[i];
    }

      printf("Mensaje recibido: %s, de %d.%d, secuencia %d (DUPLICATE)\n",
	     cadena, from->u8[0], from->u8[1], seqno);
      return;
    }
    /* Update existing history entry */
    e->seq = seqno;
  }

      uint16_t i;
    uint16_t len = packetbuf_datalen();
    char cadena[]="";
    for(i = 0; i < len; i++) {
      cadena[i]=((char *)packetbuf_dataptr())[i];
    }

    printf("Mensaje recibido: %s, de %d.%d, secuencia %d\n",
    cadena,from->u8[0], from->u8[1], seqno);

   recibido = 1;
   enviado = 0;
   cont=0;

   /*
    if(strcmp(datos, "ACK2")==0 ){recibido2=1;}

    if(recibido2==0){enviado=0;}
    */
  //printf("runicast message received is: %d \n",&*c);
  //printf("runicast message received is: %s \n",&c.c);
  /*uint16_t i;
  uint16_t len = packetbuf_datalen();
    for(i = 0; i < len; i++) {
      printf("%c ", ((uint8_t *)packetbuf_dataptr())[i]);
    }*/
    //printf("%s\n",packetbuf_dataptr());
}
static void
sent_runicast(struct runicast_conn *c, const linkaddr_t *to, uint8_t retransmissions)
{
  printf("Mensaje enviado a: %d.%d, retransmisiones %d\n",
	 to->u8[0], to->u8[1], retransmissions);
   enviado = 1;

}
static void
timedout_runicast(struct runicast_conn *c, const linkaddr_t *to, uint8_t retransmissions)
{

   cont++;
   printf("Timed out de la transmision cuando enviaba a: %d.%d, retransmisiones %d\n",
 	 to->u8[0], to->u8[1], cont);
   if(cont >= 4){recibido=0;
     printf("El mensaje ha llegado al maximo de retransmisiones: %d. No fue posible transmitirlo. \n", cont);}
}
static const struct runicast_callbacks runicast_callbacks = {recv_runicast,
							     sent_runicast,
							     timedout_runicast};
static struct runicast_conn runicast;
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(test_runicast_process, ev, data)
{
  PROCESS_EXITHANDLER(runicast_close(&runicast);)

  /* Activamos el pin de encendido*/
  GPIO_SOFTWARE_CONTROL(EXAMPLE_PORT_BASE, EXAMPLE_PIN_MASK);

  /* Configuramos como salida, inicia HIGH */
  GPIO_SET_OUTPUT(EXAMPLE_PORT_BASE, EXAMPLE_PIN_MASK);
  GPIO_SET_PIN(EXAMPLE_PORT_BASE, EXAMPLE_PIN_MASK);

  PROCESS_BEGIN();

  /* Disable the radio duty cycle and keep the radio on */
  NETSTACK_MAC.off(1);

  runicast_open(&runicast, 144, &runicast_callbacks);

  /* OPTIONAL: Sender history */
  list_init(history_table);
  memb_init(&history_mem);

  /* Receiver node: do nothing */
  if((linkaddr_node_addr.u8[0] == 0  || linkaddr_node_addr.u8[0] == 0)&&
     (linkaddr_node_addr.u8[1] == 1 || linkaddr_node_addr.u8[1] == 2)) {
    PROCESS_WAIT_EVENT_UNTIL(0);
  }

  while(1) {
    static struct etimer et;

    etimer_set(&et, 0.01*CLOCK_SECOND);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));


    //printf("recibido %d\n", recibido);

    if(recibido==1){

          GPIO_CLR_PIN(EXAMPLE_PORT_BASE, EXAMPLE_PIN_MASK);
    }
    /*
    if(enviado == 0 && recibido ==1){
      if(!runicast_is_transmitting(&runicast)) {
        linkaddr_t recv;

        packetbuf_copyfrom("ACK", 3);
        recv.u8[0] = 0;
        recv.u8[1] = 1;

        printf("%u.%u: Enviando unicast a: %u.%u\n",
  	     linkaddr_node_addr.u8[0],
  	     linkaddr_node_addr.u8[1],
  	     recv.u8[0],
  	     recv.u8[1]);

        runicast_send(&runicast, &recv, MAX_RETRANSMISSIONS);
        enviado = 1;
      }
    }
    */
    /*
      if(enviado == 0 && recibido ==1){
        if(!runicast_is_transmitting(&runicast)) {
          linkaddr_t recv;

          packetbuf_copyfrom(datos, strlen(datos));
          recv.u8[0] = 0;
          recv.u8[1] = 3;

          printf("%u.%u: Enviando mensaje a: %u.%u\n",
    	     linkaddr_node_addr.u8[0],
    	     linkaddr_node_addr.u8[1],
    	     recv.u8[0],
    	     recv.u8[1]);

          enviado2 = 1;
          runicast_send(&runicast, &recv, MAX_RETRANSMISSIONS);
        }
      }
      */
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
