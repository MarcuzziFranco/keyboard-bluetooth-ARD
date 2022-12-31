#include "Adafruit_Keypad.h"
#include <SoftwareSerial.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


SoftwareSerial bluetooth(2,3);
const byte red = 18;
const byte green = 17;
const byte blue = 16;
String color = "";
const byte ROWS = 2; // rows
const byte COLS = 3; // columns

struct RGB {
  int r;
  int g;
  int b;
};

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
  Serial.println("Iniciando arduino");
  ConfigurationPinesLed();
  bluetooth.begin(9600);
  bluetooth.println("Listo");
  customKeypad.begin();
  bluetooth.write("Iniciado...");
}

void ConfigurationPinesLed(){
  pinMode(red,OUTPUT);
  pinMode(green,OUTPUT);
  pinMode(blue,OUTPUT);

  analogWrite(red,255);
  analogWrite(green,255);
  analogWrite(blue,255);
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
    /*if(bluetooth.available() > 0) 
    {
      Serial.write(bluetooth.read()); 
    }
    if(Serial.available())
    {
      Serial.println("Serial Serial");
      bluetooth.write(Serial.read()); 
    }*/
    if(bluetooth.available() > 0){
      String bufferEntry = "";
      while(bluetooth.available() > 0){
        char entry = (char)bluetooth.read(); 
        bufferEntry += entry;
      }
      //Serial.print(bufferEntry);
      
      //Serial.println(bufferEntry.indexOf('|'));
     /* if(bufferEntry.indexOf('|')!= -1){
        Serial.println(bufferEntry.length());
        Serial.println(bufferEntry.substring(0,bufferEntry.indexOf('|'))); //Get red;
        String strRed = bufferEntry.substring(0,bufferEntry.indexOf('|'));
        int red = strRed.toInt();
        bufferEntry.remove(0,strRed.length()+1);
        Serial.print(bufferEntry);
      }*/
     struct RGB color = getValuesColorString(bufferEntry);
      analogWrite(red,color.r);
      analogWrite(green,color.g);
      analogWrite(blue,color.b);
      
      /*char* color;
      color = strdup(bufferEntry.c_str());
      struct RGB rgb = color_hex_to_rgb(color);
      Serial.println(rgb.r);
      Serial.println(rgb.g);
      Serial.println(rgb.b);*/
    }
    
     
}

struct RGB getValuesColorString(String entry){
    int values[3];
      byte count = 0;
     while(entry.indexOf('|')!= -1){
        //Serial.println(entry.length());
        //Serial.println(entry.substring(0,entry.indexOf('|'))); //Get value
        String str = entry.substring(0,entry.indexOf('|'));
        values[count]= str.toInt();
        entry.remove(0,str.length()+1);
        //Serial.println(data);
        //Serial.print(entry);
        count++;
      }
      Serial.print(values[0]);
      Serial.print(values[1]);
      Serial.print(values[2]);
      return (struct RGB){values[0], values[1], values[3]};
}

struct RGB color_hex_to_rgb(const char* hex){
  
  Serial.println(hex);
  if (hex[0] != '#' || strlen(hex) != 7) {
    return (struct RGB){0, 0, 0};
  }
  // Convertimos cada par de dígitos o letras del string a su equivalente en valor decimal
  char* end;
  long r = strtol(hex + 1, &end, 16);
  long g = strtol(end, &end, 16);
  long b = strtol(end, NULL, 16);

  // Asignamos cada valor decimal a una componente del formato RGB
  return (struct RGB){(int)r, (int)g, (int)b};
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
  viewDataSerial();
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
