void setup() {
  Serial.begin(115200);
  pinMode(8, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(8, LOW);
  Serial.println("high");
  delay(2000);
  digitalWrite(8, LOW);
  Serial.println("low");
  delay(2000);
}
