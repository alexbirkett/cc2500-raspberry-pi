#ifndef CC2500_SPI_COMMON_H
#define CC2500_SPI_COMMON_H

#include <stdint.h>

/*------------------------[CC1100 - FIFO commands]----------------------------*/
#define TXFIFO_BURST        0x7F    //write burst only
#define TXFIFO_SINGLE_BYTE  0x3F    //write single only
#define RXFIFO_BURST        0xFF    //read burst only
#define RXFIFO_SINGLE_BYTE  0xBF    //read single only
#define PATABLE_BURST       0x7E    //power control read/write
#define PATABLE_SINGLE_BYTE 0xFE    //power control read/write
/*---------------------------[END FIFO commands]------------------------------*/

void spi_write_reg(uint8_t spi_instr, uint8_t value);
uint8_t spi_read_register(uint8_t spi_instr);
void spi_write_burst(uint8_t spi_instr, uint8_t *pArr, uint8_t len);
void spi_write_strobe(uint8_t spi_instr);
void init_cc_2500();
void setup_spi();
void setup_gpio();
void reset();
uint8_t test_read_write_register();

#endif //CC2500_SPI_COMMON_H
