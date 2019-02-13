#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <stdint.h>

#include "common.h"
#include "CC1100-CC2500.h"

#define SS_PIN   10
#define GDO2      3

void spi_write_reg(uint8_t spi_instr, uint8_t value)
{
    uint8_t tbuf[2] = {0};
    tbuf[0] = spi_instr; /*| CC2500_WRITE_SINGLE;*/
    tbuf[1] = value;
    uint8_t len = 2;
    wiringPiSPIDataRW(0, tbuf, len);
}

uint8_t spi_read_register(uint8_t spi_instr)
{
    uint8_t value;
    uint8_t rbuf[2] = {0};
    rbuf[0] = spi_instr | (uint8_t)TI_CCxxx0_READ_SINGLE;
    uint8_t len = 2;
    wiringPiSPIDataRW(0, rbuf, len);
    value = rbuf[1];
    return value;
}

void spi_write_burst(uint8_t spi_instr, uint8_t *pArr, uint8_t len)
{
    uint8_t tbuf[len + 1];
    tbuf[0] = spi_instr | (uint8_t)TI_CCxxx0_WRITE_BURST;
    for (uint8_t i = 0; i < len; i++) {
        tbuf[i + 1] = pArr[i];
    }
    wiringPiSPIDataRW(0, tbuf, len + 1);
}

void spi_read_burst(uint8_t spi_instr, uint8_t *pArr, uint8_t len)
{
    uint8_t rbuf[len + 1];
    rbuf[0] = spi_instr | (uint8_t)TI_CCxxx0_READ_BURST;
    wiringPiSPIDataRW(0, rbuf, len + 1);
    for (uint8_t i = 0; i < len; i++) {
        pArr[i] = rbuf[i + 1];
    }
}

void spi_write_strobe(uint8_t spi_instr)
{
    uint8_t tbuf[1] = {0};
    tbuf[0] = spi_instr;
    //printf ("SPI_data: 0x%02X\n", tbuf[0]);
    wiringPiSPIDataRW(0, tbuf, 1);
}

void init_cc_2500()
{
    spi_write_reg(TI_CCxxx0_IOCFG2, 0x06);
    spi_write_reg(TI_CCxxx0_IOCFG1, 0x2E);
    spi_write_reg(TI_CCxxx0_IOCFG0, 0x06);
    spi_write_reg(TI_CCxxx0_FIFOTHR, 0x07);
    spi_write_reg(TI_CCxxx0_SYNC1, 0xD3);
    spi_write_reg(TI_CCxxx0_SYNC0, 0x91);
    spi_write_reg(TI_CCxxx0_PKTLEN, 0xFF);
    spi_write_reg(TI_CCxxx0_PKTCTRL1, 0x04);
    spi_write_reg(TI_CCxxx0_PKTCTRL0, 0x05);
    spi_write_reg(TI_CCxxx0_ADDR, 0x00);
    spi_write_reg(TI_CCxxx0_CHANNR, 0x00);
    spi_write_reg(TI_CCxxx0_FSCTRL1, 0x08);
    spi_write_reg(TI_CCxxx0_FSCTRL0, 0x00);
    spi_write_reg(TI_CCxxx0_FREQ2, 0x5D);
    spi_write_reg(TI_CCxxx0_FREQ1, 0x93);
    spi_write_reg(TI_CCxxx0_FREQ0, 0xB1);
    spi_write_reg(TI_CCxxx0_MDMCFG4, 0x86);
    spi_write_reg(TI_CCxxx0_MDMCFG3, 0x83);
    spi_write_reg(TI_CCxxx0_MDMCFG2, 0x03);
    spi_write_reg(TI_CCxxx0_MDMCFG1, 0x22);
    spi_write_reg(TI_CCxxx0_MDMCFG0, 0xF8);
    spi_write_reg(TI_CCxxx0_DEVIATN, 0x44);
    spi_write_reg(TI_CCxxx0_MCSM2, 0x07);
    spi_write_reg(TI_CCxxx0_MCSM1, 0x30);
    spi_write_reg(TI_CCxxx0_MCSM0, 0x18);
    spi_write_reg(TI_CCxxx0_FOCCFG, 0x16);
    spi_write_reg(TI_CCxxx0_BSCFG, 0x6C);
    spi_write_reg(TI_CCxxx0_AGCCTRL2, 0x03);
    spi_write_reg(TI_CCxxx0_AGCCTRL1, 0x40);
    spi_write_reg(TI_CCxxx0_AGCCTRL0, 0x91);
    spi_write_reg(TI_CCxxx0_WOREVT1, 0x87);
    spi_write_reg(TI_CCxxx0_WOREVT0, 0x6B);
    spi_write_reg(TI_CCxxx0_WORCTRL, 0xF8);
    spi_write_reg(TI_CCxxx0_FREND1, 0x56);
    spi_write_reg(TI_CCxxx0_FREND0, 0x10);
    spi_write_reg(TI_CCxxx0_FSCAL3, 0xA9);
    spi_write_reg(TI_CCxxx0_FSCAL2, 0x0A);
    spi_write_reg(TI_CCxxx0_FSCAL1, 0x00);
    spi_write_reg(TI_CCxxx0_FSCAL0, 0x11);
    spi_write_reg(TI_CCxxx0_RCCTRL1, 0x41);
    spi_write_reg(TI_CCxxx0_RCCTRL0, 0x00);
    spi_write_reg(TI_CCxxx0_FSTEST, 0x59);
    spi_write_reg(TI_CCxxx0_PTEST, 0x7F);
    spi_write_reg(TI_CCxxx0_AGCTEST, 0x3F);
    spi_write_reg(TI_CCxxx0_TEST2, 0x88);
    spi_write_reg(TI_CCxxx0_TEST1, 0x31);
    spi_write_reg(TI_CCxxx0_TEST0, 0x0B);
}

void setup_spi()
{
    if (wiringPiSPISetup(0, 8000000) < 0)  //4MHz SPI speed
    {
        printf("ERROR: wiringPiSPISetup failed!\r\n");
    }
    else {
        printf("wiringSPI is up\r\n");
    }
}

void setup_gpio()
{
    wiringPiSetup();            //setup wiringPi library
    pinMode(GDO2, INPUT);
}

void reset()
{
    digitalWrite(SS_PIN, LOW);
    delayMicroseconds(10);
    digitalWrite(SS_PIN, HIGH);
    delayMicroseconds(40);
    spi_write_strobe(TI_CCxxx0_SRES);
    delay(1);
}

uint8_t test_read_write_register()
{
    // read value from random register
    uint8_t initial_value = spi_read_register(TI_CCxxx0_FIFOTHR);
    uint8_t test_value = initial_value + (uint8_t)1; // chose another value to test a write and read
    spi_write_reg(TI_CCxxx0_FIFOTHR, test_value); // write new value
    uint8_t success = spi_read_register(TI_CCxxx0_FIFOTHR) == test_value;
    spi_write_reg(TI_CCxxx0_FIFOTHR, initial_value); // write back inital value
    return success;
}

