#include <cs50.h>
#include <stdio.h>

//Prototype (there will exist a function called meow but not yet)
void meow(int n);


//implementation
int main(void)
{
     meow(3);
}












//define function
void meow(int n)
{
    for(int i = 0; i < n; i++)
  { 
      printf("meow\n");
  }
}