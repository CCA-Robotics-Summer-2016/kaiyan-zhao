/*
  -- Kaiyan Zhao
  -avoids obstacles
    -detect distance in numbers of angle
    -go to the one have more space
    -go back a little bit before turn right/left
    -stop if too close
  -senses the light
    -LDR
    -set lightest light condition
    -set darkest light condition
    -differenciate light condition and light up LED light for indication

 */
 
int Left_motor_P = 9;   //The left motor positive 
int Left_motor_N = 6;   //The left motor negative 

int Right_motor_P = 10;  // The right motor positive 
int Right_motor_N = 11;  // The right motor negative 

int Ultrasonic_Echo = A0;  
int Ultrasonic_Trig = A1; 

int Front_Distance = 0; 
int Right_Distance = 0; 
int Left_Distance = 0;  
int directionn = 8;
int light0, light1;

byte LED0 = 0; //LED pin
byte LED1 = 1;
byte LED2 = 2;
byte LED3 = 3;
byte LED4 = 4;
byte LED5 = A4;
byte LED6 = 7;
byte LED7 = 8;
byte LED8 = 12;
byte LED9 = 13;

#define Dir_Forward 	8         // Forward
#define Dir_Backward 	6         // Reversing
#define Dir_Right 	4         // Turn right
#define Dir_Left 	2         // Turn left

int servopin = 5; //Servo motor
int myangle;//Angle variable
int pulsewidth;//Define pulse width variable
int val;
void servopulse(int servopin, int myangle) //Define a pulse function
{
  pulsewidth = (myangle * 11) + 500; //The pulse width of 500-2480 is converted into the pulse width.
  digitalWrite(servopin, HIGH);
  delayMicroseconds(pulsewidth);//delay
  digitalWrite(servopin, LOW);
  delay(20 - pulsewidth / 1000);
}


void setup()
{
  Serial.begin(9600);
  pinMode(Left_motor_P, OUTPUT); //motors
  pinMode(Left_motor_N, OUTPUT);
  pinMode(Right_motor_P, OUTPUT);
  pinMode(Right_motor_N, OUTPUT);
  pinMode(0, OUTPUT);   //LEDs
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(Ultrasonic_Echo, INPUT);
  pinMode(Ultrasonic_Trig, OUTPUT);

  pinMode(servopin, OUTPUT);
  digitalWrite(LED0, HIGH);
  digitalWrite(LED5, HIGH);
}
void advance(int a)     // go forward
{
  digitalWrite(Right_motor_P, HIGH);
  digitalWrite(Right_motor_N, LOW);
  analogWrite(Right_motor_P, 150);
  analogWrite(Right_motor_N, 0);
  digitalWrite(Left_motor_P, HIGH);
  digitalWrite(Left_motor_N, LOW);
  analogWrite(Left_motor_P, 150);
  analogWrite(Left_motor_N, 0);
  delay(a * 100);   //Execution time
}

void right(int b)        //turn right (single round)
{
  digitalWrite(Right_motor_P, LOW);
  digitalWrite(Right_motor_N, HIGH);
  analogWrite(Right_motor_P, 0);
  analogWrite(Right_motor_N, 150);
  digitalWrite(Left_motor_P, LOW);
  digitalWrite(Left_motor_N, LOW);
  delay(b * 100);	//Execution time
}
void left(int c)         //turn left  (single round)
{
  digitalWrite(Right_motor_P, LOW);
  digitalWrite(Right_motor_N, LOW);
  digitalWrite(Left_motor_P, LOW);
  digitalWrite(Left_motor_N, HIGH);
  analogWrite(Left_motor_P, 0);
  analogWrite(Left_motor_N, 150);
  delay(c * 100);	//Execution time
}

void Brake(int f)         //brake
{
  digitalWrite(Right_motor_P, LOW);
  digitalWrite(Right_motor_N, LOW);
  digitalWrite(Left_motor_P, LOW);
  digitalWrite(Left_motor_N, LOW);
  delay(f * 100);//Execution time
}
void Reverse(int g)          //goes back
{
  digitalWrite(Right_motor_P, LOW);
  digitalWrite(Right_motor_N, HIGH);
  analogWrite(Right_motor_P, 0);
  analogWrite(Right_motor_N, 150);
  digitalWrite(Left_motor_P, LOW);
  digitalWrite(Left_motor_N, HIGH);
  analogWrite(Left_motor_P, 0);
  analogWrite(Left_motor_N, 150);
  delay(g * 100);  
}

void detection()        //Measuring the distance in 7 directions
{
  ask_pin_F();            //  get front distance

  if (Front_Distance < 20)
  {
    Brake(1);
    Reverse(2);
  }

  if (Front_Distance < 40)
  {
    Brake(1);
    ask_pin_R();
    ask_pin_L();
    for (int i = 0; i <= 25; i++) { //delay
      servopulse(servopin, 90);
    }
    if (Left_Distance > Right_Distance)  //If the left distance is longer than the right distance
    {
      directionn = Dir_Left;      //Go right
    }
    else
    {
      directionn = Dir_Right;      //Go lift
    }
    if (Left_Distance < 20 && Right_Distance < 20)   //If the distance between the left and the right is less than 10 cm.
    {
       directionn = Dir_Backward;     //Go back
    }

    //for(int i=0;i<=25;i++) {//delay
    //servopulse(servopin,90);
    //}
  }
  else {
    directionn = Dir_Forward;        //go forward
  }
}

void ask_pin_F()   // front distance
{
  for (int i = 0; i <= 25; i++) {
    servopulse(servopin, 90);
  }
  digitalWrite(Ultrasonic_Trig, LOW);
  delayMicroseconds(2);
  digitalWrite(Ultrasonic_Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(Ultrasonic_Trig, LOW);
  float Fdistance = pulseIn(Ultrasonic_Echo, HIGH);
  Fdistance = Fdistance / 5.8 / 10;
  Serial.print("F distance:");
  Serial.println(Fdistance);
  Front_Distance = Fdistance;
}

void ask_pin_L()   // measure the right three distance and take the minimum reading
{
  for (int i = 0; i <= 35; i++) { //turn to the angle
    servopulse(servopin, 175);
  }
  digitalWrite(Ultrasonic_Trig, LOW);
  delayMicroseconds(2);
  digitalWrite(Ultrasonic_Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(Ultrasonic_Trig, LOW);
  float Ldistance = pulseIn(Ultrasonic_Echo, HIGH);
  Ldistance = Ldistance / 5.8 / 10;
  Serial.print("L distance0:");
  Serial.println(Ldistance);
  Left_Distance = Ldistance;  

  for (int i = 0; i <= 35; i++) {
    servopulse(servopin, 160);
  }
  digitalWrite(Ultrasonic_Trig, LOW);
  delayMicroseconds(2);
  digitalWrite(Ultrasonic_Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(Ultrasonic_Trig, LOW);
  Ldistance = pulseIn(Ultrasonic_Echo, HIGH);
  Ldistance = Ldistance / 5.8 / 10;
  Serial.print("L distance1:");
  Serial.println(Ldistance);
  if (Left_Distance > Ldistance) {
    Left_Distance = Ldistance;
  }

  for (int i = 0; i <= 35; i++) {
    servopulse(servopin, 145);
  }
  digitalWrite(Ultrasonic_Trig, LOW);
  delayMicroseconds(2);
  digitalWrite(Ultrasonic_Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(Ultrasonic_Trig, LOW);
  Ldistance = pulseIn(Ultrasonic_Echo, HIGH);
  Ldistance = Ldistance / 5.8 / 10;
  Serial.print("L distance2:");
  Serial.println(Ldistance);
  if (Left_Distance > Ldistance) {
    Left_Distance = Ldistance;
  }

  for (int i = 0; i <= 35; i++) {
    servopulse(servopin, 130);
  }
  digitalWrite(Ultrasonic_Trig, LOW);
  delayMicroseconds(2);
  digitalWrite(Ultrasonic_Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(Ultrasonic_Trig, LOW);
  Ldistance = pulseIn(Ultrasonic_Echo, HIGH);
  Ldistance = Ldistance / 5.8 / 10;
  Serial.print("L distance2:");
  Serial.println(Ldistance);
  if (Left_Distance > Ldistance) {
    Left_Distance = Ldistance;
  }
  for (int i = 0; i <= 35; i++) {
    servopulse(servopin, 115);
  }
  digitalWrite(Ultrasonic_Trig, LOW);
  delayMicroseconds(2);
  digitalWrite(Ultrasonic_Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(Ultrasonic_Trig, LOW);
  Ldistance = pulseIn(Ultrasonic_Echo, HIGH);
  Ldistance = Ldistance / 5.8 / 10;
  Serial.print("L distance2:");
  Serial.println(Ldistance);
  if (Left_Distance > Ldistance) {
    Left_Distance = Ldistance;
  }
  for (int i = 0; i <= 35; i++) {
    servopulse(servopin, 100);
  }
  digitalWrite(Ultrasonic_Trig, LOW);
  delayMicroseconds(2);
  digitalWrite(Ultrasonic_Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(Ultrasonic_Trig, LOW);
  Ldistance = pulseIn(Ultrasonic_Echo, HIGH);
  Ldistance = Ldistance / 5.8 / 10;
  Serial.print("L distance2:");
  Serial.println(Ldistance);
  if (Left_Distance > Ldistance) {
    Left_Distance = Ldistance;
  }
}
void ask_pin_R() //measure the right three distance and take the minimum reading
{
  for (int i = 0; i <= 35; i++) {
    servopulse(servopin, 80);
  }
  digitalWrite(Ultrasonic_Trig, LOW);
  delayMicroseconds(2);
  digitalWrite(Ultrasonic_Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(Ultrasonic_Trig, LOW);
  float Rdistance = pulseIn(Ultrasonic_Echo, HIGH);
  Rdistance = Rdistance / 5.8 / 10;
  Serial.print("R distance0:");
  Serial.println(Rdistance);
  Right_Distance = Rdistance;

  for (int i = 0; i <= 35; i++) {
    servopulse(servopin, 65);
  }
  digitalWrite(Ultrasonic_Trig, LOW);
  delayMicroseconds(2);
  digitalWrite(Ultrasonic_Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(Ultrasonic_Trig, LOW);
  Rdistance = pulseIn(Ultrasonic_Echo, HIGH);
  Rdistance = Rdistance / 5.8 / 10;
  Serial.print("R distance1:");
  Serial.println(Rdistance);
  if (Right_Distance > Rdistance) {
    Right_Distance = Rdistance;
  }

  for (int i = 0; i <= 35; i++) {
    servopulse(servopin, 50);
  }
  digitalWrite(Ultrasonic_Trig, LOW);
  delayMicroseconds(2);
  digitalWrite(Ultrasonic_Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(Ultrasonic_Trig, LOW);
  Rdistance = pulseIn(Ultrasonic_Echo, HIGH);
  Rdistance = Rdistance / 5.8 / 10;
  Serial.print("R distance1:");
  Serial.println(Rdistance);
  if (Right_Distance > Rdistance) {
    Right_Distance = Rdistance;
  }

  for (int i = 0; i <= 35; i++) {
    servopulse(servopin, 35);
  }
  digitalWrite(Ultrasonic_Trig, LOW);
  delayMicroseconds(2);
  digitalWrite(Ultrasonic_Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(Ultrasonic_Trig, LOW);
  Rdistance = pulseIn(Ultrasonic_Echo, HIGH);
  Rdistance = Rdistance / 5.8 / 10;
  Serial.print("R distance2:");
  Serial.println(Rdistance);
  if (Right_Distance > Rdistance) {
    Right_Distance = Rdistance;
  }

  for (int i = 0; i <= 35; i++) {
    servopulse(servopin, 20);
  }
  digitalWrite(Ultrasonic_Trig, LOW);
  delayMicroseconds(2);
  digitalWrite(Ultrasonic_Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(Ultrasonic_Trig, LOW);
  Rdistance = pulseIn(Ultrasonic_Echo, HIGH);
  Rdistance = Rdistance / 5.8 / 10;
  Serial.print("R distance2:");
  Serial.println(Rdistance);
  if (Right_Distance > Rdistance) {
    Right_Distance = Rdistance;
  }

  for (int i = 0; i <= 35; i++) {
    servopulse(servopin, 5);
  }
  digitalWrite(Ultrasonic_Trig, LOW);
  delayMicroseconds(2);
  digitalWrite(Ultrasonic_Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(Ultrasonic_Trig, LOW);
  Rdistance = pulseIn(Ultrasonic_Echo, HIGH);
  Rdistance = Rdistance / 5.8 / 10;
  Serial.print("R distance2:");
  Serial.println(Rdistance);
  if (Right_Distance > Rdistance) {
    Right_Distance = Rdistance;
  }
}
void light_LED() {  //light up each LED according to the previous test on lightest and darkest light condition
  light0 = analogRead(A2);
  light1 = analogRead(A3);
  if (light0 < 400 && light0 > 200) {
    digitalWrite(LED1, HIGH);
  } else {
    digitalWrite(LED1, LOW);
  }
  if (light0 < 600 && light0 > 400) {
    digitalWrite(LED2, HIGH);
  } else {
    digitalWrite(LED2, LOW);
  }
  if (light0 < 800 && light0 > 600) {
    digitalWrite(LED3, HIGH);
  } else {
    digitalWrite(LED3, LOW);
  }
  if (light0 < 1024 && light0 > 800) {
    digitalWrite(LED4, HIGH);
  } else {
    digitalWrite(LED4, LOW);
  }
  if (light1 < 400 && light1 > 200) {
    digitalWrite(LED6, HIGH);
  } else {
    digitalWrite(LED6, LOW);
  }
  if (light1 < 600 && light1 > 400) {
    digitalWrite(LED7, HIGH);
  } else {
    digitalWrite(LED7, LOW);
  }
  if (light1 < 800 && light1 > 600) {
    digitalWrite(LED8, HIGH);
  } else {
    digitalWrite(LED8, LOW);
  }
  if (light1 < 1024 && light1 > 800) {
    digitalWrite(LED9, HIGH);
  } else {
    digitalWrite(LED9, LOW);
  }
}

void loop()
{
  detection();        //Measure the angle and decide which direction to move
  light_LED();
  if (directionn == Dir_Backward)
  {
    Reverse(8);
    Serial.print(" Reverse ");
  }
  if (directionn == Dir_Right)
  {
    Reverse(1);
    right(4);
    Serial.print(" Right ");
  }
  if (directionn == Dir_Left)
  {
    Reverse(1);
    left(4);
    Serial.print(" Left ");
  }
  if (directionn == Dir_Forward)
  {
    advance(1);
    Serial.print(" Advance ");
    Serial.print("   ");
  }

}
