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
    inputFile2 = fopen("DPCM_encode.txt", "rb");

    fread(&bmpFile, sizeof(BITMAPFILEHEADER), 1, inputFile);
    fread(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, inputFile);

    int width = bmpInfo.biWidth;
    int height = bmpInfo.biHeight;
    int size = bmpInfo.biSizeImage;
    int bitCnt = bmpInfo.biBitCount;
    int stride = (((bitCnt / 8) * width) + 3) / 4 * 4;

    printf("W: %d(%d)\nH: %d\nS: %d\nD: %d\n\n", width, stride, height, size, bitCnt);

    unsigned char* outputImg = NULL;
    outputImg = (unsigned char*)calloc(size, sizeof(unsigned char));


//--------------------------------------------------
    int V = 0, f = 0, fe = 0;
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {

            fscanf(inputFile2, "%d", &V);
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
            outputImg[j * stride + 3 * i + 0] = (unsigned char)(f > 255 ? 255 : (f < 0 ? 0 : f));
            outputImg[j * stride + 3 * i + 1] = (unsigned char)(f > 255 ? 255 : (f < 0 ? 0 : f));
            outputImg[j * stride + 3 * i + 2] = (unsigned char)(f > 255 ? 255 : (f < 0 ? 0 : f));
        }
    }


    FILE* outputFile = fopen("AICenterY_DPCM_Decode.bmp", "wb");
    fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
    fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
    fwrite(outputImg, sizeof(unsigned char), size, outputFile);


    free(outputImg);
    fclose(inputFile);
    fclose(outputFile);

    return 0;
}
