#include <Arduino.h>
#include "IrManager.h"
#include <IRremote.hpp>

void IrManager::irBegin(){
   IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
}

bool IrManager::receiveIR(uint16_t &address,uint16_t &command) {
  if (IrReceiver.decode()) {

    address = IrReceiver.decodedIRData.address;
    command = IrReceiver.decodedIRData.command;

   // Serial.print(F("Decoded protocol: "));
   // Serial.print(getProtocolString(IrReceiver.decodedIRData.protocol));
  //  Serial.print(F(", decoded raw data: "));

  //  PrintULL::print(&Serial, IrReceiver.decodedIRData.decodedRawData, HEX);

  //  Serial.print(F(", decoded address: "));
  //  Serial.print(IrReceiver.decodedIRData.address, HEX);
  //  Serial.print(F(", decoded command: "));
 //   Serial.println(IrReceiver.decodedIRData.command, HEX);
    IrReceiver.resume();
    
    return true;
  }

  return false;
}