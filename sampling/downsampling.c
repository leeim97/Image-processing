#include<stdio.h>
#include<stdlib.h>
#include<string.h> 
#pragma warning(disable:4996)
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include<math.h>

int main(int argc, char* argv[]) {


	BITMAPFILEHEADER bmpFile;
	BITMAPINFOHEADER bmpInfo;
	FILE* inputFile = NULL;
	inputFile = fopen("test2Y.bmp", "rb");
	fread(&bmpFile, sizeof(BITMAPFILEHEADER), 1, inputFile);
	fread(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, inputFile);

	int width = bmpInfo.biWidth;
	int height = bmpInfo.biHeight;
	int size = bmpInfo.biSizeImage;
	int bitCnt = bmpInfo.biBitCount;
	int stride = (((bitCnt / 8) * width) + 3) / 4 * 4;
	printf("W: %d(%d)\nH: %d\nS:%d\nD: %d\n\n", width, stride, height, size, bitCnt);

	unsigned char* inputImg = NULL, * outputImg = NULL, * Y1 = NULL, * Y2 = NULL;
	Y1 = (unsigned char*)calloc(size, sizeof(unsigned char));
	inputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
	fread(inputImg, sizeof(unsigned char), size, inputFile);

	int ratio = 2;
	int width2 = bmpInfo.biWidth >> ratio;
	int height2 = bmpInfo.biHeight >> ratio;
	int stride2 = (((bitCnt / 8) * width2) + 3) / 4 * 4;
	int size2 = stride2 * height2;


	//int bitCnt = bmpInfo.biBitCount;

	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++) {
			Y1[j * width + i] = inputImg[j * stride + 3 * i + 0];

		}
	}
	Y2 = (unsigned char*)calloc(width2 * height2, sizeof(unsigned char));
	outputImg = (unsigned char*)calloc(stride2 * height2, sizeof(unsigned char));
	for (int j = 0; j < height2; j++)
	{
		for (int i = 0; i < width2; i++) {
			Y2[j * width2 + i] = Y1[(j << ratio) * width + (i << ratio)];

		}
	}




	for (int j = 0; j < height2; j++)
	{
		for (int i = 0; i < width2; i++) {
			outputImg[j * stride2 + 3 * i + 0] = Y2[j * width2 + i];
			outputImg[j * stride2 + 3 * i + 1] = Y2[j * width2 + i];
			outputImg[j * stride2 + 3 * i + 2] = Y2[j * width2 + i];
		}
	}


	FILE* outputFile = fopen("test2Ydown.bmp", "wb");
	bmpInfo.biWidth = width2;
	bmpInfo.biHeight = height2;
	bmpInfo.biSizeImage = size2;
	bmpFile.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + size2;
	fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
	fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
	fwrite(outputImg, sizeof(unsigned char), size2, outputFile);

	free(outputImg);
	free(inputImg);
	fclose(inputFile);
	fclose(outputFile);
}
