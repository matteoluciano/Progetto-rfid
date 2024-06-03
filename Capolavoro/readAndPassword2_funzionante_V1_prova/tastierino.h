#include<string.h>

#define ROWS 4
#define COLS 4
#define MAX_PASSWORD 12 //massimi caratteri password

const int rowPins[4]={14, 8, 7, 6};
const int colPins[4]={5, 4, 3, 2};
const int debounceTime = 20;  //per rimbalzo tasto

char tastierino[ROWS][COLS]{
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

void setup_tastierino(){
  for(int i=0;i<ROWS;i++){
    pinMode(rowPins[i], INPUT_PULLUP);
    digitalWrite(rowPins[i], HIGH);
  }

  for(int i=0;i<COLS;i++){
    pinMode(colPins[i], OUTPUT);
    digitalWrite(colPins[i], HIGH);
  }
}

//legge tasto da tastierino
char get_tasto(){

  char tasto=0;

  for(int i=0;i<COLS;i++){
    digitalWrite(colPins[i], LOW);

    for(int j = 0; j < ROWS; j++){     // Scorro tutte le righe cercano un tasto premuto
      if(digitalRead(rowPins[j]) == LOW){     // E' premuto il tasto
      
        delay(debounceTime);                     // attendo che il segnale si stabilizzi (vedi annotazione)
        while(digitalRead(rowPins[j]) == LOW); // attendo che il tasto sia rilasciato
        tasto = tastierino[j][i];               // Memorizzo quale sia il tasto premuto
      }
    }
    digitalWrite(colPins[i], HIGH);
  }
  return tasto;
}

//legge password dal testierino
char *leggi_tastierino(){

  char buffer[MAX_PASSWORD+1];
  char tasto=0;
  int i=0;

  while(tasto!='D'){
    if(tasto=get_tasto()){
      buffer[i++]=tasto;
    }
  }
  buffer[i-1]='\0';

  return strdup(buffer);
}