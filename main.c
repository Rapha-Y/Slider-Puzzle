#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define GRID_SIZE 4

typedef struct {
  int numbers[GRID_SIZE*GRID_SIZE];
  int size;
  int zero_pos;
} number_array;

typedef struct {
  int row_index;
  int col_index;
} zero_pos;

/* Puzzle setup functions */

void reset_array(number_array *array){
  for(int i=0;i<GRID_SIZE*GRID_SIZE;i++){
    array->numbers[i] = i;
  }
  array->size = GRID_SIZE*GRID_SIZE;
}

void pick_number(int index, number_array *array){
  int selection = array->numbers[index];
  int i;
  for(i=index;i<array->size-1;i++){
    array->numbers[i] = array->numbers[i+1];
  }
  array->numbers[i] = selection;
  array->size--;
}

void scramble_array(number_array *array){
  srand(time(NULL));
  for(int i=0;i<GRID_SIZE*GRID_SIZE;i++){
    int rn = rand() % array->size;
    pick_number(rn, array);
  }
}

void start_grid(number_array array, int grid[GRID_SIZE][GRID_SIZE]){
  for(int i=0;i<GRID_SIZE;i++){
    for(int j=0;j<GRID_SIZE;j++){
      grid[i][j] = array.numbers[GRID_SIZE*i+j];
    }
  }
}

void find_zero(zero_pos *position, int grid[GRID_SIZE][GRID_SIZE]){
  int brake=0;
  for(int i=0;i<GRID_SIZE && brake==0;i++){
    for(int j=0;j<GRID_SIZE && brake==0;j++){
      if(grid[i][j]==0){
        position->row_index = i;
        position->col_index = j;
        brake = 1;
      }
    }
  }
}

/* Gameplay functions */

int move_zero(zero_pos *position, int grid[GRID_SIZE][GRID_SIZE], char input){
  //for invalid letters
  if(input!='W' && input!='A' && input!='S' && input!='D' && input!='w' && input!='a' && input!='s' && input!='d'){
    printf("Please enter a valid input.\n");
    return 0;
  }else if(input=='W' || input=='w'){
    //for border movements
    if(position->row_index==0){
      printf("Please enter a valid movement.\n");
      return 0;
    //for valid movements
    }else{
      int aux = grid[position->row_index][position->col_index];
      grid[position->row_index][position->col_index] = grid[position->row_index-1][position->col_index];
      grid[position->row_index-1][position->col_index] = aux;
      position->row_index--;
      return 1;
    }
  }else if(input=='A' || input=='a'){
    if(position->col_index==0){
      printf("Please enter a valid movement.\n");
      return 0;
    }else{
      int aux = grid[position->row_index][position->col_index];
      grid[position->row_index][position->col_index] = grid[position->row_index][position->col_index-1];
      grid[position->row_index][position->col_index-1] = aux;
      position->col_index--;
      return 1;
    }
  }else if(input=='S' || input=='s'){
    if(position->row_index==GRID_SIZE-1){
      printf("Please enter a valid movement.\n");
      return 0;
    }else{
      int aux = grid[position->row_index][position->col_index];
      grid[position->row_index][position->col_index] = grid[position->row_index+1][position->col_index];
      grid[position->row_index+1][position->col_index] = aux;
      position->row_index++;
      return 1;
    }
  }else if(input=='D' || input=='d'){
    if(position->col_index==GRID_SIZE-1){
      printf("Please enter a valid movement.\n");
      return 0;
    }else{
      int aux = grid[position->row_index][position->col_index];
      grid[position->row_index][position->col_index] = grid[position->row_index][position->col_index+1];
      grid[position->row_index][position->col_index+1] = aux;
      position->col_index++;
      return 1;
    }
  }
}

int reset_game(char confirmation){
  if(confirmation=='R' || confirmation=='r'){
    return 1;
  }else{
    printf("Press R and Enter to restart the game.\n");
    return 0;
  }
}

int select_diff(int diff){
  if(diff>0 && diff<4){
    return 1;
  }else{
    printf("Please enter a valid input.\n");
    return 0;
  }
}

/* Player interface functions */

int check_victory(int grid[GRID_SIZE][GRID_SIZE]){
  int win = 1;
  for(int i=0;i<GRID_SIZE && win==1;i++){
    for(int j=0;j<GRID_SIZE && win==1;j++){
      if(grid[i][j] != GRID_SIZE*i+j){
        win = 0;
      }
    }
  }
  if(win==1){
    printf("Congratulations! You won!\n");
    return 1;
  }else{
    return 0;
  }
}

void show_grid(int grid[GRID_SIZE][GRID_SIZE]){
  printf(" -----------\n");
  for(int i=0;i<GRID_SIZE;i++){
    for(int j=0;j<GRID_SIZE;j++){
      printf("|%02d", grid[i][j]);
    }
    printf("|\n");
  }
  printf(" -----------\n");
}

void display_instructions(){
  printf("The objetive of this game is to line the numbers in growing order,\nfrom left to right, then from top to bottom. The only number you\nare allowed to move is the 00, swapping its position with one of its\nadjacent numbers. To move it, you must type the direction you want\nit to move, then press enter.\n\nW - up, A - left, S - down, D - right\n\n");
}

void display_diff_selector(){
  printf("Press 1 to select easy mode, 2 for normal mode or 3 for hard mode,\nthen, press Enter to confirm.\n");
}

int main(){
  number_array array;
  zero_pos position;
  int diff, swap_check, grid[GRID_SIZE][GRID_SIZE];
  char input, reset;

  display_instructions();
  while(1){
    display_diff_selector();
    scanf("\n%d", &diff);
    while(select_diff(diff)!=1){
      scanf("\n%d", &diff);
    }

    reset_array(&array);
    scramble_array(&array);
    start_grid(array, grid);
    find_zero(&position, grid);

    while(check_victory(grid)!=1){
      show_grid(grid);
      swap_check=0;
      while(swap_check==0){
        scanf("\n%c", &input);
        swap_check = move_zero(&position, grid, input);
      }
    }

    reset = 'F';
    while(reset_game(reset)!=1){
      scanf("\n%c", &reset);
    }
  }

  return 0;
}
