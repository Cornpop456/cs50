#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int main(int argc, string argv[])
{   
    
    if (argc != 2)
    {
        printf("Please enter an encreption key!");
        
        return 1;
    }
    
    int temp;
    
    int k = abs(atoi(argv[1]));
    
    string t = GetString();
    
    
   for (int i = 0, n = strlen(t); i < n; i++)
{   
    
    temp = (int) t[i];
    
    if (islower(t[i]))
    {   
        temp = 97 + (abs(97 - (temp + k)) % 26);
       
    }
    
     else if (isupper(t[i]))
    {
        temp = 65 + (abs(65 - (temp + k)) % 26);
        
    }
    
    printf("%c", (char) temp);
    
}
    printf("\n");
    
    return 0;
    
}