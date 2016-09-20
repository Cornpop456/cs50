
//Recover programm that recovers jpg files from the *.raw source files if jpg files  were accidentelly deleted.


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t  BYTE;

FILE* temp;

FILE* file;

int main(int argc, char* argv[])
{
    //open source file
    file = fopen("card.raw", "r");
    
    int sum = 1;
    //template for outout files names
    char name[10];
    
    int ones = 0;

    int tens = 0;
    // buffer for reading data from saurce 
    BYTE buffer[512];
    // make the name of the first image
    sprintf(name, "0%i%i.jpg", tens, ones);
    // go to the cycle while there is data in the saurce file 
    while(fread(buffer, 512, 1, file))
    
    {
    //check for the signature bytes of jpg 
    if (buffer[0] == 0xff &&  buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3]== 0xe0 || buffer[3] == 0xe1))
    {
        
        printf("%i:JPG\n", sum);
        
        sum += 1;
    	
    	//open temp output file and write firste buffer of bytes
        
        temp = fopen(name, "w");

        fwrite(buffer, 512, 1, temp);
        
        //start reading bytes for the current image 
        
        while(fread(buffer, 512, 1, file))
        {
        
        //check for the next signature bytes which means the start of the next image	
        if (buffer[0] == 0xff &&  buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3]== 0xe0 || buffer[3] == 0xe1)) 
        

        {	
        	fclose(temp);
        	
        	printf("JPG found!\n");
        	
        	ones += 1;

        	if (ones > 9)
        	{
        		ones = 0;

        		tens += 1;
        	}
        	//make a new name for the next image file
     		sprintf(name, "0%i%i.jpg", tens, ones);
     		//make a return for one buffer so that first cycle could detect the next jpg file
        	fseek(file, (-1 * 512 ), SEEK_CUR);
        	break;
        	
        }
            // continue writing to the current jpg file
        else
        	 fwrite(buffer, 512, 1, temp);

        }
        
    }
   
    }    
    //close files if any are open 
    fclose(temp);
    fclose(file);
    
    printf("All images recovered\n");
    
    return 0;
    
 
}

/********************************************************************************************************************
1. #define BUF_SIZE 512 нуу или хотя бы sizeof(buffer) вместо указывания размера при каждом использовании. Да и в целом все константы-имена мог бы и задефайнить.
2. Где блять проверка ошибок вызываемых библиотечных функций?
3. Глобальные переменные в main запихнуть зачем ты вообще решил сделать их глобальными?
4. Не используй sprintf если можно использовать snprintf
5. 99% комментариев не нужны, код и так понятен.
6. (-1 * 512 ) - нахуя так делать?

******************************************************************************************************************/
 
