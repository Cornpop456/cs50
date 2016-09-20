#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int main(int argc, string argv[])
{   
    
    if (argc != 2)
    {
        printf("Please enter an encreption keyword!\n");
        
        return 1;
    }
    
    string kw = argv[1];
    
    for(int y = 0, u = strlen(kw); y < u; y++)
    
        {
            if (! isalpha(kw[y]))
            {
                printf("Keyword must consist from the alphabetical characters only!\n");
                
                return 1;
            }
            
        }
    
    int temp;
    
    int key_temp;
    
    int key;
    
    int kwcount = 0;
    
    string t = GetString();
    
    char vig[strlen(t)];
    
    for (int q = 0, w = strlen(t); q < w; q++  )
    {
        if (islower(t[q]) || isupper(t[q]))
        {
            vig[q] = kw[kwcount];
            
            kwcount += 1;
            
            if (kwcount > (strlen(kw) - 1))
                kwcount = 0;
            
        }
        
        else
            vig[q] = t[q];
        
    }
    
    for (int e = 0, r = strlen(t); e < r; e++ )
    {
        temp = (int) t[e];
        
        if (islower(t[e]) || isupper(t[e]))
        
        {
            temp = (int) t[e];
            
            key_temp = (int) vig[e];
            
            if (islower(vig[e]))
            {
                key = abs(97 - key_temp);
                
                if (islower(t[e]))
                    temp = 97 + (abs(97 - (temp + key)) % 26);
                
                else if (isupper(t[e]))
                    temp = 65 + (abs(65 - (temp + key)) % 26);    
               
            }
            
            else if (isupper(vig[e]))
            {
                key = abs(65 - key_temp);
                
                if (islower(t[e]))
                    temp = 97 + (abs(97 - (temp + key)) % 26);
                
                else if (isupper(t[e]))
                    temp = 65 + (abs(65 - (temp + key)) % 26); 
                
            }
            
        }
        
        printf("%c", (char) temp);
        
    }
    
    printf("\n");
    
    return 0;
}