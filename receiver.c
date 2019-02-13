#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <stdint.h>
#include <stdlib.h>

#include "CC1100-CC2500.h"
#include "common.h"

#define SS_PIN   10
#define GDO2      3

void flush_rx()
{
    // Make sure that the radio is in IDLE state before flushing the FIFO
    // (Unless RXOFF_MODE has been changed, the radio should be in IDLE state at this point)
    delay(10);
    spi_write_strobe(TI_CCxxx0_SIDLE);
    delay(10);
    // Flush RX FIFO
    spi_write_strobe(TI_CCxxx0_SFRX);
    delay(10);
}

void rx_data_rf()
{
    printf("rx_data_rf\r\n");
    int packet_length;
    // RX: enable RX
    spi_write_strobe(TI_CCxxx0_SRX);

    int gdo2_state;
    // Wait for GDO0 to be set -> sync received
    int count = 0;

    do {
        // read the state of the GDO0_PIN value:
        gdo2_state = digitalRead(GDO2);
        //Serial.println("GD0 = 0");
        delay(1);
        count++;

        if (count > 1000) {
            flush_rx();
            printf("ERR NO DATA\r\n");
            return;
        }
    }
    while (!gdo2_state);

    // Wait for GDO0 to be cleared -> end of packet
    while (gdo2_state) {
        // read the state of the GDO0_PIN value:
        gdo2_state = digitalRead(GDO2);
        delay(100);
    }
    // Read length byte
    packet_length = spi_read_register(TI_CCxxx0_RXFIFO);
    printf("Packet length: %d", packet_length);

    for (int i = 0; i < packet_length - 1; i++) {
        printf(", byte: %d: 0x%02X", i, spi_read_register(TI_CCxxx0_RXFIFO));
    }
    printf("\r\n");
    // Make sure that the radio is in IDLE state before flushing the FIFO
    // (Unless RXOFF_MODE has been changed, the radio should be in IDLE state at this point)
    spi_write_strobe(TI_CCxxx0_SIDLE);
    // Flush RX FIFO
    spi_write_strobe(TI_CCxxx0_SFRX);
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
        rx_data_rf();
    }
}
