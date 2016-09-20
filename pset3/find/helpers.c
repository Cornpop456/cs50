/**
 * helpers.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Helper functions for Problem Set 3.
 */
       
#include <cs50.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */


/*bool search(int value, int values[], int n)
{
    // TODO: implement a searching algorithm
    
    if (n < 0)
        
        return false;
    
    for (int i = 0; i < n; i++)
    {
        if (values[i] == value)
        
            return true;
        
    }
    
    return false;
}
*/

bool search(int value, int values[], int n)
{
    
    int start = 0;
    
    int end = n;
    
    int middle = (start + end) / 2;
    
    while (end != start)
    {
        
        if (values[middle] == value)
        {
            return true; 
    
        }
        
        else if (values[middle] > value)
        {
            
            end = middle;
            
        }
        
        else if (values[middle] < value)
        {
            
            start = middle;
            
        }
        
        
        middle = (start + end) / 2;
        
        
    }
        
        
        
    return false;    
        
    
    
}









/**
 * Sorts array of n values.
 */
void bubble_sort(int values[], int n)
{
    // TODO: implement an O(n^2) sorting algorithm
    
    for (int i = 0; i < n; i++)
    {
        
        bool sorted = false;
        
        
        for (int j = 0; j < n - 1; j++)
        {
            if (values[j] > values[j + 1])
            {
                int temp = values[j];
                
                values[j] = values[j + 1];
                
                values[j + 1] = temp;
                
                sorted = true;
                
            }

        }
        
        if (sorted == false)
        
            return;
        
    }
    
}


void selection_sort(int values[], int n)
{
    int temp;
    
    for (int i = 0; i < n; i++)
    {
        int smallest_val = values[i];
        
        int smallest_index = i;
        
        for (int q = i + 1; q < n; q++)
        {
            if (values[q] < smallest_val)
            {
                smallest_val = values[q];
                
                smallest_index = q;
                
            }
        }
        temp = values[i];
        
        values[i] = smallest_val;
        
        values[smallest_index] = temp;
        
    }
    
    return;
    
}