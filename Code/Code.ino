/ *
 * Control of two stepper motors with Arduino and a Joystick
 *
 * Author: Andrea Lombardo
* Web: http: / / www . Lombard and Andrea . com
 * Post : http : //wp.me/p27dYH-KQ 
 * / 
/ / Inclusion of libraries 
#include <AccelStepper.h> 
#include <Bounce2.h> 
// definition of the Arduino pin constants
const int ledEnable = 13 ;   // the on board led will show us the activation status of the motors
const int pinSwEnable = 7 ;   // the button on the joystick module which enables or disables the control
const int pinEnable = 8 ;   // the pins that control the ENABLE status of the A4988 drivers are connected in series for this, a single pin is enough to manage both
unsigned long debounceDelay = 10 ;   // milliseconds for the button debonuce
const int jX = A0 ;  // analog pin that reads the values ​​for the X
const int stepX = 3 ;   // digital pin that sends STEP signals to the X driver
const int dirX = 4 ;   // digital pin that sends the DIRECTION signal to the X driver
long speedX , valX , mapX ; // motor X movement management variables
const int jY = A1 ;  // analog pin that reads the values ​​for the Ys
const int stepY = 5 ;   // digital pin that sends STEP signals to the Y driver
const int dirY = 6 ;   // digital pin that sends the DIRECTION signal to the Y driver
long speedY , valY , mapY ; // Y motor movement management variables
// variables used by the AccelStepper library
const int maxSpeed = 1000 ;   // according to the library documentation this value can be set up to 4000 for an Arduino UNO
const int minSpeed = 0 ;   // minimum engine speed
const float acceleration = 50.0 ;   // number of steps per second in acceleration
const int treshold = 30 ;   // the reading of the potentiometers is never 100% reliable, this value helps to determine the point to be considered as "stand still" in the movements
long tresholdUp , tresholdDown ; // service variables to accomplish the task described above
boolean enabled , moveX , moveY , enable ; // movement management variables
Bounce btnEnable = Bounce ();   // instantiate a button from the Bounce library
// instantiate the engines
AccelStepper motoreX ( AccelStepper :: DRIVER , stepX , dirX ); 
AccelStepper motorY ( AccelStepper :: DRIVER , stepY , dirY ); 
void setup () { 
  // initialize values
  speedX = speedY = 0 ; 
  enable = false ; 
  // definition of the pin modalities
  pinMode ( ledEnable , OUTPUT );
  pinMode ( pinEnable , OUTPUT );
  pinMode ( pinSwEnable , INPUT_PULLUP ); // the switch input needs to be set as INPUT_PULLUP
  digitalWrite ( ledEnable , enable );
  digitalWrite ( pinEnable ,! enable ); // The A4988 drivers disable the commands to the motor if they receive a HIGH signal on the ENABLE pin for this reason the value is opposite to that of the LED
  // configure the joystick button using the Bounce library
  btnEnable . attach ( pinSwEnable );
  btnEnable . interval ( debounceDelay );
  // calculate range of values ​​within which to consider the position of the joystick as "stand still"
  tresholdDown = ( maxSpeed / 2 ) - treshold ;   
  tresholdUp = ( maxSpeed / 2 ) + treshold ;   
  // configure engine parameters
  engineX . setMaxSpeed ( maxSpeed );
  engineX . setSpeed ( minSpeed );
  engineX . setAcceleration ( acceleration );
  motoreY . setMaxSpeed ( maxSpeed );
  motoreY . setSpeed ( minSpeed );
  motoreY . setAcceleration ( acceleration );
}
void loop () { 
  // perform the function of checking and reading the button that determines the enabling status
  checkEnable ();
  digitalWrite ( ledEnable , enable ); // show enabling status via the led on pin 13
  digitalWrite ( pinEnable ,! enable ); // set opposite value on driver ENABLE pins
  // perform analogical reading of the values ​​coming from the joystick potentiometers
  XVal = analogRead ( jX );
  valY = analogRead ( jY );
  // map the values ​​read as a function of the maximum and maximum speed
  mapX = map ( valX , 0 , 1023 , minSpeed , maxSpeed );  
  mapY = map ( valY , 0 , 1023 , minSpeed , maxSpeed );  
  // run engine command function
  pilotMotors ( mapX , mapY );
}
void pilotMotors ( long mapX , long mapY ) {  
  if ( mapX <= tresholdDown ) {  
    // x goes back
    speedX = - map ( mapX , tresholdDown , minSpeed , minSpeed , maxSpeed ); 
    moveX = true ; 
  } else if ( mapX > = tresholdUp ) {     
    // x goes on
    speedX = map ( mapX , maxSpeed , tresholdUp , maxSpeed , minSpeed );
    moveX = true ; 
  } else {  
    // x stands still
    speedX = 0 ; 
    moveX = false ; 
  }
  if ( mapY <= tresholdDown ) {  
    // y goes down
    speedY = - map ( mapY , tresholdDown , minSpeed , minSpeed , maxSpeed ); 
    moveY = true ; 
  } else if ( mapY > = tresholdUp ) {     
    // y goes up
    speedY = map ( mapY , maxSpeed , tresholdUp , maxSpeed , minSpeed );
    moveY = true ; 
  } else {  
    // y stand still
    speedY = 0 ; 
    moveY = false ; 
  }
  if ( moveX ) {  
    engineX . setSpeed ( SpeedX );
    engineX . run ();
  } else {  
    engineX . stop ();
  }
  if ( moveY ) {  
    motoreY . setSpeed ( Speedy );
    motoreY . run ();
  } else {  
    motoreY . stop ();
  }
}
void checkEnable () { 
  btnEnable . update ();
  if ( btnEnable . fell ()) {  
    enable =! enable ;
  }
}