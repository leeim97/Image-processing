#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h> 
#pragma warning(disable:4996)
#include <windows.h>
#include<math.h>

int main(int argc, char* argv[]) {

	//double Y, Cb, I, Cr, R, G, B, Diff_Y;
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

	unsigned char* inputImg = NULL, * outputImg = NULL, * Y1 = NULL, * Y2 = NULL, Y3 = NULL;
	Y1 = (unsigned char*)calloc(size, sizeof(unsigned char));
	Y2 = (unsigned char*)calloc(size, sizeof(unsigned char));
	Y3 = (unsigned char*)calloc(size, sizeof(unsigned char));
	inputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
	outputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
	fread(inputImg, sizeof(unsigned char), size, inputFile);

	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++) {
			Y1[j * width + i] = inputImg[j * stride + 3 * i + 0];

		}
	}

	//------------------------------------------------------------------
	//Robert 필터
	double x1[2][2] = { {-1,0} ,{0,1} };
	double y1[2][2] = { {0,-1},{1,0} };
	//Sobel 필터
	double x2[3][3] = { {-1,0,1} ,{-2,0,2},{-1,0,1} };
	double y2[3][3] = { {-1,-2,-1},{0,0,0},{1,2,1} };

	double Gx = 0, Gy = 0, G;
	double Rx = 0, Ry = 0, R;
	double GR;

	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++) {

			if (i == 0 || j == 0 || i == width - 1 || j == height - 1) {
				Y2[j * width + i] = 0;

			}
			else {
				Gx = 0; Gy = 0;
				Rx = 0; Ry = 0;
				int wi = i - 1;		//9칸중에 1행1열 즉, 맨 처음 칸
				int he = j - 1;


				for (int k = 0; k < 2; he++, k++) { //k는 행

					for (int l = 0, wi = i - 1; l < 2; wi++, l++) { //i는 열

						Rx += x1[k][l] * Y1[he * width + wi];
						Ry += y1[k][l] * Y1[he * width + wi];

					}
				}



				for (int k = 0; k < 3; he++, k++) { //k는 행

					for (int l = 0, wi = i - 1; l < 3; wi++, l++) { //i는 열

						Gx += x2[k][l] * Y1[he * width + wi];
						Gy += y2[k][l] * Y1[he * width + wi];

					}
				}
				G = sqrt(Gx * Gx + Gy * Gy);
				G = (G > 200 ? 255 : 0);
				R = sqrt(Rx * Rx + Ry * Ry);
				R = (R > 40 ? 255 : 0);

				if (G == 255 || R == 255) //합집합일때
					GR = 256;
				else
					GR = 0;

				Y2[j * width + i] = (unsigned char)(GR > 255 ? 255 : 0);


			}

			//G = sqrt(Gx * Gx + Gy * Gy);
			//Y2[j * width + i] = (unsigned char)(G > 255 ? 255 : (G < 0 ? 0 : G));

		}
	}



	//-------------------------------------------------------------------


	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++) {
			outputImg[j * stride + 3 * i + 0] = Y2[j * width + i];
			outputImg[j * stride + 3 * i + 1] = Y2[j * width + i];
			outputImg[j * stride + 3 * i + 2] = Y2[j * width + i];
		}
	}


	FILE* outputFile = fopen("AICenterY_SetUnion.bmp", "wb");
	fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
	fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
	fwrite(outputImg, sizeof(unsigned char), size, outputFile);

	free(outputImg);
	free(inputImg);
	fclose(inputFile);
	fclose(outputFile);
}