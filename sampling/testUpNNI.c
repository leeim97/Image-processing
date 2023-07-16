#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include<stdlib.h>
#include<math.h>
#include <windows.h>


int main(int argc, char* argv[])
{
    BITMAPFILEHEADER bmpFile;
    BITMAPINFOHEADER bmpInfo;

    FILE* inputFile1 = NULL;

    inputFile1 = fopen("test2Ydown.bmp", "rb");

    fread(&bmpFile, sizeof(BITMAPFILEHEADER), 1, inputFile1);
    fread(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, inputFile1);

    int width = bmpInfo.biWidth;
    int height = bmpInfo.biHeight;
    int size = bmpInfo.biSizeImage;
    int bitCnt = bmpInfo.biBitCount;
    int stride = (((bitCnt / 8) * width) + 3) / 4 * 4;

    int ratio = 2;
    int width2 = bmpInfo.biWidth << ratio;
    int height2 = bmpInfo.biHeight << ratio;
    int stride2 = (((bitCnt / 8) * width2) + 3) / 4 * 4;
    int size2 = stride2 * height2;

    unsigned char* inputImg1 = NULL, * outputImg1 = NULL;
    unsigned char* Y1 = NULL, * Y2 = NULL;
    inputImg1 = (unsigned char*)calloc(size, sizeof(unsigned char));
    outputImg1 = (unsigned char*)calloc(size2, sizeof(unsigned char));
    Y1 = (unsigned char*)calloc(size, sizeof(unsigned char));
    Y2 = (unsigned char*)calloc(size2, sizeof(unsigned char));
    fread(inputImg1, sizeof(unsigned char), size, inputFile1);

    for (int j = 0; j < height; j++)
        for (int i = 0; i < width; i++)
            Y1[j * width + i] = inputImg1[j * stride + 3 * i];
    //-----------------------------------------------------------------------------

    for (int j = 0; j < height2; j++){
        for (int i = 0; i < width2; i++) {
            Y2[j * width2 + i] = Y1[((j >> ratio) * width + (i >> ratio))];
           
        }
    }

    //------------------------------------------------------------------------------
    for (int j = 0; j < height2; j++)
    {
        for (int i = 0; i < width2; i++)
        {
            outputImg1[j * stride2 + 3 * i + 2] = Y2[j * width2 + i];
            outputImg1[j * stride2 + 3 * i + 1] = Y2[j * width2 + i];
            outputImg1[j * stride2 + 3 * i + 0] = Y2[j * width2 + i];
        }
    }

    FILE* outputFile1 = fopen("test2YupNNI.bmp", "wb");
    bmpInfo.biWidth = width2;
    bmpInfo.biHeight = height2;
    bmpInfo.biSizeImage = size2;
    bmpFile.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + size2;
    fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile1);
    fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile1);
    fwrite(outputImg1, sizeof(unsigned char), size2, outputFile1);

    free(outputImg1);
    free(inputImg1);
    free(Y1);
    free(Y2);

    fclose(inputFile1);
    fclose(outputFile1);

}
