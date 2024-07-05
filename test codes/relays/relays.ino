#define RELAY1_PIN 15
#define RELAY2_PIN 16
#define RELAY3_PIN 17
void setup() {
  // put your setup code here, to run once:
 pinMode(RELAY1_PIN, OUTPUT);
  pinMode(RELAY2_PIN, OUTPUT);
  pinMode(RELAY3_PIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
digitalWrite(RELAY1_PIN, HIGH);
delay(500);
digitalWrite(RELAY1_PIN, LOW);
delay(500);

digitalWrite(RELAY2_PIN, HIGH);
delay(500);

digitalWrite(RELAY2_PIN, LOW);
delay(500);


digitalWrite(RELAY3_PIN, HIGH);
delay(500);
digitalWrite(RELAY3_PIN, LOW);
delay(500);
}
