#include <Arduino.h>
#include <stdio.h>
#include <Enes100.h>

#include <math.h>
#include <assert.h>

#define SOMEVALUE 4
#define RIGHT 1
#define LEFT 0

//Prototypes
void to_pool();
String mission();
void navigation();
void turn(struct controller_pins *, float angle, int motor_speed);
void forward(struct controller_pins *, char axis, int coord, int motor_speed);
void pool_navigate_aruco(struct controller_pins *pins);
void motor_off(struct controller_pins *);
void approach_pool(struct controller_pins *);
float get_distance(struct controller_pins *);
void pool_navigate_distance_sensor(struct controller_pins *);
void obstacle_navigate_distance_sensor(struct controller_pins *);
void begin_mission(struct controller_pins *);

//Arduino pin assignments
struct controller_pins {
    int in1;
    int in2;
    int in3;
    int in4;
    int ENA;
    int ENB;
    int trigPin;
    int echoPin;
    int tx;
    int rx;
};

//initialize all input and output pins

void setup() {
    int output[] = {12,11,10,9,24,50,52};
    int input[] = {26};
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
    struct controller_pins pins =  {12, 11, 10, 9, 13, 18, 24, 26, 50, 52};
    begin_mission(&pins);
    //drive to pool
    pool_navigate_aruco(&pins);
    approach_pool(&pins);







while(1);//loop will not continue


}

void forward(struct controller_pins *pins, char axis, int coord, int motor_speed){
    analogWrite(pins->ENA, motor_speed);
    analogWrite(pins->ENB, motor_speed);
    digitalWrite(pins->in1, HIGH);
    digitalWrite(pins->in3, HIGH);
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
    motor_off(pins);

    

}

void turn(struct controller_pins *pins, float angle, int motor_speed){
    float heading = Enes100.getTheta();
    analogWrite(pins->ENA, motor_speed);
    analogWrite(pins->ENB, motor_speed);
    //while heading of otv is greater than some minimal limit of target angle, turn one direction
    while (abs(angle - heading) > SOMEVALUE){
        if (angle - heading < 0){
            digitalWrite(pins->in1, HIGH);
            digitalWrite(pins->in4, HIGH);
            

        } else {
            digitalWrite(pins->in2, HIGH);
            digitalWrite(pins->in3, HIGH);
        }

    }
    motor_off(pins);

    


}

void motor_off(struct controller_pins *pins){
    digitalWrite(pins->in1, LOW);
    digitalWrite(pins->in2, LOW);
    digitalWrite(pins->in3, LOW);
    digitalWrite(pins->in4, LOW);
}


void enes_init(){
    //Enes100.begin(const char Wet_Willies, byte WATER, int markerId, int wifiModuleTX, int wifiModuleRX);
    while(!Enes100.isConnected());


}

void pool_navigate_aruco(struct controller_pins *pins){
    //get desired angle and position based on starting location
    float angle;
    float coord;
    //

    //turn to pool angle
    turn(pins, angle, 255);
    //move foward some amount
    forward(pins, 'x', coord, 255);

    //get sensor reading
    
}



  void approach_pool(struct controller_pins *pins){
    int motor_speed;
    int dist = get_distance(pins);
    while (dist > SOMEVALUE){
        analogWrite(pins->ENA, motor_speed);
        analogWrite(pins->ENB, motor_speed);
        digitalWrite(pins->in1, HIGH);
        digitalWrite(pins->in3, HIGH);
        dist = get_distance(pins);
    }
    motor_off(pins);
    

  }

//returns sensor distance in cm
float get_distance(struct controller_pins *pins) {
  float duration, distance;
  digitalWrite(pins->trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pins->trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(pins->trigPin, LOW);

  noInterrupts(); 
  duration = pulseIn(pins->echoPin, HIGH); 
  interrupts();

  distance = (duration*.0343)/2;
  return distance;
  delay(100);
}

void pool_navigate_distance_sensor(struct controller_pins *pins){
  float stop_distance = 17.496;

  //forward
  while(get_distance(pins) > stop_distance){

  }
  motor_off(pins);
}


void obstacle_navigate_distance_sensor(struct controller_pins *pins){
  float stop_distance = 12;

  //forward
  while(get_distance(pins) > stop_distance){

  }
  motor_off(pins);
  turn(pins, 1.57, 255);
  if(get_distance(pins) < stop_distance){
    turn(pins, -1.57, 255);
  }
  else
  {
    //forward
  }
}

  void begin_mission(struct controller_pins *pins){
      //Enes100.begin(const char* teamName, byte teamType, int markerId, int wifiModuleTX, int wifiModuleRX);
  }
