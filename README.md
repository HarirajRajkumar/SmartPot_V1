# SmartPot_V1

  Hey Developer !
   
   Sensors used: 
   1 . DHT 11 for Temperature and Humidity reading
     Temperature LED pin - 5;
     Humidity LED pin - 7;
     
    READS : Temperature , Humidity using DHT11 
   CONTROL :
     Turns on Red LED and Blue LED 
     indicating Temperature and humidity is high in that present environment.
      You can feel free to change the threshold of temperature and humidity in the Threshold Area
      
      
   2 . FC-28 for reading Moisture     
     
     READS :It do reads the moisture content in the soil using a FC-28 
      
      
      LOWER the data HIGHER the Moisture ( eg : 0 gives you HIGH moisture content present in the soil ) 
      and vice-versa.
      
      From the readings it controls : 
      
      If the moisture content is low ie 1023 ( compares with the threshold value ie Moisture_Thres )
      it takes water from the reservoir only when there is water 
      present above the low level and moisture is below the threshold 
      by controlling an air pump using a relay.
    
   3 . TheTankSensor ( DIY )
      
        |~~~~~~|         
   HIGH |=~~~~~|        |=      <= denotes pin
   MED  |=~~~~~|
   LOW  |=~~~~~|
        |~~~~~~| 
        |_____=|    X in Volt   (X<5V with 330 ohm)
       
       Please do refer the circuit given
       
      The Tank Stage Indicator                                        
      READS : Using analog pin A2 , A3 and A4 I managed to read the water present in the reservoir.
       Here Water acts as conductor.. Refer DIY Tank.pdf which is given 
      
                          Digital Pins             ->                D2  D3  D4  
     
     Full - All 3 LEDS hooked on to the D2 , D3 and D4 switches on ( 1   1   1 ) 
     Half - Only 2 LEDS lights up ( ie D3 and D4 )                 ( 0   1   1 )
      Low  - Only D4 is ON                                          ( 0   0   1 )
      
      If the Low level of the tank is 0 then there is no water persent , 
      at that time I used a buzzer to notify user indicating no water present in the reservoir..
 
   ## NOTE : IT'S UNDER DEVELOPMENT.
