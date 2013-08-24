// INTERVALOCAM4.ino

#include <Wire.h>                       // I2C Library
#include <LiquidCrystal_I2C.h>          // I2C LCD Library

LiquidCrystal_I2C lcd(0x27,16,2);

// Intervaluino weeg Edition
// this code programs the Arduino as an intervalometer for a Canon EOS camera
// this enables you to create time-lapse movies with your camera.
// It features a LCD Display and two Pushbuttons to setup the interval and the
// amount of shots, Manual Mode, Foot Switch and Focus off/on
// It uses two EL817 Optocouplers to focus and release the shutter
//
//
// based on (c) Lord Yo 2008 (intervaluino a_t sporez do:t com) and extended
// by Max from MaxTechTV. Even more extented by weeg
// Licensed under a Creative Commons (Attribution Non-Commercial Share-Alike) license


// define pins
int buttonLeft = 2;                     // left button (Back / Focus / No)
int buttonSet = 3;                      // middle button (Set / Shutter)
int buttonRight = 4;                    // right button (Forward / Focus / No)
int switchManual = 6;                   // manual mode switch
int switchFocus = 7;                    // focus switch
int focus = 9;                          // goes to input of second EL817
int shutter = 10;                       // goes to input of first EL817

int footPedal = 11;                     // goes to input of foot Pedal

int focusLed = 12;                       // status LED focus 

boolean buttonRight_state = false;      // holds if a button is pressed or not
boolean buttonSet_state = false;
boolean buttonLeft_state = false;
boolean footPedal_state = false;

int intervalmode = 42;                  // used for intervalmode menu (m/s)
int mode = 42;                          // used to navigate trough the GUI
int checkmode ;                         // used to navigate trough the GUI

int shutter_on = 300;                   // how long the shutter is "pressed" (not how long the shutter is open!)
int focus_on = 400;                     // time to let the camera get focus.
int wait = 200;                         // time between focus and shutter

long interval;                          // start interval
int pics = 1;                           // amount of pics

//////////////////////
// setup runs once

void setup() {

  Serial.begin(9600);

  // initialize the lcd
  lcd.init();                              
  lcd.backlight();
  lcd.begin(16, 2);

  // show startscreen
  lcd.print(" INTERVALOMETER");
  lcd.setCursor(0,1);
  lcd.print("     READY ");
  delay(1000);

  // set pin mode
  pinMode(shutter, OUTPUT);             // Triggers the Shutter
  pinMode(focus, OUTPUT);               // Triggers the focus
  pinMode(buttonRight, INPUT_PULLUP);   // Connects buttonRight to intern PullUp Resistor
  pinMode(buttonSet, INPUT_PULLUP);     // Connects buttonSet to intern PullUp Resistor
  pinMode(buttonLeft, INPUT_PULLUP);    // Connects buttonSet to intern PullUp Resistor
  pinMode(switchFocus,INPUT_PULLUP);    // Status LED
  pinMode(switchManual,INPUT_PULLUP);   // Status LED
  pinMode(footPedal,INPUT_PULLUP);   // Status LED
 
   pinMode(focusLed,OUTPUT);             // Status LED
   pinMode(focusLed,OUTPUT);             // Status LED
 
  
    buttoncheck();

  lcd.clear();
}

/////////////
// loop runs forever

void loop() {

  ////////////////////////// MANUAL MODE ///////////////////////////
  // Trigger Focus and/or Shutter manually                        //
  // Right/Left Button Focus, Middle (Set) Button Shutter         //                                          TEXT          //
  //////////////////////////////////////////////////////////////////

focuscheck();

if (mode == 100){                       // set mode back
mode = 42;
}

  if (digitalRead(switchManual) == LOW) {     // check manual/ interval switch
    lcd.print("   MANUAL MODE");
    lcd.setCursor(0,1);
    lcd.print(" Foc./Shut./Foc.   ");
    delay(100);

    buttoncheck();
    // trigger focus      
    if (buttonLeft_state == true || buttonRight_state == true){  // trigger focus 
      lcd.setCursor(0,1);
      lcd.print("FOCUS           ");
      digitalWrite(focus, HIGH);        // turn focus on
      delay(focus_on);                  // keep focus
      digitalWrite(focus, LOW);         // turn focus off
      delay(100);                      //wait
    }
    // trigger shutter
    if (buttonSet_state == true){                  // trigger shutter
      lcd.setCursor(0,1);
      lcd.print("SHOOTING           ");
      digitalWrite(shutter, HIGH);  //press the shutter
      delay(shutter_on);            //wait the shutter release time
      digitalWrite(shutter, LOW);    //turn of status led
      delay(1000);                  //wait
    }

    // trigger shutter with Foot Pedal
    if (footPedal_state == true){          // trigger shutter with Foot Pedal
      focuscheck();
      if (digitalRead(switchFocus) == LOW) {
        digitalWrite(focus, HIGH);    // turn focus on
        delay(focus_on);              // keep focus
        digitalWrite(focus, LOW);     // turn focus off
        delay(wait);                  //wait
      }
      digitalWrite(shutter, HIGH);  //press the shutter
      //    digitalWrite(led,HIGH);        //turn on status led
      delay(shutter_on);            //wait the shutter release time
      digitalWrite(shutter, LOW);    //turn of status led
      }
              
}

  else{                 // Interval mode
        lcd.clear();
      lcd.print("  INTERVAL MODE");
 //   delay(1000);
    lcd.clear();

    // interval mode

    //////////////////////// SET INTERVAL TIME MODE////////////////////
    // Press Left Button for Seconds, Right Button for Minutes       //
    ///////////////////////////////////////////////////////////////////

    while (mode == 42){
focuscheck();
    lcd.print("  INTERVAL MODE");
      lcd.setCursor(0,1);
      lcd.print("  < sec / min >");
delay(1000);
      while (mode == 42){
        buttoncheck();

  if (digitalRead(switchManual) == LOW) {     // check switch
    lcd.clear();
lcd.print("   MANUAL MODE");
        delay(1000);
    lcd.clear();
mode = 100;
}


        if (buttonRight_state==true) {
//          lcd.clear();
//          lcd.print("Set Interval m/s < >");
          lcd.setCursor(0,1);
          lcd.print("Interval min OK?      ");
          intervalmode = 43;
        } 

        if (buttonLeft_state==true) {
  //        lcd.clear();
 //         lcd.print("Set Interval m/s < >");
          lcd.setCursor(0,1);
          lcd.print("Interval sec OK?");
          intervalmode = 0;
        } 

        if (buttonSet_state== true){
          mode = intervalmode;
          lcd.clear();
          delay(500);
        } 
      }     
    }

    /////////////////////// SET INTERVAL MINUTE MODE //////////////////
    // Press&Hold Button 1 to increase Time, Confirm with Button 2  //
    //////////////////////////////////////////////////////////////////

    if (mode == 43){
      interval = 60000;
    } 
    else{
        interval =1000;}   

    while (mode == 43){
    focuscheck();
      lcd.print("Interval in min");
      while (mode == 43){
        lcd.setCursor(0,1);
        lcd.print("      min");
        lcd.setCursor(0,1);
             lcd.print((interval/60000));
        //      lcd.print(".");
        //      lcd.print(interval%(3600));
        delay(70);
        buttoncheck();

        if (buttonRight_state==true) {
          interval = interval+30000;
        } 
        if (buttonLeft_state==true && interval > 60000) {
          interval = interval-30000;
        } 

        if (buttonSet_state== true){
          mode = 1;
          checkmode = 43;
          lcd.clear();
          delay(500);
        } 
      } 
    }

    ////////////////////// SET INTERVAL SECOND MODE///////////////////
    // Press&Hold Button 1 to increase Time, Confirm with Button 2  //
    //////////////////////////////////////////////////////////////////

    while (mode == 0){
    focuscheck();
      lcd.print("Interval in sec");
      while (mode == 0){
        lcd.setCursor(0,1);
        lcd.print("      sec");
        lcd.setCursor(0,1);
        lcd.print((interval/1000));
        lcd.print(".");
        lcd.print(interval%1000);
        delay(100);
        buttoncheck();

        if (buttonRight_state==true) {
          interval = interval+100;
        } 
        if (buttonLeft_state==true && interval > 1000) {
          interval = interval-100;
        } 

        if (buttonSet_state== true){
          mode = 1;
          checkmode = 0;
          lcd.clear();
          delay(500);
        } 
      } 
    }

    //////////////////////// CONFIRM INTERVAL ////////////////////////
    //   Press Button 1 to go back or Press Button 2 to Confirm     //
    //////////////////////////////////////////////////////////////////

    while (mode == 1){
focuscheck();
    
      lcd.print("Your Interval:");
      lcd.setCursor(0,1);

      // min mode   
      if (checkmode == 43){
        lcd.print((interval/60000));
        lcd.setCursor(5,1);
        lcd.print("min");
        //   lcd.print(".");
        //   lcd.print(interval%1000); 
        lcd.setCursor(10,1);
        lcd.print("OK?");
        delay(50);
      }

      // sec mode   
      if (checkmode == 0){
        lcd.print((interval/1000));
        lcd.print(".");
        lcd.print(interval%1000); 
        lcd.setCursor(6,1);
        lcd.print("sec");
        lcd.setCursor(13,1);
        lcd.print("OK?");
        delay(50);
      }

      while ( mode == 1){
        buttoncheck();
focuscheck();
    
        if (buttonRight_state == true || buttonLeft_state == true){
          if (checkmode == 43){
            mode = 43;
            interval = 60000;
          }
          else if (checkmode == 0){
            mode = 0;
            interval = 1000;
          }
          lcd.clear();
          delay(500);
        }
        if (buttonSet_state == true){
          mode = 3;
          interval = interval-700;
          lcd.clear();
          delay(500);
        }
      } 
    }


    /////////////////////////// SET AMOUNT /////////////////////////////
    // Press&Hold Button 1 to increase Amount, Confirm with Button 2  //
    ////////////////////////////////////////////////////////////////////

    while (mode == 3){
    focuscheck();
      lcd.print("Amount of Shots");
      while (mode == 3){

        lcd.setCursor(0,1);
        lcd.print("     Shots");
        lcd.setCursor(0,1);
        lcd.print(pics);
        delay(100);
        buttoncheck();

        if (buttonRight_state==true) {
          pics++;
        } 
        if (buttonLeft_state==true && pics > 1) {
          pics--;
        } 

        if (buttonSet_state== true){
          mode = 4;
          lcd.clear();
          delay(500);
        } 
      } 
    }

    ///////////////////////// CONFIRM AMOUNT /////////////////////////
    //   Press Button 1 to go back or Press Button 2 to Confirm     //
    //////////////////////////////////////////////////////////////////

    while (mode == 4){
focuscheck();
    
      lcd.print("Amount of Pics:");
      lcd.setCursor(0,1);
      lcd.print(pics); 
      lcd.setCursor(5,1);
      lcd.print("OK?");
      delay(50);

      while ( mode == 4){
        buttoncheck();

        if (buttonRight_state == true || buttonLeft_state == true){
          mode = 3;
          pics = 1;
          lcd.clear();
          delay(500);
        }
        if (buttonSet_state == true){
          mode = 5;
          lcd.clear();
          delay(500);
        }
      } 
    }


    //////////////////// WAIT TO START SEQUENCE //////////////////////
    //                Countdown to start Sequence                   //
    //////////////////////////////////////////////////////////////////

    while (mode == 5){
focuscheck();
    
      lcd.print("START?");
      lcd.setCursor(0,1);
      lcd.print("PRESS BUTTON");
      delay(50);

      while ( mode == 5){
        buttoncheck();
        if (buttonSet_state == true){
          lcd.clear();      
          lcd.print("Start in :");    




          for (int k = 3 ; k>0; k--){
            lcd.setCursor(1,1);
            lcd.print(k);
            delay(1000);
          }
          Serial.print(interval);
          mode =6;
          lcd.clear();
          lcd.print("SHOOTING");
        } 
      }
    }

    /////////////////////// SHOOTING SEQUENCE ////////////////////////
    //   Press Button 1 to go back or Press Button 2 to Confirm     //
    //////////////////////////////////////////////////////////////////

    while (mode == 6){
        if (checkmode == 43){
      lcd.setCursor(9,0);
      lcd.print("Pc   ");      
      lcd.setCursor(12,0);
      lcd.print(pics);            
      lcd.setCursor(0,1);
      lcd.print(pics*((interval+700)/60000));
      lcd.print("    min left    ");
      }

      else if (checkmode == 0){
      lcd.setCursor(9,0);
      lcd.print("Pc    ");      
      lcd.setCursor(12,0);
      lcd.print(pics); 
      lcd.setCursor(0,1);
      lcd.print(pics*((interval+700)/1000));
      lcd.print("    secs left    ");
      }


      buttoncheck();
      if (digitalRead(switchFocus) == LOW) {
        digitalWrite(focus, HIGH);    // turn focus on
        delay(focus_on);              // keep focus
        digitalWrite(focus, LOW);     // turn focus off
        delay(wait);                  //wait
      }
      digitalWrite(shutter, HIGH);  //press the shutter
      //    digitalWrite(led,HIGH);        //turn on status led
      delay(shutter_on);            //wait the shutter release time
      digitalWrite(shutter, LOW);    //turn of status led
      //    digitalWrite(led,LOW);        //release shutter
      pics--;                       //count pictures down
      delay(interval);              //wait for next round

      if (pics == 0){
        mode=7;
        lcd.clear();
        lcd.print("FINISHED...");
        lcd.setCursor(0,1);
        lcd.print("NEW ONE, OK?");
        delay(50);

        while (mode == 7){
          buttoncheck();

          if (buttonSet_state == true){
            lcd.clear();
            mode=42;
            interval = 1000;            // start interval
            pics = 1;                   // amount of pics
          }
        }
      } 
    }

  }
}

void buttoncheck(){
  buttonRight_state = !digitalRead(buttonRight); // read state of Button 1
  buttonSet_state = !digitalRead(buttonSet); // read state of Button 2
  buttonLeft_state = !digitalRead(buttonLeft); // read state of Button 2
  footPedal_state = !digitalRead(footPedal); // read state of Button 2
  //  switchFocus_state = !digitalRead(switchFocus); // read state of Button 2
  //  switchManual_state = !digitalRead(switchManual); // read state of Button 2
}

void focuscheck(){

if (digitalRead(switchFocus) == LOW) {  // turn LED on when Focus on
digitalWrite(focusLed, HIGH);
}
else{
digitalWrite(focusLed, LOW);            // turn LED off when Focus off
}
}