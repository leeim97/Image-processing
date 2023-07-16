#include<stdio.h>
#include<stdlib.h>
#include<string.h> 
#pragma warning(disable:4996)
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include<math.h>

int main(int argc, char* argv[]) {

	double Y, Cb, I, Cr, R, G, B, Diff_Y;
	BITMAPFILEHEADER bmpFile;
	BITMAPINFOHEADER bmpInfo;
	FILE* inputFile = NULL;
	inputFile = fopen("AICenterY.bmp", "rb");
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
	Y2 = (unsigned char*)calloc(size, sizeof(unsigned char));
	inputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
	outputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
	fread(inputImg, sizeof(unsigned char), size, inputFile);

	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++) {
			Y1[j * width + i] = inputImg[j * stride + 3 * i + 0];

		}
	}
	double gamma = 2.5;

	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++) {
			Y2[j * width + i] = (unsigned char)(255 * pow(Y1[j * width + i] / 255.0, 1 / gamma));
																					//감마한 파일을 다시 불러서 감마 분의 일의 역수 즉 감마를 넣어주면 원본나옴
																					//지수때문에 math.h 필수
		}
	}




	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++) {
			outputImg[j * stride + 3 * i + 0] = Y2[j * width + i];
			outputImg[j * stride + 3 * i + 1] = Y2[j * width + i];
			outputImg[j * stride + 3 * i + 2] = Y2[j * width + i];
		}
	}
	/*for (int i = 0; i < 256; i++) {
		printf("%d %d\n",i, Hist[i]);
	}*/


	FILE* outputFile = fopen("AICenterYGamma.bmp", "wb");
	fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
	fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
	fwrite(outputImg, sizeof(unsigned char), size, outputFile);

	free(outputImg);
	free(inputImg);
	fclose(inputFile);
	fclose(outputFile);
}
