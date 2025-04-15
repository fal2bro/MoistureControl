const int In1 = 5;
const int In2 = 6;
void setup() {
  // put your setup code here, to run once:
  pinMode(In1, OUTPUT);
  pinMode(In2, OUTPUT);
  
  
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(In2,HIGH);
  digitalWrite(In1,LOW);
  delay(3000);
}
