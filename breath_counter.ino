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

int data_in = analogRead(A14);

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
    Handshake = 1;
  }
  while (!oneCount){
      //read into datBuf
    for (n = 0; n < BUFFLENGTH; n=n+1){
      //datBuf[n] = getADC();  // read physical value
      datBuf[n] = analogRead(A14);
      Serial.println(datBuf[n]);
      Serial.flush();
      delay(12);
     }
  
    float thresh = (1024/3.3)*2;   //voltage level above 2V is seen as a peak
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
      Serial.print("BREATH RATE IS:");
      Serial.println(breath);
  
      if (breath > 20)
       digitalWrite(red, HIGH); 
      else
        digitalWrite(green, HIGH); 
      oneCount = 1;
  }
  
}



