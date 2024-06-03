#include"tastierino.h"
#include"rfid.h"
#include"wifi.h"
#include"scheda_sd.h"

//se la password è sbagliata 3 volte restituisce questo a nao
#define ERRATO "ERRATO"
//se la password è giusta restituisce questo a nao
#define CORRETTO "CORRETTO"

void setup(){
  
  Serial.begin(9600);
  delay(1000);

  setup_wifi();

  setup_lettura();

  setup_tastierino();

  setup_sd();
}

void loop(){
  WiFiClient client = server.available();

  if(client.connected()){

    client = server.available();

    if(client.available()){

      Serial.println("Client available");
      Serial.print("Received: ");

      char received = client.read();

      Serial.println(received);

      String uid="";  //per ricordarsi l'id (tipo string)
      static char *id=(char*)malloc(sizeof(char)*strlen(uid.c_str()));

      if(received=='1'){   //legge id della carta

        abilito_rfid(); //abilito pin rfid e disabilito sd
        Serial.println("Attendo carta...");

        while(uid==""){
          if(tessera.PICC_IsNewCardPresent() && tessera.PICC_ReadCardSerial()){
            Serial.println("Rilevata carta");
            uid=getUID();
            Serial.println("UID:" + uid);
          }
        }

        strcpy(id, uid.c_str());

        Serial.print("id parsificato=");
        Serial.println(id);

        client.write(id);

      }else if(received=='2'){  //controllo password ed eventuale nuova tessera

        abilito_sd(); //abilito pin sd e disabilito rfid

        if(new_tessera(id)){
          Serial.println("Nuova tessera! La stiamo registrando...");
          add_id(id);
        }
        
        int tentativi=3;
        char operazione[MAX_LINE+1];

        while(strcmp(operazione, CORRETTO) && tentativi!=0){

          Serial.println("Inserire password (fino a 12 caratteri), premere d per confermare");

          char *password_letta=leggi_tastierino();
          Serial.print("password = ");
          Serial.println(password_letta);

          if(tentativi==1){ //se ha finito i tentativi
            strcpy(operazione, ERRATO);
            tentativi--;
          }else if(controllo_password(id, password_letta)){ //restituisce 1 se password giusta
            Serial.println("Password corretta");
            strcpy(operazione, CORRETTO);
          }else{  //se ha sbagliato password
            Serial.print("Password errata riprovare, tentativi rimasti: ");
            Serial.println(tentativi-1);
            tentativi--;
          }
          free(password_letta);
        }
        client.write(operazione);
      }else if(received=='3'){  //visualizzare
        visualizzare_credito(id);
        free(id);
      }else{  //caricare (non apre file nuovo per la riscrittura dei dati)

        int ricarica=received;
        ricarica_tessera(id, ricarica);
        free(id);
      }
    }
    client.stop();
    Serial.println("client disconnected");
  }
}

void abilito_rfid(){
  digitalWrite(CS_SD, HIGH);
  digitalWrite(CS_RFID, LOW);
}

void abilito_sd(){
  digitalWrite(CS_RFID, HIGH);
  digitalWrite(CS_SD, LOW);
}