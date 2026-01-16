const int LED_matrix_size_x = 9;
const int LED_matrix_size_y = 6;
const int PWM_period_ms = 1;


uint8_t play_matrix[LED_matrix_size_x][LED_matrix_size_y] = {
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0},
};

bool LED_matrix[LED_matrix_size_y][LED_matrix_size_x] = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0},
};

uint8_t LED_column_pins[LED_matrix_size_x] = {4, 5, 6, 7, 8, 9, 10, 11, 12};
uint8_t LED_row_pins[LED_matrix_size_y] = {3, 2, A1, A2, A3, 13};

int current_step_on_columns = 0;
unsigned long long prev_ms_LED_matrix_control = 0;
unsigned long long ms = 0;

void LED_matrix_update(){
  for (int i = 0; i < LED_matrix_size_y; i++){
    for (int j = 0; j < LED_matrix_size_x; j++){
      LED_matrix[LED_matrix_size_y-i-1][j] = play_matrix[j][i] != 0;      
    }
  }
}

void LED_matrix_control(){
  if(current_step_on_columns >= LED_matrix_size_x){
    current_step_on_columns = 0;
  }
  if (ms - prev_ms_LED_matrix_control >= PWM_period_ms){
    if (current_step_on_columns == 0){
      analogWrite(LED_column_pins[LED_matrix_size_x - 1], 0);
    }
    else{
      analogWrite(LED_column_pins[current_step_on_columns-1], 0);
    }
    analogWrite(LED_column_pins[current_step_on_columns], 255);

    for (int i = 0; i < LED_matrix_size_y; i++){
      if (LED_matrix[i][current_step_on_columns]) {
        analogWrite(LED_row_pins[i], 255);
      }
      else{
        analogWrite(LED_row_pins[i], 0);
      }
    }

    current_step_on_columns++;
    prev_ms_LED_matrix_control = ms;
  }
}







void setup() {
  Serial.begin(9600);
  for (int i = 0; i < LED_matrix_size_x; i++){
    pinMode(LED_column_pins[i], OUTPUT);
  }
  for (int i = 0; i < LED_matrix_size_y; i++){
    pinMode(LED_row_pins[i], OUTPUT);
  }
}



void loop() {
  ms = millis();

  LED_matrix_update();
  LED_matrix_control();
}
