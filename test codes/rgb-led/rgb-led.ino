// *Interfacing RGB LED with Arduino 
// * Author: Osama Ahmed 

//Defining  variable and the GPIO pin on Arduino
int redPin= 32;
int greenPin = 25;
int  bluePin = 33;

void setup() {
  //Defining the pins as OUTPUT
  pinMode(redPin,  OUTPUT);              
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}
void  loop() {
  setColor(255, 0, 0); // Red Color
  delay(1000);
  setColor(0,  255, 0); // Green Color
  delay(1000);
  setColor(0, 0, 255); // Blue Color
  delay(1000);
  setColor(255, 255, 255); // White Color
  delay(1000);
  setColor(170, 0, 255); // Purple Color
  delay(1000);
  setColor(127, 127,  127); // Light Blue
  delay(1000);
}
void setColor(int redValue, int greenValue,  int blueValue) {
  analogWrite(redPin, redValue);
  analogWrite(greenPin,  greenValue);
  analogWrite(bluePin, blueValue);
}
