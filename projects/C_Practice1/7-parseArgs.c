#include <stdio.h>
#include <stdlib.h> // 提供 exit()的原型
extern int mc;
void parseArgs(int argc, char **argv)
{

  if (argc!=4)
  {
    printf("Wrong input.\n");
    printf("The first parameter is the name of program.\n");
    printf("The second parameter is the input file name.\n");
    printf("The thied parameter is \"-mc\" or \"-e\".\"-mc\"means switching to Morse code.\"-e\"means converting to English.\n");
    printf("The fourth parameter is the output file name.\n");
    exit(EXIT_FAILURE);
  }
  else
  {

      if(argv[2]=="-mc")
      {
          mc=1;
      }
      else if(argv[2]=="-e")
      {
          mc=0;
      }
      
  }

}