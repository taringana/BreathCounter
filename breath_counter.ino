// Author: Justice Amoh
// Date: 03/29/2017
// Description: Pmod AD1 - AD7476A SPI ADC - Teensy 3.2 Code

//Edited: Taringana Guranungo 
//Date: 07/17/2017
//Description: Breathing Rate counter for a PZT circuit


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

int led = 13;
int red = 31;
int green = 32;

void setup(){
    pinMode(led, OUTPUT);
    pinMode(red, OUTPUT);
    pinMode(green, OUTPUT);
    pinMode(A14, INPUT);
  
    Serial.begin(9600);
}

void loop()
{ 
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)

  //wait for 20 seconds while the pzt gets stable before starting to get data
  while (!Handshake){  
    timewaiting = 0;
    sprintf(sMsg, "%u", 16383); // format text to be displayed
    Serial.println(sMsg);  // display text on serial monitor      
    Serial.flush();

    
     while ((timewaiting < 70)){  //when no matlab is in use

        digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
        delay(100);               // wait for 0.1 second
        digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
        delay(900);               // wait for 0.9 second 
                         
        Serial.println(timewaiting);  // display text on serial monitor      
        Serial.flush();
        timewaiting++;
    }
    Handshake = 1;
  }
  while (!oneCount){

    float thresh = (1024/3.3)*2;   //voltage level above 2V is seen as a peak
    // float restore = (4095/3.3) *1.65; // the peak area is left when the voltage goes below 1.65
    int flag = 0;
    int breath = 0;
      
    //read into datBuf
    datBuf[0] = analogRead(A14);
    Serial.println(datBuf[n]);
    Serial.flush();
    delay(12);  //delay by 12milli seconds so that 5000samples are recorded in one minute 
    
    for (n = 1; n < BUFFLENGTH; n=n+1){
      //datBuf[n] = getADC();  // read physical value
      datBuf[n] = analogRead(A14);
       if ((datBuf[n] <= thresh) && (datBuf[n-1] > thresh) && (flag == 0))
      {
          breath =  breath + 1;
          flag = 1;  
          digitalWrite(green, HIGH);
      } else if ( datBuf[n] > thresh){
          flag = 0;
          digitalWrite(green, LOW);
      }  
      Serial.print("Breath count is:");
      Serial.println(breath);
      //Serial.println(datBuf[n]);
      Serial.flush();
      delay(12);  //delay by 12milli seconds so that 5000samples are recorded in one minute
     }
  

    //calculate the breathing rate
    for (n = 1; n < BUFFLENGTH; n=n+1)
    {
//      if ((datBuf[n] <= thresh) && (datBuf[n-1] > thresh) && (flag == 0))
//      {
//          breath =  breath + 1;
//          flag = 1;   
//      } else if ( datBuf[n] > thresh){
//          flag = 0;
//      }  
//      Serial.print("Breath count is:");
//      Serial.println(breath);
     }
     
     //print the breath count
     Serial.print("BREATH RATE IS:");
     Serial.println(breath);
  
     if (breath > 20)
        digitalWrite(red, HIGH); 
     else
        digitalWrite(green, HIGH); 
     oneCount = 1;
  }
  
}



