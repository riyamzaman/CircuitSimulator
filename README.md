# CircuitSimulator

With a circuit description file that describes a circuit using various directives, the C program will print the output of the circuit for all possible values.

The input variables used in the circuit are provided using the INPUTVAR directive, which is followed by the number of input variables and the names of the input variables. All the input variables are named with capitalized identifiers. An identifier consists of at least one character (A-Z) followed by a series of zero or many characters (A-Z) or digit (0-9). 

An example specification: INPUTVAR 3 IN1 IN2 IN3

The outputs produced by the circuit is specified using the OUTPUTVAR directive, which is followed by the number of outputs and the names of the outputs.

An example specification: OUTPUTVAR 1 OUT 1

The circuits used in the description file will be built using the following supported building blocks: NOT, AND, OR, NAND, NOR, and XOR. The building blocks can produce temporary variables as outputs, and can use either the input variables or temporary variables as input. Output variables will never be used as inputs in a building block. All the temporary variables will also be named with lowercase idenitifiers (e.g. temp1, temp2, etc.)

The example specification of each building block is as follows:
- NOT:
  NOT IN1 OUT1
 - AND:
  AND IN1 IN2 OUT1
 - OR:
  OR IN1 IN2 OUT1
 - NAND:
  NAND IN1 IN2 OUT1
 - NOR:
  NOR IN1 IN2 OUT1
 - XOR:
  XOR IN1 IN2 OUT1
