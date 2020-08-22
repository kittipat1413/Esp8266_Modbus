
#include "ModbusRTU.h"

char DEBUG_buff[100];
char payload_buff[50];


#if defined(USE_SOFTWARE_SERIAL)
ModbusRTU::ModbusRTU(SoftwareSerial &next, uint32_t baud): modbus(&next){
  modbus->begin(baud);
  modbus->flush();
}
#else
ModbusRTU::ModbusRTU(HardwareSerial &next, uint32_t baud): modbus(&next){
  modbus->begin(baud);
  modbus->flush();
}
#endif

// 0x01,     0x03,            0x00, 0x00,      0x00, 0x02,          0xC4, 0x0B
// Address , Function Code    Start Address    Number of read       Modbus CRC
int ModbusRTU::holdingRegisterRead(uint8_t id, uint16_t address, uint16_t nb, uint8_t **buf){
  
  int i;
  uint8_t Command_Buf[8];
  memset(Command_Buf, 0, sizeof Command_Buf);

  serialFlush();
  memset(payload_buff,0,50);

    
  //Builds a RTU request header
  Command_Buf[0] = id;
  Command_Buf[1] = 0x03;
  Command_Buf[2] = address >> 8;
  Command_Buf[3] = address & 0x00ff;
  Command_Buf[4] = nb >> 8;
  Command_Buf[5] = nb & 0x00ff;

  //Gen CRC
  uint16_t command_crc = 0xFFFF;
      for(i = 0; i < 6; i++){
          command_crc = crc16_update(command_crc, (uint8_t)Command_Buf[i]);
      }
  Command_Buf[6] = (uint8_t)command_crc;
  Command_Buf[7] = (uint8_t)(command_crc >> 8); 
  //Send command
  for(i = 0 ; i < 8 ; i++){
      modbus->write(Command_Buf[i]);
  }

  //Waits for Data
  delay(5000);

  //Read data
  if(modbus->available() > 0){
       int payload_size = modbus->readBytes(payload_buff, 50);
       Serial1.println("=======================");
       sprintf(DEBUG_buff, "CRC from payload %X , %X ", payload_buff[payload_size-2],payload_buff[payload_size-1]);
       Serial1.println(DEBUG_buff);
       Serial1.println("=======================");
       Serial1.print("payload : ");
       Serial1.println(payload_buff);
       Serial1.println("=======================");
       Serial1.print("payload_size : ");
       Serial1.println(payload_size);


      uint16_t crc = 0xFFFF;
      for(i = 0; i < payload_size-2; i++){
          crc = crc16_update(crc, (uint8_t)payload_buff[i]);
      }

      Serial1.println("=======================");
      sprintf(DEBUG_buff, "CRC %X", crc);
      Serial1.println(DEBUG_buff);
      
      if((uint8_t)crc == (uint8_t)payload_buff[payload_size-2] && (uint8_t)(crc >> 8) == (uint8_t)payload_buff[payload_size-1]){      
          Serial1.println("CRC Check OK !!!!!");
          Serial1.println("=======================");
          payload_buff[payload_size-1] = '\0';          
          payload_buff[payload_size-2] = '\0';
          *buf =  (uint8_t *)payload_buff;
         
          // Re-check
          for(i = 0; i < 13; i++){
            Serial1.print(payload_buff[i],HEX);
          }
          Serial1.println("");
          for(i = 0; i < 13; i++){
            Serial1.print(*(*(buf)+i),HEX);
          }
          Serial1.println("");
          Serial1.println("=======================");
          return 1;
       }
      else{
          Serial1.println("CRC Check Error !!!!!");
          Serial1.println("=======================");
          memset(payload_buff,0,50);
          return -1;
       }

  }
  else{
      Serial1.println("UART TIME OUT !!!!!");
      Serial1.println("=======================");
      memset(payload_buff,0,50);
      return -1;
  }
  

  
}



uint16_t ModbusRTU::crc16_update(uint16_t crc, uint8_t a) {
  int i;

  crc ^= (uint16_t)a;
  for (i = 0; i < 8; ++i) {
    if (crc & 1)
      crc = (crc >> 1) ^ 0xA001;
    else
      crc = (crc >> 1);
  }

  return crc;
}


void ModbusRTU::serialFlush(){
  while(modbus->available() > 0) {
    char t = modbus->read();
  }
}   


 