#include "spi-arch.h"
#include "spi.h"

#include "contiki.h"
#include <stdio.h>
#include "dev/leds.h"
#include "sys/etimer.h"
#if CONTIKI_TARGET_ZOUL
#include "dev/gpio.h"
#include "dev/ioc.h"
#endif

#define pin 3
#define SPI_SEL_PORT_BASE GPIO_A_NUM
#define SPI_SEL_PIN_MASK  GPIO_PIN_MASK(pin)

#define EXAMPLE_PORT_BASE  GPIO_PORT_TO_BASE(GPIO_A_NUM)
#define EXAMPLE_PIN_MASK   GPIO_PIN_MASK(5)

PROCESS(test_spi_process, "Test SPI");
AUTOSTART_PROCESSES(&test_spi_process);

static struct etimer et1;
static struct etimer et2;
static struct etimer et3;
static struct etimer et4;
static struct etimer et5;
static struct etimer et6;


uint8_t instance = 1;
uint32_t frecuency1 = 1000000;
uint32_t frecuency2 = 31500;
int bandera = 0;
static uint8_t registro;



PROCESS_THREAD(test_spi_process, ev, data)
{
  PROCESS_BEGIN();

  spix_init(1);
  spix_cs_init(SPI_SEL_PORT_BASE,pin);
  spix_set_mode(1, SSI_CR0_FRF_MOTOROLA, 0, SSI_CR0_SPH, 8);
  spix_set_clock_freq(instance,frecuency1);

  etimer_set(&et1, CLOCK_SECOND * 0.1);
  etimer_set(&et2, CLOCK_SECOND * 0.1);
  etimer_set(&et3, CLOCK_SECOND * 0.1);
  etimer_set(&et4, CLOCK_SECOND * 0.1);
  etimer_set(&et5, CLOCK_SECOND * 0.1);
  etimer_set(&et6, CLOCK_SECOND * 0.5);

  /*GPIO_SOFTWARE_CONTROL(EXAMPLE_PORT_BASE, EXAMPLE_PIN_MASK);
  /* And set as output, starting low
  ioc_set_over(GPIO_A_NUM, 5, IOC_OVERRIDE_DIS);
  GPIO_SET_OUTPUT(EXAMPLE_PORT_BASE, EXAMPLE_PIN_MASK);
  GPIO_CLR_PIN(EXAMPLE_PORT_BASE,EXAMPLE_PIN_MASK);*/

  while(1) {
    //envio dato calibracion reloj

    //GPIO_SET_PIN(EXAMPLE_PORT_BASE,EXAMPLE_PIN_MASK);
    SPIX_CS_SET(SPI_SEL_PORT_BASE,pin);

    spix_set_mode(1, SSI_CR0_FRF_MOTOROLA, 0, SSI_CR0_SPH, 8);
    SPI_WRITE(0xc2);
    spix_set_mode(1, SSI_CR0_FRF_MOTOROLA, 0, SSI_CR0_SPH, 16);
    spix_set_clock_freq(instance,frecuency2);
    SPI_WRITE(0x0000);
    //SPI_WRITE_FAST(0x0000);
    //SPI_WRITE(0x0000);

    SPIX_CS_CLR(SPI_SEL_PORT_BASE,pin);

    spix_set_mode(1, SSI_CR0_FRF_MOTOROLA, 0, SSI_CR0_SPH, 16);

    SPIX_CS_SET(SPI_SEL_PORT_BASE,pin);


    SPI_WRITE(0x0122);


    SPIX_CS_CLR(SPI_SEL_PORT_BASE,pin);


  //  PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et1));



    //envio dato banda de operacion
    spix_set_mode(1, SSI_CR0_FRF_MOTOROLA, 0, SSI_CR0_SPH, 16);
    spix_set_clock_freq(instance,frecuency1);


    //PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et2));
    // dato un solo canal activo 1
    SPIX_CS_SET(SPI_SEL_PORT_BASE,pin);


    SPI_WRITE(0x0002);


    SPIX_CS_CLR(SPI_SEL_PORT_BASE,pin);


    //PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et3));
    spix_set_mode(1, SSI_CR0_FRF_MOTOROLA, 0, SSI_CR0_SPH, 16);
    // dato un time-out 100ms y duracion de bit
    SPIX_CS_SET(SPI_SEL_PORT_BASE,pin);


    SPI_WRITE(0x0746);


    SPIX_CS_CLR(SPI_SEL_PORT_BASE,pin);

    //PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et4));
    spix_set_mode(1, SSI_CR0_FRF_MOTOROLA, 0, SSI_CR0_SPH, 16);
    // dato cambio de canal 15-23khz
    SPIX_CS_SET(SPI_SEL_PORT_BASE,pin);


    SPI_WRITE(0x08E0);


    SPIX_CS_CLR(SPI_SEL_PORT_BASE,pin);

    spix_set_mode(1, SSI_CR0_FRF_MOTOROLA, 0, SSI_CR0_SPH, 16);
    // dato cambio de canal 15-23khz
    SPIX_CS_SET(SPI_SEL_PORT_BASE,pin);


    SPI_WRITE(0x0320);


    SPIX_CS_CLR(SPI_SEL_PORT_BASE,pin);

    //PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et5));*/

    spix_set_mode(1, SSI_CR0_FRF_MOTOROLA, 0, SSI_CR0_SPH, 16);

    SPIX_CS_SET(SPI_SEL_PORT_BASE,pin);


    SPI_WRITE(0x4000);
    //SPI_READ(registro);


    SPIX_CS_CLR(SPI_SEL_PORT_BASE,pin);

    spix_set_mode(1, SSI_CR0_FRF_MOTOROLA, 0, SSI_CR0_SPH, 16);

    SPIX_CS_SET(SPI_SEL_PORT_BASE,pin);


    SPI_WRITE(0x4100);
    //SPI_READ(registro);


    SPIX_CS_CLR(SPI_SEL_PORT_BASE,pin);


    spix_set_mode(1, SSI_CR0_FRF_MOTOROLA, 0, SSI_CR0_SPH, 16);

    SPIX_CS_SET(SPI_SEL_PORT_BASE,pin);


    SPI_WRITE(0x4300);
    //SPI_READ(registro);


    SPIX_CS_CLR(SPI_SEL_PORT_BASE,pin);

    //printf("valor spi read 0x%x\n\r", registro);




    // introducir r7 nuevo 100ms

    // Espera tiempo y se apaga
    printf("ENVIO DATO\n");
    leds_off(LEDS_ALL);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et6));
    //GPIO_SET_PIN(EXAMPLE_PORT_BASE,EXAMPLE_PIN_MASK);
    /*etimer_reset(&et1);
    etimer_reset(&et2);
    etimer_reset(&et3);
    etimer_reset(&et4);
    etimer_reset(&et5);*/
    etimer_reset(&et6);
    leds_on(LEDS_ALL);

    bandera = 1;

  }

  PROCESS_END();
}
