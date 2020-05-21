#include<iostream>
#include<Windows.h>
#include<malloc.h>
#include<cstdlib>
#include<cstdio>
#include<cstring>
#include<conio.h>
#include<math.h>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2\imgproc\types_c.h>
#include <opencv2/opencv.hpp>

#define OFFSETX 80
#define OFFSETY 80
#define LENA "../../Lena.bmp"
#define PI 3.1415926
using namespace std;
using namespace cv;

void defultDisplay();
void rotateDisplay();
void switchQuarter();
void convert2Gray();
void sobelDisplay();
void gaussianDisplay();
void binaryzationDisplay();
void histogramDisplay_Gray();
void histogramDisplay_RGB();
void histogramDisplay_HOG();

int main() {
	while (1) {
		system("cls");
		cout << "准备好见见Lena了吗？" << endl << endl;
		cout << "选择进行的功能" << endl << "1.输出图像" << endl<< "2.旋转图像" << endl 
			<< "3.交换二三象限" << endl << "4.灰度图像" << endl<< "5.Sobel边缘图像" << endl 
			<< "6.高斯平滑" << endl << "7.二值化" << endl<< "8.灰度直方图" << endl 
			<< "9.RGB直方图" << endl << "0.方块梯度直方图" << endl<<"其余任意键退出"<<endl;
		switch (_getch()) {
		case '1':defultDisplay(); break;
		case '2':rotateDisplay(); break;
		case '3':switchQuarter(); break;
		case '4':convert2Gray(); break;
		case '5':sobelDisplay(); break;
		case '6':gaussianDisplay(); break;
		case '7':binaryzationDisplay(); break;
		case '8':histogramDisplay_Gray(); break;
		case '9':histogramDisplay_RGB(); break;
		case '0':histogramDisplay_HOG(); break;
		default:return 0;
		}
	}
	return 0;
}
void defultDisplay() {
	system("cls");
	cout << "///原图输出///" << endl;
	int r, g, b;
	char* buf; //定义文件读取缓冲区
	FILE* fp;                            //定义文件指针//定义保存文件指针
	DWORD picWidth, picHeight;           //读取图像的长,宽
	BITMAPFILEHEADER bf;                      //图像文件头
	BITMAPINFOHEADER bi;                      //图像文件头信息
	HDC dc = GetDC(GetForegroundWindow());//获取窗口句柄获取绘图设备
	if ((fp = fopen(LENA, "rb")) == nullptr) {
		cout << "未找到" <<LENA<< endl << "按任意键返回主菜单"<<endl; _getch();
		return ;
	}
	else cout << ">>成功打开" << LENA << endl;
	fread(&bf, sizeof(BITMAPFILEHEADER), 1, fp);//读取BMP文件头
	fread(&bi, sizeof(BITMAPINFOHEADER), 1, fp);//读取BMP文件信息头
	picWidth = bi.biWidth;                          //获取图像的宽
	picHeight = bi.biHeight;                        //获取图像的高
	buf = new char[picWidth * picHeight * 3];                //分配缓冲区大小
	fseek(fp, long(sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)), 0);//定位到像素起始位置
	if (buf)
		fread(buf, 1, picWidth * picHeight * 3, fp);//开始读取数据
	fclose(fp);
	for (int j = 0; j < picHeight; j++) {
		for (int i = 0; i < picWidth; i++) {
			b = *buf++; g = *buf++; r = *buf++;
			SetPixel(dc, OFFSETX + i, OFFSETY + picHeight - j, RGB(r, g, b));
		}
	}
	cout << "按任意键返回主菜单" << endl;
	_getch();
	system("cls");
}
void rotateDisplay() {
	system("cls");
	cout << "///顺时针旋转90度输出///" << endl;
	char* buf;                             
	FILE* fp, * fpw;                            
	DWORD picWidth, picHeight;           
	BITMAPFILEHEADER bf;                      
	BITMAPINFOHEADER bi;                      
	if ((fp = fopen(LENA, "rb")) == nullptr) {
		cout << "未找到" <<LENA<< endl<<"按任意键返回主菜单"<<endl; _getch();
		return ;
	}
	else cout << ">>成功打开" << LENA << endl;
	fread(&bf, sizeof(BITMAPFILEHEADER), 1, fp);
	fread(&bi, sizeof(BITMAPINFOHEADER), 1, fp);
	picWidth = bi.biWidth;                          
	picHeight = bi.biHeight;                       
	buf = new char[picWidth * picHeight * 3];                
	fseek(fp, long(sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)), 0);
	if (buf)
		fread(buf, 1, picWidth * picHeight * 3, fp);
	fclose(fp);
	int r, g, b;
	char* output = new char[picWidth * picHeight * 3];
	HDC dc = GetDC(GetForegroundWindow());
	if ((fpw = fopen("../../Lena_Rotated.bmp", "wb")) == nullptr) {
		cout << ">>文件创建失败" << endl;
		return;
	}
	fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, fpw);
	fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, fpw);
	for (int j = 0; j < picHeight; j++) {
		for (int i = 0; i < picWidth; i++) {
			b = *buf++; g = *buf++; r = *buf++;
			SetPixel(dc, OFFSETY + j, OFFSETX + i, RGB(r, g, b));
		}
	}
	buf -= picWidth * picHeight * 3;
	for (int i = picWidth - 1; i >= 0; i--) {
		for (int j = 0; j < picHeight; j++) {
			*output++ = buf[(j * picWidth + i) * 3]; 
			*output++ = buf[(j * picWidth + i) * 3+1];
			*output++ = buf[(j * picWidth + i) * 3+2];
		}
	}
	output -= picWidth * picHeight * 3;
	fwrite(output, 1, picWidth * picHeight * 3, fpw);
	fclose(fpw);
	cout << ">>图像保存至../../Lena_Rotated.bmp" << endl;
	delete[] buf; delete[] output;
	cout << "按任意键返回主菜单" << endl;
	_getch();
	system("cls");
	
}
void switchQuarter() {
	system("cls");
	cout << "///交换二三象限///" << endl;
	int r, g, b;
	char* buf;                             
	FILE* fp, *fpw;                           
	DWORD picWidth, picHeight;           
	BITMAPFILEHEADER bf;                      
	BITMAPINFOHEADER bi;                      
	HDC dc = GetDC(GetForegroundWindow());
	if ((fp = fopen(LENA, "rb")) == nullptr) {
		cout << "未找到"<< LENA << endl << "按任意键返回主菜单" <<endl; _getch();
		return ;
	}
	else cout << ">>成功打开" << LENA << endl;
	fread(&bf, sizeof(BITMAPFILEHEADER), 1, fp);
	fread(&bi, sizeof(BITMAPINFOHEADER), 1, fp);
	picWidth = bi.biWidth;                          
	picHeight = bi.biHeight;                       
	buf = new char[picWidth * picHeight * 3];
	fseek(fp, long(sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)), 0);
	if (buf)
		fread(buf, 1, picWidth * picHeight * 3, fp);
	fclose(fp);
	char* buf1 = new char[picWidth * picHeight * 3 / 4];
	char* buf2 = new char[picWidth * picHeight * 3 / 4];
	char* buf3 = new char[picWidth * picHeight * 3 / 4];
	char* buf4 = new char[picWidth * picHeight * 3 / 4];
	if ((fpw = fopen("../../Lena_Quarter_Switched.bmp", "wb")) == nullptr) {
		cout << ">>文件创建失败" << endl;
		return ;
	}
	fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, fpw);
	fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, fpw);
	for (int j = 0; j < picHeight / 2; j++) {
		for (int i = 0; i < picWidth/2; i++) {//新第二象限
			*buf2++ = b = *buf++;
			*buf2++ = g = *buf++;
			*buf2++ = r = *buf++;
			SetPixel(dc, OFFSETX + i, OFFSETY + picHeight/2 - j, RGB(r, g, b));
		}
		for (int i = picHeight / 2 ; i < picWidth; i++) {//新第四象限
			*buf4++ = b = *buf++;
			*buf4++ = g = *buf++;
			*buf4++ = r = *buf++;
			SetPixel(dc, OFFSETX + i, OFFSETY + picHeight - j, RGB(r, g, b));
		}
	}
	for (int j = picHeight / 2; j < picHeight; j++) {
		for (int i = 0; i < picWidth / 2; i++) {//新第三象限
			*buf3++ = b = *buf++;
			*buf3++ = g = *buf++;
			*buf3++ = r = *buf++;
			SetPixel(dc, OFFSETX + i, OFFSETY + 3*picHeight / 2 - j, RGB(r, g, b));
		}
		for (int i = picHeight / 2; i < picWidth; i++) {//新第一象限
			*buf1++ = b = *buf++;
			*buf1++ = g = *buf++;
			*buf1++ = r = *buf++;
			SetPixel(dc, OFFSETX + i, OFFSETY + picHeight - j, RGB(r, g, b));
		}
	}
	buf -= picWidth * picHeight * 3;
	buf1 -= picWidth * picHeight * 3 / 4;
	buf2 -= picWidth * picHeight * 3 / 4;
	buf3 -= picWidth * picHeight * 3 / 4;
	buf4 -= picWidth * picHeight * 3 / 4;
	fseek(fpw, long(sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)), 0);
	for (int i = 0; i < picHeight / 2; i++) {
		fwrite(buf3 + i* picWidth * 3 / 2, 1, picWidth * 3 / 2, fpw);
		fwrite(buf4 + i * picWidth * 3 / 2, 1, picWidth * 3 / 2, fpw);
	}
	for (int i = 0; i < picHeight / 2; i++) {
		fwrite(buf2 + i * picWidth * 3 / 2, 1, picWidth * 3 / 2, fpw);
		fwrite(buf1 + i * picWidth * 3 / 2, 1, picWidth * 3 / 2, fpw);
	}
	fclose(fpw);
	cout << ">>图像保存至../../Lena_Quarter_Switched.bmp" << endl;
	delete[] buf; delete[] buf1; delete[] buf2; delete[] buf3; delete[] buf4;
	cout << "按任意键返回主菜单" << endl;
	_getch();
	system("cls");
}
void convert2Gray() {	
	system("cls");
	cout << "///灰度图像///" << endl;
	Mat inputMat = imread("../../Lena_rotated.bmp");
	if (inputMat.empty()) {
		cout << "未找到../../Lena_rotated.bmp" << endl << "按任意键返回主菜单" <<endl; _getch();
		return;
	}
	else cout << ">>成功打开" << "../../Lena_rotated.bmp" << endl;
	cvtColor(inputMat, inputMat, CV_BGR2GRAY);
	imshow("Lena灰度图", inputMat);
	waitKey(0);
	imwrite("../../Lena_Gray.bmp", inputMat);
	cout << ">>图像保存至../../Lena_Gray.bmp" << endl;
	cout << "按任意键返回主菜单" << endl;
	_getch();
	system("cls");
}
void sobelDisplay() {
	system("cls");
	cout << "///Sobel边缘图像///" << endl;
	Mat inputMat = imread("../../Lena_gray.bmp"), outputMatX, outputMatY, outputMat;
	if (inputMat.empty()) {
		cout << "未找到../../Lena_gray.bmp" << endl << "按任意键返回主菜单" <<endl;_getch();
		return;
	}
	else cout << ">>成功打开" << "../../Lena_gray.bmp" << endl;
	Sobel(inputMat, outputMatX, CV_16S, 1, 0, 3);
	Sobel(inputMat, outputMatY, CV_16S, 0, 1, 3);
	convertScaleAbs(outputMatX, outputMatX);
	convertScaleAbs(outputMatY, outputMatY);
	addWeighted(outputMatX, 0.5, outputMatY, 0.5, 0, outputMat);
	imshow("Sobel边缘图", outputMat);
	waitKey(0);
	imwrite("../../Lena_Sobel.bmp", outputMat);
	cout << ">>图像保存至../../Lena_Sobel.bmp" << endl;	
	cout << "按任意键返回主菜单" << endl;
	_getch();
	system("cls");
}
void gaussianDisplay() {
	system("cls");
	cout << "///高斯平滑///" << endl;
	Mat inputMat = imread("../../Lena_Gray.bmp"),outputMat;
	if (inputMat.empty()) {
		cout << "未找到../../Lena_Gray.bmp" << endl << "按任意键返回主菜单" <<endl; _getch();
		return;
	}
	else cout << ">>成功打开" << "../../Lena_Gray.bmp" << endl;
	GaussianBlur(inputMat, outputMat, Size(25, 25), 0, 0);
	imshow("Lena灰度图", inputMat);
	imshow("高斯平滑输出图像", outputMat);
	waitKey(0);
	imwrite("../../Lena_Gaussian_Flur.bmp", outputMat);
	cout << ">>图像保存至../../Lena_Gaussian_Flur.bmp" << endl;
	cout << "按任意键返回主菜单" << endl;
	_getch();
	system("cls");
}
void binaryzationDisplay() {
	system("cls");
	cout << "///二值化///" << endl;
	Mat inputMat = imread("../../Lena_gray.bmp"), outputMat1, outputMat2, outputMat3, outputMat4;
	if (inputMat.empty()) {
		cout << "未找到../../Lena_gray.bmp" << endl << "按任意键返回主菜单" <<endl; _getch();
		return;
	}
	else cout << ">>成功打开" << "../../Lena_gray.bmp" << endl;
	threshold(inputMat, outputMat1, 30, 255, CV_THRESH_BINARY);
	threshold(inputMat, outputMat2, 60, 255, CV_THRESH_BINARY);
	threshold(inputMat, outputMat3, 90, 255, CV_THRESH_BINARY);
	threshold(inputMat, outputMat4, 120, 255, CV_THRESH_BINARY);
	imshow("二值化输出图像_阈值30", outputMat1); imshow("二值化输出图像_阈值60", outputMat2);
	imshow("二值化输出图像_阈值90", outputMat3); imshow("二值化输出图像_阈值120", outputMat4);
	waitKey(0);
	imwrite("../../Lena_Binary_30.bmp", outputMat1); imwrite("../../Lena_Binary_60.bmp", outputMat2);
	imwrite("../../Lena_Binary_90.bmp", outputMat3); imwrite("../../Lena_Binary_120.bmp", outputMat4);
	cout << ">>图像保存至" << endl << "../../Lena_Binary_30.bmp" << endl << "../../Lena_Binary_60.bmp"
		<< endl << "../../Lena_Binary_90.bmp" << endl << "../../Lena_Binary_120.bmp" << endl;
	cout << "按任意键返回主菜单" << endl;
	_getch();
	system("cls");
}
void histogramDisplay_Gray() {
	system("cls");
	cout << "///灰度直方图///" << endl;
	Mat inputMat = imread("../../Lena.bmp"), hist;
	if (inputMat.empty()) {
		cout << "未找到../../Lena.bmp" << endl << "按任意键返回主菜单" <<endl; _getch();
		return;
	}
	else cout << ">>成功打开" << "../../Lena.bmp" << endl;
	cvtColor(inputMat, inputMat, CV_BGR2GRAY);	//转换为灰度图像
	Mat histImage = Mat::zeros(Size(256, 256), CV_8UC3);
	int channels = 0;
	int histsize[] = { 256 };
	float midranges[] = { 0,255 };
	const float* ranges[] = { midranges };
	calcHist(&inputMat, 1, &channels, Mat(), hist, 1, histsize, ranges, true, false);
	double histMaxValue;
	minMaxLoc(hist, 0, &histMaxValue, 0, 0);
	for (int i = 0; i < 256; i++) {
		int value = cvRound(256 * 0.9 * (hist.at<float>(i) / histMaxValue));
		line(histImage, Point(i, histImage.rows - 1), Point(i, histImage.rows - 1 - value), Scalar(255, 255, 255));
	}
	imshow("Lena灰度图", inputMat);
	imshow("灰度直方图", histImage);
	waitKey(0);
	imwrite("../../Lena_Hist_Gray.bmp", histImage);
	cout << ">>图像保存至../../Lena_Hist_Gray.bmp" << endl;
	cout << "按任意键返回主菜单" << endl;
	_getch();
	system("cls");
}
void histogramDisplay_RGB() {
	system("cls");
	cout << "///RGB直方图///" << endl;
	Mat inputMat = imread("../../Lena.bmp"),hist;
	if (inputMat.empty()) {
		cout << "未找到../../Lena.bmp" << endl << "按任意键返回主菜单" <<endl; _getch();
		return;
	}
	else cout << ">>成功打开" << "../../Lena.bmp" << endl;
	Mat histImageB = Mat::zeros(Size(256, 256), CV_8UC3), 
		histImageG = Mat::zeros(Size(256, 256), CV_8UC3),
		histImageR = Mat::zeros(Size(256, 256), CV_8UC3);
	int channels = 0;
	int histsize[] = { 256 };
	float midranges[] = { 0,255 };
	const float* ranges[] = { midranges };
	calcHist(&inputMat, 1, &channels, Mat(), hist, 1, histsize, ranges, true, false);	
	double g_dhistmaxvalue;
	minMaxLoc(hist, 0, &g_dhistmaxvalue, 0, 0);
	for (int i = 0; i < 256; i++) {
		int value = cvRound(256 * 0.9 * (hist.at<float>(i) / g_dhistmaxvalue));
		line(histImageB, Point(i, histImageB.rows - 1), Point(i, histImageB.rows - 1 - value), Scalar(255, 0, 0));
	}
	imshow("B通道直方图", histImageB);
	channels = 1;
	calcHist(&inputMat, 1, &channels, Mat(), hist, 1, histsize, ranges, true, false);
	for (int i = 0; i < 256; i++) {
		int value = cvRound(256 * 0.9 * (hist.at<float>(i) / g_dhistmaxvalue));
		line(histImageG, Point(i, histImageG.rows - 1), Point(i, histImageG.rows - 1 - value), Scalar(0, 255, 0));
	}
	imshow("G通道直方图", histImageG);
	channels = 2;
	calcHist(&inputMat, 1, &channels, Mat(), hist, 1, histsize, ranges, true, false);	
	for (int i = 0; i < 256; i++) {
		int value = cvRound(256 * 0.9 * (hist.at<float>(i) / g_dhistmaxvalue));
		line(histImageR, Point(i, histImageR.rows - 1), Point(i, histImageR.rows - 1 - value), Scalar(0, 0, 255));
	}
	imshow("R通道直方图", histImageR);
	waitKey(0);
	imwrite("../../Lena_Hist_B.bmp", histImageB); cout << ">>图像保存至../../Lena_Hist_B.bmp" << endl;
	imwrite("../../Lena_Hist_G.bmp", histImageG); cout << ">>图像保存至../../Lena_Hist_G.bmp" << endl;
	imwrite("../../Lena_Hist_R.bmp", histImageR); cout << ">>图像保存至../../Lena_Hist_R.bmp" << endl;	
	cout << "按任意键返回主菜单" << endl;
	_getch();
	system("cls");
}
void histogramDisplay_HOG() {
	system("cls");
	cout << "///Sobel边缘图像///" << endl;
	Mat inputMat = imread("../../Lena.bmp"), outputMatX, outputMatY;
	if (inputMat.empty()) {
		cout << "未找到../../Lena.bmp" << endl << "按任意键返回主菜单" << endl; _getch();
		return;
	}
	else cout << ">>成功打开" << "../../Lena.bmp" << endl;
	cvtColor(inputMat, inputMat, CV_BGR2GRAY);
	//Sobel(inputMat, outputMatX, CV_16S, 1, 0, 3);
	//Sobel(inputMat, outputMatY, CV_16S, 0, 1, 3);
	//HOGDescriptor hog = HOGDescriptor(Size(512,512),Size(128,128), Size(128, 128), Size(128, 128),8);
	//hog.compute(inputMat,);

}