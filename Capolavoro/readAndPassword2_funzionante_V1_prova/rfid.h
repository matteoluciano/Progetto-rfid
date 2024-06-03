#include<MFRC522.h>
#include<SPI.h>

//per rfid
#define RST_PIN 9
#define CS_RFID 10

//oggetto tessera
MFRC522 tessera(CS_RFID, RST_PIN);

void setup_lettura(){
  SPI.begin();

  tessera.PCD_Init();
  delay(10);
}

String getUID(){

  String ret="";

  for(int i=0;i<tessera.uid.size;i++){
    ret+=String(tessera.uid.uidByte[i], HEX) + ""; 
  }

  tessera.PICC_HaltA();
  return ret;
}