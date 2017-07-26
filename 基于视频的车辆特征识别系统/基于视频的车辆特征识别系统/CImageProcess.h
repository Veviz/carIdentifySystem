#include "highgui.h"
#include <cv.h>
#include<opencv2/ml/ml.hpp>
#include "stdafx.h"
using namespace cv;

#define VERTICAL 0
#define HORIZONTAL 1
#define N 10//颜色模板个数
#define WHITE 1
#define GRAY 2
#define YELLOW 3
#define PINK 4
#define RED 5
#define PURPLE 6
#define GREEN 7
#define BLUE 8
#define BROWN 9
#define BLACK 10

#define PURPLE_EXAM 2
#define BLUE_EXAM 28
#define GREEN_EXAM 18
#define YELLOW_EXAM 14
#define RED_EXAM 5
#define PINK_EXAM 2
#define BROWN_EXAM 6
#define GRAY_EXAM 15
#define WHITE_EXAM 8
#define BLACK_EXAM 23

class CImageProcess
{
public:
	CImageProcess();
	~CImageProcess();
private:
	float purple[PURPLE_EXAM][3];
	float blue[BLUE_EXAM][3];
	float green[GREEN_EXAM][3];
	float yellow[YELLOW_EXAM][3];
	float red[RED_EXAM][3];
	float pink[PINK_EXAM][3];
	float brown[BROWN_EXAM][3];
	float gray[GRAY_EXAM][3];
	float white[WHITE_EXAM][3];
	float black[BLACK_EXAM][3];
public:
	void ColorSvm();
	int max(int count[],int num=10);//找出1—10中最大的下标，0号单元做哨兵
public:
	IplImage *g_gray;
	int g_thresh;
	CvMemStorage *g_storage;
	int w;
	int **num;
public:
	int HistogramBins;
	float HistogramRange[2];
	float *HistogramRangel[1];
	CvHistogram * MyHistogram;
	CString Color;
	CString CarType;
public:	
	IplImage* BackgroundImg;//背景图像
	IplImage *noBgImg;
public:
	BOOL GetHistogram(unsigned char *pImageData,int nWidth,int nHeight,int nWidthStep,int *pHistogram);
	void BackgroundInit(char * argv);
	void ForegroundImageExtraction(IplImage *curFrame,IplImage*BackgroundImg,IplImage *foregroundImg);
	void Sharpen(IplImage *OriginalImg,IplImage *SharpenImg);
	void BinaryZation(IplImage *OriginalImg,IplImage *BinaryImg);
	void ConvertGray(IplImage *OriginalImg,IplImage *GrayImg);
	int  CarContursExtraction( IplImage *OriginalImg,IplImage **CarConturs);
	void SideFaceStandardization(IplImage *Img,IplImage *StdImg);
	double SpaceOccupancyExtraction(IplImage *StdImg);
	double LengthHeightRatioExtration(IplImage *StdImg);
	double TailCarHeightRatioExtration(IplImage *StdImg);
	int AxleNumberExtraion(IplImage *StdImg);
	double AxleDistanceCarLengthRatioExtration(IplImage *StdImg);
	void RemoveNoise(IplImage*binaryImg,int row,int col,int Direction=VERTICAL);
	void CarTypeJudge(double Occupancy,double LengthHeightRatio,double TailCarHeightRatio );
	void BackgroundUpdate(IplImage *curFrame,IplImage *preFrame);
	void NoBackgroudImageExtraction(IplImage *curFrame,IplImage *foregroundImg);
	bool Judge(IplImage * foreground);
protected:
	int itos(int num,char *str,int cur);
	bool IsNoise(IplImage *binaryImg,int x,int y,int row,int col);
	int Otsu(unsigned char *pImageData, int nWidth, int nHeight, int nWidthStep);
	void LightUnitizing(IplImage *OriginalImg,IplImage *LightUnitizingImg);
	void ImageStretchByHistogram(IplImage *OriginalImg,IplImage *StretchImg);
	void rgb2hsv(const double R,const double G,const double B,double &H,double &S,double &V);
	void ShadowRemove(double &R,double &G,double &B,const double bR,const double bG, const double bB);
public:
	void Laplace(IplImage *src,IplImage *workImg);
	IplImage* Robert( IplImage *src, int lWidth, int lHeight );
	int OSTUthred(CvHistogram * MyHistogram);
public:
	void OnFileOpen();
	void OnFileSaveAs();
	void OnGussSmooth();
	void OnLaplacionSharp();
	void OnBinaryConvert();
	void OnImgOpen();
	void OnOpencvAvi();
	void OnTrackbar();
	void OnGrayConvert();
	void OnBackgroundIsolation();
	void OnPixelRead();
};