const int LED_matrix_size_x = 3;
const int LED_matrix_size_y = 3;
const int PWM_period_ms = 2;

bool LED_matrix[LED_matrix_size_y][LED_matrix_size_x] = {
  {true, true, true},
  {false, true, false},
  {false, true, false}
};

uint8_t LED_column_pins[LED_matrix_size_x] = {2, 3, 4};
uint8_t LED_row_pins[LED_matrix_size_y] = {5, 6, 7};

int current_step_on_columns = 0;
unsigned long long prev_ms_LED_matrix_control = 0;
unsigned long long ms = 0;

void LED_matrix_control(){
  if(current_step_on_columns >= LED_matrix_size_x){
    current_step_on_columns = 0;
  }
  if (ms - prev_ms_LED_matrix_control >= PWM_period_ms){
    if (current_step_on_columns == 0){
      digitalWrite(LED_column_pins[LED_matrix_size_x - 1], LOW);
    }
    else{
      digitalWrite(LED_column_pins[current_step_on_columns-1], LOW);
    }
    digitalWrite(LED_column_pins[current_step_on_columns], HIGH);

    for (int i = 0; i < LED_matrix_size_y; i++){
      if (LED_matrix[i][current_step_on_columns]) {
        digitalWrite(LED_row_pins[i], HIGH);
      }
      else{
        digitalWrite(LED_row_pins[i], LOW);
      }
    }

    current_step_on_columns++;
    prev_ms_LED_matrix_control = ms;
  }
}


void setup() {
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
}



void loop() {
  ms = millis();
  LED_matrix_control();
}
