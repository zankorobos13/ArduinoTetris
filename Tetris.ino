const int LED_matrix_size_x = 3;
const int LED_matrix_size_y = 3;
const int PWM_period_ms = 20;

bool LED_matrix[LED_matrix_size_y][LED_matrix_size_x] = {
  {true, false, true},
  {false, true, false},
  {true, false, true}
};

uint8_t LED_column_pins[] = {2, 3, 4};
uint8_t LED_row_pins[] = {5, 6, 7};

void setup() {
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
}

int current_step_on_columns = 0;
unsigned long long prev_ms = 0;
unsigned long long ms = 0;

void loop() {
  ms = millis();
    
  if(current_step_on_columns >= LED_matrix_size_x){
    current_step_on_columns = 0;
  }
  if (ms - prev_ms >= PWM_period_ms){
    if (current_step_on_columns == 0){
      digitalWrite(LED_column_pins[LED_matrix_size_x - 1], LOW);
    }
    else{
      digitalWrite(LED_column_pins[current_step_on_columns-1], LOW);
    }
    digitalWrite(LED_column_pins[current_step_on_columns], HIGH);

    current_step_on_columns++;
    prev_ms = ms;
  }
}
