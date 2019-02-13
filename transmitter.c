#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <stdint.h>
#include <stdlib.h>
#include "CC1100-CC2500.h"
#include "common.h"

#define SS_PIN   10
#define GDO2      3

void transmit_packet()
{
    printf("transmit_packet\n");
    spi_write_strobe(TI_CCxxx0_SIDLE);
    spi_write_strobe(TI_CCxxx0_SFTX);
    uint8_t data[5];
    data[0] = 5;
    data[1] = 1;
    data[2] = 2;
    data[3] = 3;
    data[4] = 4;
    spi_write_burst(TXFIFO_BURST, data, 5);
    spi_write_strobe(TI_CCxxx0_STX);
}

int main(int argc, char **argv)
{
    setup_spi();
    setup_gpio();
    uint8_t partnum = spi_read_register(TI_CCxxx0_PARTNUM);
    uint8_t version = spi_read_register(TI_CCxxx0_VERSION);

    printf("partnumber: 0x%02X, version: 0x%02X\r\n", partnum, version);

    if (!test_read_write_register()) {
        printf("could not read and write spi");
        exit(EXIT_FAILURE);
    }
    reset();

    init_cc_2500();

    while (1) {
        transmit_packet();

        delayMicroseconds(1000000);
    }
}
