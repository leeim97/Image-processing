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
    inputFile2 = fopen("bitstream.txt", "wb");


    fread(&bmpFile, sizeof(BITMAPFILEHEADER), 1, inputFile);
    fread(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, inputFile);

    int width = bmpInfo.biWidth;
    int height = bmpInfo.biHeight;
    int size = bmpInfo.biSizeImage;
    int bitCnt = bmpInfo.biBitCount;
    int stride = (((bitCnt / 8) * width) + 3) / 4 * 4;


    printf("W: %d(%d)\nH: %d\nS: %d\nD: %d\n\n", width, stride, height, size, bitCnt);

    unsigned char* inputImg = NULL, * Y1 = NULL, * outputImg = NULL, * Y3 = NULL;
    char* Y2 = NULL;
    inputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
    outputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
    Y1 = (unsigned char*)calloc(size, sizeof(unsigned char));
    Y2 = (char*)calloc(size, sizeof(char));
    Y3 = (unsigned char*)calloc(size, sizeof(unsigned char));

    fread(inputImg, sizeof(unsigned char), size, inputFile);

    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            Y1[j * width + i] = inputImg[j * stride + 3 * i]; //픽셀값을 입력받음

        }
    }
    //--------------------------------------------------------------------
    int f = 0, fe = 0;

    for (int j = 0; j < height; j++) {      //txt파일로 보내기 위해 Quantization해주기
        for (int i = 0; i < width; i++) {
            fe = f;
            if (i == 0) {       //첫 열인 경우
                f = Y1[j * width + i] - 128;  //받아온 픽셀에서 임의로 정한 128을 빼준다
                f /= 10;                 //Quantization
                Y2[j * width + i] = f; //Quantization한 값을 받는다

                f *= 10;                //다음 픽셀에 사용되므로 f를  *10와 128을 더한다
                f += 128;
            }
            else {              //나머지 열의 픽셀값 구하기

                f = Y1[j * width + i] - fe; //현재 픽셀에서 이전에 reconstruction된 값 빼주기
                f /= 10;         //Quantization
                Y2[j * width + i] = f;  //Quantization한 값을 받는다

                f *= 10;          //다음 픽셀에 사용되므로 f를  *10와 128을 더한다
                f += fe;
            }

        }

    }
    int k = 0;
    for (int j = 0; j < height; j++) {      //Fixe-Length Code
        for (int i = 0; i < width; i++) {       //bitstream.txt에 써주는 과정
            k = Y2[j * width + i];      //양자화 된 값을 입력받음
            if (k < -15) {                      //-15미만 00000부터 14초과 11111까지 txt파일에 작성
                fprintf(inputFile2, "00000");
            }
            else if (k == -15) {
                fprintf(inputFile2, "00001");
            }
            else if (k == -14) {
                fprintf(inputFile2, "00010");
            }
            else if (k == -13) {
                fprintf(inputFile2, "00011");
            }
            else if (k == -12) {
                fprintf(inputFile2, "00100");
            }
            else if (k == -11) {
                fprintf(inputFile2, "00101");
            }
            else if (k == -10) {
                fprintf(inputFile2, "00110");
            }
            else if (k == -9) {
                fprintf(inputFile2, "00111");
            }
            else if (k == -8) {
                fprintf(inputFile2, "01000");
            }
            else if (k == -7) {
                fprintf(inputFile2, "01001");
            }
            else if (k == -6) {
                fprintf(inputFile2, "01010");
            }
            else if (k == -5) {
                fprintf(inputFile2, "01011");
            }
            else if (k == -4) {
                fprintf(inputFile2, "01100");
            }
            else if (k == -3) {
                fprintf(inputFile2, "01101");
            }
            else if (k == -2) {
                fprintf(inputFile2, "01110");
            }
            else if (k == -1) {
                fprintf(inputFile2, "01111");
            }
            else if (k == 0) {
                fprintf(inputFile2, "10000");
            }
            else if (k == 1) {
                fprintf(inputFile2, "10001");
            }
            else if (k == 2) {
                fprintf(inputFile2, "10010");
            }
            else if (k == 3) {
                fprintf(inputFile2, "10011");
            }
            else if (k == 4) {
                fprintf(inputFile2, "10100");
            }
            else if (k == 5) {
                fprintf(inputFile2, "10101");
            }
            else if (k == 6) {
                fprintf(inputFile2, "10110");
            }
            else if (k == 7) {
                fprintf(inputFile2, "10111");
            }
            else if (k == 8) {
                fprintf(inputFile2, "11000");
            }
            else if (k == 9) {
                fprintf(inputFile2, "11001");
            }
            else if (k == 10) {
                fprintf(inputFile2, "11010");
            }
            else if (k == 11) {
                fprintf(inputFile2, "11011");
            }
            else if (k == 12) {
                fprintf(inputFile2, "11100");
            }
            else if (k == 13) {
                fprintf(inputFile2, "11101");
            }
            else if (k == 14) {
                fprintf(inputFile2, "11110");
            }
            else if (k > 14) {
                fprintf(inputFile2, "11111");
            }





            fprintf(inputFile2, " "); //가독성을 위해서 한칸씩 띄어줌
        }
    }



    int V = 0, G = 0;
    f = 0, fe = 128;              //위의 값을 reconstruction해주어 사진 만들기
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {

            V = Y2[j * width + i];      //아까 양자화 된 값을 입력 받아옴

            if (i == 0)
                //Quntization이 되어있고 F-L code에서와 같이 -15미만,14초과 부분을 각각 -4,3으로 변경
                //첫열의 reconstrucntion

            {
                if (V < -15) {
                    V = -16;
                }
                else if (V > 14) {
                    V = 15;
                }
                else
                    V = V;

                f = V * 10;
                f += 128;

            }
            else
            {                //첫열 이외의 경우  
                             //Quntization이 되어있고 F-L code에서와 같이 -15미만,14초과 부분을 각각 -4,3으로 변경


                if (V < -15) {
                    V = -16;
                }
                else if (V > 14) {
                    V = 15;
                }
                else
                    V = V;
                f = V * 10;
                f += fe;

            }
            fe = f;


            Y3[j * width + i] = (unsigned char)(f > 255 ? 255 : (f < 0 ? 0 : f));//복원한 값을 입력받음

        }

    }

    //복원된 값 사진으로 출력
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            outputImg[j * stride + 3 * i + 0] = Y3[j * width + i];
            outputImg[j * stride + 3 * i + 1] = Y3[j * width + i];
            outputImg[j * stride + 3 * i + 2] = Y3[j * width + i];


        }
    }

    FILE* outputFile = fopen("reconEncY.bmp", "wb");
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



}
