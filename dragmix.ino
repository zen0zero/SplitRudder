#include <Servo.h>

Servo rud_sx;  // create servo object to control a servo
Servo rud_dx;  // create servo object to control a servo

// pwm pins on the nano are: 3, 5, 6, 9, 10, and 11

int PWMMIN = 1000;
int PWMMAX = 2000;
int DEADBAND = 10; // central deadband for rudder input, in usecs

// rudder input pin
int RUD_INPIN = 3; int rudpwm;
int rudcenterpwm;
int offset;

// global airbrake value -- open/closed for both
int airbrake_INPIN = 5;
int airbrake = 0;
int airbrake_present = 0;

int RUD_DXPIN = 9; int dxpwm;
int RUD_SXPIN = 10; int sxpwm;

void setup() {
  Serial.begin(115200);
  delay(2);
  Serial.println("=== Startup ===");
  
  // receiver input for rudder
  pinMode(RUD_INPIN, INPUT);
  delay(10);

  // read initial rudder center value
  rudcenterpwm = pulseIn(RUD_INPIN, HIGH);
  //rudcenterpwm = 1500;
  Serial.print("Rudder PWM, center: ");Serial.println(rudcenterpwm);
  delay(10);

  // do we have some sort of input on the airbrake pin?
  airbrake = pulseIn(airbrake_INPIN, HIGH);
  Serial.print("Airbrake value: ");Serial.println(airbrake);
  if ((airbrake > -20 ) and (airbrake < 2100)) {
    airbrake_present = 1;
    Serial.print("Airbrake present, value: ");Serial.println(airbrake);
  }
  else {
    Serial.print("No Airbrake input: ");Serial.println(airbrake);
    airbrake = 0;
  }

  // servo outputs
  rud_sx.attach(RUD_SXPIN);  // attaches the servo to the servo object
  rud_dx.attach(RUD_DXPIN);  // attaches the servo to the servo object
  delay(5);
  
  // test servo center
  rud_sx.writeMicroseconds(1500);
  rud_dx.writeMicroseconds(1500);
  delay(2000);

  Serial.println("== end start ==");
}

// never exceed sane values
int pwmsanitize(int pwmin){
  int out;
  if ((pwmin > PWMMIN) and (pwmin < PWMMAX)){
    //Serial.print("PWM: ");Serial.println(pwmin);
    out = pwmin;
  }
  else if (pwmin > PWMMAX) {
    Serial.print("*** WARN HIGH PWM: ");Serial.println(pwmin);
    out = 2000;
  }
  else if (pwmin < PWMMIN) {
    Serial.print("*** WARN LOW PWM: ");Serial.println(pwmin);
    out = 1000;
  }
 //Serial.print("PWM out: ");Serial.println(out);
 return out; 
}

void loop() {
  if (airbrake_present) {
  // only allow for half of airbrake command on rudder
  airbrake = (pulseIn(airbrake_INPIN, HIGH)-1000)/2;
  //Serial.print("airbrake: ");Serial.println(airbrake);
  }
  
  rudpwm = pulseIn(RUD_INPIN, HIGH);
  //Serial.print("IN: ");Serial.println(rudpwm);
  
  offset = rudcenterpwm - rudpwm;
  //Serial.print("Offset: ");Serial.println(offset);
  
  // left rudder, positive offset from center
  if (offset > DEADBAND) {
    sxpwm = pwmsanitize(PWMMIN+(offset*2)+airbrake);
    rud_dx.writeMicroseconds(PWMMAX-airbrake);
    rud_sx.writeMicroseconds(sxpwm);
    Serial.print("OUT SX: ");Serial.println(sxpwm);
    Serial.print("OUT DX: ");Serial.println(PWMMAX-airbrake);
  }
  // right rudder, negative offset from center
  else if (offset < -DEADBAND) {
    dxpwm = pwmsanitize(PWMMAX+(offset*2)-airbrake);
    rud_dx.writeMicroseconds(dxpwm);
    rud_sx.writeMicroseconds(PWMMIN+airbrake);
    //Serial.print("OUT DX: ");Serial.println(dxpwm);
    //Serial.print("OUT SX: ");Serial.println(PWMMIN+airbrake);
  }
  // no rudder input, airbrake only
  else {
    rud_dx.writeMicroseconds(PWMMAX-airbrake);
    rud_sx.writeMicroseconds(PWMMIN+airbrake);    
  }
  delay(2);
}
