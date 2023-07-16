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
	inputFile = fopen("AICenter.bmp", "rb");
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
	/*for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++) {
			Y2[j * width + i] = 256; //Y2 전체를 검은색으로

		}
	}*/
	//-----------------------------------------
	double pi = 3.1415926535;
	double seta = pi / 4.0;
	int x2, y2;
	int centerx = width / 2, centery = height / 2;//centerx,centery 각각 중점 256으로
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++) {
			x2 = (cos(seta) * (i - centerx) - sin(seta) * (j - centery) + centerx);
			y2 = (sin(seta) * (i - centerx) + cos(seta) * (j - centery) + centery);
			//printf("%d %d\n", x2, y2);
			if (x2 < 0 || y2 < 0)
				continue;
			if (x2 > 512 || y2 > 512)
				continue;
			Y2[j * width + i] = Y1[y2 * width + x2];

		}
	}

	//------------------------------------------


	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++) {
			outputImg[j * stride + 3 * i + 0] = Y2[j * width + i];
			outputImg[j * stride + 3 * i + 1] = Y2[j * width + i];
			outputImg[j * stride + 3 * i + 2] = Y2[j * width + i];
		}
	}


	FILE* outputFile = fopen("AICenter_Rotation.bmp", "wb");
	fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
	fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
	fwrite(outputImg, sizeof(unsigned char), size, outputFile);

	free(outputImg);
	free(inputImg);
	fclose(inputFile);
	fclose(outputFile);
}
