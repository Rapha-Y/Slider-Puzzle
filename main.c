#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define GRID_SIZE 4

typedef struct {
  int numbers[GRID_SIZE*GRID_SIZE];
  int size;
} number_array;

//fills the array with numbers in order
void reset_array(number_array *array){
  for(int i=0;i<GRID_SIZE*GRID_SIZE;i++){
    array->numbers[i] = i;
  }
  array->size = GRID_SIZE*GRID_SIZE;
}

//moves an element of the array to the current size limit
void pick_number(int index, number_array *array){
  int selection = array->numbers[index];
  int i;
  for(i=index;i<array->size-1;i++){
    array->numbers[i] = array->numbers[i+1];
  }
  array->numbers[i] = selection;
  array->size--;
}

//randomizes the position of the numbers
void scramble_array(number_array *array){
  srand(time(NULL));
  for(int i=0;i<GRID_SIZE*GRID_SIZE;i++){
    int rn = rand() % array->size;
    pick_number(rn, array);
  }
}

//fills the grid with elements of the array
void start_grid(number_array array, int grid[GRID_SIZE][GRID_SIZE]){
  for(int i=0;i<GRID_SIZE;i++){
    for(int j=0;j<GRID_SIZE;j++){
      grid[i][j] = array.numbers[GRID_SIZE*i+j];
    }
  }
}

//displays the puzzle grid on the screen
void show_grid(int grid[GRID_SIZE][GRID_SIZE]){
  for(int i=0;i<GRID_SIZE;i++){
    for(int j=0;j<GRID_SIZE;j++){
      printf("|%02d", grid[i][j]);
    }
    printf("|\n");
  }
  printf(" -----------\n");
}

//checks if game is over - function must be tested later
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
    printf("Congratulations! You won!");
    return 1;
  }else{
    return 0;
  }
}

//shows controls to the player - maybe use multiple printfs?
void display_instructions(){
  printf("The objetive of this game is to line the numbers in growing order,\nfrom left to right, then from top to bottom. The only number you\nare allowed to move is the 00, swapping its position with one of its\nadjacent numbers. To move it, you must type the direction you want\nit to move, then press enter.\n\nW - up, A - left, S - down, D - right\n\n -----------\n");
}

int main(){
  number_array array;
  int grid[GRID_SIZE][GRID_SIZE];

  display_instructions();
  reset_array(&array);
  scramble_array(&array);
  start_grid(array, grid);
  show_grid(grid);

  return 0;
}
