#include<SPI.h>
#include<SD.h>

#define MAX_LINE 1024

#define ROW 35
#define COLS 50

#define CS_SD 15
#define FINE "fine"
#define MAX_N 12

//oggetto file
File fp;

void setup_sd(){

  if(!SD.begin(CS_SD)){
    Serial.println("Errore in scheda sd");
    return;
  }else{
    Serial.println("Trovata scheda sd");
  }
}

//lettura da file
/*struttura file:
id;password;credito*/
char *lettura(){

  char buffer[MAX_LINE+1];
  int i=0;
  buffer[i]=fp.read();

  while(buffer[i]!=';' && buffer[i]!='\n'){ //modificare condizione

    if(buffer[i]==-1){
      Serial.println("Fine file raggiunto");
      return FINE;
    }
    i++;
    buffer[i]=fp.read();
  }
  buffer[i]='\0';
  Serial.print("Parola trovata:");
  Serial.println(buffer);

  return strdup(buffer);
}

/*void go_to_id(){  //scorre fino al prossimo id

  Serial.print("Posizionamento id: ");

  while(fp.read()!='\n'){Serial.print("Cerco a capo ");}
  Serial.println("Id selezionato");
}*/

//cerca password nel file
char *search_password(char *id){
  char *read=NULL;

  Serial.print("Sto cercando password, id da trovare: ");
  Serial.println(id);
  fp=SD.open("database.txt", FILE_READ);
  if(!fp){
    Serial.println("Errore in apertura file");
    while(1){}
  }
  
  while(strcmp(read=lettura(), id)){
    Serial.println("Id non corrisponde");
    free(read);
  }
  free(read);

  char *password=lettura();
  Serial.print("Password trovata:");
  Serial.println(password);
  fp.close();
  return password;
}

//ritorna 1 se la password corrisponde all'id
int controllo_password(char *id, char *password_letta){
  int ret=0;

  char *password=search_password(id);

  if(!strcmp(password_letta, password)){
    ret=1;
  }

  free(password);
  return ret;
}

int new_tessera(char *id){
  
  fp=SD.open("database.txt", FILE_READ);
  if(!fp){
    Serial.println("Errore in apertura file");
    while(1){}
  }

  char *parola=NULL;

  Serial.println("Controllo se nuova tessera");

  while(strcmp(parola=lettura(), FINE)){
    if(!strcmp(parola, id)){
      free(parola);
      fp.close();
      Serial.println("Tessera già registrata!");
      return 0;
    }
    free(parola);
  }
  Serial.println("Fine file, tessera non trovata");

  fp.close();
  return 1;
}

void add_id(char *id){

  char *password1="1";
  char *password2="2";

  while(strcmp(password1, password2)){

    Serial.println("Inserire la nuova password! (fino a 12 caratteri)");
    password1=leggi_tastierino();
  
    Serial.println("Ripeti password!");
    password2=leggi_tastierino();

    if(strcmp(password1, password2)){
      Serial.println("Password diverse!");
    }
  }

  fp=SD.open("database.txt", FILE_WRITE);
  if(!fp){
    Serial.println("Errore in apertura file");
    while(1){}
  }

  Serial.print("id = ");
  Serial.println(id);

  fp.print(id);
  fp.print(";");
  fp.print(password1);
  fp.print(";");
  fp.print("0");
  fp.println();

  Serial.println("Nuova tessera aggiunta!");

  fp.close();
  free(password1);
  free(password2);
}

int search_credito(char *id){
  fp=SD.open("database.txt", FILE_READ);
  if(!fp){
    Serial.println("Errore in apertura file");
    while(1){}
  }

  char *read=NULL;
  while(strcmp(id, read=lettura())){
    Serial.println("Id non corrisponde");
    free(read);
  }
  free(read);
  read=lettura(); //per passare la password
  free(read);

  char *credito=lettura();
  int credito_int=atoi(credito);
  free(credito);

  Serial.println("Credito trovato!");
  fp.close();
  return credito_int;
}

void visualizzare_credito(char *id){

  int credito=search_credito(id);
  Serial.print("Credito= ");
  Serial.print(credito);
  Serial.println("€");
}

char *read_line(){
  char buffer[MAX_LINE+1];

  int i=0;
  while((buffer[i]=fp.read())!='\n'){
    if(buffer[i]==-1){
      Serial.println("Fine file raggiunto");
      return FINE;
    }
    i++;
  }
  buffer[i]='\0';
  Serial.print("Linea trovata:");
  Serial.println(buffer);
  return strdup(buffer);
}

int indice_modifica(char *id){
  fp=SD.open("database.txt", FILE_READ);
  if(!fp){
    Serial.println("Errore in apertura file");
    while(1){}
  }

  int i=0;
  char *read=NULL;
  while(strcmp(id, read=lettura())){
      free(read);
      i++;
  }
  free(read);

  int index=i/3;
  fp.close();
  return index;
}

void show_matrix(char **matrix){
  Serial.println("Inizio matrice:");
  for(int i=0;matrix[i];i++){
    Serial.println(matrix[i]);
  }
  Serial.println("Fine matrice");
}

void free_matrix(char **matrix){
  for(int i=0;matrix[i];i++){
    free(matrix[i]);
  }
  free(matrix);
}

void modifica_credito(char *s, int credito){
  char buffer[MAX_LINE+1];
  int count=0;
  int i=0;
  while(count<2) {
    buffer[i] = s[i];
    if (s[i] == ';') {
        count++;
    }
    i++;
  }

  Serial.print("Stringa senza credito(caratteri strani perchè non c'è ancora '\0')=");
  Serial.println(buffer);

  char buffer2[MAX_N];  //il credito massimo 12 cifre
  sprintf(buffer2, "%d", credito);

  int j=0;
  while((buffer[i]=buffer2[j])){
    i++;
    j++;
  }

  strcpy(s, buffer);
}

void nuovo_file(char **matrix){
  /*if(SD.exists("database.txt")) {
    SD.remove("database.txt"); // Rimuovi il file
    Serial.println("File eliminato");
    delay(200);
  }*/

  fp=SD.open("database.txt", FILE_WRITE); //crea il nuovo file
  if(!fp){
    Serial.println("Errore in apertura file");
    while(1){}
  }

  int i=0;
  while(matrix[i]){
    fp.println(matrix[i]);
    Serial.println("Scritto su file nuovo");
    i++;
  }
  fp.close();
}

void ricarica_tessera(char *id, int ricarica){  //crea un altro file dove al fondo mette i dati dell'id modificato
  int index=indice_modifica(id);
  Serial.print("Riga file da modificare=");
  Serial.println(index);

  char **matrix=(char**)malloc(sizeof(char*)*ROWS+1); //allocazione dinamica matrice
  for(int i=0;i<ROWS+1;i++){
    matrix[i]=(char*)malloc(sizeof(char)*COLS+1);
  }
  Serial.println("Matrice allocata");

  char *linea=NULL;
  char *linea_da_modificare=NULL;
  int i=0;

  fp=SD.open("database.txt", FILE_READ);
  if(!fp){
    Serial.println("Errore in apertura file");
    while(1){}
  }

  while(strcmp(linea=read_line(), FINE)){ //riempo tutto il file in una matrice tranne la linea da modificare
    if(i==index){
      linea_da_modificare=strdup(linea);  //salvo la linea con credito da modificare
      free(linea);

      linea=read_line();  //inserisco la riga dopo, al posto di quella da modificare
      if(strcmp(linea, FINE)){
        matrix[i]=strdup(linea);
      }
    }else{
      matrix[i]=strdup(linea);
    }
    i++;
    free(linea);
  }
  fp.close();
  
  modifica_credito(linea_da_modificare, ricarica);

  Serial.print("Linea modificata=");
  Serial.println(linea_da_modificare);  //modificata con nuovo credito

  matrix[i-1]=strdup(linea_da_modificare);
  matrix[i]=NULL;

  show_matrix(matrix);

  nuovo_file(matrix); //crea nuovo file e lo riempe con la matrice

  free_matrix(matrix);
  free(linea_da_modificare);
}





