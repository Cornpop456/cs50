/**
 * fifteen.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE* file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = GetInt();
        
        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(5000);
        }

        // sleep thread for animation's sake
        usleep(5000);
    }
    
    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).  
 */
void init(void)
{
    // TODO
    int a = d;
    int counter = (a * a) - 1;
    for (int i = 0; i < d; i++)
    {
    
        for (int q = 0; q < d; q++)
        {
           if (counter == 0)
            {
                
                board[i][q] = 0;
                break;
            }
           
           
            if (a % 2 == 0)
            {
                if (counter == 2)
                {
                    board[i][q] = 1;
                    board[i][q + 1] = 2;
                    board[i][q + 2] = 0;
                    break;
                }
    
            }        

            board[i][q] = counter;
            
            counter -= 1;
            
        }
    }
    
    return;
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    // TODO

    for (int a = 0; a < d; a++)
    {
        
        for (int w = 0; w < d; w++)
        { 
           
            if (board[a][w] == 0)
            {   
                
                printf(" %c  ", '_');
                
            }
            
            else if (board[a][w] >= 10)
                printf(" %i ", board[a][w] );         
            
        
            else if (board[a][w] < 10 && board[a][w] != 0)
                printf(" %i  ", board[a][w] ); 
        
        }
        
        printf("\n\n");
        
    }
    
    return;            
        
    }
    


/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
    
    // TODO
    
    int temp;
    
    if (tile > (d * d) -1 || tile < 0)
        return false;
        
    for (int i = 0; i < d; i++)
    {
        for (int s = 0; s < d; s++)
        {
            if (board[i][s] == tile )
            {   
                
                if (board[i][s + 1] == 0 &&  s + 1 >= 0 && s + 1 <= d - 1 )
                {
                    temp = board[i][s + 1];
                    board[i][s + 1] = board[i][s];
                    board[i][s] = temp;
                    return true;
        
                }
                
                else if(board[i][s - 1] == 0 &&  s - 1 >= 0 && s - 1 <= d - 1 )
                {
                    temp = board[i][s - 1];
                    board[i][s - 1] = board[i][s];
                    board[i][s] = temp;
                    return true;
        
                }
               
                else if(board[i - 1][s] == 0  && i - 1 >= 0 && i - 1 <= d - 1  )
                {
                    temp = board[i - 1][s];
                    board[i - 1][s] = board[i][s];
                    board[i][s] = temp;
                    return true;
        
                }
               
                 else if(board[i + 1][s] == 0  && i + 1 >= 0 && i + 1 <= d - 1 )
                {
                    temp = board[i + 1][s];
                    board[i + 1][s] = board[i][s];
                    board[i][s] = temp;
                    return true;
        
                }
               
                
             }   
        }    
        
    }
    
    return false;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
    
    int counter = 1;
    for (int f = 0; f < d; f++)
    {
    
        for (int g = 0; g < d; g++)
        {
        
            if (board[f][g] == counter)
            {    
                counter += 1;
                
                if (counter > (d * d) - 1)
                    return true;
            }
        
            else if (board[f][g] != counter)
                return false;
        
        }
    
}
    
    return false;
}