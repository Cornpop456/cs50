#include <cs50.h>
#include <math.h>
#include <stdio.h>

int main(void){
    
    
    
int quaters, dimes, nickels, pennies, coins, cents;

quaters = 25;

dimes = 10;

nickels = 5;

pennies = 1;
 
coins = 0;

float amount = 0.0;

while (amount <= 0)
{
    printf( "O hai! How much change is owed?\n");
    amount = GetFloat();
}   

    amount = round(amount * 100);
    
    cents = (int) amount;
    
    if (cents >= 25)
    {
    coins += cents / quaters;
    cents = cents % quaters;
    }
     
    if (cents >= 10)
    {
      
    coins += cents / dimes;
    cents = cents % dimes;
        
    }
    
    
    if (cents >= 5)
    
    
    {
    
    coins += cents / nickels;
    cents = cents % nickels;
        
    }

    if (cents >= 1)
    
    
    {
    
    coins += cents / pennies;
    cents = cents % pennies;
        
    }
    
    printf("%i\n", coins);
    
    return 0;
}