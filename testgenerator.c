/* Test file generator!

      Aaron Meurer
         Oran Wallace
            Sheng Lundquist

              This file is used to generate the file “tests” which is used by “queuemanager_test.c” to perform    various functions of our “queuemanager.c” program.

*/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>



int main() {

  int i = 0;
  FILE *fp, *output;
  char *mode = "w";
  char outputFilename[] = "tests";
  int input=0, number=0, pid=0;
  int psw=0, page=0, reg1=0, reg2=0, reg3=0;

  if(fp == NULL){
    printf("File does not exsist");
    exit(1);
  }

  output = fopen(outputFilename, mode);

  if(output == NULL){
    printf("Cannot get output file");

    exit(1);
  }

  printf("1) Init_Q \n");
  printf("2) Enqueue \n");
  printf("3) Dequeue \n");
  printf("4) Delete \n");
  printf("5) List \n");
  printf("6) Exit \n");



  while(input != 6){

    printf("Enter an option: \n");
    scanf("%d", &input);

    switch(input){
    case 1:
      fprintf(output, "init_Q\n");
      break;
    case 2:
      printf("How many would you like to enqueue?");
      scanf("%d", &number);

      for(i = 0; i < number; i++){
        psw = random() % 50;
        page = random() % 25;
        reg1 = random() % 20;
        reg2 = random() % 20;
        reg3 = random() % 20;

        pid++;

        fprintf(output, "enqueue %d %d %d %d %d %d\n", pid, psw, page, reg1, reg2, reg3);
      }
      number = 0;
      i = 0;
      break;
    case 3:

      printf("How many would you like to dequeue?");
      scanf("%d", &number);

      for(i = 0; i < number; i++){
        fprintf(output, "dequeue\n");
      }
      number = 0;
      break;
    case 4:

      printf("Please enter the process-number for the process to delete.");
      scanf("%d", &number);

      fprintf(output, "delete %d\n", number);

      number = 0;
      break;
    case 5:

      fprintf(output, "list\n");
      break;
    default:

      if(input == 6){
        printf("Goodbye.\n");
      }else{
        printf("Invalid option.\n");
      }
      break;
    }
  }


  fclose(output);

  return 0;

}
