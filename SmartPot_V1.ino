/* SmartPot V1 - 12/AUG/2016
 * 
 * Developed by Hariraj.R an arduino startup.
 *
 * Hey Developer !
 *  
 *  Sensors used: 
 *  1 . DHT 11 for Temperature and Humidity reading
 *    Temperature LED pin - 5;
 *    Humidity LED pin - 7;
 *    
 *     READS : Temperature , Humidity using DHT11 
 *     CONTROL :
 *     Turns on Red LED and Blue LED 
 *     indicating Temperature and humidity is high in that present environment.
 *     You can feel free to change the threshold of temperature and humidity in the Threshold Area
 *     
 *     
 *  2 . FC-28 for reading Moisture     
 *    
 *    READS :It do reads the moisture content in the soil using a FC-28 
 *     
 *     
 *     LOWER the data HIGHER the Moisture ( eg : 0 gives you HIGH moisture content present in the soil ) 
 *     and vice-versa.
 *     
 *     From the readings it controls : 
 *     
 *     If the moisture content is low ie 1023 ( compares with the threshold value ie Moisture_Thres )
 *     it takes water from the reservoir only when there is water 
 *     present above the low level and moisture is below the threshold 
 *     by controlling an air pump using a relay.
 *   
 *  3 . TheTankSensor ( DIY )
 *     
 *      |~~~~~~|         
 * HIGH |=~~~~~|        |=      <= denotes pin
 * MED  |=~~~~~|
 * LOW  |=~~~~~|
 *      |~~~~~~| 
 *      |_____=|    X in Volt   (X<5V with 330 ohm)
 *      
 *      Please do refer the circuit given
 *      
 *     The Tank Stage Indicator                                        
 *     READS : Using analog pin A2 , A3 and A4 I managed to read the water present in the reservoir.
 *      Here Water acts as conductor.. Refer DIY Tank.pdf which is given 
 *     
 *                          Digital Pins             ->                D2  D3  D4  
 *     
 *     Full - All 3 LEDS hooked on to the D2 , D3 and D4 switches on ( 1   1   1 ) 
 *     Half - Only 2 LEDS lights up ( ie D3 and D4 )                 ( 0   1   1 )
 *     Low  - Only D4 is ON                                          ( 0   0   1 )
 *     
 *     If the Low level of the tank is 0 then there is no water persent , 
 *     at that time I used a buzzer to notify user indicating no water present in the reservoir..
 *
 *   NOTE : IT'S UNDER DEVELOPMENT.
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

// ------------------Threshold Area  - Keep Changing----------------------------

 const byte Temp_Thres = 30;
 const byte Humi_Thres = 70;
 const int  Moisture_Thres = 200;
 
// --------------ONLY HUMANS ALLOWED IN HERE ;-)----------------------------
 
// Pin Declaration

// Reservoir variable declaration
 const byte Res_L = 2 ; // Reservoir LOW 
 const byte Res_M = 3 ;
 const byte Res_H = 4 ;
 
 const byte Res_Read_L = A1;
 const byte Res_Read_M = A2;
 const byte Res_Read_H = A3;
 
 const byte Buzz = 9;
 
 const byte MoisturePin = A0;
 
 const byte Temp_Indi = 5;
 const byte Humi_Indi = 7;


 
const byte Pump_Control = 8; // Controls the air pump


void setup()
{
  
  // Begin Serial Communication at 9600 bits per second 
  Serial.begin(9600);
  
  

  Serial.println ( " Version 1 . Date 14-AUG-2016 ");
  Serial.println (" Works by reading only on these parameters ");
  Serial.println (" TEMPERATURE \n HUMIDITY \n MOISTURE \n");
  Serial.println( " Status \t Humidity(%)  \t Temperature (C)  \t Moisture"); 
  
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

  
  dht.setup(6); // digital pin 6  
}


void loop()
{ 
  unsigned long MilliTimer = millis() ; // Start Millis timer
  int Moisture = analogRead ( A0 ); 
  
  /* Reads Moisture level
   *  LOWER the value HIGHER the moisture ( ex : 0 gives you higher moisture )
   *  and vice-versa
   */
  
 // Uncomment to see the value of the moisture
 
  //Serial.print ( Moisture );
  //Serial.println ("\t");
   
  /*
   * This part of the code reads the amount of water present in the reservoir
   * 
   */
   if ( MilliTimer - TankTimer > 2000 ) // Reads for every two seconds
   {
    
    TankTimer = MilliTimer ;     
    
     float V_Low = (analogRead(Res_Read_L)*5.12)/1024;

    // Serial.print (V_Low );
    // Serial.print ( "\t");


  /* If the pin present at the bottom of the tank reads 
   * State 1 : 0 ( 0v ) indicates no water !
   *        DO : 
   *     1.1. Buzzer on and off for 3 seconds
   *     1.2. Switch off the pump 
   *     1.3. Switches off low level reservoir indicator LED
   * State 2 : 1 ( 5v ) indicates presence of water !!
   *       DO : 
   *     2.1. Turn on Air pump if the moisture goes below the threshold value for 10 seconds
   *     2.2. Turn on low level reservoir indicator LED
   *     2.3. Switches off buzzer
   */    
 if ( V_Low >= 1 )  // Indicates water is present
 {
    digitalWrite ( Res_L , HIGH ); // Low level LED indicator ON (2.2)
    
    if ( BuzzState == HIGH )  // Turns off Buzzer if it is HIGH ( 2.3 )
  { 
    BuzzState = LOW ; 
  }
  digitalWrite ( Buzz , BuzzState );
  
// If the moisture is below the threshold turns off Pump
   if ( Moisture < Moisture_Thres ){
   Pump_State = LOW;
    digitalWrite ( Pump_Control , Pump_State );
   }// close low moisture
   
// and turns ON the pump only when the moisture is above the moisture threshold for 3 seconds
// Note : You can change the threshold value.. It's all yours

  else if ( (Moisture > Moisture_Thres)  )  // (2.1)
 {    
 if ( MilliTimer - previousTimer > 10000 )  // Air Pump ON for 10 seconds through relay 
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
 }// close high moisture
}// close moisture above the threshold check
 

  
  else if (  V_Low < 1 ) // Indicates water is not there
  {
    
    digitalWrite ( Res_L , LOW );  // ( 1.3 )
    Pump_State = 0;
    digitalWrite ( Pump_Control , Pump_State ); // (1.2)
    
  if( MilliTimer - BuzzLowTime >= 3000 )  //Buzzer Beep for 3 seconds (1.1)
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
     
    // Close read for every two seconds
  
  
  float V_Mid = (analogRead ( Res_Read_M )*5.12)/1024;
  
 // Serial.print (V_Mid );
  //Serial.print ( "\t");

 if ( V_Mid >= 1 )
  digitalWrite ( Res_M , HIGH );
  else if ( V_Mid == 0 && V_Mid < 1 )
  digitalWrite ( Res_M , LOW );
  

  float V_High = (analogRead ( Res_Read_H )*5.12)/1024;
  //Serial.println (V_High);

  if ( V_High == 0 && V_High <= 0.5 )
  digitalWrite ( Res_H , LOW );
  else if ( V_High >= 1 )
  digitalWrite ( Res_H , HIGH );
   }// Close 2 Seconds water level check   


 // Water level detecting code ends here
 
// Read data from DHT for after every 3 seconds 
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

}// Close loop
    
     
  
      


