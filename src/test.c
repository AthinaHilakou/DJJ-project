#include <stdio.h>
#include <stdlib.h>
#include "../headers/data.h"


int my_test_fun(){
    int lines;
    char* name = "datasets/given/00000020.bin";
    Data file_data = import_data(name, &lines);
    return 0;
}













