#include <Arduino.h>
#include <stdio.h>
#include <Enes100.h>

#include <math.h>
#include <assert.h>
#include <Servo.h>

#define SOMEVALUE 0.1
#define FORWARD_SPEED 255
#define TURN_SPEED 200
#define SLOW_SPEED 180
#define POOL_DISTANCE 15

//Prototypes
void forward_nav(char axis, int coord, int motor_speed);
void actual_mission();
void servo();
void pool_nav();
void turn(float angle, int motor_speed);
void forward(char axis, int coord, int motor_speed);
void motor_off();
float get_distance();
void obstacle_navigate_distance_sensor();
void begin_mission();
void mission();
void color_sense();
void GetData();
void get_depth();
void release();

//Arduino pin assignments
enum controller_pins {
    in1 = 12,       //drive motors
    in2 = 11,
    in3 = 10,
    in4 = 9,
    ENA = 13,
    ENB = 8,
    trigPin = 24,       //obstacle sensor
    echoPin = 26,
    tx = 53,        //wifi module
    rx = 52,
    salt_in = A1,   //salt voltage
    motor1 = 7,     //syringe motor
    motor2 = 6,
    control = 44,      //servo
    s0 = 16,       //color senser
    s1 = 17,
    s2 = 18,
    s3 = 19,
    out = 20,
    trigPin2 = 22,      //depth sensor
    echoPin2 = 26




};


//initialize all input and output pins

void setup() {
    Serial.begin(9600);
    pinMode(7, OUTPUT);
    pinMode(6, OUTPUT);
    int output[] = {in1, in2, in3, in4, trigPin, s1, s2, s3, motor1, motor2, trigPin2};
    int input[] = {echoPin, out, echoPin2}; 
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
//actual_mission();  
//servo();

Serial.println("Works");

actual_mission();

//right -> in4, in1






while(1);//loop will not continue


}

void forward(char axis, int coord, int motor_speed){
    //Turn on motors forward
    analogWrite(ENA, motor_speed);
    analogWrite(ENB, motor_speed);
    digitalWrite(in1, HIGH);
    digitalWrite(in3, HIGH);
    int position;
    if (axis == 'x')
        position = Enes100.getX();
    else 
        position = Enes100.getY();
    while (abs(position-coord) > SOMEVALUE){
        //delay(200);
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
    motor_off();
    
    
    //while heading of otv is greater than some minimal limit of target angle, turn one direction
    while (abs(heading - angle) > SOMEVALUE){
            //Enes100.println(heading);
            digitalWrite(in1, HIGH);
            digitalWrite(in4, HIGH);
            delay(100);
            motor_off();
            delay(800);
            heading = Enes100.getTheta();

       

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
    Serial.println("Testing");
    Enes100.begin("Wet Willies", WATER, 11, tx, rx);
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
    
    forward_nav('y', coord, FORWARD_SPEED);
    //forward_nav('y', coord, FORWARD_SPEED);
  

    int dist;
    analogWrite(ENA, SLOW_SPEED);
    analogWrite(ENB, SLOW_SPEED);
    while ((dist = get_distance()) > POOL_DISTANCE){
        
        digitalWrite(in2, LOW);
        digitalWrite(in4, LOW);
        digitalWrite(in1, HIGH);
        digitalWrite(in3, HIGH);
    }
    motor_off();
}

void mission(){
    //lower arm
   
    
    servo();
    //for (pos = 0; pos <= 90; pos++) { // goes from 0 degrees to 180 degrees
       // servo.write(pos);              // tell servo to go to position in variable 'pos'
        //delay(20);
    //}   

     
    //get salinity
    String salt;
    
    delay(500);
    //Serial.println("stop");
    while (analogRead(salt_in) > 1000);
    delay(100);
    Enes100.println(analogRead(salt_in));
    Serial.println(analogRead(salt_in));

    salt = (analogRead(salt_in) < 800) ? "salted" : "not salted";
    Enes100.print("This water is: ");
    Enes100.println(salt);

    //get depth
    get_depth();
    
   
    //get water
    digitalWrite(motor1, LOW);
    digitalWrite(motor2, HIGH);
    delay(20000);
    //Unwind motor
    digitalWrite(motor1, LOW);
    digitalWrite(motor2, HIGH);

    //get color
    color_sense();


    delay(10000);





}



void color_sense(){
    int green=0;    
    //This is where we're going to stock our values
   digitalWrite(s0,HIGH);  //Putting S0/S1 on HIGH/HIGH levels means the output frequency scalling is at 100%  (recommended)
   digitalWrite(s1,LOW); //LOW/LOW is off HIGH/LOW is 20% and  LOW/HIGH is  2%
   digitalWrite(s2,HIGH);
   digitalWrite(s3,HIGH);
   Serial.print("Green value= ");

   green=pulseIn(out,LOW);       //here  we wait until "out" go LOW, we start measuring the duration and stops when "out"  is HIGH again
   Serial.print("Green: " );          //it's a time duration measured,  which is related to frequency as the sensor gives a frequency depending on the color
   Serial.println(green);        //The higher the frequency the lower the duration
   delay(2000);
   Serial.println(green > 30? "Green": "Clear");
}





void get_depth(){

    long duration;
    int distance;
    int depth;
    float sum = 0;
    for (int i =0; i < 3; i++, sum+=depth) {
    // Clears the trigPin
    digitalWrite(trigPin, LOW);
    delay(2000);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delay(5000);
    digitalWrite(trigPin, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);
    // Calculating the distance
    distance = duration * 0.034 * 10 / 2 ;
    // Calculating the depth of the water
    depth = 70 - distance;
    // Determine what depth  
    // Prints the distance on the Serial Monitor
    Serial.print("Depth: ");
    Serial.println(depth);
    delay(1000);
    }
    //Sum of depths divided by three for average
    sum /= 3;
    //Prints the average depth on the Serial Monitor
    Serial.print("sum: ");
    Serial.println(sum);
}

void actual_mission(){
    //Initial setup
    motor_off();
    begin_mission();

    //navigate to pool
    pool_nav();
    //mission
    mission();


}

void servo(){

    Servo servo;
    servo.attach(control);
    servo.write(200);
    delay(2000);
    servo.write(90);

}




void forward_nav(char axis, int coord, int motor_speed){
    
    
    int position;
    
    position = Enes100.getY();
    while (abs(position-coord) > SOMEVALUE){
        //delay(200);
        analogWrite(ENA, motor_speed);
        analogWrite(ENB, motor_speed);
        digitalWrite(in1, HIGH);
        digitalWrite(in3, HIGH);
        delay(200);
        position = Enes100.getY();
        if (Enes100.getTheta()  > (M_PI/2 + SOMEVALUE)) {
            analogWrite(ENA, TURN_SPEED);
            analogWrite(ENB, TURN_SPEED);
            digitalWrite(in2, HIGH);
            digitalWrite(in3, HIGH);
            delay(200);
            motor_off();
        }
        if (Enes100.getTheta()  < (M_PI/2 - SOMEVALUE)) {
            analogWrite(ENA, TURN_SPEED);
            analogWrite(ENB, TURN_SPEED);
            digitalWrite(in1, HIGH);
            digitalWrite(in4, HIGH);
            delay(200);
            motor_off();

        }


    }
    motor_off();


    

}