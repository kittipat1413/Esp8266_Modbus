#include "ModbusRTU.h"

ModbusRTU Modbus(Serial, 115200); 

void setup() {
  // put your setup code here, to run once:

  //DEBUG
  Serial1.begin(115200);
}

uint8_t *buf;
void loop() {
  // put your main code here, to run repeatedly:
  Serial1.println("Send data");
  int ret = Modbus.holdingRegisterRead(1,0x00,0x03,&buf);
  if(ret>0){
    Serial1.println("Data OK!!");
    for(int i = 0; i < 13; i++){
            Serial1.print(buf[i],HEX);
            Serial1.print("||");            
        }
        Serial1.println("");
  }    
  delay(5000);

}
