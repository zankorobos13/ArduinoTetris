void LED_arr_by_delay(uint8_t LED_pins[], int delta_time ){
  digitalWrite(LED_pins[0], HIGH);
  delay(delta_time);
  for (int i = 1; i < sizeof(LED_pins) / sizeof(LED_pins[0]) + 1; i++){
    digitalWrite(LED_pins[i-1], LOW);
    digitalWrite(LED_pins[i], HIGH);
    delay(delta_time);
  }
  digitalWrite(LED_pins[sizeof(LED_pins) / sizeof(LED_pins[0])], LOW);
}

void setup() {
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  
}

void loop() {
  uint8_t pins[] = {10, 11, 12};
  LED_arr_by_delay(pins, 2);
  
}
