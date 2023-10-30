

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


void setup() {
  Serial.begin(9600);
  
//initialize all input and output pins
    struct controller_pins pins =  {12, 11, 10, 9, 13, 18, 24, 26, 50, 52};
    int output[] = {9,12,13,11,10,3,13,18, 48, 52, 50};
    int input[] = {4,5, 26};
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
    struct controller_pins pins =  {12, 11, 10, 9, 13, 18, 24, 26, 50, 52, 48};

    forward(&pins, 255);
    if (get_distance(&pins) <= 4){
      motor_off(&pins);
      turn(&pins, "RIGHT", 133);
      delay(1000);
    }
  motor_off(&pins);


}

void forward(struct controller_pins *pins, int motor_speed){
    analogWrite(pins->ENA, motor_speed);
    analogWrite(pins->ENB, motor_speed);

    digitalWrite(pins->in1, HIGH);
    digitalWrite(pins->in3, HIGH);

}

void turn(struct controller_pins *pins, String direction, int motor_speed){
    analogWrite(pins->ENA, motor_speed);
    analogWrite(pins->ENB, motor_speed);

    if (direction == "LEFT"){
        digitalWrite(pins->in1, HIGH);
        digitalWrite(pins->in4, HIGH);
       

    } else {
        digitalWrite(pins->in2, HIGH);
        digitalWrite(pins->in3, HIGH);
    }


}

void motor_off(struct controller_pins *pins){
    digitalWrite(pins->in1, LOW);
    digitalWrite(pins->in2, LOW);
    digitalWrite(pins->in3, LOW);
    digitalWrite(pins->in4, LOW);
}

float get_distance(struct controller_pins *pins) {
  float duration, distance;
  digitalWrite(pins ->trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pins->trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(pins->trigPin, LOW);
  Serial.println(pins->echoPin);
  //noInterrupts(); 
  duration = pulseIn(pins->echoPin, HIGH); 
  Serial.println(duration);

  //interrupts();

  distance = (duration*.0343*10)/2;
  Serial.println(duration);
  return distance;
  delay(100);
}
