/* SmartPot V1 - 12/AUG/2016
 *  
 *  Sensors used : 
 *  1 . DHT 11 for Temperature and Humidity
 *  2 . FC-28 for Moisture reading
 *  
 *  
 *  3 . TheTankSensor
 *     Tank Stages 
 *     Full - All 3 LEDS hooked on to the D2 , D3 and D4 switches on 
 *     Half - Only 2 LEDS lights up ( ie D3 and D4 )
 *     Low  - Only D4 is on
 *  
 *  
 */

/* Libraries included
 *  
 *  DHT 
 *  
 */
#include "DHT.h"

// Class Object 
DHT dht;

// Variables for Timer functions


long previousTimer = 0;
int previousHighTimer = 0;

// State
boolean BuzzState = LOW;
boolean Pump_State = 0;

// previous millis time variable
long BuzzLowTime = 0;


long dhtPreviousTimer = 0;
long TankTimer = 0;

// Threshold Area  - Keep Changing

// Qualifier data type variable name
// const is a qualifier
 const byte Temp_Thres = 30;
 const byte Humi_Thres = 70;
 const int  Moisture_Thres = 200;
 
// Pin Declaration

 const byte Res_L = 2 ;
 const byte Res_M = 3 ;
 const byte Res_H = 4 ;
 const byte Buzz = 9;
 
 const byte MoisturePin = A0;
 
 const byte Temp_Indi = 5;
 const byte Humi_Indi = 7;


 
const byte Pump_Control = 8; // Controls the air pump


void setup()
{
  
  // Begin Serial Communication at 9600 bits per second 
  Serial.begin(9600);
  
  
  
  Serial.println( "Status \t Humidity (%) \t Temperature (C)\t Moisture"); 
  // Prints the MENU 

  // Digital Pin 2 , 3 , 4 as output for Reservoir level indicator
  pinMode ( Res_L , OUTPUT );    // Res_L - 2 LED OUTPUT
  pinMode ( Res_M , OUTPUT );    // Res_M - 3 LED OUTPUT
  pinMode ( Res_H , OUTPUT );    // Res_H - 4 LED OUTPUT
  
  pinMode ( Temp_Indi , OUTPUT );  // pin 5 LED OUTPUT 
  pinMode ( Humi_Indi , OUTPUT );  // pin 7 LED OUTPUT

  pinMode ( Pump_Control , OUTPUT ); // 

  pinMode ( Buzz , OUTPUT );
  
 // Check all the LEDS 
  digitalWrite ( Res_L, HIGH );
  digitalWrite ( Res_H , HIGH );
  digitalWrite ( Res_M, HIGH );
  digitalWrite ( Temp_Indi , HIGH );
  digitalWrite ( Humi_Indi , HIGH );
  digitalWrite ( Buzz , HIGH );
  
  delay(500);
  
  digitalWrite ( Res_L , LOW );
  digitalWrite ( Res_M, LOW );
  digitalWrite ( Res_H, LOW );
  digitalWrite ( Temp_Indi ,  LOW );
  digitalWrite ( Humi_Indi ,  LOW );
  digitalWrite ( Buzz , LOW );

  
  delay(500);

  
  /* Analog pin A0 is used to read the amount of moisture present in the soil
   *  
   *  When Moisture  content is High in the soil - 0
   *                 and Low - 1024 
   */
  pinMode ( MoisturePin , INPUT );

  
  dht.setup(6); // data pin 6

  
}

void loop()
{ 
  unsigned long MilliTimer = millis() ;
  
  int Moisture = analogRead ( A0 );
  Serial.print ( Moisture );
  Serial.println ("\t");
   
  /*
   * This part of the code reads the amount of water present in the reservoir
   * 
   */
   if ( MilliTimer - TankTimer > 2000 )
   {
    
     TankTimer = MilliTimer ;     
     float V_Low = (analogRead(A1)*5.12)/1024;
    // Serial.print (V_Low );
    // Serial.print ( "\t");
  
 if ( V_Low >= 1 ){
    digitalWrite ( 2 , HIGH ); 
    
    if ( BuzzState == HIGH )
  { 
    BuzzState = LOW ; 
  }
  digitalWrite ( Buzz , BuzzState );

   if ( Moisture < Moisture_Thres ){
   Pump_State = LOW;
    digitalWrite ( Pump_Control , Pump_State );
   }
  else if ( (Moisture > Moisture_Thres)  )
 {    
 if ( MilliTimer - previousTimer > 10000 )  // Pump relay ON for 10 seconds
  // Moisture
  {
   previousTimer = MilliTimer ; 
    if ( Pump_State == LOW )
    {
      Pump_State = HIGH ;
      }
    else{
      Pump_State = LOW ;
      }   
   digitalWrite ( Pump_Control , Pump_State );    
 }
 }
}// close moisture above the threshold check
 
 
  else if (  V_Low < 1 ){
    
    digitalWrite ( 2 , LOW );
    Pump_State = 0;
    digitalWrite ( Pump_Control , Pump_State );
    
  if( MilliTimer - BuzzLowTime >= 3000 )  //Buzzer Beep for 3 seconds
    {
       BuzzLowTime = MilliTimer ;
      
       if ( BuzzState == LOW )
       {
         BuzzState = HIGH ; 
       }
       else if ( BuzzState == HIGH )
       { 
         BuzzState = LOW ;
       }
        digitalWrite ( Buzz , BuzzState );
    } // Close buzzer 3 seconds
    
  }// Close vlow The low one    
     
   }
  
  
  float V_Mid = (analogRead ( A2 )*5.12)/1024;
 // Serial.print (V_Mid );
  //Serial.print ( "\t");

 if ( V_Mid >= 1 )
  digitalWrite ( 3 , HIGH );
  else if ( V_Mid == 0 && V_Mid < 1 )
  digitalWrite ( 3 , LOW );
  

  float V_High = (analogRead ( A3 )*5.12)/1024;
  //Serial.println (V_High);

  if ( V_High == 0 && V_High <= 0.5 )
  digitalWrite ( 4 , LOW );
  else if ( V_High >= 1 )
  digitalWrite ( 4 , HIGH );
// Close 2 Seconds water level check   


 // Water level detecting code ends here
 
// Checking DHT for every 3 seconds
  if ( MilliTimer - dhtPreviousTimer > 3000)
{
  dhtPreviousTimer = MilliTimer ;
  
  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();

  Serial.print(dht.getStatusString());
  Serial.print("\t");
  Serial.print(humidity, 1);
  Serial.print("\t\t");
  Serial.print(temperature, 1);
  Serial.print("\t\t");
  Serial.println(Moisture);
  

  // Threshold value for the temperature LED to turn on is 30*C
   if ( temperature >= Temp_Thres )
   digitalWrite ( Temp_Indi , HIGH );
   else 
   digitalWrite ( Temp_Indi , LOW );

  // Similarly for Humidity above 70%
   boolean State = 0;
   State = ( humidity  > Humi_Thres ) ? HIGH : LOW ;
   digitalWrite ( Humi_Indi , State );
}// Close DHT Sensing

}
    
     
  
      


