class Figure{
public:
    static const int BLOCK_SIZE = 3;    
    int spawn_coordinates[BLOCK_SIZE][2];
    int rotation_array[4][BLOCK_SIZE][2];

    int current_rotation = 0;
    int coordinates[BLOCK_SIZE][2];

    Figure(int spawn_coords[BLOCK_SIZE][2], int rot_arr[4][BLOCK_SIZE][2], int curr_rot){
        current_rotation = curr_rot;
        for (int i = 0; i < BLOCK_SIZE; i++){
            for (int j = 0; j < 2; j++){
                spawn_coordinates[i][j] = spawn_coords[i][j];
                coordinates[i][j] = spawn_coordinates[i][j];
            }
        }
        for (int k = 0; k < 4; k++){
            for (int i = 0; i < BLOCK_SIZE; i++){
                for (int j = 0; j < 2; j++){
                    rotation_array[k][i][j] = rot_arr[k][i][j];
                }
            }
        }
        
    }

    void Rotate(){
        for (int i = 0; i < BLOCK_SIZE; i++){
            coordinates[i][0] += rotation_array[current_rotation][i][0];
            coordinates[i][1] += rotation_array[current_rotation][i][1];
        }


        current_rotation++;
        if (current_rotation > 4){
            current_rotation = 0;
        }
    }
};

class I_Figure : public Figure{
public:
    int spawn_coordinates[BLOCK_SIZE][2] = {{0, 1}, {0, 2}, {0, 3}};
    int rotation_array[4][BLOCK_SIZE][2] = {
        {{1, 1}, {0, 0}, {-1, -1}},
        {{-1, 1}, {0, 0}, {1, -1}},
        {{-1, -1}, {0, 0}, {1, 1}},
        {{1, -1}, {0, 0}, {-1, 1}}
    };
    I_Figure() : Figure(spawn_coordinates, rotation_array, 0){
        
    }
};