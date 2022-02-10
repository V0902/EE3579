void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop() {
  static int loop_count = 0;
  // put your main code here, to run repeatedly:
  int delay_ms = 750;
  Serial.println("Hello World!");
  Serial.print("This is iteration # ");
  Serial.println(loop_count);
  Serial.println();
  loop_count++;
  delay(delay_ms);

}
