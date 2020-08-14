#include <DHT.h>
#include <DHT_U.h>
#include "DHT.h"

#include <SPI.h>
//#include <SD.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include <TinyGPS++.h>
#include "LiquidCrystal.h"
TinyGPSPlus gps;

  
LiquidCrystal_I2C lcd(0x27 ,20,4);

#define DHTPIN 11
#define DHTTYPE DHT11
DHT dht(DHTPIN , DHTTYPE);

#define potpin A0 
int potv ;

#include<RadioLib.h>
SX1278 lora = new Module(10,2,3);


//#include <SoftwareSerial.h>
float rpm=0 , rpmsec= 0 ;
long i = 0 ;
double LAT = 0;
double LNG = 0;
float gpsSpeed = 0;
double Dist = 0 ;
double Hlat = 17.5947 ;
double Hlng = 78.1231 ;
float Hallspeed = 0 ;
float Hallspeed1 =0;
double HDist=0 ;
int lap = 0;
const int cs = 4;
String Date  ;
String Time ;
float t =0;
float h =0;
int Hour ,Minute, Second ;
int Day,Month , Year ;
float circum = 1.83 ; //in meter
unsigned long interval = 1000;


//File myFile ;
unsigned long Pmillis = 0;
unsigned long Cmillis = 0 ;



//SoftwareSerial ss(11,10);

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);//gps
  Serial2.begin(9600);//hall
  Serial3.begin(9600);//xbee communication
  Serial.println("Setting LoRa Module");
  
 // pinMode(potpin , INPUT);
  lcd.init();
  //SPI.begin() ;
  lcd.backlight();
  dht.begin();
  lcd.setCursor(0,0);
  lcd.print("    IITH Racing    ");
  lcd.setCursor(0,1);
  lcd.print("      The Team     ");
  delay(1000);
  lcd.clear();
  
  Serial.print("Hlat="); Serial.println(Hlat,6);
  lcd.setCursor(0,0);
  lcd.print("Hlat : ");
  lcd.setCursor(7,0);
  lcd.print(Hlat,6);
  
  
  
  Serial.print("Hlng="); Serial.println(Hlng ,6);
  lcd.setCursor(0,1);
  lcd.print("Hlng : ");
  lcd.setCursor(7,1);
  lcd.print(Hlng,6);
  delay(500);

  

  lcd.clear();
  lcd.setCursor(0,0);
  //lora.setBandwidth(250);
  //lora.setSpreadingFactor(7);
  //lora.setCRC(false);
  //lora.setFrequency(433);
  
  int state = lora.begin();
  //Serial.println("began");
  if(state == ERR_NONE){
    Serial.println(F("Success"));
    //lcd.print("LoRa Activated");
   // delay(500);
    //lcd.clear();
  }
  else{
    Serial.println(F("LoRa Failed...Check Wirirng..."));
    //lcd.print("LoRa is Not Connected...");
    while(1);
  }

  /*Serial.println("Initializing SD card...");
  if(!SD.begin(cs)){
    Serial3.println("Unable to initialize the card....");
    Serial.println("Card , or not present...");
    
    //while(1);
  }
  Serial.println("Card Initialized.");
  lcd.print("Card Initialized");
 
  delay(100);
   lcd.clear();

  myFile  = SD.open("Day1.txt",FILE_WRITE);
  if(myFile){
    //Serial.println("I did my part...");
    myFile.print("Date"); myFile.print(",");
    myFile.print("Time"); myFile.print(",");
    myFile.print("LAT"); myFile.print(",");
    myFile.print("LNG"); myFile.print(",");
    myFile.print("GPSspeed(km/hr.)"); myFile.print(",");
    myFile.print("HallSpeed(m/s)"); myFile.print(",");
    myFile.print("GPSdistance(km)"); myFile.print(",");
    myFile.print("Halldistance(m)"); myFile.print(",");
    //myFile.print("Lap"); myFile.print(",");
    myFile.println("Tempreature");

    myFile.close();
     
  }*/
  
  Pmillis = millis() ;
  
  
  

}

void loop() {

 
  

  
  Serial.println("getting into while loop");
  if (!Serial1.available()){
    

 
    //lcd.print("Serial not avi");
    //delay(1000);
    //lcd.clear();
  }
  
  while(Serial1.available() > 0)//instead of IF there was while i changed it  to check
  { 
    //lcd.setCursor(0,0);
    //lcd.print(" GPS is Working ");
    //delay(1000);
    //lcd.clear();
    //Serial.print(gps.location.lat(),6);
    
     
     if(gps.encode(Serial1.read()));
     {
      
     if(gps.location.isUpdated()) // it was also isValid here 
     {Serial.println("it should work");
      
      
      //Serial.println("Location is updated");
      
      Serial.print("LAT=");
      LAT = gps.location.lat();
      Serial.println(LAT,6 );      
      Serial.print("LNG=");
      LNG = gps.location.lng();
      Serial.println(LNG ,6);  
      Serial.print("gpsSpeed=");
      gpsSpeed = gps.speed.kmph();
      Serial.println(gpsSpeed);
      Serial.print("Distance Travelled=");
      Dist = gps.distanceBetween(gps.location.lat(),gps.location.lng(),Hlat,Hlng)/1000.0  ;
      Serial.println(Dist , 6);
  }
     }
 
  }
  Temperature();
  DatenTime();
  hall() ;
  //pot();
  Cmillis = millis() ;
  disp() ;
  Serial.println("Yaha pee aa raha hain.....");
  //datalogging() ;
  communication() ;
  //Serial.println(Cmillis); Serial.println("          "); Serial.println(Pmillis);
  i = i+1 ;
  }



  
  void Temperature() {

    h = dht.readHumidity();
    t = dht.readTemperature();


    if (isnan(h) || isnan(t) ){
      Serial.println("Failed to read an data from DTH.");
     
    }  
  }

  /*void datalogging()
  { File myFile ;
    //lcd.print("AM_WORKING");
    Serial.println("working.");
    //lcd.clear();
    myFile = SD.open("Day1.txt",FILE_WRITE);
    if(myFile){
      myFile.print(Date); myFile.print(",");
      myFile.print(Time); myFile.print(",");
      myFile.print(LAT); myFile.print(",");
      myFile.print(LNG); myFile.print(",");
      myFile.print(gpsSpeed); myFile.print(",");
      myFile.print(Hallspeed); myFile.print(",");
      myFile.print(Dist); myFile.print(",");
      myFile.print(HDist); myFile.print(",");
      myFile.print(lap); myFile.print(",");
      myFile.println(t);
      

      myFile.close();
      
      
    }
  }  
*/
     
   void DatenTime(){
    Serial.println("DatenTime");
    Hour = gps.time.hour() ;
    Minute = gps.time.minute() ;
    Second = gps.time.second() ;

    Day = gps.date.day();
    Month = gps.date.month();
    Year = gps.date.year();

    //Indian Time
    Minute = Minute + 30;
    if(Minute > 60){
      Hour = Hour +1;
      Minute = Minute -60;
    }

    Hour = Hour + 5 ;
    if(Hour > 24){
      Day = Day +1;
      Hour = Hour - 24;

      
      
    }

    //Date
    Date = String(Day)+ "/" + String(Month) + "/" + String(Year) ;
    Serial.println(Date);
    //Time
    Time = String(Hour)+ ":" + String(Minute)+ ":" +String(Second);
    Serial.println(Time);
    
} 

 /*void Hall(){
  char c = Serial2.peek() ;
    if ( c == '\r' || c == '\n' ){
    Serial2.read() ;
    }
    else
    {
      Hallspeed = Serial2.read() ;
      if  (Hallspeed > 0) {
        
        Hallspeed1 = Hallspeed ;}
      
      if(Hallspeed < 0) {
        Serial.print("HallSpeed = ");
        Serial.println(Hallspeed1);
      }
      else {
        
        Serial.print("HallSpeed = ");
        Serial.println(Hallspeed);
      }
      
      
  
  }
 }
*/

 void hall(){
  int i = 0 , ind =0  ;
  
  String s ,is ;
  //float rpm ;
  
  if(Serial2.available() > 0){
    s = Serial2.readStringUntil('\n');
    Serial.println(s);
    rpm  = s.toFloat();
    Hallspeed  =  ((rpm*circum)*3)/50 ; //in meter per second
    ind = s.indexOf(' ');
    is  = s.substring(ind + 1);
    i   = is.toInt();
    HDist = i*circum ;//in meter
    rpmsec = rpm*9 ;  
    
  }

  



  
 }
  void communication(){

    String Final = String(Date)+",  " +String(Time) + ",  " + String(LAT,6)+ ",  "+ String(LNG,6) + ",  " + String(gpsSpeed) + ",  " + String(Hallspeed)+ ",  "+
    String(Dist) +",  " + String(HDist) + ",  " /*+ String(lap) + ","*/ + String(rpmsec) + ",  " + String(t)/*+ ",  " + String(potv)*/  ;

    lora.transmit(Final);

    
    
    Serial.println(String(Date)+"," +String(Time) + "," + String(LAT,6)+ ","+ String(LNG,6) + "," + String(gpsSpeed) + "," + String(Hallspeed)+ ","+
    String(Dist) +"," + String(HDist) + "," /*+ String(lap) + ","*/ + String(rpmsec)  + ",  " + String(t)/* + ",  "+ String(potv)*/);
   
    Serial3.println(String(Date)+",  " +String(Time) + ",  " + String(LAT,6)+ ",  "+ String(LNG,6) + ",  " + String(gpsSpeed) + ",  " + String(Hallspeed)+ ",  "+
    String(Dist) +",  " + String(HDist) + ",  " /*+ String(lap) + ","*/ + String(rpmsec) + ",  " + String(t)/*+ ",  " + String(potv)*/);
 }
 
 void disp(){
  Serial.println("///////////////////////////////////////");
  if(Cmillis - Pmillis >1000 && Cmillis - Pmillis < 2000){
  //i = i+1;
  Serial.println("......................................................................");
  //Serial.println(i);
  Pmillis = Cmillis ; 
  lcd.setCursor(0,0);
  lcd.print("Speed  :");
  lcd.setCursor(8,0);
  lcd.print(Hallspeed);
  lcd.setCursor(15,0);
  //lcd.print(i);
  lcd.print("km/hr");
  lcd.setCursor(0,1);
  lcd.print("Tempr  :");
  lcd.setCursor(8,1);
  lcd.print(t);
  lcd.setCursor(18,1);
  lcd.print("dC");
  lcd.setCursor(0,2);
  lcd.print("RpmSec :");
  lcd.setCursor(8,2);
  lcd.print(rpmsec);
  //lcd.print(i);
  //lcd.setCursor(15,2);
  //lcd.print("km/hr.");
  //lcd.print(i);
  lcd.setCursor(0,3);
  lcd.print("D: ");
  lcd.setCursor(3,3);
  lcd.print(HDist);
  lcd.setCursor(12,3);
  lcd.print(Time);
 
  
 }}


 /*void pot() {
  potv  = analogRead(potpin);
    if(potv  >= 0  && potv <= 10){
    //Serial.println("0-1 cm");
    potv = map(potv , 0 , 10 , 0,10);
  //potv = potv ;
  }
  
  if(potv  >= 11 && potv <= 100){
    potv = map(potv , 11,100 , 11 ,20 );
   // Serial.println("1-2 cm");
   }
  
  if(potv  >= 101 && potv <= 442){ 
  //Serial.println("2-3 cm");
  potv = map(potv , 101 , 442 , 21, 30  ); }
  
  if(potv  >= 443 && potv <= 838){
  //Serial.println("3-4 cm");
  potv = map(potv , 443 ,838 ,31 , 40 );}
  
  if(potv  >= 839 && potv <=1006 ){
  //Serial.println("4-5 cm ");
  potv = map(potv , 839 , 1006 , 41, 50);}  

  if(potv  >= 1007  && potv <= 1023 ){
  //Serial.println("5-6.2cm");
  potv = map(potv , 1007,1023 , 51, 62);}

  Serial.println(String("potv:    ")+ String(potv));
  






  
 }*/
