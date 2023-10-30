//Function prototypes
void to_pool();
String mission();
void navigation();

void turn(struct controller_pins *, String direction, int motor_speed);
void forward(struct controller_pins *, int motor_speed);
void pool_navigate_aruco();
void motor_off(struct controller_pins *);

//Arduino pin assignments
struct controller_pins {
    int in1;
    int in2;
    int in3;
    int in4;
    int ENA;
    int ENB;
};


void setup() {
  Serial.begin(9600);
  
//initialize all input and output pins
    struct controller_pins pins =  {12, 11, 10, 9, 13, 18};
    int output[] = {9,12,13,11,10,3,13,18};
    int input[] = {4,5};
    int num_output = sizeof(output)/2;
    int num_input = sizeof(input)/2;
   Serial.println(num_output);
  

    for (int i = 0; i < num_output; i++) {
        pinMode(output[i], OUTPUT);
    }
    for (int i = 0; i < num_input; i++){
        pinMode(input[i], OUTPUT);
    }
}


void loop() {

    struct controller_pins pins =  {12, 11, 10, 9, 13, 8};
    forward(&pins, 255);
    delay(5000);
  motor_off(&pins);
    turn(&pins, "LEFT", 133);
  delay(1000);
  motor_off(&pins);
  forward(&pins, 255);
  delay(5000);







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
