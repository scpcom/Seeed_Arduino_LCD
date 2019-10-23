#include <Arduino.h>
#include <User_Setup.h>

#ifdef HASSPI
#ifdef KENDRYTE_K210
  #include <SPIClass.h>
#else
  #include <SPI.h>
#endif
#endif

class TFT_Interface 
{
public:
    #ifdef HASSPI
    TFT_Interface(SPIClass* spi);
    #else
    TFT_Interface();
    #endif
    ~TFT_Interface();
    void begin();
    void end();
    #ifdef HASSPI
    void beginTransaction(SPISettings settings);
    void endTransaction();
    #else
    void beginTransaction();
    void endTransaction();
    #endif
    byte transfer(uint8_t data);
    uint16_t transfer16(uint16_t data);
    void transfer(void *buf, size_t count);
    #if defined (__SAMD51__)
    void transfer(const void* txbuf, void* rxbuf, size_t count, bool block = true);
    #endif
private:
    #ifdef HASSPI
    SPIClass* _SPI;
    #endif
};