# Esp8266_Modbus
Dev ESP8266 Modbus RTU (rs485)
* Supported Modbus Function code 03 holdingRegisterRead 

# EX

```C++
#include "ModbusRTU.h"

ModbusRTU Modbus(Serial, 9600); 

void setup() {
  // put your setup code here, to run once:

  //DEBUG
  Serial1.begin(115200);
}

uint8_t *buf;
void loop() {
  // put your main code here, to run repeatedly:
  Serial1.println("Read data");
  int res_size = Modbus.holdingRegisterRead(1,0x00,0x03,&buf);
  if(res_size > 0){
    Serial1.println("Data OK!!");
    Serial1.print("Response size : ");
    Serial1.println(res_size);
    Serial1.print("Response data : ");
    for(int i = 0; i < res_size; i++){
            Serial1.print(buf[i],HEX);
            Serial1.print("||");            
        }
        Serial1.println("");
  }    
  delay(5000);

}
```
**`holdingRegisterRead(1,0x00,0x03,&buf)`** 
* 1 = Sensor address
* 0x00 = Starting register address that we want to read
* 0x03 = Number of register that we want to read (In this case we read 3 register starting from 0x00)
* &buf = Response data will be stored in this buffer

**`ModbusRTU Modbus(Serial, 9600)`** 

This function will send ModbusRTU request data through the Serial port @ baud rate 9600 
