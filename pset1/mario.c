#include <stdio.h>
#include <cs50.h>

int main(void)
{
int count , spaces;    
string hash = "#";
string space_string = " ";

int hashtag = 2;

while (true)
  {
    printf("height: ");
    count = GetInt();
    if (count >= 0 && count < 24)
     {  spaces = count - 1; 
        break;
     }
  }
  
//space = count - 1;
for (int i = 0; i < count; i++)

{
    for (int f = 0; f < spaces; f++){
        printf("%s", space_string);
        }
        
    for (int s = 0; s < hashtag; s++){
        printf("%s", hash);
        }    
        
   printf("\n");
   spaces -= 1;
   hashtag +=1;
    
    } 

   return 0; 
}   

