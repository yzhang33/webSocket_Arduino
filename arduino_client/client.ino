#include <WiFiNINA.h>       // use this for MKR1010 and Nano 33 IoT boards
#include <ArduinoHttpClient.h>
#include <LiquidCrystal.h>
#include "arduino_secrets.h"
#include "figures.h"

WiFiClient netSocket;               // network socket to server
const char server[] = "192.168.1.157";  // server name
String route = "/msg";              // API route
// request timestamp in ms:
long lastRequest = 0;
// interval between requests:
int interval = 10000;
const int rs = 12, en =11, d4 = 5, d5=4, d6=3, d7=2;
LiquidCrystal lcd(rs,en,d4,d5,d6,d7);

String oldStr="";

void setup() {
  Serial.begin(9600);               // initialize serial communication
  while (!Serial);        // wait for serial monitor to open

  // while you're not connected to a WiFi AP,
  while ( WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(SECRET_SSID);           // print the network name (SSID)
    WiFi.begin(SECRET_SSID, SECRET_PASS);         // try to connect
    delay(2000);
  }

  // When you're connected, print out the device's network status:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  lcd.begin(16,2);
}

void loop() {
  if (millis() - lastRequest > interval ) {

    Serial.println("making request");
    HttpClient http(netSocket, server, 3000);      // make an HTTP client
    http.get(route);  // make a GET request

    while (http.connected()) {       // while connected to the server,
      if (http.available()) {       
        String result = http.responseBody();  // read only the body
        Serial.print(result); 

        //compare string with previous string 
        if(oldStr.compareTo(result) != 0){
          lcd.setCursor(0,0);
          showDisplay(result);
          oldStr = result;
        }
      }
    }
    // when there's nothing left to the response,close
    http.stop();               
    lastRequest = millis();
  }
}
//display text for figures
void showDisplay(String data){
  lcd.clear();
  boolean figure = false;
 
  //star
  if(data.compareTo("star") == 0){
    figure = true;
    //create some stars
    lcd.createChar(0, star);
    int pos1 = 3;
    
    lcd.createChar(pos1, star);
    int pos2 = 5;
    lcd.createChar(pos2, star);
    int pos3 = 8;
    lcd.createChar(pos3, star);
    int pos4 = 11;
    lcd.createChar(pos4, star);
    int pos5 = random(1,16);
    lcd.createChar(pos5, star);
     
    lcd.begin(16, 2);  
    lcd.write(byte(0));
    lcd.write(pos1);
    lcd.write(pos2);
    lcd.write(pos3);
    lcd.write(pos4);
    lcd.write(pos5);
    
  }
  //hello chinese
  if(data.compareTo("hello") == 0){
    figure = true;
    
    lcd.createChar(0, ch1);
    lcd.createChar(1, ch2);
    lcd.createChar(2, ch3);
    lcd.createChar(3, ch4);
    lcd.begin(16, 2);  
    lcd.write(byte(0));
    lcd.write(1);
    lcd.write(2);
    lcd.write(3);
  }
  //hammer
   if(data.compareTo("hammer") == 0){
     figure = true;
     showHammer();
   }
   //lighting
   if(data.compareTo("lightning") == 0){
    figure = true;
    showLighting();
   }
   //man
    if(data.compareTo("man") == 0){
       figure = true;
       showMan();
    }
   //hammer lighting man
   if(data.compareTo("hammer lighting man") == 0){
      figure = true;
      hammerLightingMan();
   }
   //man run
   if(data.compareTo("man run") == 0){
      figure = true;
      manRun();
   }
   //story
   if(data.compareTo("story") == 0){
      figure = true;
      showMan();
      delay(1000);
      showHammer();
      delay(2000);
      showLighting();
      delay(2000);
      lcd.clear();
      hammerLightingMan();
      delay(1000);
      lcd.clear();
      manRun();
      
   }
   //if not figure
   if(!figure){
    if(data.length() > 16){
      lcd.print(data.substring(0,16));
      lcd.setCursor(0,1);
      lcd.print(data.substring(16,data.length()));
    }else{
      lcd.print(data);
    }
   }
   
}
//showMan
void showMan(){
   int c = 0;
   while(c < 20){
     lcd.createChar(1, armsup);
     lcd.createChar(2,armsdown);
     lcd.setCursor(10,1);
     lcd.write(1); 
     delay(500);
     lcd.setCursor(10,1);
     lcd.write(2); 
     delay(500);
     c++;
   }
}
//show lighting
void showLighting(){
   lcd.createChar(1, hammer);
   lcd.setCursor(4,0);
   lcd.write(1); 
   lcd.createChar(2,lighting1);
   lcd.createChar(3,lighting2);
   lcd.createChar(4,lighting3);
   lcd.createChar(5,lighting4);
   lcd.createChar(6,lighting5);
   lcd.setCursor(5,0);
   lcd.write(2); 
   lcd.write(3);  
   lcd.write(4);  
   lcd.write(5);  
   lcd.write(6);  
}
//show hammer
void showHammer(){
   lcd.createChar(1, hammer);
   lcd.setCursor(4,0);
   lcd.write(1);  
}
//man run display 
void manRun(){
  lcd.createChar(1, manrun);
  lcd.setCursor(16,1);
  lcd.write(1); 
  for(int pos=0;pos < 17; pos++){
    lcd.scrollDisplayLeft();
    delay(300);
  }
  //run right
  lcd.clear();
  lcd.createChar(1, manrunright);
  lcd.setCursor(0,1);
  lcd.write(1); 
  for(int pos=0;pos < 17; pos++){
    lcd.scrollDisplayRight();
    delay(300);
  }
  //run left
  lcd.clear();
  lcd.createChar(1, manrun);
  lcd.setCursor(16,0);
  lcd.write(1); 
  for(int pos=0;pos < 17; pos++){
    lcd.scrollDisplayLeft();
    delay(300);
  }
}
//hammer lighting man display
void hammerLightingMan(){
  lcd.createChar(1, hammer);
  lcd.setCursor(4,0);
  lcd.write(1); 
  lcd.createChar(2,lighting1);
  lcd.createChar(3,lighting2);
  lcd.createChar(4,lighting3);
  lcd.createChar(5,lighting4);
  lcd.createChar(6,lighting5);
  lcd.setCursor(5,0);
  lcd.write(2); 
  lcd.write(3);  
  lcd.write(4);  
  lcd.write(5);  
  lcd.write(6);  
  lcd.createChar(7, armsup);
  lcd.setCursor(9,1);
  lcd.write(7); 
}
