#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define GRID_SIZE 4

typedef struct {
  int numbers[(GRID_SIZE+1)*(GRID_SIZE+1)];
  int size;
  int zero_pos;
} number_array;

typedef struct {
  int row_index;
  int col_index;
} zero_pos;

typedef struct s_el {
  char elem;
  struct s_el *prev;
} s_elem;

typedef struct {
  int size;
  s_elem *top;
} stack;

/* Puzzle setup functions */

void reset_array(number_array *array, int diff){
  for(int i=0;i<(GRID_SIZE+diff-2)*(GRID_SIZE+diff-2);i++){
    array->numbers[i] = i;
  }
  array->size = (GRID_SIZE+diff-2)*(GRID_SIZE+diff-2);
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

void scramble_array(number_array *array, int diff){
  srand(time(NULL));
  for(int i=0;i<(GRID_SIZE+diff-2)*(GRID_SIZE+diff-2);i++){
    int rn = rand() % array->size;
    pick_number(rn, array);
  }
}

void start_grid(number_array array, int grid[GRID_SIZE+1][GRID_SIZE+1], int diff){
  for(int i=0;i<GRID_SIZE+diff-2;i++){
    for(int j=0;j<GRID_SIZE+diff-2;j++){
      grid[i][j] = array.numbers[(GRID_SIZE+diff-2)*i+j];
    }
  }
}

void find_zero(zero_pos *position, int grid[GRID_SIZE+1][GRID_SIZE+1], int diff){
  int brake=0;
  for(int i=0;i<GRID_SIZE+diff-2 && brake==0;i++){
    for(int j=0;j<GRID_SIZE+diff-2 && brake==0;j++){
      if(grid[i][j]==0){
        position->row_index = i;
        position->col_index = j;
        brake = 1;
      }
    }
  }
}

/* Gameplay functions */

void create_stack(stack *stk){
  stk->size = 0;
  stk->top = NULL;
}

void store_stack(stack *stk, char move){
  s_elem *new_elem = malloc(sizeof(s_elem));
  new_elem->elem = move;
  new_elem->prev = stk->top;
  stk->top = new_elem;
  stk->size++;
}

char take_stack(stack *stk){
  if(stk->size==0){
    return 'F';
  }
  s_elem *aux = stk->top;
  char move = aux->elem;
  stk->top = aux->prev;
  free(aux);
  stk->size--;
  return move;
}

void clear_stack(stack *stk){
  s_elem *aux;
  while(stk->size>0){
    aux = stk->top;
    stk->top = aux->prev;
    free(aux);
    stk->size--;
  }
}

//would be nice to re-write movements in their own functions
int move_zero(zero_pos *position, int grid[GRID_SIZE+1][GRID_SIZE+1], char input, int diff, stack *stk){
  //for forced reset
  if(input=='R' || input=='r'){
    printf("Are you sure you want to start a new game? You will lose this game's progress.\nPress Y for yes or any letter for no.\n");
    char confirmation;
    scanf("\n%c", &confirmation);
    if(confirmation=='Y' || confirmation=='y'){
      return 2;
    }else{
      printf("Your game has been resumed. Please enter your next movement.\n");
      return 0;
    }
  }else if(input=='B' || input=='b'){
    char move = take_stack(stk);
    if(move=='F'){
      printf("There are no movements left to undo.\n");
      return 0;
    }else if(move=='W' || move=='w'){
      int aux = grid[position->row_index][position->col_index];
      grid[position->row_index][position->col_index] = grid[position->row_index+1][position->col_index];
      grid[position->row_index+1][position->col_index] = aux;
      position->row_index++;
      return 1;
    }else if(move=='A' || move=='a'){
      int aux = grid[position->row_index][position->col_index];
      grid[position->row_index][position->col_index] = grid[position->row_index][position->col_index+1];
      grid[position->row_index][position->col_index+1] = aux;
      position->col_index++;
      return 1;
    }else if(move=='S' || move=='s'){
      int aux = grid[position->row_index][position->col_index];
      grid[position->row_index][position->col_index] = grid[position->row_index-1][position->col_index];
      grid[position->row_index-1][position->col_index] = aux;
      position->row_index--;
      return 1;
    }else if(move=='D' || move=='d'){
      int aux = grid[position->row_index][position->col_index];
      grid[position->row_index][position->col_index] = grid[position->row_index][position->col_index-1];
      grid[position->row_index][position->col_index-1] = aux;
      position->col_index--;
      return 1;
    }//an else could be added here in case of an unexpected exception
  //for invalid letters
  }else if(input=='W' || input=='w'){
    //for border movements
    if(position->row_index==0){
      printf("Please enter a valid movement.\n");
      return 0;
    //for valid movements
    }else{
      store_stack(stk, input);
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
      store_stack(stk, input);
      int aux = grid[position->row_index][position->col_index];
      grid[position->row_index][position->col_index] = grid[position->row_index][position->col_index-1];
      grid[position->row_index][position->col_index-1] = aux;
      position->col_index--;
      return 1;
    }
  }else if(input=='S' || input=='s'){
    if(position->row_index==(GRID_SIZE+diff-2)-1){
      printf("Please enter a valid movement.\n");
      return 0;
    }else{
      store_stack(stk, input);
      int aux = grid[position->row_index][position->col_index];
      grid[position->row_index][position->col_index] = grid[position->row_index+1][position->col_index];
      grid[position->row_index+1][position->col_index] = aux;
      position->row_index++;
      return 1;
    }
  }else if(input=='D' || input=='d'){
    if(position->col_index==(GRID_SIZE+diff-2)-1){
      printf("Please enter a valid movement.\n");
      return 0;
    }else{
      store_stack(stk, input);
      int aux = grid[position->row_index][position->col_index];
      grid[position->row_index][position->col_index] = grid[position->row_index][position->col_index+1];
      grid[position->row_index][position->col_index+1] = aux;
      position->col_index++;
      return 1;
    }
  }else{
    printf("Please enter a valid input.\n");
    return 0;
  }
}

int reset_game(char confirmation, stack *stk){
  if(confirmation=='R' || confirmation=='r'){
    clear_stack(stk);
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

int check_victory(int grid[GRID_SIZE+1][GRID_SIZE+1], int diff, int *score){
  int win = 1;
  for(int i=0;i<GRID_SIZE+diff-2 && win==1;i++){
    for(int j=0;j<GRID_SIZE+diff-2 && win==1;j++){
      if(grid[i][j] != (GRID_SIZE+diff-2)*i+j){
        win = 0;
      }
    }
  }
  if(win==1){
    printf("Congratulations! You won!\n");
    (*score)++;
    return 1;
  }else{
    return 0;
  }
}

void show_grid(int grid[GRID_SIZE+1][GRID_SIZE+1], int diff){
  printf(" ");
  for(int i=0;i<diff-1;i++){
    printf("---");
  }
  printf("--------\n");
  for(int i=0;i<GRID_SIZE+diff-2;i++){
    for(int j=0;j<GRID_SIZE+diff-2;j++){
      printf("|%02d", grid[i][j]);
    }
    printf("|\n");
  }
  printf(" ");
  for(int i=0;i<diff-1;i++){
    printf("---");
  }
  printf("--------\n");
}

void display_instructions(){
  printf("The objetive of this game is to line the numbers in growing order,\nfrom left to right, then from top to bottom. The only number you\nare allowed to move is the 00, swapping its position with one of its\nadjacent numbers. To move it, you must type the direction you want\nit to move, then press enter.\n\nW - up, A - left, S - down, D - right\nR - re-shuffle/new game, B - back/undo\n\n");
}

void display_diff_selector(){
  printf("Press 1 to select easy mode, 2 for normal mode or 3 for hard mode,\nthen, press Enter to confirm.\n");
}

void display_score(int score){
  if(score>0){
    if(score==1){
      printf("You have won 1 game so far.\n", score);
    }else{
      printf("You have won %d games so far.\n", score);
    }
  }
}

int main(){
  number_array array;
  zero_pos position;
  stack stk;
  int score=0, diff, swap_check = 0, grid[GRID_SIZE+1][GRID_SIZE+1];
  char input, reset;

  display_instructions();
  while(1){
    display_score(score);
    display_diff_selector();
    scanf("\n%d", &diff);
    while(select_diff(diff)!=1){
      scanf("\n%d", &diff);
    }

    reset_array(&array, diff);
    scramble_array(&array, diff);
    start_grid(array, grid, diff);
    find_zero(&position, grid, diff);
    create_stack(&stk);

    while(check_victory(grid, diff, &score)!=1 && swap_check!=2){
      show_grid(grid, diff);
      swap_check = 0;
      while(swap_check==0){
        scanf("\n%c", &input);
        swap_check = move_zero(&position, grid, input, diff, &stk);
      }
    }

    if(swap_check!=2){
      reset = 'F';
    }else{
      swap_check = 0;
      reset = 'R';
    }
    while(reset_game(reset, &stk)!=1){
      scanf("\n%c", &reset);
    }
  }

  return 0;
}
