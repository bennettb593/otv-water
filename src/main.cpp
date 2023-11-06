#include <Arduino.h>
#include <stdio.h>
#include <Enes100.h>

#include <math.h>
#include <assert.h>

#define SOMEVALUE 4
#define FORWARD_SPEED 255
#define TURN_SPEED 133
#define SLOW_SPEED 133

//Prototypes
void pool_nav();
void turn(float angle, int motor_speed);
void forward(char axis, int coord, int motor_speed);
void motor_off();
float get_distance();
void obstacle_navigate_distance_sensor();
void begin_mission();
void mission();

//Arduino pin assignments
enum controller_pins {
    in1 = 12,
    in2 = 11,
    in3 = 10,
    in4 = 9,
    ENA = 13,
    ENB = 18,
    trigPin = 24,
    echoPin = 26,
    tx = 53,
    rx = 52,
    salt_in = A0,
    salt_out


};


//initialize all input and output pins

void setup() {
    int output[] = {in1, in2, in3, in4, trigPin};
    int input[] = {echoPin};
    int num_output = sizeof(output) / 2;
    int num_input = sizeof(input) / 2;


    for (int i = 0; i < num_output; i++) {
        pinMode(output[i], OUTPUT);
    }
    for (int i = 0; i < num_input; i++){
        pinMode(input[i], INPUT);
    }
}


void loop() {
    //Initial setup
    motor_off();
    begin_mission();

    //navigate to pool
    pool_nav();
    //mission
    mission();








while(1);//loop will not continue


}

void forward(char axis, int coord, int motor_speed){
    //Turn on motors forward
    analogWrite(ENA, motor_speed);
    analogWrite(ENB, motor_speed);
    digitalWrite(in1, HIGH);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    digitalWrite(in2, LOW);
    int position;
    if (axis == 'x')
        position = Enes100.getX();
    else 
        position = Enes100.getY();
    while (abs(position-coord) > SOMEVALUE){
        if (axis == 'x')
            position = Enes100.getX();
        else 
            position = Enes100.getY();

    }
    motor_off();

    

}

void turn(float angle, int motor_speed){
    float heading = Enes100.getTheta();
    analogWrite(ENA, motor_speed);
    analogWrite(ENB, motor_speed);

    digitalWrite(in1, LOW);
    digitalWrite(in4, LOW);
    digitalWrite(in1, LOW);
    digitalWrite(in4, LOW);
    //while heading of otv is greater than some minimal limit of target angle, turn one direction
    while (abs(angle - heading) > SOMEVALUE){
        if (angle - heading < 0){
            digitalWrite(in1, HIGH);
            digitalWrite(in4, HIGH);
          
            

        } else {
            digitalWrite(in2, HIGH);
            digitalWrite(in3, HIGH);
        }

    }
    motor_off();

    


}

void motor_off(){
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
}







//returns sensor distance in cm
float get_distance() {
  float duration, distance;
  delay(100);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(200);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(1000);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH); 
  distance = (duration*.0343)/2;
  return distance;

}


void obstacle_navigate_distance_sensor(){
  float stop_distance = 12;

  //forward
  while(get_distance() > stop_distance){

  }
  motor_off();
  turn(1.57, 255);
  if(get_distance() < stop_distance){
    turn(-1.57, 255);
  }
  else
  {
    //forward
  }
}

  void begin_mission(){
    Enes100.begin("Wet Willies", WATER, 19, tx, rx);
    Enes100.println("Connected to vision system.");
  }





void pool_nav(){


    /*
    
    
    drive to pool

    */

    
   
    //get desired angle and position based on starting location
    float angle;
    float coord;

    if (Enes100.getY() < 1){
        angle = M_PI/2;
        coord = 1.2;
    }else{
        angle = -M_PI/2;
        coord = 0.8;
    }

    Enes100.print("Drive to angle: ");
    Enes100.println(angle);
    Enes100.println("Drive to y: ");
    Enes100.println(coord);
    //turn to pool angle
    turn(angle, TURN_SPEED);
    //move foward some amount
    forward('y', coord, FORWARD_SPEED);
  

    int dist;
    analogWrite(ENA, SLOW_SPEED);
    analogWrite(ENB, SLOW_SPEED);
    while ((dist = get_distance()) > SOMEVALUE){
        
        digitalWrite(in2, LOW);
        digitalWrite(in4, LOW);
        digitalWrite(in1, HIGH);
        digitalWrite(in3, HIGH);
    }
    motor_off();
}

void mission(){
    //get depth
    String salt;


    //get salinity
    digitalWrite(salt_out, HIGH);
    delay(500);
    while (analogRead(salt_in) < SOMEVALUE);
    delay(1000);
    //Enes100.println(analogRead(salt_in));
    salt = (analogRead(salt_in) > 400) ? "salted" : "not salted";
    Enes100.print("This water is: ");
    Enes100.println(salt);



}

