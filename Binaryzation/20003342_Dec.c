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
    inputFile = fopen("GateY.bmp", "rb");
    inputFile2 = fopen("bitstream.txt", "rb");

    fread(&bmpFile, sizeof(BITMAPFILEHEADER), 1, inputFile);
    fread(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, inputFile);

    int width = bmpInfo.biWidth;
    int height = bmpInfo.biHeight;
    int size = bmpInfo.biSizeImage;
    int bitCnt = bmpInfo.biBitCount;
    int stride = (((bitCnt / 8) * width) + 3) / 4 * 4;

    printf("W: %d(%d)\nH: %d\nS: %d\nD: %d\n\n", width, stride, height, size, bitCnt);

    unsigned char* outputImg = NULL, * Y2 = NULL;
    outputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
    Y2 = (unsigned char*)calloc(size, sizeof(unsigned char));

    //--------------------------------------------------
    char in[6] = { 0 };     //txt파일에서 3개씩 입력받는 문자를 저장하기위한 공간
    int V = 0, f = 0, fe = 0, cnt = 0;
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {

            fscanf(inputFile2, "%5s", &in); //문자 5개씩 입력 받아와서 저장

            if (strcmp(in, "00000") == 0) {     // 00000인경우 -15미만으로 -16대입에서 11111 14초과로 15대입까지의 경우
                V = -16;
            }
            else if (strcmp(in, "00001") == 0) {
                V = -15;
            }
            else if (strcmp(in, "00010") == 0) {
                V = -14;
            }
            else if (strcmp(in, "00011") == 0) {
                V = -13;
            }
            else if (strcmp(in, "00100") == 0) {
                V = -12;
            }
            else if (strcmp(in, "00101") == 0) {
                V = -11;
            }
            else if (strcmp(in, "00110") == 0) {
                V = -10;
            }
            else if (strcmp(in, "00111") == 0) {
                V = -9;
            }
            else if (strcmp(in, "01000") == 0) {
                V = -8;
            }
            else if (strcmp(in, "01001") == 0) {
                V = -7;
            }
            else if (strcmp(in, "01010") == 0) {
                V = -6;
            }
            else if (strcmp(in, "01011") == 0) {
                V = -5;
            }
            else if (strcmp(in, "01100") == 0) {
                V = -4;
            }
            else if (strcmp(in, "01101") == 0) {
                V = -3;
            }
            else if (strcmp(in, "01110") == 0) {
                V = -2;
            }
            else if (strcmp(in, "01111") == 0) {
                V = -1;
            }
            else if (strcmp(in, "10000") == 0) {
                V = 0;
            }
            else if (strcmp(in, "10001") == 0) {
                V = 1;
            }
            else if (strcmp(in, "10010") == 0) {
                V = 2;
            }
            else if (strcmp(in, "10011") == 0) {
                V = 3;
            }
            else if (strcmp(in, "10100") == 0) {
                V = 4;
            }
            else if (strcmp(in, "10101") == 0) {
                V = 5;
            }
            else if (strcmp(in, "10110") == 0) {
                V = 6;
            }
            else if (strcmp(in, "10111") == 0) {
                V = 7;
            }
            else if (strcmp(in, "11000") == 0) {
                V = 8;
            }
            else if (strcmp(in, "11001") == 0) {
                V = 9;
            }
            else if (strcmp(in, "11010") == 0) {
                V = 10;
            }
            else if (strcmp(in, "11011") == 0) {
                V = 11;
            }
            else if (strcmp(in, "11100") == 0) {
                V = 12;
            }
            else if (strcmp(in, "11101") == 0) {
                V = 13;
            }
            else if (strcmp(in, "11110") == 0) {
                V = 14;
            }
            else if (strcmp(in, "11111") == 0) {
                V = 15;
            }

            //reconstruction 과정
            if (i == 0)
            {
                f = V * 10;
                f += 128;
            }
            else
            {
                f = V * 10;
                f += fe;
            }
            fe = f;

            Y2[j * width + i] = (unsigned char)(f > 255 ? 255 : (f < 0 ? 0 : f));
        }
    }
    //사진 출력
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            outputImg[j * stride + 3 * i + 0] = Y2[j * width + i];
            outputImg[j * stride + 3 * i + 1] = Y2[j * width + i];
            outputImg[j * stride + 3 * i + 2] = Y2[j * width + i];

        }
    }

    FILE* outputFile = fopen("reconDecY.bmp", "wb");
    fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
    fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
    fwrite(outputImg, sizeof(unsigned char), size, outputFile);


    free(outputImg);
    fclose(inputFile);
    fclose(outputFile);
    free(Y2);

    return 0;
}
