#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define GRID_SIZE 4

typedef struct {
  int numbers[GRID_SIZE*GRID_SIZE];
  int size;
} number_array;

void reset_array(number_array *array){
  for(int i=0;i<GRID_SIZE*GRID_SIZE;i++){
    array->numbers[i] = i;
  }
  array->size = GRID_SIZE*GRID_SIZE;
}

void print_array(number_array *array){
  for(int i=0;i<GRID_SIZE*GRID_SIZE;i++){
    printf("%d ", array->numbers[i]);
  }
  printf("\n");
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

int main(){
  number_array array;
  reset_array(&array);
  scramble_array(&array);
  print_array(&array);
  return 0;
}
