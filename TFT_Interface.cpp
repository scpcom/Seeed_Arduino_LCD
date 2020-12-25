/**
    The MIT License (MIT)

    A interface abstraction layer

    Copyright (C) 2019  Seeed Technology Co.,Ltd.

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/
#include <TFT_Interface.h>

#ifdef K210_ST7789_SIPEED
#include <st7789.h>

#define SIPEED_ST7789_RST_PIN    37
#define SIPEED_ST7789_DCX_PIN    38
#define SIPEED_ST7789_SS_PIN     36
#define SIPEED_ST7789_SCLK_PIN   39

// default peripheral
#define SIPEED_ST7789_RST_GPIONUM  6
#define SIPEED_ST7789_DCX_GPIONUM  7
#define SIPEED_ST7789_SS           3
#endif

#ifdef HASSPI
TFT_Interface::TFT_Interface(SPIClass* spi) {
    this->_SPI = spi;
}
#else
TFT_Interface::TFT_Interface() {

}
#endif

TFT_Interface::~TFT_Interface() {

}

void TFT_Interface::begin() {
    #ifdef K210_ST7789_SIPEED
    uint8_t spiNum = this->_SPI->busId();
    uint32_t _freq = SPI_FREQUENCY;
    int8_t _dcxPin = SIPEED_ST7789_DCX_PIN;
    int8_t _rstPin = SIPEED_ST7789_RST_PIN;
    uint8_t _dmaCh = TFT_DMA_CH;
    if( (spi_id_t)spiNum == SPI0)
    {
        fpioa_set_function(SIPEED_ST7789_SS_PIN  , (fpioa_function_t)(FUNC_SPI0_SS0 + SIPEED_ST7789_SS));
        fpioa_set_function(SIPEED_ST7789_SCLK_PIN, (fpioa_function_t)FUNC_SPI0_SCLK);
    }
    else if((spi_id_t)spiNum == SPI1)
    {
        fpioa_set_function(SIPEED_ST7789_SS_PIN  , (fpioa_function_t)(FUNC_SPI1_SS0 + SIPEED_ST7789_SS));
        fpioa_set_function(SIPEED_ST7789_SCLK_PIN, (fpioa_function_t)FUNC_SPI1_SCLK);
    }
    sysctl_set_spi0_dvp_data(1);

    tft_hard_init(spiNum, SIPEED_ST7789_SS, SIPEED_ST7789_RST_GPIONUM, SIPEED_ST7789_DCX_GPIONUM, _freq, _rstPin,  _dcxPin, _dmaCh);
    #elif HASSPI
    this->_SPI->begin();
    #else
    interface_begin();
    #endif
    return;
}

void TFT_Interface::end() {
    #ifdef K210_ST7789_SIPEED
    // todo
    #elif HASSPI
    this->_SPI->end();
    #else
    interface_end();
    #endif
    return;
}


byte TFT_Interface::transfer(uint8_t data) {
    #ifdef K210_ST7789_SIPEED
    tft_write_byte(&data, 1);
    return data;
    #elif HASSPI
    return this->_SPI->transfer(data);
    #else
    return interface_transfer(data);
    #endif
}

uint16_t TFT_Interface::transfer16(uint16_t data) {
    #ifdef K210_ST7789_SIPEED
    uint32_t w = ((uint32_t)data << 16) | (uint32_t)data;
    tft_fill_data(&w, 2);
    return data;
    #elif HASSPI
    return this->_SPI->transfer16(data);
    #else
    return interface_transfer16(data);
    #endif
}
void TFT_Interface::transfer(void* buf, size_t count) {
    #ifdef K210_ST7789_SIPEED
    tft_write_byte((uint8_t *)buf, count);
    #elif HASSPI
    return this->_SPI->transfer(buf, count);
    #else
    return interface_transfer(buf, count);
    #endif
}
#if defined (__SAMD51__)
void TFT_Interface::transfer(const void* txbuf, void* rxbuf, size_t count, bool block) {
    #ifdef HASSPI
    return this->_SPI->transfer(txbuf, rxbuf, count, block);
    #else
    return;
    #endif
}
#endif

#ifdef HASSPI
void TFT_Interface::beginTransaction(SPISettings settings) {
#ifndef K210_ST7789_SIPEED
    this->_SPI->beginTransaction(settings);
#endif
}
void TFT_Interface::endTransaction() {
#ifndef K210_ST7789_SIPEED
    this->_SPI->endTransaction();
#endif
}
#else
void TFT_Interface::beginTransaction() {

}
void TFT_Interface::endTransaction() {

}
#endif

void TFT_Interface::writeCommand(uint8_t c) {
    #ifdef K210_ST7789_SIPEED
    tft_write_command(c);
    #elif HASSPI
    DC_C;
    transfer(c);
    #else
    interface_writeCommand(c);
    #endif

}
void TFT_Interface::writeData(uint8_t d) {
    #ifdef HASSPI
    DC_D;
    transfer(d);
    #else
    interface_writeData(d);
    #endif
}

#ifdef K210_ST7789_SIPEED
void TFT_Interface::fillData(uint32_t *buf, size_t count) {
    size_t aligned_len = (count + 3) & ~3;
    tft_fill_data(buf, aligned_len/2);
}
#endif
