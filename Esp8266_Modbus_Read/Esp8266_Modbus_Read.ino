#include "ModbusRTU.h"

//uint8_t read_Volt[8] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x03, 0x05, 0xCB};
//uint8_t read_Amp[8] = {0x01, 0x03, 0x00, 0x03, 0x00, 0x03, 0xF5, 0xCB};
//uint8_t read_Watt[8] = {0x01, 0x03, 0x00, 0x07, 0x00, 0x04, 0xF5, 0xC8};
//uint8_t read_Var[8] = {0x01, 0x03, 0x00, 0x0B, 0x00, 0x04, 0x35, 0xCB};
//uint8_t read_VA[8] = {0x01, 0x03, 0x00, 0x0F, 0x00, 0x04, 0x74, 0x0A};
//uint8_t read_PF[8] = {0x01, 0x03, 0x00, 0x13, 0x00, 0x04, 0xB5, 0xCC};

char data_buffer[50];
ModbusRTU Modbus(Serial, 9600); 

void setup() {
  //DEBUG
  Serial1.begin(115200);
  Modbus.setDebugOutput(false);
}

uint8_t *buf;
void loop() {
  // put your main code here, to run repeatedly:
  Serial1.println("Read Volt");
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
      
    uint16_t Va = (((uint16_t)buf[0])<<8 | (uint16_t)buf[1] );
    uint16_t Vb = (((uint16_t)buf[2])<<8 | (uint16_t)buf[3] );
    uint16_t Vc = (((uint16_t)buf[4])<<8 | (uint16_t)buf[5] );
    sprintf(data_buffer, "Va : %.2f , Vb : %.2f, Vc : %.2f ",Va/10.0,Vb/10.0,Vc/10.0);
    Serial1.println(data_buffer);  
  }    
  delay(5000);

}
