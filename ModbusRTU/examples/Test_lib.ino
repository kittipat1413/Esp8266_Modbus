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
