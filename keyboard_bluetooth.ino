#include "Adafruit_Keypad.h"
#include <SoftwareSerial.h>

SoftwareSerial bluetooth(2,3);

const byte ROWS = 2; // rows
const byte COLS = 3; // columns

//define the symbols on the buttons of the keypads
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'}
};
byte rowPins[ROWS] = {4, 5}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {6, 7, 8}; //connect to the column pinouts of the keypad

byte bufferKeys[6];
int len = sizeof(bufferKeys)/sizeof(bufferKeys[0]);
int keyCode;

//initialize an instance of class NewKeypad
Adafruit_Keypad customKeypad = Adafruit_Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
  Serial.begin(9600);
  bluetooth.begin(9600);
  bluetooth.println("Listo");
  customKeypad.begin();
  bluetooth.write("Iniciado...");
}

void printBuffer(){

  for (int i=0;i<len;i++){
     Serial.println(bufferKeys[i]);
  }
    Serial.println("/////");
}


void addKeyBuffer(int keyCode){
  if(!existKeyToBuffer(keyCode)){
    int index = getCount();
    bufferKeys[index] = keyCode;
  } 
  //printBuffer();
  //bluetooth.write(bufferKeys);
 // sendDataStateKeys();
  
}

void deleteKeyBuffer(int keyCode){
  
  int index = getKeyIndexBuffer(keyCode);
  if(index != -1){
    bufferKeys[index] = 0;
  } 
  //sendDataStateKeys();
  //bluetooth.write(bufferKeys);
  //printBuffer();
}

bool existKeyToBuffer(int KeyCode){
  bool exist = false;
  for (int i=0;i<len;i++){
    if(keyCode == bufferKeys[i]) exist=true;
  }
  return exist; 
}

int getKeyIndexBuffer(int KeyCode){
   int indexKey = -1;
  for (int i=0;i<len;i++){
    if(keyCode == bufferKeys[i]) return i;
  }
  return indexKey; 
}

int getCount(){
  int count = 0;
  for (int i=0;i<len;i++){
    if(bufferKeys[i] != 0) count++;
  }
  return count;
}

void viewDataSerial(){
    if(bluetooth.available()) Serial.write(bluetooth.read());
    if(Serial.available()) bluetooth.write(Serial.read());
     //buttonState = digitalRead(PIN_KEY); 
}

void sendDataStateKeys(char stateKey, char keyCode){

    bluetooth.write(stateKey);
    bluetooth.write('-');
    bluetooth.write(keyCode); 
    bluetooth.write('@'); 
}

void loop() {
  // put your main code here, to run repeatedly:
  customKeypad.tick();

  while(customKeypad.available()){
    keypadEvent e = customKeypad.read();
    keyCode = e.bit.KEY;
    //Serial.println((char)e.bit.KEY);
    
    if(e.bit.EVENT == KEY_JUST_PRESSED){ 
      sendDataStateKeys('P',e.bit.KEY);
    }
    else if(e.bit.EVENT == KEY_JUST_RELEASED){
      sendDataStateKeys('R',e.bit.KEY);
    }
    
    if(customKeypad.isPressed((char)e.bit.KEY)) sendDataStateKeys('H',e.bit.KEY);
   
    //if(customKeypad.isPressed((char)e.bit.KEY)) Serial.println("PRESSED"); //tecla precionada ??
  }

   
  
  delay(10);
}
