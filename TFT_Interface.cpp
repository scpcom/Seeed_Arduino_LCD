#include <TFT_Interface.h>

#ifdef HASSPI
TFT_Interface::TFT_Interface(SPIClass* spi)
{
    this->_SPI = spi;
}
#else 
TFT_Interface::TFT_Interface()
{

}
#endif

TFT_Interface::~TFT_Interface()
{

}

void TFT_Interface::begin()
{
    #ifdef HASSPI
      this->_SPI->begin();
    #else
    #endif
    return;
}

void TFT_Interface::end()
{
    #ifdef HASSPI
        this->_SPI->end();
    #else
    #endif
    return;
}


byte TFT_Interface::transfer(uint8_t data)
{
    #ifdef HASSPI
        return this->_SPI->transfer(data);
    #else
        return 0;
    #endif
}

uint16_t TFT_Interface::transfer16(uint16_t data)
{
    #ifdef HASSPI
        return this->_SPI->transfer16(data);
    #else
        return 0;
    #endif
}
void TFT_Interface::transfer(void *buf, size_t count)
{
    #ifdef HASSPI
        return this->_SPI->transfer(buf, count);
    #else
        return;
    #endif
}
#if defined (__SAMD51__)
void TFT_Interface::transfer(const void* txbuf, void* rxbuf, size_t count, bool block)
{
     #ifdef HASSPI
        return this->_SPI->transfer(txbuf, rxbuf, count, block);
    #else
        return;
    #endif
}
#endif

 #ifdef HASSPI
void TFT_Interface::beginTransaction(SPISettings settings)
{
    this->_SPI->beginTransaction(settings);
}
void TFT_Interface::endTransaction()
{
    this->_SPI->endTransaction();
}
#else
void TFT_Interface::beginTransaction()
{

}
void TFT_Interface::endTransaction()
{

}
#endif