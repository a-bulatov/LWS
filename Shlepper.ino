
#define RUDDER_IN    2 
#define THROTTLE_IN  3
#define MODE_IN      4
#define RUDDER_OUT  9
#define TRACK_LEFT  10
#define TRACK_RIGHT 11
#define WATER_LEFT  5
#define WATER_RIGHT 6

int rudder, throttle, half_throttle, halh_rudder, throttle_left, throttle_right; 
bool mode, prev_mode;

#include <MegaServo.h>
//https://playground.arduino.cc/Code/MegaServo

MegaServo rudder_servo, track_left, track_right, water_left, water_right;

void setup() {
  //Serial.begin(9600*2);
  pinMode(RUDDER_IN, INPUT); // PWM input pin
  pinMode(THROTTLE_IN, INPUT);
  pinMode(MODE_IN, INPUT); 
  mode = pulseIn(MODE_IN, HIGH) > 900;
  prev_mode = !mode;
}

void info() {
  Serial.print(" R "); Serial.print(rudder);  
  Serial.print(" T "); Serial.print(throttle);
  Serial.print(" M "); Serial.print(mode);
  Serial.print(" TR "); Serial.print(throttle_left*2);  
  Serial.print(" TL "); Serial.print(throttle_right*2);
  Serial.println();
}

void loop() {
  mode = pulseIn(MODE_IN, HIGH) > 900;
  rudder = map(pulseIn(RUDDER_IN, HIGH),468,1006,5,40)-3;
  throttle = map(pulseIn(THROTTLE_IN, HIGH),550,1011,0,45);
  //info();
  if(mode) {
    // Сухопутный режим
    if(mode!=prev_mode){
      Serial.println(" Ground");
      track_left.attach(TRACK_LEFT);
      track_right.attach(TRACK_RIGHT);
      rudder_servo.detach();
      water_left.detach();
      water_right.detach();
      prev_mode = mode;
    }
    half_throttle = throttle/2; 
    halh_rudder   = rudder/2;
    if(rudder>=18 && rudder<=20) rudder=19;  
    throttle_left = half_throttle + (22-halh_rudder) -2;
    throttle_right= half_throttle + halh_rudder +2;
    if(throttle_left>=18 && throttle_left<=20) throttle_left=19;
    if(throttle_right>=18 && throttle_right<=20) throttle_right=19;
  
    track_left.write(throttle_left*2);
    track_right.write(throttle_right*2);
  } else {
    // водный режим
    if(mode!=prev_mode){
      Serial.println(" Water");
      track_left.detach();
      track_right.detach();
      rudder_servo.attach(RUDDER_OUT);
      water_left.attach(WATER_LEFT);
      water_right.attach(WATER_RIGHT);
      prev_mode = mode;
    }
    rudder_servo.write(rudder);
    water_left.write(throttle);
    water_right.write(throttle);
  }
}
