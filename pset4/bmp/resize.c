
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char* argv[])
{  
    // ensure proper usage
    if (argc != 4)
    {
        printf("Usage: resize n infile outfile\n");
        return 1;
    }

    // remember filenames
    char* infile = argv[2];
    char* outfile = argv[3];
    
    int resize = atoi(argv[1]);

    if (resize < 1 || resize > 100)
    {
        printf("Factor must be in range 1..100!\n");
        return 1;
    }


    // open input file 
    FILE* inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 2;
    }



    // open output file
    FILE* outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf , bf2;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);
    
    bf2 = bf;
    
    //bf.bfSize = bf.bfSize * resize;


    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi, bi2;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);
    
    bi2 = bi;
    

    

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
    
    bi2.biHeight = bi.biHeight  * resize;
    
    bi2.biWidth = bi.biWidth * resize;
    
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) %4) % 4;
    
    int new_padding = (4 - (bi2.biWidth  * sizeof(RGBTRIPLE)) %4) % 4;
    
    bi2.biSizeImage = (bi.biWidth * sizeof(RGBTRIPLE) + new_padding) * abs(bi.biHeight);
    
    bf2.bfSize = bf.bfSize - bi.biSizeImage + bi2.biSizeImage;
    
    
    // write outfile's BITMAPFILEHEADER
    fwrite(&bf2, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi2, sizeof(BITMAPINFOHEADER), 1, outptr);

  
    for (int i = 0, biHeight = abs(bi.biHeight)  ; i < biHeight; i++)
    {
            for (int j = 0; j < resize; j++)
            {
                for (int d = 0; d < (bi.biWidth)  ; d++)
                {
                 // temporary storage
                RGBTRIPLE triple;
        
                // read RGB triple from infile
                
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
        
                for (int f = 0; f < resize; f++)
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                
                }
        
            for (int k = 0; k < padding; k++)
            {
                fputc(0x00, outptr);
            }
            
            if (j < resize - 1)
                fseek(inptr, -bi.biWidth * sizeof(RGBTRIPLE), SEEK_CUR);
        }
            
            fseek(inptr, padding, SEEK_CUR);
        
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // that's all folks
    return 0;
}
