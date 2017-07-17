// Author: Justice Amoh
// Date: 03/29/2017
// Description: Pmod AD1 - AD7476A SPI ADC - Teensy 3.2 Code

#include <SPI.h>

/* -------------------------------------------------------------------- */
/*        Global Variables            */
/* -------------------------------------------------------------------- */

#define BUFFLENGTH 5000

char sMsg[2];  // character string to keep message that is displayed on serial monitor
uint16_t  datBuf[BUFFLENGTH]={0};
char a;
int n;
int timewaiting;
char Handshake=0;
char oneCount=0;


const int CSPin   = 10;
const int MOSIPin = 11;
const int MISOPin = 12;
const int SCKPin  = 14;

int led = 13;
int red = 31;
int green = 32;

// SPI Settings: speed, mode and endianness
SPISettings settings(1000000, MSBFIRST, SPI_MODE2);  // 1MHz, MSB, 


int getADC(){
    int data, data_LB, data_HB;

    SPI.beginTransaction(settings);
    digitalWrite(CSPin,LOW);
    data_HB = SPI.transfer(0);
    data_LB = SPI.transfer(0);
    data =  (data_HB << 8) | data_LB; 
    digitalWrite(CSPin,HIGH);
    SPI.endTransaction();
    return data;
}

void setup(){
    pinMode(led, OUTPUT);
    pinMode(red, OUTPUT);
    pinMode(green, OUTPUT);
  
    pinMode(CSPin, OUTPUT);
    Serial.begin(9600);

    // Configure SPI Pins
    SPI.begin();
    SPI.setMISO(MISOPin);
    SPI.setMOSI(MOSIPin);
    SPI.setSCK(SCKPin);

}




void loop()
{ 
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  
  while (!Handshake){  
    timewaiting = 0;
    sprintf(sMsg, "%u", 16383); // format text to be displayed
    Serial.println(sMsg);  // display text on serial monitor      
    Serial.flush();

    // wait for ACK
    a = 'b';
   // while ((a != 'a') & (timewaiting < 70)){
    
     while ((timewaiting < 70)){  //when no matlab is in use
        a = Serial.read();      

        digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
        delay(100);               // wait for a second
        digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
        delay(900);               // wait for a second 
                         
        Serial.println(timewaiting);  // display text on serial monitor      
        Serial.flush();
        timewaiting++;
    }

    
   //uncomment if using matlab
//   if (timewaiting < 70){ 
      Handshake = 1;
//    }else{
//      for (n=0; n<5; n=n+1){
//        digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
//        delay(50);               // wait for a second
//        digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
//        delay(50);               // wait for a second        
//      }
//      Serial.end(); Serial.begin(9600);  // reset the port
//    }
  }
  while (!oneCount){
      //read into datBuf
    for (n = 0; n < BUFFLENGTH; n=n+1){
      datBuf[n] = getADC();  // read physical value
      delay(12);
     }
  
    float thresh = (4095/3.3)*2;   //voltage level above 2V is seen as a peak
    // float restore = (4095/3.3) *1.65; // the peak area is left when the voltage goes below 1.65
    int flag = 0;
    int breath = 0;
    //calculate the breathing rate
     for (n = 1; n < BUFFLENGTH; n=n+1)
     {
        if ((datBuf[n] <= thresh) && (datBuf[n-1] > thresh) && (flag == 0))
        {
          breath =  breath + 1;
          flag = 1;   
        } else if ( datBuf[n] > thresh){
          flag = 0;
        }  
        Serial.print("Breath count is:");
        Serial.println(breath);
     }
     
     //print the breath count
      Serial.print("Breath rate is:");
      Serial.println(breath);
  
      if (breath > 20)
       digitalWrite(red, HIGH); 
      else
        digitalWrite(green, HIGH); 
      oneCount = 1;
  }
  
  //transmit
//  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
//  for (n = 0; n < BUFFLENGTH; n=n+1){
//    sprintf(sMsg, "%u", datBuf[n]); // format text to be displayed
//    Serial.println(sMsg);  // display text on serial monitor    
//    Serial.flush();    
//   }
//  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW   
  Handshake = 0;
}



