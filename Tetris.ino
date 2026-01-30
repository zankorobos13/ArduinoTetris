#include "Figure.h"

const int LED_matrix_size_x = 9;
const int LED_matrix_size_y = 6;
const int PWM_period_ms = 0;

int FALL_SPEED_MS = 1000;
const uint8_t NOISE_PIN = A4;
const uint8_t LEFT_BTN = A6;
const uint8_t RIGHT_BTN = A4;
const uint8_t ROTATE_BTN = A5;
int DEBOUNCE_TIME_MS = 300;

int I_spawn_coordinates[3][2] = {{0, 1}, {0, 2}, {0, 3}};
int I_rotation_array[4][3][2] = {
        {{1, 1}, {0, 0}, {-1, -1}},
        {{-1, 1}, {0, 0}, {1, -1}},
        {{-1, -1}, {0, 0}, {1, 1}},
        {{1, -1}, {0, 0}, {-1, 1}}
};

int L_spawn_coordinates[3][2] = {{0, 2}, {1, 2}, {1, 3}};
int L_rotation_array[4][3][2] = {
        {{1, 0}, {0, 1}, {-1, 0}},
        {{0, 1}, {-1, 0}, {0, -1}},
        {{-1, 0}, {0, -1}, {1, 0}},
        {{0, -1}, {1, 0}, {0, 1}}
};

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
unsigned long long prev_ms_left = 0;
unsigned long long prev_ms_right = 0;
unsigned long long prev_ms_rotate = 0;
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



bool isFigureOnMatrix = false;
unsigned long long prev_ms_figure_fall = 0;
Figure current_figure {L_spawn_coordinates, L_rotation_array, 0};
bool a = false;

void DEBUG(){
  for(int i = 0; i < Figure::BLOCK_SIZE; i++){
    Serial.print(i);
    Serial.print(" i: ");
    Serial.print(current_figure.coordinates[i][0]);
    Serial.print(" j: ");
    Serial.print(current_figure.coordinates[i][1]);
    Serial.println();
  }
  for(int i = 0; i < LED_matrix_size_x; i++){
    for(int j = 0; j < LED_matrix_size_y; j++){
      Serial.print(play_matrix[i][j]);
    }
    Serial.println();
  }
  Serial.println("------------");

}
// Вынести перемещение фигур в класс
void Game(){
  if (!isFigureOnMatrix){
    int rotation = 0;
    int spawn_coordinates[3][2];
    int rotation_array[4][3][2];
    if (random(2) == 1){
      for (int i = 0; i < 3; i++){
        for (int j = 0; j < 2; j++){
            spawn_coordinates[i][j] = I_spawn_coordinates[i][j];
        }
      }
      for (int k = 0; k < 4; k++){
        for (int i = 0; i < 3; i++){
          for (int j = 0; j < 2; j++){
              rotation_array[k][i][j] = I_rotation_array[k][i][j];
          }
        }
      }
    }
    else{
      for (int i = 0; i < 3; i++){
        for (int j = 0; j < 2; j++){
            spawn_coordinates[i][j] = L_spawn_coordinates[i][j];
        }
      }
      for (int k = 0; k < 4; k++){
        for (int i = 0; i < 3; i++){
          for (int j = 0; j < 2; j++){
              rotation_array[k][i][j] = L_rotation_array[k][i][j];
          }
        }
      }
      rotation = random(4);
    }
    Figure new_figure {spawn_coordinates, rotation_array, 0};
    current_figure = new_figure;
    isFigureOnMatrix = true;
  }
  
  for (int i = 0; i < Figure::BLOCK_SIZE; i++){
      play_matrix[current_figure.coordinates[i][0]][current_figure.coordinates[i][1]] = 2;
  }

  if (ms - prev_ms_figure_fall >= FALL_SPEED_MS){
    if(CheckFigurePositionForFall()){
      for (int i = 0; i < Figure::BLOCK_SIZE; i++){
        play_matrix[current_figure.coordinates[i][0]][current_figure.coordinates[i][1]] = 0;
        current_figure.coordinates[i][0]++;
      }
    }
    else{
      isFigureOnMatrix = false;
      for (int i = 0; i < Figure::BLOCK_SIZE; i++){
        play_matrix[current_figure.coordinates[i][0]][current_figure.coordinates[i][1]] = 1;
      }
    }
    prev_ms_figure_fall = ms;
  }
  
  int read_left = analogRead(LEFT_BTN);
  int read_right = analogRead(RIGHT_BTN);
  int read_rotate = analogRead(ROTATE_BTN);

  if (read_left > 512 && ms - prev_ms_left > DEBOUNCE_TIME_MS && CheckFigurePositionLeft()){
    for (int i = 0; i < Figure::BLOCK_SIZE; i++){
      play_matrix[current_figure.coordinates[i][0]][current_figure.coordinates[i][1]] = 0;
      current_figure.coordinates[i][1]--;
    }
    prev_ms_left = ms;
  }

  if (read_right > 512 && ms - prev_ms_right > DEBOUNCE_TIME_MS && CheckFigurePositionRight()){
    for (int i = 0; i < Figure::BLOCK_SIZE; i++){
      play_matrix[current_figure.coordinates[i][0]][current_figure.coordinates[i][1]] = 0;
      current_figure.coordinates[i][1]++;
    }
    prev_ms_right = ms;
  }

  if (read_rotate > 512 && ms - prev_ms_rotate > DEBOUNCE_TIME_MS && CheckFigureForRotation()){
    for (int i = 0; i < Figure::BLOCK_SIZE; i++){
      play_matrix[current_figure.coordinates[i][0]][current_figure.coordinates[i][1]] = 0;
    }
    current_figure.Rotate();
    prev_ms_rotate = ms;
  }

  BlockRowCheckAndClear();
  SetFallSpeedByTime();
}

bool CheckFigurePositionForFall(){
  bool isAvailableToFall = true;
  for (int i = 0; i < Figure::BLOCK_SIZE; i++){
    
    if (current_figure.coordinates[i][0] == LED_matrix_size_x - 1){
      isAvailableToFall = false;
      break;
    }
    else if(play_matrix[current_figure.coordinates[i][0] + 1][current_figure.coordinates[i][1]] == 1){
      isAvailableToFall = false;
      break;
    }
  }
  return isAvailableToFall;
}

bool CheckFigurePositionLeft(){
  bool isAvailableToLeft = true;
  for (int i = 0; i < Figure::BLOCK_SIZE; i++){
    
    if (current_figure.coordinates[i][1] == 0){
      isAvailableToLeft = false;
      break;
    }
    else if(play_matrix[current_figure.coordinates[i][0]][current_figure.coordinates[i][1] - 1] == 1){
      isAvailableToLeft = false;
      break;
    }
  }
  return isAvailableToLeft;
}

bool CheckFigurePositionRight(){
  bool isAvailableToRight = true;
  for (int i = 0; i < Figure::BLOCK_SIZE; i++){
    
    if (current_figure.coordinates[i][1] == LED_matrix_size_y - 1){
      isAvailableToRight = false;
      break;
    }
    else if(play_matrix[current_figure.coordinates[i][0]][current_figure.coordinates[i][1] + 1] == 1){
      isAvailableToRight = false;
      break;
    }
  }
  return isAvailableToRight;
}

bool CheckFigureForRotation(){
  bool isAvailableToRotate = true;
  
  for (int i = 0; i < Figure::BLOCK_SIZE; i++){
    int new_x = current_figure.coordinates[i][0] + current_figure.rotation_array[current_figure.current_rotation][i][0];
    int new_y = current_figure.coordinates[i][1] + current_figure.rotation_array[current_figure.current_rotation][i][1];

    if (new_x < 0 || new_y < 0 || new_x > LED_matrix_size_x - 1 || new_y > LED_matrix_size_y - 1 || play_matrix[new_x][new_y] == 1){
      isAvailableToRotate = false;
    }
    
  }
  
  return isAvailableToRotate;
}

void BlockRowCheckAndClear(){
  for (int i = 0; i < LED_matrix_size_x; i++){
    bool isFullRow = true;
    for (int j = 0; j < LED_matrix_size_y; j++){
      if (play_matrix[i][j] != 1){
        isFullRow = false;
        break;
      }
    }
    if (isFullRow){
      for (int j = 0; j < LED_matrix_size_y; j++){
        play_matrix[i][j] = 0;
      }
      for (int j = i; j > 0; j--){
        for (int k = 0; k < LED_matrix_size_y; k++){
          play_matrix[j][k] = play_matrix[j-1][k];
          play_matrix[j-1][k] = 0;
        }
      }
    }
  }
}

void SetFallSpeedByTime(){
  if (FALL_SPEED_MS > 100){
    FALL_SPEED_MS = 1000 - (ms/300);
  }
  if (DEBOUNCE_TIME_MS > 100){
    DEBOUNCE_TIME_MS = 300 - (ms/1350);
  }
}

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(NOISE_PIN) + micros());
  for (int i = 0; i < LED_matrix_size_x; i++){
    pinMode(LED_column_pins[i], OUTPUT);
  }
  for (int i = 0; i < LED_matrix_size_y; i++){
    pinMode(LED_row_pins[i], OUTPUT);
  }
  pinMode(NOISE_PIN, INPUT);
  pinMode(LEFT_BTN, INPUT);
  pinMode(RIGHT_BTN, INPUT);
  pinMode(ROTATE_BTN, INPUT);
}



void loop() {
  ms = millis();

  LED_matrix_update();
  LED_matrix_control();
  Game();
}
