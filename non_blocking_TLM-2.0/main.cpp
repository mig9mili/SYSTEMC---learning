
#define DEBUG

#include "Top.h"

int sc_main(int argc, char* argv[])
{
  Top Top("top");
  sc_start(20,SC_SEC);
  
  cout << "\n***** Messages have been written to file output.txt                    *****\n";
  cout << "***** Select 'Download files after run' to read file in EDA Playground *****\n\n";

  return 0;
}