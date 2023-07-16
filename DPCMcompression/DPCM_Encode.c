#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h> 
#include <windows.h>
#include<math.h>

int main(int argc, char* argv[]) {

    BITMAPFILEHEADER bmpFile;
    BITMAPINFOHEADER bmpInfo;
    FILE* inputFile = NULL, * inputFile2 = NULL;
    inputFile = fopen("AICenterY.bmp", "rb");
    inputFile2 = fopen("DPCM_encode.txt", "wb");


    fread(&bmpFile, sizeof(BITMAPFILEHEADER), 1, inputFile);
    fread(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, inputFile);

    int width = bmpInfo.biWidth;
    int height = bmpInfo.biHeight;
    int size = bmpInfo.biSizeImage;
    int bitCnt = bmpInfo.biBitCount;
    int stride = (((bitCnt / 8) * width) + 3) / 4 * 4;


    printf("W: %d(%d)\nH: %d\nS: %d\nD: %d\n\n", width, stride, height, size, bitCnt);

    unsigned char* inputImg = NULL, * Y1 = NULL,*outputImg=NULL,*Y3=NULL;
    char* Y2 = NULL;
    inputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
    outputImg= (unsigned char*)calloc(size, sizeof(unsigned char));
    Y1 = (unsigned char*)calloc(size, sizeof(unsigned char));
    Y2 = (char*)calloc(size, sizeof(char));
    Y3 = (unsigned char*)calloc(size, sizeof(unsigned char));

    fread(inputImg, sizeof(unsigned char), size, inputFile);

    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            Y1[j * width + i] = inputImg[j * stride + 3 * i];

        }
    }
    //--------------------------------------------------------------------
    int f = 0, fe = 0;

    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            if (i == 0) {
                f = Y1[j * width + i] - 128;
                f /= 5;
                Y2[j * width + i] = f;
               
                f *= 5;
                f += 128;
            }
            else {

                f = Y1[j * width + i] - fe;
                f /= 5;
                Y2[j * width + i] = f;
               
                f *= 5;
                f += fe;
            }
            fe = f;
            //printf("%d ", Y2[j*width+ i]);
        }
        printf("\n");
    }


    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            fprintf(inputFile2, "%d ", Y2[j * width + i]);
        }
    }
    int V = 0;
    f = 0, fe = 0;
        for (int j = 0; j < height; j++) {
            for (int i = 0; i < width; i++) {
    
                V = Y2[j * width + i];
                if (i == 0)
                {
                    f = V * 5;
                    f += 128;
                }
                else
                {
                    f = V * 5;
                    f += fe;
                }
                fe = f;
               // printf("%d ", f);
                Y3[j * width + i] = (unsigned char)(f > 255 ? 255 : (f < 0 ? 0 : f));
                Y3[j * width + i] = (unsigned char)(f > 255 ? 255 : (f < 0 ? 0 : f));
                Y3[j * width + i] = (unsigned char)(f > 255 ? 255 : (f < 0 ? 0 : f));
            }
            //printf("\n");
        }

        for (int j = 0; j < height; j++) {
            for (int i = 0; i < width; i++) {
                outputImg[j * stride + 3 * i + 0] = Y3[j * width + i];
                outputImg[j * stride + 3 * i + 1] = Y3[j * width + i];
                outputImg[j * stride + 3 * i + 2] = Y3[j * width + i];

            
                }
            }

    FILE* outputFile = fopen("AICenterY_DPCM_Encode.bmp", "wb");
    fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
    fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
    fwrite(outputImg, sizeof(unsigned char), size, outputFile);

    free(outputImg);
    free(inputImg);
    fclose(inputFile);
    fclose(inputFile2);
    fclose(outputFile);


    free(Y1);
    free(Y2);
    free(Y3);
    

    return 0;
}
