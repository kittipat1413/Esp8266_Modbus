
#include "ModbusRTU.h"

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
       
      DEBUG_MB(F("======================="));
      sprintf(DEBUG_buff, "CRC from payload %X , %X ", payload_buff[payload_size-2],payload_buff[payload_size-1]);
      DEBUG_MB(DEBUG_buff);
      DEBUG_MB(F("======================="));
      DEBUG_MB(F("payload_size : "));
      DEBUG_MB(payload_size);
      DEBUG_MB(F("======================="));
      DEBUG_MB(F("payload : "));
      for(i = 0; i < payload_size; i++){
          sprintf(DEBUG_buff, "%02X / ", payload_buff[i]);
          DEBUG_MB_HEX(DEBUG_buff);
        }
      DEBUG_MB_HEX("\n");

      uint16_t crc = 0xFFFF;
      for(i = 0; i < payload_size-2; i++){
          crc = crc16_update(crc, (uint8_t)payload_buff[i]);
      }

      DEBUG_MB(F("======================="));
      sprintf(DEBUG_buff, "CRC %X", crc);
      DEBUG_MB(DEBUG_buff);
      
      if((uint8_t)crc == (uint8_t)payload_buff[payload_size-2] && (uint8_t)(crc >> 8) == (uint8_t)payload_buff[payload_size-1]){      
          // Serial1.println("CRC Check OK !!!!!");
          DEBUG_MB(F("CRC Check OK !!!!!"));
          DEBUG_MB(F("======================="));
          payload_buff[payload_size-1] = '\0';          
          payload_buff[payload_size-2] = '\0';
          
          //Point buf to payload_buff
          DEBUG_MB(F("buf payload : "));
          *buf =  (uint8_t *)payload_buff;
          for(i = 0; i < payload_size-2; i++){
            sprintf(DEBUG_buff, "%02X / ", *(*(buf)+i));
            DEBUG_MB_HEX(DEBUG_buff);
            // Serial1.print(*(*(buf)+i),HEX);
          }
          DEBUG_MB_HEX("\n");
          DEBUG_MB(F("======================="));
          return (payload_size-2);
       }
      else{
          DEBUG_MB(F("CRC Check Error !!!!!"));
          DEBUG_MB(F("======================="));
          memset(payload_buff,0,50);
          return -1;
       }

  }
  else{
      DEBUG_MB(F("UART TIME OUT !!!!!"));
      DEBUG_MB(F("======================="));
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


template <typename Generic>
void ModbusRTU::DEBUG_MB(Generic text) {
  if (_debug) {
    /*Uncomment for using Serial debug*/
    //Serial.print("*MB: ");
    //Serial.println(text);
    /*Uncomment for using Serial1 debug*/
    Serial1.print("*MB: ");
    Serial1.println(text);
  }
}

void ModbusRTU::DEBUG_MB_HEX(char* text) {
  if (_debug) {
    /*Uncomment for using Serial debug*/
    //Serial.print(text);
    /*Uncomment for using Serial1 debug*/
    Serial1.print(text);
  }
} 