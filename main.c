#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

const int MAX_INSTRUCTIONS = 1000; // holds a maximum of 1000 instructions to be translated
const int INSTRUCTION_LENGTH = 32; // 32-bit instructions can only be 32 bits in length

void introduction();
int bitinstructionValidation(const char *bits); // checks bit instructions for 0s and 1s only
void removeSpaces(char *source, char *remove); // removes spaces in instruction input
void displayInstructs(char bits[MAX_INSTRUCTIONS][INSTRUCTION_LENGTH + 1], int count); // displays instruction inputs after input operation
void bitInput(char bits[MAX_INSTRUCTIONS][INSTRUCTION_LENGTH + 1], int *count); // checks bit instructions for length 32
void saveInstructionsToFile(char bits[MAX_INSTRUCTIONS][INSTRUCTION_LENGTH + 1], int count);
// saves 32 bit instructions to FILE 'instructions.txt'
void readInstructionFile(char bitInstructions[MAX_INSTRUCTIONS][INSTRUCTION_LENGTH+1], int *count); // to read instructions into bitInstruct
void translation(int *count, char bitInstructions[MAX_INSTRUCTIONS][INSTRUCTION_LENGTH+1]); // translating the 32-bit instructions
void output(); // prints the output of all of the translated instructions


int main(void) {
  char bits[MAX_INSTRUCTIONS][INSTRUCTION_LENGTH + 1]; // 10000 instructions max, 32-char in length
  char bitInstructions[MAX_INSTRUCTIONS][INSTRUCTION_LENGTH + 1]; // 10000 instructions max, 32-char in length
  int count = 0;

  introduction();
  bitInput(bits, &count); // begins input of 32-bit instructions
  displayInstructs(bits, count); // display all of the instructions passed through so far
  saveInstructionsToFile(bits, count); // save instructions to file
  readInstructionFile(bitInstructions, &count); // read instructions to a different array for translation from file instructions.txt
  
  translation(&count, bitInstructions);
  output();
  
  return 0;
}

void introduction(){
  printf("Welcome to the 32-Bit instruction to LEGv8 converter!\n");
  printf("You may input a maximum of 1000 instructions!\nNow beginning.....\n");
}

int bitinstructionValidation(const char *bits) {
  for (int i = 0; i < INSTRUCTION_LENGTH; i++) {
    if (bits[i] != '0' && bits[i] != '1') {
      printf("\t\t\t\t\t\t\tInvalid instruction, please try again.\n");
      printf("************************************\n\n");
      
      // the asterisks are for formatting, easier display
      return 0;
    }
  }
  printf("\t\t\t\t\t\t\tValid instruction.\n");
  printf("\t\t\t_______________________________________________\n\n");
  
  // the asterisks are for formatting, easier display
  
  return 1;
}

void removeSpaces(char *source, char *remove) {
  while (*source) {
    if (!isspace((unsigned char)*source)) {
      *remove++ = *source;
    }
    source++;
  }
  *remove = '\0';
}

void displayInstructs(char bits[MAX_INSTRUCTIONS][INSTRUCTION_LENGTH + 1], int count) {

  printf("\t\t\t_______________________________________________\n\n");
  
  printf("\n\t\t\t\t\t\t\tStored Instructions:\n");
  for (int i = 0; i < count; i++) {
    printf("\t\t\t\t\t%s\n", bits[i]);
  }
  
  if(count == 0){
    printf("\t\t\t\t\t\t\tThere's no instructions stored.");
  }

}


void bitInput(char bits[MAX_INSTRUCTIONS][INSTRUCTION_LENGTH + 1], int *count) {
  char instructionStop;
  char spaces[INSTRUCTION_LENGTH + 1];
  int i = 0;

  do {
    printf("\n\n\t\t\t************************************************\n");
    printf("\t\t\tQ = End instruction input, and begin translation\n");
    printf("\t\t\t************************************************\n\n");
    
    printf("\t\t\t\t\t\t\tONE INPUT PER LINE\nPlease enter 32-bit instructions, press ENTER to indicate a NEW instruction: "); // Asking user to input instructions
    char input[INSTRUCTION_LENGTH * 2]; // To accommodate 32 bits plus possible spaces and '\n' and '\0'
    
    if (fgets(input, sizeof(input), stdin)) {
      
      // Remove newline character if present
      input[strcspn(input, "\n")] = '\0';

      if (input[0] == 'q' || input[0] == 'Q') { // IF USER IS DONE INPUTTING INSTRUCTIONS
        instructionStop = input[0];
        break; // STOP INPUT OPERATION
      }

      removeSpaces(input, spaces); // removeSpaces function call to remove spaces
      if (strlen(spaces) != INSTRUCTION_LENGTH) { // checking length of instruction
        printf("\n************************************\n");
        printf("\t\t\tATTENTION ERROR:\n");
        printf("Invalid 32-bit instruction LENGTH found on instruction line %d. Please try again.\n", *count + 1);
        printf("************************************\n\n");
        
        continue;
      }

      strcpy(bits[*count], spaces);

      if (bitinstructionValidation(bits[*count])) { 
        // if instructions pass validation test of checking for just 0s and 1s

        
        (*count)++; // add to counter

        
        if (*count >= MAX_INSTRUCTIONS) { // if count detects 100 instructions
          printf("Maximum instructions reached.\n"); // display there are maximum instructions
          break; // end input operation
        }
      }
      
    } 
    
    else { // else, display error to user and clear buffer
      printf("Input error, please try again.\n");
      while (getchar() != '\n'); // clearing the input buffer
    }

    
  } while (instructionStop != 'q' && instructionStop != 'Q'); // input will remain open while not q or Q
}

void saveInstructionsToFile(char bits[MAX_INSTRUCTIONS][INSTRUCTION_LENGTH + 1], int count) {
  
  FILE *instructionFile = fopen("instructions.txt", "w"); // opening file in writing mode
  // if there was something there it will be overwritten with the new run of the program
  
  if (instructionFile == NULL) {
    printf("There was an error in opening the file. Please try again.\n");
    return;
  }

  for (int i = 0; i < count - 1; i++) { // reads in info from bits[i] for line count, adds newline except for the last line
    fprintf(instructionFile, "%s\n", bits[i]);
  }
  
  if (count > 0) { // prints last line in without a newline
    fprintf(instructionFile, "%s", bits[count - 1]); 
  }

  
  fclose(instructionFile); // closes file
  
  printf("Success! Your 32-bit instructions were saved to instructions.txt.\n");
  // lets user know the instructions were saved
}

void readInstructionFile(char bitInstructions[MAX_INSTRUCTIONS][INSTRUCTION_LENGTH+1], int *count){
  FILE *instructionFile = fopen("instructions.txt", "r");

  if (instructionFile == NULL) {
    printf("Error opening file for reading!\n");
    return;
  }
  char bitInstruct[INSTRUCTION_LENGTH + 1]; // buffer to hold a 32-bit instruction plus the null terminator

  int i = 0;
  while (fgets(bitInstruct, sizeof(bitInstruct), instructionFile) != NULL && i < MAX_INSTRUCTIONS) {
    bitInstruct[strcspn(bitInstruct, "\n")] = '\0'; // removes the newline character
    strcpy(bitInstructions[i], bitInstruct); // copies bitInstruct over to bitInstructions
    i++;
  }

  *count = i; // updating count to the number of instructions read

  fclose(instructionFile);

  printf("\n\t\t\t\t\tInstructions from file:\n");
  for (int i = 0; i < *count; i++) {
    printf("\t\t\t\t\t%s\n", bitInstructions[i]);
  }
  
}

void translation(int *count, char bitInstructions[MAX_INSTRUCTIONS][INSTRUCTION_LENGTH+1]){
  char bitOpcode[7]; // bitOpcode will hold the first 6 digits of the 32-bit instructions for comparison later
  char opcodes[MAX_INSTRUCTIONS][5]; // instructions are typically 3, or 4 letters + null

  // char variables for type R
  char secondRegister[MAX_INSTRUCTIONS][6]; // 5 bits for second register
  char shift[MAX_INSTRUCTIONS][7]; // 6 bits for shift
  char firstRegister[MAX_INSTRUCTIONS][6]; // 5 bits for destination register
  char destinationRegister[MAX_INSTRUCTIONS][6]; // 5 bits for destination register

  // char variables for type I
  char alImmediate[MAX_INSTRUCTIONS][13]; // 12 bits for AL&immediate
  char sourceReg[MAX_INSTRUCTIONS][6]; // 5 bits for source reg
  char typeIdestination[MAX_INSTRUCTIONS][6]; // 5 bits for destination reg

  // char variables for type D

  char addressOffset[MAX_INSTRUCTIONS][10]; // 9 bits for address offset
  char baseRegister[MAX_INSTRUCTIONS][6]; // 5 bits for base register
  char typeDregister[MAX_INSTRUCTIONS][6]; // 5 bits for destination register
  char twoBits[MAX_INSTRUCTIONS][3]; // 2 bits held

  FILE *translated_instructions = fopen("translated_instructions.txt", "w"); // opening file in writing mode
  if (translated_instructions == NULL) {
    printf("There was an error in opening the translated instructions file. Please try again.\n");
    return;
  }
  
  printf("\n\n************************************\n");
  printf("Now beginning translation:\n");
  printf("************************************\n\n");
  
  
  for (int i = 0; i < *count; i++) { // read in every line until count
    strncpy(bitOpcode, bitInstructions[i], 5);
    bitOpcode[6] = '\0';
    

    // ugly coding section because string literals don't work with switch cases

    // THIS IS TO DETERMINE THE OPCODE, AT THE START OF THE 32-BIT INSTRUCTION
    if (strcmp(bitOpcode, "10001") == 0){ // IF THE FIRST 6 CHARACTERS ARE EQUAL TO CERTAIN DIGITS
      strcpy(opcodes[i], "ADD");
    }
    if (strcmp(bitOpcode, "11001") == 0){
      strcpy(opcodes[i], "SUB");
    }
    if (strcmp(bitOpcode, "10010") == 0){
      strcpy(opcodes[i], "ADDI");
    }
    if (strcmp(bitOpcode, "11010") == 0){
      strcpy(opcodes[i], "SUBI");
    }
    if (strcmp(bitOpcode, "11111") == 0){
      
      if (bitInstructions[i][30] == '1'){ // IF THE 31ST CHARACTER IS 1
        strcpy(opcodes[i], "LDUR");
      }
      else
        strcpy(opcodes[i], "STUR");
      
    }



    
    // *****************************************************
    
    // BREAKING DOWN THE REST OF THE INSTRUCTIONS BASED ON TYPE FORMAT NOW

    // TYPE R BREAKING DOWN (ADD, SUB):
    if (strcmp(opcodes[i], "ADD") == 0 || strcmp(opcodes[i], "SUB") == 0) {
      printf("\nTRANSLATING TYPE R INSTRUCTION:\n");
      // jumping to character 11, that's where we're gonna start reading
      // we already determined what label it is
      strncpy(secondRegister[i], bitInstructions[i] + 11, 5); // copying only 11-15 into string secondRegister
      secondRegister[i][5] = '\0'; // null termination of string

      printf("Second Register: %s\n", secondRegister[i]);

      // SHIFT?
      strncpy(shift[i], bitInstructions[i] + 16, 6); // copying only 17-22 into string firstRegister
      shift[i][6] = '\0'; // null termination of string
      printf("shift: %s\n", shift[i]);


      // FIRST REGISTER
      strncpy(firstRegister[i], bitInstructions[i] + 22, 5); // copying only 23-27 into string firstRegister
      firstRegister[i][5] = '\0'; // null termination of string
      printf("First Register: %s\n", firstRegister[i]);

      // DESTINATION REGISTER
      strncpy(destinationRegister[i], bitInstructions[i] + 27, 5); // copying only 28-32 into string destinationRegister
      destinationRegister[i][5] = '\0'; // null termination of string
      printf("Destination Register: %s\n", destinationRegister[i]);


      /************** SAVES TRANSLATIONS ******************/

      // converts destinationRegister, firstRegister, secondRegister
      // then prints into translated_instructions file
      
      fprintf(translated_instructions, "%s X%d, X%d, X%d\n", opcodes[i], 
        (int)strtol(destinationRegister[i], NULL, 2), 
        (int)strtol(firstRegister[i], NULL, 2), 
        (int)strtol(secondRegister[i], NULL, 2));
    }
  


    
    // *****************************************************
    // TYPE I BREAKING DOWN
    // example:
    // 1101000100 OPCODE
    // 000001000011 AL&IMMEDIATE
    // 00100 SOURCE REGISTER
    // 00011 DESTINATION REGISTER

    else if (strcmp(opcodes[i], "ADDI") == 0 || strcmp(opcodes[i], "SUBI") == 0) {
      printf("\nTRANSLATING TYPE I INSTRUCTION:\n");
      // starting immediately after the OP code, at character 10
      strncpy(alImmediate[i], bitInstructions[i] + 10, 12); // copying only 11-15 into alImmediate
        alImmediate[i][12] = '\0'; // null termination of string

      printf("Al & Immediate: %s\n", alImmediate[i]);

      // SHIFT?
      strncpy(sourceReg[i], bitInstructions[i] + 22, 5); // copying 23-29 into string sourceReg
      sourceReg[i][5] = '\0'; // null termination of string
      printf("Source Register: %s\n", sourceReg[i]);

      // DESTINATION REGISTER
      strncpy(typeIdestination[i], bitInstructions[i] + 27, 5); // copying 28-32 into string typeIdestination
      typeIdestination[i][5] = '\0'; // null termination of string
      printf("Destination Register: %s\n", typeIdestination[i]);


      /************** SAVES TRANSLATIONS ******************/
      // CONVERT SOURCEREG, TYPE I DESTINATION AND alImmediate (12 digits)
      // then PRINT into file translated_instructions
      
      fprintf(translated_instructions, "%s X%d, X%d, #%d\n", opcodes[i], 
        (int)strtol(typeIdestination[i], NULL, 2), 
        (int)strtol(sourceReg[i], NULL, 2), 
        (int)strtol(alImmediate[i], NULL, 2));
      
    }


    // *****************************************************
    // TYPE D BREAKING DOWN
    // example:
    // 11111000000000010000001010000100
    // 11111000000 11 bits
    // 000000100 9 bits address offset
    // 10100 5 bits base register
    // 00100 5 bits destination register
    
    else if (strcmp(opcodes[i], "LDUR") == 0 || strcmp(opcodes[i], "STUR") == 0) {
      printf("\nTRANSLATING TYPE D INSTRUCTION:\n");
      // starting immediately after the OP code, at character 10
      strncpy(addressOffset[i], bitInstructions[i] + 11, 9); // copying 11-15 into string addressOffset
      addressOffset[i][9] = '\0'; // null termination of string

      printf("Address Offset: %s\n", addressOffset[i]);

      // 2 BIT HOLDER
      strncpy(twoBits[i], bitInstructions[i] + 20, 2); // copying 11-15 into string twoBits
      twoBits[i][2] = '\0'; // null termination of string
      printf("Two unused bits: %s\n", twoBits[i]);

      // BASE REGISTER
      strncpy(baseRegister[i], bitInstructions[i] + 22, 5); // copying  23-29 into string baseRegister
      baseRegister[i][5] = '\0'; // null termination of string
      printf("Base Register: %s\n", baseRegister[i]);

      // DESTINATION REGISTER
      strncpy(typeDregister[i], bitInstructions[i] + 27, 5); // copying 28-32 into string typeDregister
      typeDregister[i][5] = '\0'; // null termination of string
      printf("Destination Register: %s\n", typeDregister[i]);

      /************** SAVES TRANSLATIONS ******************/

      // converting typeDregister, baseRegister, addressOffset
      // then PRINT into file translated_instructions
      
      fprintf(translated_instructions, "%s X%d, [X%d, #%d]\n", opcodes[i], 
      (int)strtol(typeDregister[i], NULL, 2), 
      (int)strtol(baseRegister[i], NULL, 2), 
      (int)strtol(addressOffset[i], NULL, 2));

      
    }

    
  }

  fclose(translated_instructions); // closes file
  

  
}

void output() {
  int i = 0; // when iterating through file to print
  
  printf("\n\n************************************\n");
  printf("Your translated instructions are:\n");
  printf("************************************\n\n");

  
  // opening file var instructionFile to open translated_instructions in read mode
  FILE *instructionFile = fopen("translated_instructions.txt", "r");

  // if reading file failrs
  if (instructionFile == NULL) {
    printf("Error opening file for reading!\n");
    return;
  }

  // buffer when reading in from file
  char translatedInstructions[INSTRUCTION_LENGTH + 1];
  

  // reading every line from file and displaying to user what's in translated_instructions.txt
  while (fgets(translatedInstructions, sizeof(translatedInstructions), instructionFile) != NULL && i < MAX_INSTRUCTIONS) {
  
    translatedInstructions[strcspn(translatedInstructions, "\n")] = '\0'; // removes endline
    printf("%s\n", translatedInstructions); // prints instructions from file
    i++;
  }
  
  // close the file
  fclose(instructionFile);
  printf("\n\nThank you for using this program! Your translated instructions can be found in translated_instructions.txt\n\nNOTE:\nAny instructions (32-bit and LEGv8) translated during this run of the program\nwill be overwritten in their files:\n\n'instructions.txt' and 'translated_instructions.txt'\n\nwhen using the program again next time.");
  
  
}
