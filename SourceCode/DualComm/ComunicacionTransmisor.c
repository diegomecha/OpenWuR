#include <stdlib.h>
#include <stdio.h>

#include "contiki.h"
#include "net/rime/rime.h"

#include "lib/list.h"
#include "lib/memb.h"

#include "dev/button-sensor.h"
#include "dev/leds.h"

#include "net/netstack.h"
#include "dev/gpio.h"

#include "cc1200-conf.h"
#include "cc1200-const.h"
#include "cc1200-arch.h"
#include "cc1200-rf.h"
#include "spi.h"

#define MAX_RETRANSMISSIONS 4
#define NUM_HISTORY_ENTRIES 4

#define BUFFER_SIZE   65 // Define el Payload

#define EXAMPLE_PORT_BASE  GPIO_PORT_TO_BASE(GPIO_A_NUM)
#define EXAMPLE_PIN_MASK   GPIO_PIN_MASK(2)
#define EXAMPLE_PORT_BASE2  GPIO_PORT_TO_BASE(GPIO_A_NUM)
#define EXAMPLE_PIN_MASK2   GPIO_PIN_MASK(5)

/*---------------------------------------------------------------------------*/
PROCESS(test_runicast_process, "runicast test");
AUTOSTART_PROCESSES(&test_runicast_process);
/*---------------------------------------------------------------------------*/

int enviado = 2;
int enviado2 = 2;
int recibido = 0;
char *datos ="";

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

uint8_t buffer[BUFFER_SIZE];
/*---------------------------------------------------------------------------*/

void SendPing() {

  // ENVIO DATO 1111111111111 01010101 10010110 01101001

// Carrier burst
  buffer[0] = 0x00;
  buffer[1] = 0x00;
  buffer[2] = 0x00;
  buffer[3] = 0x00;
  buffer[4] = 0x00;
  buffer[5] = 0x00;
  buffer[6] = 0x00;
  buffer[7] = 0x00;
  buffer[8] = 0x00;
  buffer[9] = 0x00;

  buffer[10] = 0xAA;
  buffer[11] = 0xAA;
  buffer[12] = 0xAA;
  buffer[13] = 0xAA;
  buffer[14] = 0xAA;
  buffer[15] = 0xAA;
  buffer[16] = 0xAA;
  buffer[17] = 0xAA;
  buffer[18] = 0xAA;
  buffer[19] = 0xAA;
  buffer[20] = 0xAA;
  buffer[21] = 0xAA;
  buffer[22] = 0xAA;
  buffer[23] = 0xAA;
  buffer[24] = 0xAA;
  buffer[25] = 0xAA;

  buffer[26] = 0x00;
  buffer[27] = 0xAA;
  buffer[28] = 0x00;
  buffer[29] = 0xAA;
  buffer[30] = 0x00;
  buffer[31] = 0xAA;
  buffer[32] = 0x00;
  buffer[33] = 0xAA;
  buffer[34] = 0x00;
  buffer[35] = 0xAA;
  buffer[36] = 0x00;
  buffer[37] = 0xAA;
  buffer[38] = 0x00;

  buffer[39] = 0xAA;
  buffer[40] = 0x00;
  buffer[41] = 0x00;
  buffer[42] = 0xAA;
  buffer[43] = 0x00;
  buffer[44] = 0xAA;
  buffer[45] = 0xAA;
  buffer[46] = 0x00;

  buffer[47] = 0x00;
  buffer[48] = 0xAA;
  buffer[49] = 0xAA;
  buffer[50] = 0x00;
  buffer[51] = 0xAA;
  buffer[52] = 0x00;
  buffer[53] = 0x00;
  buffer[54] = 0xAA;

  buffer[55] = 0x00;
  buffer[56] = 0x00;
  buffer[57] = 0x00;
  buffer[58] = 0x00;
  buffer[59] = 0x00;
  buffer[60] = 0x00;
  buffer[61] = 0x00;
  buffer[62] = 0x00;
  buffer[63] = 0x00;
  buffer[64] = 0x00;


  cc1200_driver.send(buffer, 65);
}

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
      printf("Unicast mensaje recibido: %s, de %d.%d, secuencia %d (DUPLICATE)\n",
	     packetbuf_dataptr(), from->u8[0], from->u8[1], seqno);
      return;
    }
    /* Update existing history entry */
    e->seq = seqno;
  }

  printf("Unicast mensaje recibido: %s, de %d.%d, secuencia %d\n",
	(packetbuf_dataptr()),from->u8[0], from->u8[1], seqno);

}
static void
sent_runicast(struct runicast_conn *c, const linkaddr_t *to, uint8_t retransmissions)
{
  printf("Unicast mensaje enviado a: %d.%d, retransmisiones %d\n",
	 to->u8[0], to->u8[1], retransmissions);
   enviado = 1;
}
static void
timedout_runicast(struct runicast_conn *c, const linkaddr_t *to, uint8_t retransmissions)
{
  printf("Unicast mensaje timed out cuando enviaba a: %d.%d, retransmissiones %d\n",
	 to->u8[0], to->u8[1], retransmissions);
}
static const struct runicast_callbacks runicast_callbacks = {recv_runicast,
							     sent_runicast,
							     timedout_runicast};
static struct runicast_conn runicast;
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(test_runicast_process, ev, data)
{
  PROCESS_EXITHANDLER(runicast_close(&runicast);)

  PROCESS_BEGIN();

  /* Disable the radio duty cycle and keep the radio on */
  NETSTACK_MAC.off(1);

  /* Activamos el pin de encendido*/
  /*GPIO_SOFTWARE_CONTROL(EXAMPLE_PORT_BASE, EXAMPLE_PIN_MASK);


  /* Configuramos como salida, inicia HIGH */
  /*GPIO_SET_OUTPUT(EXAMPLE_PORT_BASE, EXAMPLE_PIN_MASK);
  GPIO_CLR_PIN(EXAMPLE_PORT_BASE, EXAMPLE_PIN_MASK);*/

  /* Activamos el pin de encendido*/
  GPIO_SOFTWARE_CONTROL(EXAMPLE_PORT_BASE2, EXAMPLE_PIN_MASK2);


  /* Configuramos como salida, inicia HIGH */
  GPIO_SET_OUTPUT(EXAMPLE_PORT_BASE2, EXAMPLE_PIN_MASK2);
  GPIO_SET_PIN(EXAMPLE_PORT_BASE2, EXAMPLE_PIN_MASK2);


  runicast_open(&runicast, 144, &runicast_callbacks);

  /* OPTIONAL: Sender history */
  list_init(history_table);
  memb_init(&history_mem);

  cc1200_driver.init();
	// Inicio cc1200
	cc1200_driver.on();

  /* Receiver node: do nothing */
  if((linkaddr_node_addr.u8[0] == 0) &&
     linkaddr_node_addr.u8[1] == 2) {
    PROCESS_WAIT_EVENT_UNTIL(0);
  }

  /* Start the user button using the "SENSORS_ACTIVATE" macro */
  SENSORS_ACTIVATE(button_sensor);

  while(1) {

    static struct etimer et;

    //PROCESS_YIELD();

    etimer_set(&et, 0.01*CLOCK_SECOND);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    PROCESS_WAIT_EVENT_UNTIL(ev == sensors_event &&
                            data == &button_sensor);

    if(ev == sensors_event) {
      if( data == &button_sensor) {
          etimer_set(&et, 0.1*CLOCK_SECOND);

		    	SendPing();

		       enviado = 0;
          recibido = 0;
      }
    }


    if(enviado == 0){
      if(!runicast_is_transmitting(&runicast)) {
        linkaddr_t recv;
        packetbuf_copyfrom("Felix2.4", 8);
        recv.u8[0] = 0;
        recv.u8[1] = 2;

        /*printf("%u.%u: Enviando unicast a: %u.%u\n",
  	     linkaddr_node_addr.u8[0],
  	     linkaddr_node_addr.u8[1],
  	     recv.u8[0],
  	     recv.u8[1]);*/

        runicast_send(&runicast, &recv, MAX_RETRANSMISSIONS);

      }
    }
    if(enviado==1){

      GPIO_CLR_PIN(EXAMPLE_PORT_BASE2, EXAMPLE_PIN_MASK2);
      printf("apagando\n");
      // Apagamos el nodo sensor
      //GPIO_CLR_PIN(EXAMPLE_PORT_BASE, EXAMPLE_PIN_MASK);
      //enviado = 0;

    }
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
