#include <iostream>
#include <stdlib.h>

int main(){
   srand( (unsigned int)time(NULL));
   std::cout << rand() % 2 << std::endl;

   return 0;
}
