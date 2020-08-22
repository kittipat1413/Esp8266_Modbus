#ifndef __ModbusRTU_H__
#define __ModbusRTU_H__

#include <Arduino.h>


//#define USE_SOFTWARE_SERIAL //Comment this line for use  HardwareSerial


#if defined(USE_SOFTWARE_SERIAL)
#include <SoftwareSerial.h>
#endif

class ModbusRTU{
 private:
  void flushSerial();
#if defined(USE_SOFTWARE_SERIAL)
   SoftwareSerial *modbus;
#else
   HardwareSerial *modbus;
#endif
 
 public:
    ModbusRTU(){};//Empty contructor

#if defined(USE_SOFTWARE_SERIAL)
  ModbusRTU(SoftwareSerial &next, uint32_t baud);//Constructor
#else
  ModbusRTU(HardwareSerial &next, uint32_t baud);//Constructor
#endif

    int holdingRegisterRead(uint8_t id, uint16_t address, uint16_t nb, uint8_t **buf);
    uint16_t crc16_update(uint16_t crc, uint8_t a);
    void serialFlush();

};
#endif






