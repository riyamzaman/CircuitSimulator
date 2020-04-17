#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char* name;
    int index;
    struct Node* next;
} Node;

void insert(Node** front, char* str, int num) 
{
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        printf("malloc failed!\n");
        exit(1);
    }
    new_node -> name = str;
    new_node -> index = num;
    new_node -> next = NULL;

    Node* ptr = *front;

    if (*front == NULL) {
        *front = new_node;
        return;
    }
    
    while (ptr -> next != NULL) {
        ptr = ptr -> next;
    }
    ptr -> next = new_node;
    return;
}

int insertTemp(Node** front, char* str) 
{
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        printf("malloc failed!\n");
        exit(1);
    }
    new_node -> name = str;
    new_node -> index = 0;
    new_node -> next = NULL;

    Node* ptr = *front;
    int c = 1;

    if (*front == NULL) {
        *front = new_node;
        new_node -> index = 0;
        return 0;
    }
    
    while (ptr -> next != NULL) {
        ptr = ptr -> next;
        c++;
    }
    ptr -> next = new_node;
    new_node -> index = c;
    return c;
}

void printLL(Node* front) 
{
    Node* prev = NULL;
    Node* ptr = front;

    while (ptr != NULL) {
        if (prev == NULL) {
            printf("%s: %d --> ", ptr -> name, ptr -> index);
        } else if (prev -> index != ptr -> index) {
            printf("%s: %d --> ", ptr -> name, ptr -> index);
        }
        prev = ptr;
        ptr = ptr -> next;
    }
    printf("\\");
}

void freeLL(Node* front) {
    Node* ptr;
    while (front != NULL) {
        ptr = front;
        front = front -> next;
        free(ptr -> name);
        free(ptr);
    }
}

void printArray(int r, int c1, int c2, int** inputs, int** outputs) 
{
    int i;
    int j;
    for (i = 0; i < r; i++) {
        for (j = 0; j < c1; j++) {
            printf("%d ", inputs[i][j]);
        }
        for (j = 0; j < c2; j++) {
            printf("%d ", outputs[i][j]);
        }
        printf("\n");
    }
}

int inArray(Node* front, char* str)
{
    Node* ptr = front;
    while (ptr != NULL) {
        if (strcmp(ptr -> name, str) == 0) {
            return (ptr -> index);
        }
        ptr = ptr -> next;
    }
    return -1;
}

int** resize(int rows, int oldWidth, int width, int** tempArray)
{
    int i;
    int j;
    for (i = 0; i < rows; i++) {
        tempArray[i] = realloc(tempArray[i], width * sizeof(int));
        if (tempArray[i] == NULL) {
            printf("realloc failed!\n");
            exit(1);
        }
        for (j = oldWidth - 1; j < width; j++) {
            tempArray[i][j] = 0;
        }
    }
    return tempArray;
}

void notGate(int** in1Array, int in1_index, int** outArray, int out_index, int rows) {
    int i;
    for (i = 0; i < rows; i++) {
        outArray[i][out_index] = ~(in1Array[i][in1_index]) + 2;
    }
}

void andGate(int** in1Array, int in1_index, int** in2Array, int in2_index, int** outArray, int out_index, int rows) {
    int i;
    for (i = 0; i < rows; i++) {
        outArray[i][out_index] = in1Array[i][in1_index] & in2Array[i][in2_index];
    }
}

void orGate(int** in1Array, int in1_index, int** in2Array, int in2_index, int** outArray, int out_index, int rows) {
    int i;
    for (i = 0; i < rows; i++) {
        outArray[i][out_index] = in1Array[i][in1_index] | in2Array[i][in2_index];
    }
}

void xorGate(int** in1Array, int in1_index, int** in2Array, int in2_index, int** outArray, int out_index, int rows) {
    int i;
    for (i = 0; i < rows; i++) {
        outArray[i][out_index] = in1Array[i][in1_index] ^ in2Array[i][in2_index];
    }
}

void nandGate(int** in1Array, int in1_index, int** in2Array, int in2_index, int** outArray, int out_index, int rows) {
    int i;
    for (i = 0; i < rows; i++) {
        outArray[i][out_index] = ~(in1Array[i][in1_index] & in2Array[i][in2_index]) + 2;
    }
}

void norGate(int** in1Array, int in1_index, int** in2Array, int in2_index, int** outArray, int out_index, int rows) {
    int i;
    for (i = 0; i < rows; i++) {
        outArray[i][out_index] = ~(in1Array[i][in1_index] | in2Array[i][in2_index]) + 2;
    }
}

int main(int argc, char* argv[])
{
    FILE* f = fopen(argv[1], "r");

    if (f == NULL) {
        printf("error opening file %s\n", argv[1]);
        return 1;
    }

    int inputNum;
    int outputNum;
    int i;
    char* input = malloc(11*sizeof(char));
    if (input == NULL) {
        printf("malloc failed!\n");
        exit(1);
    }
    Node* inputFront = NULL;
    char* output = malloc(11*sizeof(char));
    if (output == NULL) {
        printf("malloc failed!\n");
        exit(1);
    }
    Node* outputFront = NULL;

    fscanf(f, "INPUTVAR %d", &inputNum);    // Map input variable names in LL
    for (i = 0; i < inputNum; i++) {
        fscanf(f, "%s", input);
        insert(&inputFront, input, i);
        input = malloc(sizeof(char) * 11);
        if (input == NULL) {
            printf("malloc failed!\n");
            exit(1);
        }
    }
    fscanf(f, "\n");

    fscanf(f, "OUTPUTVAR %d", &outputNum);  // Map output variable names in LL
    for (i = 0; i < outputNum; i++) {
        fscanf(f, "%s", output);
        insert(&outputFront, output, i);
        output = malloc(sizeof(char) * 11);
        if (output == NULL) {
            printf("malloc failed!\n");
            exit(1);
        }
    }
    fscanf(f, "\n");

    int rows = (1 << inputNum);    // Generate number of rows for table of ins

    int** inputArray = malloc(rows * sizeof(int*)); // Create 2D array to store ins/outs
    if (inputArray == NULL) {
        printf("malloc failed!\n");
        exit(1);
    }
    for (i = 0; i < rows; i++) {
        inputArray[i] = (int*) malloc((inputNum) * sizeof(int));
        if (inputArray[i] == NULL) {
            printf("malloc failed!\n");
            exit(1);
        }
    }
    int j;
    int c;
    for (i = 0; i < rows; i++) {
        c = inputNum - 1;
        for (j = 0; j < inputNum; j++) {
            inputArray[i][j] = ((i >> c) & 1);
            c--;
        }
    }
    int** outputArray = malloc(rows * sizeof(int*)); // Create 2D array to store outs
    if (outputArray == NULL) {
        printf("malloc failed!\n");
        exit(1);
    }
    for (i = 0; i < rows; i++) {
        outputArray[i] = (int*) malloc((outputNum) * sizeof(int));
        if (outputArray[i] == NULL) {
            printf("malloc failed!\n");
            exit(1);
        }
    }

    Node* tempFront = NULL;
    char directive[10];
    char* in1 = malloc(11*sizeof(char));
    if (in1 == NULL) {
        printf("malloc failed!\n");
        exit(1);
    }
    char* in2 = malloc(11*sizeof(char));
    if (in2 == NULL) {
        printf("malloc failed!\n");
        exit(1);
    }
    char* out = malloc(11*sizeof(char));
    if (out == NULL) {
        printf("malloc failed!\n");
        exit(1);
    }
    int in1_index;
    int in2_index;
    int out_index;

    int temp_in1 = 0;   // bool
    int temp_in2 = 0;   // bool
    int temp_out = 0;   // bool

    int** tempArray = malloc(rows*sizeof(int*));
    if (tempArray == NULL) {
        printf("malloc failed!\n");
        exit(1);
    }
    for (i = 0; i < rows; i++) {
        tempArray[i] = (int*) malloc(sizeof(int));
        if (tempArray[i] == NULL) {
            printf("malloc failed!\n");
            exit(1);
        }
    }

    while (fscanf(f, "%s", directive) != EOF) {
        if (strcmp(directive, "NOT") == 0) {
            fscanf(f, "%s", in1);
            if (inArray(inputFront, in1) == -1) {
                // Temp variable case
                in1_index = inArray(tempFront, in1);    
                temp_in1 = 1;
            } else {
                in1_index = inArray(inputFront, in1);
                temp_in1 = 0;
            }
            fscanf(f, "%s", out);
            if (inArray(outputFront, out) == -1) {
                // Temp variable case
               if (tempFront == NULL) {
                    insert(&tempFront, out, 0);
                    out = malloc(sizeof(char) * 11);
                    if (out == NULL) {
                        printf("malloc failed!\n");
                        exit(1);
                    }
                    out_index = 0;
                    temp_out = 1;
               } else if (inArray(tempFront, out) == -1) {
                    out_index = insertTemp(&tempFront, out);
                    out = malloc(sizeof(char) * 11);
                    if (out == NULL) {
                        printf("malloc failed!\n");
                        exit(1);
                    }
                    tempArray = resize(rows, out_index + 1, out_index + 2, tempArray);
                    temp_out = 1;
               } else {
                   out_index = inArray(tempFront, out);
                   temp_out = 1;
               }
            } else {
                out_index = inArray(outputFront, out);
                temp_out = 0;
            }
            if (temp_in1 == 1) {
                if (temp_out == 1) {
                    notGate(tempArray, in1_index, tempArray, out_index, rows);
                } else {
                    notGate(tempArray, in1_index, outputArray, out_index, rows);
                }
            } else {
                if (temp_out == 1) {
                    notGate(inputArray, in1_index, tempArray, out_index, rows);
                } else {
                    notGate(inputArray, in1_index, outputArray, out_index, rows);
                }
            }
            fscanf(f, "\n");
        } else if (strcmp(directive, "AND") == 0) {
           fscanf(f, "%s", in1);
            if (inArray(inputFront, in1) == -1) {
                // Temp variable case
                in1_index = inArray(tempFront, in1);    
                temp_in1 = 1;
            } else {
                in1_index = inArray(inputFront, in1);
                temp_in1 = 0;
            }
            fscanf(f, "%s", in2);
            if (inArray(inputFront, in2) == -1) {
                // Temp variable case
                in2_index = inArray(tempFront, in2);    
                temp_in2 = 1;
            } else {
                in2_index = inArray(inputFront, in2);
                temp_in2 = 0;
            }
            fscanf(f, "%s", out);
            if (inArray(outputFront, out) == -1) {
                // Temp variable case
               if (tempFront == NULL) {
                    insert(&tempFront, out, 0);
                    out = malloc(sizeof(char) * 11);
                    if (out == NULL) {
                        printf("malloc failed!\n");
                        exit(1);
                    }
                    out_index = 0;
                    temp_out = 1;
               } else if (inArray(tempFront, out) == -1) {
                    out_index = insertTemp(&tempFront, out);
                    out = malloc(sizeof(char) * 11);
                    if (out == NULL) {
                        printf("malloc failed!\n");
                        exit(1);
                    }
                    tempArray = resize(rows, out_index + 1, out_index + 2, tempArray);
                    temp_out = 1;
               } else {
                   out_index = inArray(tempFront, out);
                   temp_out = 1;
               }
            } else {
                out_index = inArray(outputFront, out);
                temp_out = 0;
            }

            if (temp_in1 == 1) {
                if (temp_in2 == 1) {
                    if (temp_out == 1) {
                        andGate(tempArray, in1_index, tempArray, in2_index, tempArray, out_index, rows);
                    } else {
                        andGate(tempArray, in1_index, tempArray, in2_index, outputArray, out_index, rows);
                    }
                } else {    // temp_in2 == 0
                    if (temp_out == 1) {
                        andGate(tempArray, in1_index, inputArray, in2_index, tempArray, out_index, rows);
                    } else {
                        andGate(tempArray, in1_index, inputArray, in2_index, outputArray, out_index, rows);
                    }
                }
            } else { // temp_in1 == 0
                if (temp_in2 == 1) {
                    if (temp_out == 1) {
                        andGate(inputArray, in1_index, tempArray, in2_index, tempArray, out_index, rows);
                    } else {
                        andGate(inputArray, in1_index, tempArray, in2_index, outputArray, out_index, rows);
                    }
                } else { //temp_in2 == 0
                    if (temp_out == 1) {
                        andGate(inputArray, in1_index, inputArray, in2_index, tempArray, out_index, rows);
                    } else {
                        andGate(inputArray, in1_index, inputArray, in2_index, outputArray, out_index, rows);
                    }
                }
            }  
            fscanf(f, "\n");

        } else if (strcmp(directive, "OR") == 0) {
           fscanf(f, "%s", in1);
            if (inArray(inputFront, in1) == -1) {
                // Temp variable case
                in1_index = inArray(tempFront, in1);    
                temp_in1 = 1;
            } else {
                in1_index = inArray(inputFront, in1);
                temp_in1 = 0;
            }
            fscanf(f, "%s", in2);
            if (inArray(inputFront, in2) == -1) {
                // Temp variable case
                in2_index = inArray(tempFront, in2);    
                temp_in2 = 1;
            } else {
                in2_index = inArray(inputFront, in2);
                temp_in2 = 0;
            }
            fscanf(f, "%s", out);
            if (inArray(outputFront, out) == -1) {
                // Temp variable case
               if (tempFront == NULL) {
                    insert(&tempFront, out, 0);
                    out = malloc(sizeof(char) * 11);
                    if (out == NULL) {
                        printf("malloc failed!\n");
                        exit(1);
                    }
                    out_index = 0;
                    temp_out = 1;
               } else if (inArray(tempFront, out) == -1) {
                    out_index = insertTemp(&tempFront, out);
                    out = malloc(sizeof(char) * 11);
                    if (out == NULL) {
                        printf("malloc failed!\n");
                        exit(1);
                    }
                    tempArray = resize(rows, out_index + 1, out_index + 2, tempArray);
                    temp_out = 1;
                   // Insert at end of LL
                   // Retrieve index
                   // Expand Array
               } else {
                   out_index = inArray(tempFront, out);
                   temp_out = 1;
               }
            } else {
                out_index = inArray(outputFront, out);
                temp_out = 0;
            }

            if (temp_in1 == 1) {
                if (temp_in2 == 1) {
                    if (temp_out == 1) {
                        orGate(tempArray, in1_index, tempArray, in2_index, tempArray, out_index, rows);
                    } else {
                        orGate(tempArray, in1_index, tempArray, in2_index, outputArray, out_index, rows);
                    }
                } else {    // temp_in2 == 0
                    if (temp_out == 1) {
                        orGate(tempArray, in1_index, inputArray, in2_index, tempArray, out_index, rows);
                    } else {
                        orGate(tempArray, in1_index, inputArray, in2_index, outputArray, out_index, rows);
                    }
                }
            } else { // temp_in1 == 0
                if (temp_in2 == 1) {
                    if (temp_out == 1) {
                        orGate(inputArray, in1_index, tempArray, in2_index, tempArray, out_index, rows);
                    } else {
                        orGate(inputArray, in1_index, tempArray, in2_index, outputArray, out_index, rows);
                    }
                } else { //temp_in2 == 0
                    if (temp_out == 1) {
                        orGate(inputArray, in1_index, inputArray, in2_index, tempArray, out_index, rows);
                    } else {
                        orGate(inputArray, in1_index, inputArray, in2_index, outputArray, out_index, rows);
                    }
                }
            }  
            fscanf(f, "\n");
        } else if (strcmp(directive, "XOR") == 0) {
           fscanf(f, "%s", in1);
            if (inArray(inputFront, in1) == -1) {
                // Temp variable case
                in1_index = inArray(tempFront, in1);    
                temp_in1 = 1;
            } else {
                in1_index = inArray(inputFront, in1);
                temp_in1 = 0;
            }
            fscanf(f, "%s", in2);
            if (inArray(inputFront, in2) == -1) {
                // Temp variable case
                in2_index = inArray(tempFront, in2);    
                temp_in2 = 1;
            } else {
                in2_index = inArray(inputFront, in2);
                temp_in2 = 0;
            }
            fscanf(f, "%s", out);
            if (inArray(outputFront, out) == -1) {
                // Temp variable case
               if (tempFront == NULL) {
                    insert(&tempFront, out, 0);
                    out = malloc(sizeof(char) * 11);
                    if (out == NULL) {
                        printf("malloc failed!\n");
                        exit(1);
                    }
                    out_index = 0;
                    temp_out = 1;
               } else if (inArray(tempFront, out) == -1) {
                    out_index = insertTemp(&tempFront, out);
                    out = malloc(sizeof(char) * 11);
                    if (out == NULL) {
                        printf("malloc failed!\n");
                        exit(1);
                    }
                    tempArray = resize(rows, out_index + 1, out_index + 2, tempArray);
                    temp_out = 1;
               } else {
                   out_index = inArray(tempFront, out);
                   temp_out = 1;
               }
            } else {
                out_index = inArray(outputFront, out);
                temp_out = 0;
            }

            if (temp_in1 == 1) {
                if (temp_in2 == 1) {
                    if (temp_out == 1) {
                        xorGate(tempArray, in1_index, tempArray, in2_index, tempArray, out_index, rows);
                    } else {
                        xorGate(tempArray, in1_index, tempArray, in2_index, outputArray, out_index, rows);
                    }
                } else {    // temp_in2 == 0
                    if (temp_out == 1) {
                        xorGate(tempArray, in1_index, inputArray, in2_index, tempArray, out_index, rows);
                    } else {
                        xorGate(tempArray, in1_index, inputArray, in2_index, outputArray, out_index, rows);
                    }
                }
            } else { // temp_in1 == 0
                if (temp_in2 == 1) {
                    if (temp_out == 1) {
                        xorGate(inputArray, in1_index, tempArray, in2_index, tempArray, out_index, rows);
                    } else {
                        xorGate(inputArray, in1_index, tempArray, in2_index, outputArray, out_index, rows);
                    }
                } else { //temp_in2 == 0
                    if (temp_out == 1) {
                        xorGate(inputArray, in1_index, inputArray, in2_index, tempArray, out_index, rows);
                    } else {
                        xorGate(inputArray, in1_index, inputArray, in2_index, outputArray, out_index, rows);
                    }
                }
            }  
            fscanf(f, "\n");
        } else if (strcmp(directive, "NAND") == 0) {
           fscanf(f, "%s", in1);
            if (inArray(inputFront, in1) == -1) {
                // Temp variable case
                in1_index = inArray(tempFront, in1);    
                temp_in1 = 1;
            } else {
                in1_index = inArray(inputFront, in1);
                temp_in1 = 0;
            }
            fscanf(f, "%s", in2);
            if (inArray(inputFront, in2) == -1) {
                // Temp variable case
                in2_index = inArray(tempFront, in2);    
                temp_in2 = 1;
            } else {
                in2_index = inArray(inputFront, in2);
                temp_in2 = 0;
            }
            fscanf(f, "%s", out);
            if (inArray(outputFront, out) == -1) {
                // Temp variable case
               if (tempFront == NULL) {
                    insert(&tempFront, out, 0);
                    out = malloc(sizeof(char) * 11);
                    if (out == NULL) {
                        printf("malloc failed!\n");
                        exit(1);
                    }
                    out_index = 0;
                    temp_out = 1;
               } else if (inArray(tempFront, out) == -1) {
                    out_index = insertTemp(&tempFront, out);
                    out = malloc(sizeof(char) * 11);
                    if (out == NULL) {
                        printf("malloc failed!\n");
                        exit(1);
                    }
                    tempArray = resize(rows, out_index + 1, out_index + 2, tempArray);
                    temp_out = 1;
               } else {
                   out_index = inArray(tempFront, out);
                   temp_out = 1;
               }
            } else {
                out_index = inArray(outputFront, out);
                temp_out = 0;
            }

            if (temp_in1 == 1) {
                if (temp_in2 == 1) {
                    if (temp_out == 1) {
                        nandGate(tempArray, in1_index, tempArray, in2_index, tempArray, out_index, rows);
                    } else {
                        nandGate(tempArray, in1_index, tempArray, in2_index, outputArray, out_index, rows);
                    }
                } else {    // temp_in2 == 0
                    if (temp_out == 1) {
                        nandGate(tempArray, in1_index, inputArray, in2_index, tempArray, out_index, rows);
                    } else {
                        nandGate(tempArray, in1_index, inputArray, in2_index, outputArray, out_index, rows);
                    }
                }
            } else { // temp_in1 == 0
                if (temp_in2 == 1) {
                    if (temp_out == 1) {
                        nandGate(inputArray, in1_index, tempArray, in2_index, tempArray, out_index, rows);
                    } else {
                        nandGate(inputArray, in1_index, tempArray, in2_index, outputArray, out_index, rows);
                    }
                } else { //temp_in2 == 0
                    if (temp_out == 1) {
                        nandGate(inputArray, in1_index, inputArray, in2_index, tempArray, out_index, rows);
                    } else {
                        nandGate(inputArray, in1_index, inputArray, in2_index, outputArray, out_index, rows);
                    }
                }
            }  
            fscanf(f, "\n");
        } else if (strcmp(directive, "NOR") == 0) {
           fscanf(f, "%s", in1);
            if (inArray(inputFront, in1) == -1) {
                // Temp variable case
                in1_index = inArray(tempFront, in1);    
                temp_in1 = 1;
            } else {
                in1_index = inArray(inputFront, in1);
                temp_in1 = 0;
            }
            fscanf(f, "%s", in2);
            if (inArray(inputFront, in2) == -1) {
                // Temp variable case
                in2_index = inArray(tempFront, in2);    
                temp_in2 = 1;
            } else {
                in2_index = inArray(inputFront, in2);
                temp_in2 = 0;
            }
            fscanf(f, "%s", out);
            if (inArray(outputFront, out) == -1) {
                // Temp variable case
               if (tempFront == NULL) {
                    insert(&tempFront, out, 0);
                    out = malloc(sizeof(char) * 11);
                    if (out == NULL) {
                        printf("malloc failed!\n");
                        exit(1);
                    }
                    out_index = 0;
                    temp_out = 1;
               } else if (inArray(tempFront, out) == -1) {
                    out_index = insertTemp(&tempFront, out);
                    out = malloc(sizeof(char) * 11);
                    if (out == NULL) {
                        printf("malloc failed!\n");
                        exit(1);
                    }
                    tempArray = resize(rows, out_index + 1, out_index + 2, tempArray);
                    temp_out = 1;
               } else {
                   out_index = inArray(tempFront, out);
                   temp_out = 1;
               }
            } else {
                out_index = inArray(outputFront, out);
                temp_out = 0;
            }

            if (temp_in1 == 1) {
                if (temp_in2 == 1) {
                    if (temp_out == 1) {
                        norGate(tempArray, in1_index, tempArray, in2_index, tempArray, out_index, rows);
                    } else {
                        norGate(tempArray, in1_index, tempArray, in2_index, outputArray, out_index, rows);
                    }
                } else {    // temp_in2 == 0
                    if (temp_out == 1) {
                        norGate(tempArray, in1_index, inputArray, in2_index, tempArray, out_index, rows);
                    } else {
                        norGate(tempArray, in1_index, inputArray, in2_index, outputArray, out_index, rows);
                    }
                }
            } else { // temp_in1 == 0
                if (temp_in2 == 1) {
                    if (temp_out == 1) {
                        norGate(inputArray, in1_index, tempArray, in2_index, tempArray, out_index, rows);
                    } else {
                        norGate(inputArray, in1_index, tempArray, in2_index, outputArray, out_index, rows);
                    }
                } else { //temp_in2 == 0
                    if (temp_out == 1) {
                        norGate(inputArray, in1_index, inputArray, in2_index, tempArray, out_index, rows);
                    } else {
                        norGate(inputArray, in1_index, inputArray, in2_index, outputArray, out_index, rows);
                    }
                }
            }  
            fscanf(f, "\n");
        }
    }

    fclose(f);
    
    printArray(rows, inputNum, outputNum, inputArray, outputArray);
    printf("\n");

    free(input);
    free(output);
    freeLL(inputFront);
    freeLL(outputFront);
    freeLL(tempFront);
    free(in1);
    free(in2);
    free(out);

    for (i = 0; i < rows; i++) {
        free(inputArray[i]);
        free(outputArray[i]);
        free(tempArray[i]);
    }
    free(inputArray);
    free(outputArray);
    free(tempArray);

    return 0;
}