#include "stdafx.h"
#include "CImageProcess.h"
#include "highgui.h"
#include <cv.h>
#include <iostream>
#include "InitDialog.h"
using namespace cv;
using namespace std;



CImageProcess::CImageProcess()
{
	// TODO: 在此处添加构造代码
	BackgroundImg=NULL;
	Color="无";
	CarType="无";
	num=NULL;
	g_gray=NULL;
	g_thresh=100;
	g_storage =NULL;
	HistogramBins = 256;
	HistogramRange[0]=0;
	HistogramRange[1]=255;
	HistogramRangel[0]=&HistogramRange[0];
	MyHistogram=cvCreateHist(1,&HistogramBins,CV_HIST_ARRAY,HistogramRangel);
	float purple_exam[PURPLE_EXAM][3]={{209.1,133,122},{168.3,138,150}};
	float blue_exam[BLUE_EXAM][3]={{63.75,129,120},{68.85,132,110},{37.95,109,128},{76.5,128,102},{114.75,117,97},{135.15,108,95},{140.25,117,101},{112.2,118,112},
{181.05,106,107},{191.25,113,114},{10.2,117,93},{226.95,119,131},{216.75,116,128},{211.65,111,126},{158.1,98,117},{140.25,99,106},{124.95,111,112},
{109.65,113,114},{89.25,121,118},{114.75,119,97},{130.05,93,122},{73.95,190,26},{61.2,185,37},{61.2,150,68},{2.55,214,0},{2.55,118,43},{17.85,118,43},
{22.5,165,48}};
	float green_exam[GREEN_EXAM][3]={{173.4,96,132},{145.35,91,131},{204,111,130},{198.9,112,141},{122.4,89,149},{109.65,96,143},{89.25,113,137},{81.6,115,121},{73.95,122,130},
{221.85,121,130},{206.55,113,138},{147.9,110,142},{204,111,150},{183.6,101,156},{219.3,113,148},{135.15,110,147},{137.7,121,142},{219.3,113,148}};
	float yellow_exam[YELLOW_EXAM][3]={{224.4,133,165},{216.75,128,120},{211.65,131,209},{196.35,151,211},{188.7,155,204},{153,145,174},{102,131,146},{150.45,178,182},
{147.9,185,181},{211.65,144,164},{158.1,184,178},{198.9,153,166},{206.55,145,182},{198.9,156,164}};
	float red_exam[RED_EXAM][3]={{86.7,151,142},{119.85,178,162},{112.2,180,141},{89.25,163,144},{132.6,178,171}};
	float pink_exam[PINK_EXAM][3]={{198.9,151,128},{216.75,147,126}};
	float brown_exam[BROWN_EXAM][3]={{107.1,145,149},{89.25,135,139},{68.85,134,132},{76.5,138,135},{127.5,151,167},{94.35,139,143}};
	float gray_exam[GRAY_EXAM][3]={{107.1,126,130},{130.05,126,126},{153,126,127},{163.2,125,127},{188.7,125,128},{183.6,126,129},{204,121,142},{137.7,126,136},{94.35,127,137},
{99.45,127,137},{183.6,126,136},{137.3,126,136},{130.05,125,131},{183.6,123,139},{175.95,126,136}};
	float white_exam[WHITE_EXAM][3]={{252.45,128,134},{255,128,128},{247.35,133,126},{244.8,132,133},{249.9,128,136},{242.25,136,127},{249.9,131,129},{232.05,128,128}};
	float black_exam[BLACK_EXAM][3]={{0,128,128},{2.55,127,124},{10.2,121,132},{7.65,125,124},{17.85,117,131},{7.65,133,106},{22.95,121,116},{22.95,113,137},{28.05,114,128},
{12.75,134,101},{5.1,128,126},{12.75,126,132},{20.4,129,135},{17.85,122,129},{25.5,128,129},{33.16,126,130},{30.6,128,128},{45.9,126,121},{48.45,124,127},
{33.15,127,128},{45.9,125,125},{66.3,126,125},{40.8,128,130}};
	int i;
	for(i=0;i<PURPLE_EXAM;i++){
		purple[i][0]=purple_exam[i][0];
		purple[i][1]=purple_exam[i][1];
		purple[i][2]=purple_exam[i][2];
	}
	for(i=0;i<BLUE_EXAM;i++){
		blue[i][0]=blue_exam[i][0];
		blue[i][1]=blue_exam[i][1];
		blue[i][2]=blue_exam[i][2];
	}
	for(i=0;i<GREEN_EXAM;i++){
		green[i][0]=green_exam[i][0];
		green[i][1]=green_exam[i][1];
		green[i][2]=green_exam[i][2];
	}
	for(i=0;i<YELLOW_EXAM;i++){
		yellow[i][0]=yellow_exam[i][0];
		yellow[i][1]=yellow_exam[i][1];
		yellow[i][2]=yellow_exam[i][2];
	}
	for(i=0;i<RED_EXAM;i++){
		red[i][0]=red_exam[i][0];
		red[i][1]=red_exam[i][1];
		red[i][2]=red_exam[i][2];
	}
	for(i=0;i<PINK_EXAM;i++){
		pink[i][0]=pink_exam[i][0];
		pink[i][1]=pink_exam[i][1];
		pink[i][2]=pink_exam[i][2];
	}
	for(i=0;i<BROWN_EXAM;i++){
		brown[i][0]=brown_exam[i][0];
		brown[i][1]=brown_exam[i][1];
		brown[i][2]=brown_exam[i][2];
	}
	for(i=0;i<GRAY_EXAM;i++){
		gray[i][0]=gray_exam[i][0];
		gray[i][1]=gray_exam[i][1];
		gray[i][2]=gray_exam[i][2];
	}
	for(i=0;i<WHITE_EXAM;i++){
		white[i][0]=white_exam[i][0];
		white[i][1]=white_exam[i][1];
		white[i][2]=white_exam[i][2];
	}
	for(i=0;i<BLACK_EXAM;i++){
		black[i][0]=black_exam[i][0];
		black[i][1]=black_exam[i][1];
		black[i][2]=black_exam[i][2];
	}
}

CImageProcess::~CImageProcess()
{
	if(BackgroundImg)
		cvReleaseImage(&BackgroundImg);
	BackgroundImg=NULL;
	if(num)
	{
		for(int i = 0; i != w; i++)
		{
			if(num[i])
				delete[] num[i];
		}
		delete[] num;
		num=NULL;
	}
}
void CImageProcess::ColorSvm(){
	IplImage *temp=cvCreateImage(cvGetSize(noBgImg),noBgImg->depth,noBgImg->nChannels);
	cvCopy(noBgImg,temp);
	Mat image(temp);
	int i;
	/****************************************************************************
	********                       建立训练样本                       ***********
	****************************************************************************/
	float redbrown_lables[RED_EXAM+BROWN_EXAM]={RED,RED,RED,RED,RED,BROWN,BROWN,BROWN,BROWN,BROWN,BROWN};
	Mat redbrown_lableMat(RED_EXAM+BROWN_EXAM,1,CV_32FC1,redbrown_lables);
	float redyellow_lable[RED_EXAM+YELLOW_EXAM]={RED,RED,RED,RED,RED,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW};
	Mat redyellow_lableMat(RED_EXAM+YELLOW_EXAM,1,CV_32FC1,redyellow_lable);
	float redgreen_lable[RED_EXAM+GREEN_EXAM]={RED,RED,RED,RED,RED,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN};
	Mat redgreen_lableMat(RED_EXAM+GREEN_EXAM,1,CV_32FC1,redgreen_lable);
	float redpink_lable[RED_EXAM+PINK_EXAM]={RED,RED,RED,RED,RED,PINK,PINK};
	Mat redpink_lableMat(RED_EXAM+PINK_EXAM,1,CV_32FC1,redpink_lable);
	float redgray_lable[RED_EXAM+GRAY_EXAM]={RED,RED,RED,RED,RED,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY};
	Mat redgray_lableMat(RED_EXAM+GRAY_EXAM,1,CV_32FC1,redgray_lable);
	float redblue_lable[RED_EXAM+BLUE_EXAM]={RED,RED,RED,RED,RED,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,
		BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE};
	Mat redblue_lableMat(RED_EXAM+BLUE_EXAM,1,CV_32FC1,redblue_lable);
	float redpurple_lable[RED_EXAM+PURPLE_EXAM]={RED,RED,RED,RED,RED,PURPLE,PURPLE};
	Mat redpurple_lableMat(RED_EXAM+PURPLE_EXAM,1,CV_32FC1,redpurple_lable);
	float brownyellow_lable[BROWN_EXAM+YELLOW_EXAM]={BROWN,BROWN,BROWN,BROWN,BROWN,BROWN,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW};
	Mat brownyellow_lableMat(BROWN_EXAM+YELLOW_EXAM,1,CV_32FC1,brownyellow_lable);
	float browngreen_lable[BROWN_EXAM+GREEN_EXAM]={BROWN,BROWN,BROWN,BROWN,BROWN,BROWN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,
		GREEN,GREEN,GREEN,GREEN};
	Mat browngreen_lableMat(BROWN_EXAM+GREEN_EXAM,1,CV_32FC1,browngreen_lable);
	float brownpink_lable[BROWN_EXAM+PINK_EXAM]={BROWN,BROWN,BROWN,BROWN,BROWN,BROWN,PINK,PINK};
	Mat brownpink_lableMat(BROWN_EXAM+PINK_EXAM,1,CV_32FC1,brownpink_lable);
	float browngray_lable[BROWN_EXAM+GRAY_EXAM]={BROWN,BROWN,BROWN,BROWN,BROWN,BROWN,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY};
	Mat browngray_lableMat(BROWN_EXAM+GRAY_EXAM,1,CV_32FC1,browngray_lable);
	float brownblue_lable[BROWN_EXAM+BLUE_EXAM]={BROWN,BROWN,BROWN,BROWN,BROWN,BROWN,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,
		BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE};
	Mat brownblue_lableMat(BROWN_EXAM+BLUE_EXAM,1,CV_32FC1,brownblue_lable);
	float brownpurple_lable[BROWN_EXAM+PURPLE_EXAM]={BROWN,BROWN,BROWN,BROWN,BROWN,BROWN,PURPLE,PURPLE};
	Mat brownpurple_lableMat(BROWN_EXAM+PURPLE_EXAM,1,CV_32FC1,brownpurple_lable);
	float yellowgreen_lable[YELLOW_EXAM+GREEN_EXAM]={YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,GREEN,GREEN,GREEN,GREEN,
		GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN};
	Mat yellowgreen_lableMat(YELLOW_EXAM+GREEN_EXAM,1,CV_32FC1,yellowgreen_lable);
	float yellowpink_lable[YELLOW_EXAM+PINK_EXAM]={YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,PINK,PINK};
	Mat yellowpink_lableMat(YELLOW_EXAM+PINK_EXAM,1,CV_32FC1,yellowpink_lable);
	float yellowgray_lable[YELLOW_EXAM+GRAY_EXAM]={YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,GRAY,GRAY,GRAY,GRAY,GRAY,
		GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY};
	Mat yellowgray_lableMat(YELLOW_EXAM+GRAY_EXAM,1,CV_32FC1,yellowgray_lable);
	float greenpink_lable[GREEN_EXAM+PINK_EXAM]={GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,PINK,PINK};
	Mat greenpink_lableMat(GREEN_EXAM+PINK_EXAM,1,CV_32FC1,greenpink_lable);
	float greengray_lable[GREEN_EXAM+GRAY_EXAM]={GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GRAY,GRAY,
		GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY};
	Mat greengray_lableMat(GREEN_EXAM+GRAY_EXAM,1,CV_32FC1,greengray_lable);
	float greenblue_lable[GREEN_EXAM+BLUE_EXAM]={GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,BLUE,BLUE,
		BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE};
	Mat greenblue_lableMat(GREEN_EXAM+BLUE_EXAM,1,CV_32FC1,greenblue_lable);
	float greenpurple_lable[GREEN_EXAM+PURPLE_EXAM]={GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,PURPLE,PURPLE};
	Mat greenpurple_lableMat(GREEN_EXAM+PURPLE_EXAM,1,CV_32FC1,greenpurple_lable);
	float purpleblue_lable[PURPLE_EXAM+BLUE_EXAM]={PURPLE,PURPLE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,
		BLUE,BLUE,BLUE,BLUE,BLUE,BLUE};
	Mat purpleblue_lableMat(PURPLE_EXAM+BLUE_EXAM,1,CV_32FC1,purpleblue_lable);
	float purplegray_lable[PURPLE_EXAM+GRAY_EXAM]={PURPLE,PURPLE,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY};
	Mat purplegray_lableMat(PURPLE_EXAM+GRAY_EXAM,1,CV_32FC1,purplegray_lable);
	float purplepink_lable[PURPLE_EXAM+PINK_EXAM]={PURPLE,PURPLE,PINK,PINK};
	Mat purplepink_lableMat(PURPLE_EXAM+PINK_EXAM,1,CV_32FC1,purplepink_lable);
	float pinkgray_lable[PINK_EXAM+GRAY_EXAM]={PINK,PINK,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY};
	Mat pinkgray_lableMat(PINK_EXAM+GRAY_EXAM,1,CV_32FC1,pinkgray_lable);
	float pinkblue_lable[PINK_EXAM+BLUE_EXAM]={PINK,PINK,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,
		BLUE,BLUE,BLUE,BLUE,BLUE};
	Mat pinkblue_lableMat(PINK_EXAM+BLUE_EXAM,1,CV_32FC1,pinkblue_lable);
	float bluegray_lable[BLUE_EXAM+GRAY_EXAM]={BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,
		BLUE,BLUE,BLUE,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY};
	Mat bluegray_lableMat(BLUE_EXAM+GRAY_EXAM,1,CV_32FC1,bluegray_lable);
	float whitered_lable[WHITE_EXAM+RED_EXAM]={WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,RED,RED,RED,RED,RED};
	Mat whitered_lableMat(WHITE_EXAM+RED_EXAM,1,CV_32FC1,whitered_lable);
	float whitebrown_lable[WHITE_EXAM+BROWN_EXAM]={WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,BROWN,BROWN,BROWN,BROWN,BROWN,BROWN};
	Mat whitebrown_lableMat(WHITE_EXAM+BROWN_EXAM,1,CV_32FC1,whitebrown_lable);
	float whiteyellow_lable[WHITE_EXAM+YELLOW_EXAM]={WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,
		YELLOW,YELLOW,YELLOW,YELLOW};
	Mat whiteyellow_lableMat(WHITE_EXAM+YELLOW_EXAM,1,CV_32FC1,whiteyellow_lable);
	float whitegreen_lable[WHITE_EXAM+GREEN_EXAM]={WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,
		GREEN,GREEN,GREEN,GREEN,GREEN,GREEN};
	Mat whitegreen_lableMat(WHITE_EXAM+GREEN_EXAM,1,CV_32FC1,whitegreen_lable);
	float whitepink_lable[WHITE_EXAM+PINK_EXAM]={WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,PINK,PINK};
	Mat whitepink_lableMat(WHITE_EXAM+PINK_EXAM,1,CV_32FC1,whitepink_lable);
	float whitegray_lable[WHITE_EXAM+GRAY_EXAM]={WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY};
	Mat whitegray_lableMat(WHITE_EXAM+GRAY_EXAM,1,CV_32FC1,whitegray_lable);
	float whitepurple_lable[WHITE_EXAM+PURPLE_EXAM]={WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,PURPLE,PURPLE};
	Mat whitepurple_lableMat(WHITE_EXAM+PURPLE_EXAM,1,CV_32FC1,whitepurple_lable);
	float whiteblue_lable[WHITE_EXAM+BLUE_EXAM]={WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,
		BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE};
	Mat whiteblue_lableMat(WHITE_EXAM+BLUE_EXAM,1,CV_32FC1,whiteblue_lable);
	float whiteblack_lable[WHITE_EXAM+BLACK_EXAM]={WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,
		BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK};
	Mat whiteblack_lableMat(WHITE_EXAM+BLACK_EXAM,1,CV_32FC1,whiteblack_lable);
	float blackred_lable[BLACK_EXAM+RED_EXAM]={BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,
		BLACK,BLACK,RED,RED,RED,RED,RED};
	Mat blackred_lableMat(BLACK_EXAM+RED_EXAM,1,CV_32FC1,blackred_lable);
	float blackbrown_lable[BLACK_EXAM+BROWN_EXAM]={BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,
		BLACK,BLACK,BROWN,BROWN,BROWN,BROWN,BROWN,BROWN};
	Mat blackbrown_lableMat(BLACK_EXAM+BROWN_EXAM,1,CV_32FC1,blackbrown_lable);
	float blackyellow_lable[BLACK_EXAM+YELLOW_EXAM]={BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,
		BLACK,BLACK,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW};
	Mat blackyellow_lableMat(BLACK_EXAM+YELLOW_EXAM,1,CV_32FC1,blackyellow_lable);
	float blackgreen_lable[BLACK_EXAM+GREEN_EXAM]={BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,
		BLACK,BLACK,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN};
	Mat blackgreen_lableMat(BLACK_EXAM+GREEN_EXAM,1,CV_32FC1,blackgreen_lable);
	float blackpink_lable[BLACK_EXAM+PINK_EXAM]={BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,
		BLACK,BLACK,PINK,PINK};
	Mat blackpink_lableMat(BLACK_EXAM+PINK_EXAM,1,CV_32FC1,blackpink_lable);
	float blackgray_lable[BLACK_EXAM+GRAY_EXAM]={BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,
		BLACK,BLACK,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY,GRAY};
	Mat blackgray_lableMat(BLACK_EXAM+GRAY_EXAM,1,CV_32FC1,blackgray_lable);
	float blackpurple_lable[BLACK_EXAM+PURPLE_EXAM]={BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,
		BLACK,BLACK,PURPLE,PURPLE};
	Mat blackpurple_lableMat(BLACK_EXAM+PURPLE_EXAM,1,CV_32FC1,blackpurple_lable);
	float blackblue_lable[BLACK_EXAM+BLUE_EXAM]={BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,
		BLACK,BLACK,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE};
	Mat blackblue_lableMat(BLACK_EXAM+BLUE_EXAM,1,CV_32FC1,blackblue_lable);


	float redbrown_trainData[RED_EXAM+BROWN_EXAM][3];
	for(i=0;i<RED_EXAM;i++){
		redbrown_trainData[i][0]=red[i][0];
		redbrown_trainData[i][1]=red[i][1];
		redbrown_trainData[i][2]=red[i][2];
	}
	for(i=RED_EXAM;i<RED_EXAM+BROWN_EXAM;i++){
		redbrown_trainData[i][0]=brown[i-RED_EXAM][0];
		redbrown_trainData[i][1]=brown[i-RED_EXAM][1];
		redbrown_trainData[i][2]=brown[i-RED_EXAM][2];
	}
	Mat redbrown_trainDataMat(RED_EXAM+BROWN_EXAM,3,CV_32FC1,redbrown_trainData);
	float redyellow_trainData[RED_EXAM+YELLOW_EXAM][3];
	for(i=0;i<RED_EXAM;i++){
		redyellow_trainData[i][0]=red[i][0];
		redyellow_trainData[i][1]=red[i][1];
		redyellow_trainData[i][2]=red[i][2];
	}
	for(i=RED_EXAM;i<RED_EXAM+YELLOW_EXAM;i++){
		redyellow_trainData[i][0]=yellow[i-RED_EXAM][0];
		redyellow_trainData[i][1]=yellow[i-RED_EXAM][1];
		redyellow_trainData[i][2]=yellow[i-RED_EXAM][2];
	}
	Mat redyellow_trainDataMat(RED_EXAM+YELLOW_EXAM,3,CV_32FC1,redyellow_trainData);
	float redgreen_trainData[RED_EXAM+GREEN_EXAM][3];
	for(i=0;i<RED_EXAM;i++){
		redgreen_trainData[i][0]=red[i][0];
		redgreen_trainData[i][1]=red[i][1];
		redgreen_trainData[i][2]=red[i][2];
	}
	for(i=RED_EXAM;i<RED_EXAM+GREEN_EXAM;i++){
		redgreen_trainData[i][0]=green[i-RED_EXAM][0];
		redgreen_trainData[i][1]=green[i-RED_EXAM][1];
		redgreen_trainData[i][2]=green[i-RED_EXAM][2];
	}
	Mat redgreen_trainDataMat(RED_EXAM+GREEN_EXAM,3,CV_32FC1,redgreen_trainData);
	float redpink_trainData[RED_EXAM+PINK_EXAM][3];
	for(i=0;i<RED_EXAM;i++){
		redpink_trainData[i][0]=red[i][0];
		redpink_trainData[i][1]=red[i][1];
		redpink_trainData[i][2]=red[i][2];
	}
	for(i=RED_EXAM;i<RED_EXAM+PINK_EXAM;i++){
		redpink_trainData[i][0]=pink[i-RED_EXAM][0];
		redpink_trainData[i][1]=pink[i-RED_EXAM][1];
		redpink_trainData[i][2]=pink[i-RED_EXAM][2];
	}
	Mat redpink_trainDataMat(RED_EXAM+PINK_EXAM,3,CV_32FC1,redpink_trainData);
	float redgray_trainData[RED_EXAM+GRAY_EXAM][3];
	for(i=0;i<RED_EXAM;i++){
		redgray_trainData[i][0]=red[i][0];
		redgray_trainData[i][1]=red[i][1];
		redgray_trainData[i][2]=red[i][2];
	}
	for(i=RED_EXAM;i<RED_EXAM+GRAY_EXAM;i++){
		redgray_trainData[i][0]=gray[i-RED_EXAM][0];
		redgray_trainData[i][1]=gray[i-RED_EXAM][1];
		redgray_trainData[i][2]=gray[i-RED_EXAM][2];
	}
	Mat redgray_trainDataMat(RED_EXAM+GRAY_EXAM,3,CV_32FC1,redgray_trainData);
	float redblue_trainData[RED_EXAM+BLUE_EXAM][3];
	for(i=0;i<RED_EXAM;i++){
		redblue_trainData[i][0]=red[i][0];
		redblue_trainData[i][1]=red[i][1];
		redblue_trainData[i][2]=red[i][2];
	}
	for(i=RED_EXAM;i<RED_EXAM+BLUE_EXAM;i++){
		redblue_trainData[i][0]=blue[i-RED_EXAM][0];
		redblue_trainData[i][1]=blue[i-RED_EXAM][1];
		redblue_trainData[i][2]=blue[i-RED_EXAM][2];
	}
	Mat redblue_trainDataMat(RED_EXAM+BLUE_EXAM,3,CV_32FC1,redblue_trainData);
	float redpurple_trainData[RED_EXAM+PURPLE_EXAM][3];
	for(i=0;i<RED_EXAM;i++){
		redpurple_trainData[i][0]=red[i][0];
		redpurple_trainData[i][1]=red[i][1];
		redpurple_trainData[i][2]=red[i][2];
	}
	for(i=RED_EXAM;i<RED_EXAM+PURPLE_EXAM;i++){
		redpurple_trainData[i][0]=purple[i-RED_EXAM][0];
		redpurple_trainData[i][1]=purple[i-RED_EXAM][1];
		redpurple_trainData[i][2]=purple[i-RED_EXAM][2];
	}
	Mat redpurple_trainDataMat(RED_EXAM+PURPLE_EXAM,3,CV_32FC1,redpurple_trainData);
	float brownyellow_trainData[BROWN_EXAM+YELLOW_EXAM][3];
	for(i=0;i<BROWN_EXAM;i++){
		brownyellow_trainData[i][0]=brown[i][0];
		brownyellow_trainData[i][1]=brown[i][1];
		brownyellow_trainData[i][2]=brown[i][2];
	}
	for(i=BROWN_EXAM;i<BROWN_EXAM+YELLOW_EXAM;i++){
		brownyellow_trainData[i][0]=yellow[i-BROWN_EXAM][0];
		brownyellow_trainData[i][1]=yellow[i-BROWN_EXAM][1];
		brownyellow_trainData[i][2]=yellow[i-BROWN_EXAM][2];
	}
	Mat brownyellow_trainDataMat(BROWN_EXAM+YELLOW_EXAM,3,CV_32FC1,brownyellow_trainData);
	float browngreen_trainData[BROWN_EXAM+GREEN_EXAM][3];
	for(i=0;i<BROWN_EXAM;i++){
		browngreen_trainData[i][0]=brown[i][0];
		browngreen_trainData[i][1]=brown[i][1];
		browngreen_trainData[i][2]=brown[i][2];
	}
	for(i=BROWN_EXAM;i<BROWN_EXAM+GREEN_EXAM;i++){
		browngreen_trainData[i][0]=green[i-BROWN_EXAM][0];
		browngreen_trainData[i][1]=green[i-BROWN_EXAM][1];
		browngreen_trainData[i][2]=green[i-BROWN_EXAM][2];
	}
	Mat browngreen_trainDataMat(BROWN_EXAM+GREEN_EXAM,3,CV_32FC1,browngreen_trainData);
	float brownpink_trainData[BROWN_EXAM+PINK_EXAM][3];
	for(i=0;i<BROWN_EXAM;i++){
		brownpink_trainData[i][0]=brown[i][0];
		brownpink_trainData[i][1]=brown[i][1];
		brownpink_trainData[i][2]=brown[i][2];
	}
	for(i=BROWN_EXAM;i<BROWN_EXAM+PINK_EXAM;i++){
		brownpink_trainData[i][0]=pink[i-BROWN_EXAM][0];
		brownpink_trainData[i][1]=pink[i-BROWN_EXAM][1];
		brownpink_trainData[i][2]=pink[i-BROWN_EXAM][2];
	}
	Mat brownpink_trainDataMat(BROWN_EXAM+PINK_EXAM,3,CV_32FC1,brownpink_trainData);
	float browngray_trainData[BROWN_EXAM+GRAY_EXAM][3];
	for(i=0;i<BROWN_EXAM;i++){
		browngray_trainData[i][0]=brown[i][0];
		browngray_trainData[i][1]=brown[i][1];
		browngray_trainData[i][2]=brown[i][2];
	}
	for(i=BROWN_EXAM;i<BROWN_EXAM+GRAY_EXAM;i++){
		browngray_trainData[i][0]=gray[i-BROWN_EXAM][0];
		browngray_trainData[i][1]=gray[i-BROWN_EXAM][1];
		browngray_trainData[i][2]=gray[i-BROWN_EXAM][2];
	}
	Mat browngray_trainDataMat(BROWN_EXAM+GRAY_EXAM,3,CV_32FC1,browngray_trainData);
	float brownblue_trainData[BROWN_EXAM+BLUE_EXAM][3];
	for(i=0;i<BROWN_EXAM;i++){
		brownblue_trainData[i][0]=brown[i][0];
		brownblue_trainData[i][1]=brown[i][1];
		brownblue_trainData[i][2]=brown[i][2];
	}
	for(i=BROWN_EXAM;i<BROWN_EXAM+BLUE_EXAM;i++){
		brownblue_trainData[i][0]=blue[i-BROWN_EXAM][0];
		brownblue_trainData[i][1]=blue[i-BROWN_EXAM][1];
		brownblue_trainData[i][2]=blue[i-BROWN_EXAM][2];
	}
	Mat brownblue_trainDataMat(BROWN_EXAM+BLUE_EXAM,3,CV_32FC1,brownblue_trainData);
	float brownpurple_trainData[BROWN_EXAM+PURPLE_EXAM][3];
	for(i=0;i<BROWN_EXAM;i++){
		brownpurple_trainData[i][0]=brown[i][0];
		brownpurple_trainData[i][1]=brown[i][1];
		brownpurple_trainData[i][2]=brown[i][2];
	}
	for(i=BROWN_EXAM;i<BROWN_EXAM+PURPLE_EXAM;i++){
		brownpurple_trainData[i][0]=purple[i-BROWN_EXAM][0];
		brownpurple_trainData[i][1]=purple[i-BROWN_EXAM][1];
		brownpurple_trainData[i][2]=purple[i-BROWN_EXAM][2];
	}
	Mat brownpurple_trainDataMat(BROWN_EXAM+PURPLE_EXAM,3,CV_32FC1,brownpurple_trainData);
	float yellowgreen_trainData[YELLOW_EXAM+GREEN_EXAM][3];
	for(i=0;i<YELLOW_EXAM;i++){
		yellowgreen_trainData[i][0]=yellow[i][0];
		yellowgreen_trainData[i][1]=yellow[i][1];
		yellowgreen_trainData[i][2]=yellow[i][2];
	}
	for(i=YELLOW_EXAM;i<YELLOW_EXAM+GREEN_EXAM;i++){
		yellowgreen_trainData[i][0]=green[i-YELLOW_EXAM][0];
		yellowgreen_trainData[i][1]=green[i-YELLOW_EXAM][1];
		yellowgreen_trainData[i][2]=green[i-YELLOW_EXAM][2];
	}
	Mat yellowgreen_trainDataMat(YELLOW_EXAM+GREEN_EXAM,3,CV_32FC1,yellowgreen_trainData);
	float yellowpink_trainData[YELLOW_EXAM+PINK_EXAM][3];
	for(i=0;i<YELLOW_EXAM;i++){
		yellowpink_trainData[i][0]=yellow[i][0];
		yellowpink_trainData[i][1]=yellow[i][1];
		yellowpink_trainData[i][2]=yellow[i][2];
	}
	for(i=YELLOW_EXAM;i<YELLOW_EXAM+PINK_EXAM;i++){
		yellowpink_trainData[i][0]=pink[i-YELLOW_EXAM][0];
		yellowpink_trainData[i][1]=pink[i-YELLOW_EXAM][1];
		yellowpink_trainData[i][2]=pink[i-YELLOW_EXAM][2];
	}
	Mat yellowpink_trainDataMat(YELLOW_EXAM+PINK_EXAM,3,CV_32FC1,yellowpink_trainData);
	float yellowgray_trainData[YELLOW_EXAM+GRAY_EXAM][3];
	for(i=0;i<YELLOW_EXAM;i++){
		yellowgray_trainData[i][0]=yellow[i][0];
		yellowgray_trainData[i][1]=yellow[i][1];
		yellowgray_trainData[i][2]=yellow[i][2];
	}
	for(i=YELLOW_EXAM;i<YELLOW_EXAM+GRAY_EXAM;i++){
		yellowgray_trainData[i][0]=gray[i-YELLOW_EXAM][0];
		yellowgray_trainData[i][1]=gray[i-YELLOW_EXAM][1];
		yellowgray_trainData[i][2]=gray[i-YELLOW_EXAM][2];
	}
	Mat yellowgray_trainDataMat(YELLOW_EXAM+GRAY_EXAM,3,CV_32FC1,yellowgray_trainData);
	float greenpink_trainData[GREEN_EXAM+PINK_EXAM][3];
	for(i=0;i<GREEN_EXAM;i++){
		greenpink_trainData[i][0]=green[i][0];
		greenpink_trainData[i][1]=green[i][1];
		greenpink_trainData[i][2]=green[i][2];
	}
	for(i=GREEN_EXAM;i<GREEN_EXAM+PINK_EXAM;i++){
		greenpink_trainData[i][0]=pink[i-GREEN_EXAM][0];
		greenpink_trainData[i][1]=pink[i-GREEN_EXAM][1];
		greenpink_trainData[i][2]=pink[i-GREEN_EXAM][2];
	}
	Mat greenpink_trainDataMat(GREEN_EXAM+PINK_EXAM,3,CV_32FC1,greenpink_trainData);
	float greengray_trainData[GREEN_EXAM+GRAY_EXAM][3];
	for(i=0;i<GREEN_EXAM;i++){
		greengray_trainData[i][0]=green[i][0];
		greengray_trainData[i][1]=green[i][1];
		greengray_trainData[i][2]=green[i][2];
	}
	for(i=GREEN_EXAM;i<GREEN_EXAM+GRAY_EXAM;i++){
		greengray_trainData[i][0]=gray[i-GREEN_EXAM][0];
		greengray_trainData[i][1]=gray[i-GREEN_EXAM][1];
		greengray_trainData[i][2]=gray[i-GREEN_EXAM][2];
	}
	Mat greengray_trainDataMat(GREEN_EXAM+GRAY_EXAM,3,CV_32FC1,greengray_trainData);
	float greenblue_trainData[GREEN_EXAM+BLUE_EXAM][3];
	for(i=0;i<GREEN_EXAM;i++){
		greenblue_trainData[i][0]=green[i][0];
		greenblue_trainData[i][1]=green[i][1];
		greenblue_trainData[i][2]=green[i][2];
	}
	for(i=GREEN_EXAM;i<GREEN_EXAM+BLUE_EXAM;i++){
		greenblue_trainData[i][0]=blue[i-GREEN_EXAM][0];
		greenblue_trainData[i][1]=blue[i-GREEN_EXAM][1];
		greenblue_trainData[i][2]=blue[i-GREEN_EXAM][2];
	}
	Mat greenblue_trainDataMat(GREEN_EXAM+BLUE_EXAM,3,CV_32FC1,greenblue_trainData);
	float greenpurple_trainData[GREEN_EXAM+PURPLE_EXAM][3];
	for(i=0;i<GREEN_EXAM;i++){
		greenpurple_trainData[i][0]=green[i][0];
		greenpurple_trainData[i][1]=green[i][1];
		greenpurple_trainData[i][2]=green[i][2];
	}
	for(i=GREEN_EXAM;i<GREEN_EXAM+PURPLE_EXAM;i++){
		greenpurple_trainData[i][0]=purple[i-GREEN_EXAM][0];
		greenpurple_trainData[i][1]=purple[i-GREEN_EXAM][1];
		greenpurple_trainData[i][2]=purple[i-GREEN_EXAM][2];
	}
	Mat greenpurple_trainDataMat(GREEN_EXAM+PURPLE_EXAM,3,CV_32FC1,greenpurple_trainData);
	float purpleblue_trainData[PURPLE_EXAM+BLUE_EXAM][3];
	for(i=0;i<PURPLE_EXAM;i++){
		purpleblue_trainData[i][0]=purple[i][0];
		purpleblue_trainData[i][1]=purple[i][1];
		purpleblue_trainData[i][2]=purple[i][2];
	}
	for(i=PURPLE_EXAM;i<PURPLE_EXAM+BLUE_EXAM;i++){
		purpleblue_trainData[i][0]=blue[i-PURPLE_EXAM][0];
		purpleblue_trainData[i][1]=blue[i-PURPLE_EXAM][1];
		purpleblue_trainData[i][2]=blue[i-PURPLE_EXAM][2];
	}
	Mat purpleblue_trainDataMat(PURPLE_EXAM+BLUE_EXAM,3,CV_32FC1,purpleblue_trainData);
	float purplegray_trainData[PURPLE_EXAM+GRAY_EXAM][3];
	for(i=0;i<PURPLE_EXAM;i++){
		purplegray_trainData[i][0]=purple[i][0];
		purplegray_trainData[i][1]=purple[i][1];
		purplegray_trainData[i][2]=purple[i][2];
	}
	for(i=PURPLE_EXAM;i<PURPLE_EXAM+GRAY_EXAM;i++){
		purplegray_trainData[i][0]=gray[i-PURPLE_EXAM][0];
		purplegray_trainData[i][1]=gray[i-PURPLE_EXAM][1];
		purplegray_trainData[i][2]=gray[i-PURPLE_EXAM][2];
	}
	Mat purplegray_trainDataMat(PURPLE_EXAM+GRAY_EXAM,3,CV_32FC1,purplegray_trainData);
	float purplepink_trainData[PURPLE_EXAM+PINK_EXAM][3];
	for(i=0;i<PURPLE_EXAM;i++){
		purplepink_trainData[i][0]=purple[i][0];
		purplepink_trainData[i][1]=purple[i][1];
		purplepink_trainData[i][2]=purple[i][2];
	}
	for(i=PURPLE_EXAM;i<PURPLE_EXAM+PINK_EXAM;i++){
		purplepink_trainData[i][0]=pink[i-PURPLE_EXAM][0];
		purplepink_trainData[i][1]=pink[i-PURPLE_EXAM][1];
		purplepink_trainData[i][2]=pink[i-PURPLE_EXAM][2];
	}
	Mat purplepink_trainDataMat(PURPLE_EXAM+PINK_EXAM,3,CV_32FC1,purplepink_trainData);
	float pinkgray_trainData[PINK_EXAM+GRAY_EXAM][3];
	for(i=0;i<PINK_EXAM;i++){
		pinkgray_trainData[i][0]=pink[i][0];
		pinkgray_trainData[i][1]=pink[i][1];
		pinkgray_trainData[i][2]=pink[i][2];
	}
	for(i=PINK_EXAM;i<PINK_EXAM+GRAY_EXAM;i++){
		pinkgray_trainData[i][0]=gray[i-PINK_EXAM][0];
		pinkgray_trainData[i][1]=gray[i-PINK_EXAM][1];
		pinkgray_trainData[i][2]=gray[i-PINK_EXAM][2];
	}
	Mat pinkgray_trainDataMat(PINK_EXAM+GRAY_EXAM,3,CV_32FC1,pinkgray_trainData);
	float pinkblue_trainData[PINK_EXAM+BLUE_EXAM][3];
	for(i=0;i<PINK_EXAM;i++){
		pinkblue_trainData[i][0]=pink[i][0];
		pinkblue_trainData[i][1]=pink[i][1];
		pinkblue_trainData[i][2]=pink[i][2];
	}
	for(i=PINK_EXAM;i<PINK_EXAM+BLUE_EXAM;i++){
		pinkblue_trainData[i][0]=blue[i-PINK_EXAM][0];
		pinkblue_trainData[i][1]=blue[i-PINK_EXAM][1];
		pinkblue_trainData[i][2]=blue[i-PINK_EXAM][2];
	}
	Mat pinkblue_trainDataMat(PINK_EXAM+BLUE_EXAM,3,CV_32FC1,pinkblue_trainData);
	float bluegray_trainData[BLUE_EXAM+GRAY_EXAM][3];
	for(i=0;i<BLUE_EXAM;i++){
		bluegray_trainData[i][0]=blue[i][0];
		bluegray_trainData[i][1]=blue[i][1];
		bluegray_trainData[i][2]=blue[i][2];
	}
	for(i=BLUE_EXAM;i<BLUE_EXAM+GRAY_EXAM;i++){
		bluegray_trainData[i][0]=gray[i-BLUE_EXAM][0];
		bluegray_trainData[i][1]=gray[i-BLUE_EXAM][1];
		bluegray_trainData[i][2]=gray[i-BLUE_EXAM][2];
	}
	Mat bluegray_trainDataMat(BLUE_EXAM+GRAY_EXAM,3,CV_32FC1,bluegray_trainData);
	float whitered_trainData[WHITE_EXAM+RED_EXAM][3];
	for(i=0;i<WHITE_EXAM;i++){
		whitered_trainData[i][0]=white[i][0];
		whitered_trainData[i][1]=white[i][1];
		whitered_trainData[i][2]=white[i][2];
	}
	for(i=WHITE_EXAM;i<WHITE_EXAM+RED_EXAM;i++){
		whitered_trainData[i][0]=red[i-WHITE_EXAM][0];
		whitered_trainData[i][1]=red[i-WHITE_EXAM][1];
		whitered_trainData[i][2]=red[i-WHITE_EXAM][2];
	}
	Mat whitered_trainDataMat(WHITE_EXAM+RED_EXAM,3,CV_32FC1,whitered_trainData);
	float whitebrown_trainData[WHITE_EXAM+BROWN_EXAM][3];
	for(i=0;i<WHITE_EXAM;i++){
		whitebrown_trainData[i][0]=white[i][0];
		whitebrown_trainData[i][1]=white[i][1];
		whitebrown_trainData[i][2]=white[i][2];
	}
	for(i=WHITE_EXAM;i<WHITE_EXAM+BROWN_EXAM;i++){
		whitebrown_trainData[i][0]=brown[i-WHITE_EXAM][0];
		whitebrown_trainData[i][1]=brown[i-WHITE_EXAM][1];
		whitebrown_trainData[i][2]=brown[i-WHITE_EXAM][2];
	}
	Mat whitebrown_trainDataMat(WHITE_EXAM+BROWN_EXAM,3,CV_32FC1,whitebrown_trainData);
	float whiteyellow_trainData[WHITE_EXAM+YELLOW_EXAM][3];
	for(i=0;i<WHITE_EXAM;i++){
		whiteyellow_trainData[i][0]=white[i][0];
		whiteyellow_trainData[i][1]=white[i][1];
		whiteyellow_trainData[i][2]=white[i][2];
	}
	for(i=WHITE_EXAM;i<WHITE_EXAM+YELLOW_EXAM;i++){
		whiteyellow_trainData[i][0]=yellow[i-WHITE_EXAM][0];
		whiteyellow_trainData[i][1]=yellow[i-WHITE_EXAM][1];
		whiteyellow_trainData[i][2]=yellow[i-WHITE_EXAM][2];
	}
	Mat whiteyellow_trainDataMat(WHITE_EXAM+YELLOW_EXAM,3,CV_32FC1,whiteyellow_trainData);
	float whitegreen_trainData[WHITE_EXAM+GREEN_EXAM][3];
	for(i=0;i<WHITE_EXAM;i++){
		whitegreen_trainData[i][0]=white[i][0];
		whitegreen_trainData[i][1]=white[i][1];
		whitegreen_trainData[i][2]=white[i][2];
	}
	for(i=WHITE_EXAM;i<WHITE_EXAM+GREEN_EXAM;i++){
		whitegreen_trainData[i][0]=green[i-WHITE_EXAM][0];
		whitegreen_trainData[i][1]=green[i-WHITE_EXAM][1];
		whitegreen_trainData[i][2]=green[i-WHITE_EXAM][2];
	}
	Mat whitegreen_trainDataMat(WHITE_EXAM+GREEN_EXAM,3,CV_32FC1,whitegreen_trainData);
	float whitepink_trainData[WHITE_EXAM+PINK_EXAM][3];
	for(i=0;i<WHITE_EXAM;i++){
		whitepink_trainData[i][0]=white[i][0];
		whitepink_trainData[i][1]=white[i][1];
		whitepink_trainData[i][2]=white[i][2];
	}
	for(i=WHITE_EXAM;i<WHITE_EXAM+PINK_EXAM;i++){
		whitepink_trainData[i][0]=green[i-WHITE_EXAM][0];
		whitepink_trainData[i][1]=green[i-WHITE_EXAM][1];
		whitepink_trainData[i][2]=green[i-WHITE_EXAM][2];
	}
	Mat whitepink_trainDataMat(WHITE_EXAM+PINK_EXAM,3,CV_32FC1,whitepink_trainData);
	float whitegray_trainData[WHITE_EXAM+GRAY_EXAM][3];
	for(i=0;i<WHITE_EXAM;i++){
		whitegray_trainData[i][0]=white[i][0];
		whitegray_trainData[i][1]=white[i][1];
		whitegray_trainData[i][2]=white[i][2];
	}
	for(i=WHITE_EXAM;i<WHITE_EXAM+GRAY_EXAM;i++){
		whitegray_trainData[i][0]=gray[i-WHITE_EXAM][0];
		whitegray_trainData[i][1]=gray[i-WHITE_EXAM][1];
		whitegray_trainData[i][2]=gray[i-WHITE_EXAM][2];
	}
	Mat whitegray_trainDataMat(WHITE_EXAM+GRAY_EXAM,3,CV_32FC1,whitegray_trainData);
	float whitepurple_trainData[WHITE_EXAM+PURPLE_EXAM][3];
	for(i=0;i<WHITE_EXAM;i++){
		whitepurple_trainData[i][0]=white[i][0];
		whitepurple_trainData[i][1]=white[i][1];
		whitepurple_trainData[i][2]=white[i][2];
	}
	for(i=WHITE_EXAM;i<WHITE_EXAM+PURPLE_EXAM;i++){
		whitepurple_trainData[i][0]=purple[i-WHITE_EXAM][0];
		whitepurple_trainData[i][1]=purple[i-WHITE_EXAM][1];
		whitepurple_trainData[i][2]=purple[i-WHITE_EXAM][2];
	}
	Mat whitepurple_trainDataMat(WHITE_EXAM+PURPLE_EXAM,3,CV_32FC1,whitepurple_trainData);
	float whiteblue_trainData[WHITE_EXAM+BLUE_EXAM][3];
	for(i=0;i<8;i++){
		whiteblue_trainData[i][0]=white[i][0];
		whiteblue_trainData[i][1]=white[i][1];
		whiteblue_trainData[i][2]=white[i][2];
	}
	for(i=WHITE_EXAM;i<WHITE_EXAM+BLUE_EXAM;i++){
		whiteblue_trainData[i][0]=blue[i-WHITE_EXAM][0];
		whiteblue_trainData[i][1]=blue[i-WHITE_EXAM][1];
		whiteblue_trainData[i][2]=blue[i-WHITE_EXAM][2];
	}
	Mat whiteblue_trainDataMat(WHITE_EXAM+BLUE_EXAM,3,CV_32FC1,whiteblue_trainData);
	float whiteblack_trainData[WHITE_EXAM+BLACK_EXAM][3];
	for(i=0;i<WHITE_EXAM;i++){
		whiteblack_trainData[i][0]=white[i][0];
		whiteblack_trainData[i][1]=white[i][1];
		whiteblack_trainData[i][2]=white[i][2];
	}
	for(i=WHITE_EXAM;i<WHITE_EXAM+BLACK_EXAM;i++){
		whiteblack_trainData[i][0]=black[i-WHITE_EXAM][0];
		whiteblack_trainData[i][1]=black[i-WHITE_EXAM][1];
		whiteblack_trainData[i][2]=black[i-WHITE_EXAM][2];
	}
	Mat whiteblack_trainDataMat(WHITE_EXAM+BLACK_EXAM,3,CV_32FC1,whiteblack_trainData);
	float blackred_trainData[BLACK_EXAM+RED_EXAM][3];
	for(i=0;i<BLACK_EXAM;i++){
		blackred_trainData[i][0]=black[i][0];
		blackred_trainData[i][1]=black[i][1];
		blackred_trainData[i][2]=black[i][2];
	}
	for(i=BLACK_EXAM;i<BLACK_EXAM+RED_EXAM;i++){
		blackred_trainData[i][0]=red[i-BLACK_EXAM][0];
		blackred_trainData[i][1]=red[i-BLACK_EXAM][1];
		blackred_trainData[i][2]=red[i-BLACK_EXAM][2];
	}
	Mat blackred_trainDataMat(BLACK_EXAM+RED_EXAM,3,CV_32FC1,blackred_trainData);
	float blackbrown_trainData[BLACK_EXAM+BROWN_EXAM][3];
	for(i=0;i<BLACK_EXAM;i++){
		blackbrown_trainData[i][0]=black[i][0];
		blackbrown_trainData[i][1]=black[i][1];
		blackbrown_trainData[i][2]=black[i][2];
	}
	for(i=BLACK_EXAM;i<BLACK_EXAM+BROWN_EXAM;i++){
		blackbrown_trainData[i][0]=brown[i-BLACK_EXAM][0];
		blackbrown_trainData[i][1]=brown[i-BLACK_EXAM][1];
		blackbrown_trainData[i][2]=brown[i-BLACK_EXAM][2];
	}
	Mat blackbrown_trainDataMat(BLACK_EXAM+BROWN_EXAM,3,CV_32FC1,blackbrown_trainData);
	float blackyellow_trainData[BLACK_EXAM+YELLOW_EXAM][3];
	for(i=0;i<BLACK_EXAM;i++){
		blackyellow_trainData[i][0]=black[i][0];
		blackyellow_trainData[i][1]=black[i][1];
		blackyellow_trainData[i][2]=black[i][2];
	}
	for(i=BLACK_EXAM;i<BLACK_EXAM+YELLOW_EXAM;i++){
		blackyellow_trainData[i][0]=yellow[i-BLACK_EXAM][0];
		blackyellow_trainData[i][1]=yellow[i-BLACK_EXAM][1];
		blackyellow_trainData[i][2]=yellow[i-BLACK_EXAM][2];
	}
	Mat blackyellow_trainDataMat(BLACK_EXAM+YELLOW_EXAM,3,CV_32FC1,blackyellow_trainData);
	float blackgreen_trainData[BLACK_EXAM+GREEN_EXAM][3];
	for(i=0;i<BLACK_EXAM;i++){
		blackgreen_trainData[i][0]=black[i][0];
		blackgreen_trainData[i][1]=black[i][1];
		blackgreen_trainData[i][2]=black[i][2];
	}
	for(i=BLACK_EXAM;i<BLACK_EXAM+GREEN_EXAM;i++){
		blackgreen_trainData[i][0]=green[i-BLACK_EXAM][0];
		blackgreen_trainData[i][1]=green[i-BLACK_EXAM][1];
		blackgreen_trainData[i][2]=green[i-BLACK_EXAM][2];
	}
	Mat blackgreen_trainDataMat(BLACK_EXAM+GREEN_EXAM,3,CV_32FC1,blackgreen_trainData);
	float blackpink_trainData[BLACK_EXAM+PINK_EXAM][3];
	for(i=0;i<BLACK_EXAM;i++){
		blackpink_trainData[i][0]=black[i][0];
		blackpink_trainData[i][1]=black[i][1];
		blackpink_trainData[i][2]=black[i][2];
	}
	for(i=BLACK_EXAM;i<BLACK_EXAM+PINK_EXAM;i++){
		blackpink_trainData[i][0]=pink[i-BLACK_EXAM][0];
		blackpink_trainData[i][1]=pink[i-BLACK_EXAM][1];
		blackpink_trainData[i][2]=pink[i-BLACK_EXAM][2];
	}
	Mat blackpink_trainDataMat(BLACK_EXAM+PINK_EXAM,3,CV_32FC1,blackpink_trainData);
	float blackgray_trainData[BLACK_EXAM+GRAY_EXAM][3];
	for(i=0;i<BLACK_EXAM;i++){
		blackgray_trainData[i][0]=black[i][0];
		blackgray_trainData[i][1]=black[i][1];
		blackgray_trainData[i][2]=black[i][2];
	}
	for(i=BLACK_EXAM;i<BLACK_EXAM+GRAY_EXAM;i++){
		blackgray_trainData[i][0]=gray[i-BLACK_EXAM][0];
		blackgray_trainData[i][1]=gray[i-BLACK_EXAM][1];
		blackgray_trainData[i][2]=gray[i-BLACK_EXAM][2];
	}
	Mat blackgray_trainDataMat(BLACK_EXAM+GRAY_EXAM,3,CV_32FC1,blackgray_trainData);
	float blackpurple_trainData[BLACK_EXAM+PURPLE_EXAM][3];
	for(i=0;i<BLACK_EXAM;i++){
		blackpurple_trainData[i][0]=black[i][0];
		blackpurple_trainData[i][1]=black[i][1];
		blackpurple_trainData[i][2]=black[i][2];
	}
	for(i=BLACK_EXAM;i<BLACK_EXAM+PURPLE_EXAM;i++){
		blackpurple_trainData[i][0]=purple[i-BLACK_EXAM][0];
		blackpurple_trainData[i][1]=purple[i-BLACK_EXAM][1];
		blackpurple_trainData[i][2]=purple[i-BLACK_EXAM][2];
	}
	Mat blackpurple_trainDataMat(BLACK_EXAM+PURPLE_EXAM,3,CV_32FC1,blackpurple_trainData);
	float blackblue_trainData[BLACK_EXAM+BLUE_EXAM][3];
	for(i=0;i<BLACK_EXAM;i++){
		blackblue_trainData[i][0]=black[i][0];
		blackblue_trainData[i][1]=black[i][1];
		blackblue_trainData[i][2]=black[i][2];
	}
	for(i=BLACK_EXAM;i<BLACK_EXAM+BLUE_EXAM;i++){
		blackblue_trainData[i][0]=blue[i-BLACK_EXAM][0];
		blackblue_trainData[i][1]=blue[i-BLACK_EXAM][1];
		blackblue_trainData[i][2]=blue[i-BLACK_EXAM][2];
	}
	Mat blackblue_trainDataMat(BLACK_EXAM+BLUE_EXAM,3,CV_32FC1,blackblue_trainData);
	
	/****************************************************************************
	*********                        设置SVM参数                       **********
	****************************************************************************/
	//可线性分割的分属两类的训练样本
	//训练支持向量机，建立SVM模型
	CvSVMParams redbrown_params;
	redbrown_params.svm_type=CvSVM::C_SVC;
	redbrown_params.kernel_type=CvSVM::LINEAR;
	redbrown_params.term_crit=cvTermCriteria(CV_TERMCRIT_ITER,100,1e-6);
	CvSVM redbrown_svm;
	redbrown_svm.train(redbrown_trainDataMat,redbrown_lableMat,Mat(),Mat(),redbrown_params);

	CvSVMParams redyellow_params;
	redyellow_params.svm_type=CvSVM::C_SVC;
	redyellow_params.kernel_type=CvSVM::LINEAR;
	redyellow_params.term_crit=cvTermCriteria(CV_TERMCRIT_ITER,100,1e-6);
	CvSVM redyellow_svm;
	redyellow_svm.train(redyellow_trainDataMat,redyellow_lableMat,Mat(),Mat(),redyellow_params);

	CvSVMParams redgreen_params;
	redgreen_params.svm_type=CvSVM::C_SVC;
	redgreen_params.kernel_type=CvSVM::LINEAR;
	redgreen_params.term_crit=cvTermCriteria(CV_TERMCRIT_ITER,100,1e-6);
	CvSVM redgreen_svm;
	redgreen_svm.train(redgreen_trainDataMat,redgreen_lableMat,Mat(),Mat(),redgreen_params);

	CvSVMParams redpink_params;
	redpink_params.svm_type=CvSVM::C_SVC;
	redpink_params.kernel_type=CvSVM::LINEAR;
	redpink_params.term_crit=cvTermCriteria(CV_TERMCRIT_ITER,100,1e-6);
	CvSVM redpink_svm;
	redpink_svm.train(redpink_trainDataMat,redpink_lableMat,Mat(),Mat(),redpink_params);

	CvSVMParams redgray_params;
	redgray_params.svm_type=CvSVM::C_SVC;
	redgray_params.kernel_type=CvSVM::LINEAR;
	redgray_params.term_crit=cvTermCriteria(CV_TERMCRIT_ITER,100,1e-6);
	CvSVM redgray_svm;
	redgray_svm.train(redgray_trainDataMat,redgray_lableMat,Mat(),Mat(),redgray_params);

	CvSVMParams redblue_params;
	redblue_params.svm_type=CvSVM::C_SVC;
	redblue_params.kernel_type=CvSVM::LINEAR;
	redblue_params.term_crit=cvTermCriteria(CV_TERMCRIT_ITER,100,1e-6);
	CvSVM redblue_svm;
	redblue_svm.train(redblue_trainDataMat,redblue_lableMat,Mat(),Mat(),redblue_params);

	CvSVMParams redpurple_params;
	redpurple_params.svm_type=CvSVM::C_SVC;
	redpurple_params.kernel_type=CvSVM::LINEAR;
	redpurple_params.term_crit=cvTermCriteria(CV_TERMCRIT_ITER,100,1e-6);
	CvSVM redpurple_svm;
	redpurple_svm.train(redpurple_trainDataMat,redpurple_lableMat,Mat(),Mat(),redpurple_params);

	CvSVMParams brownyellow_params;
	brownyellow_params.svm_type=CvSVM::C_SVC;
	brownyellow_params.kernel_type=CvSVM::LINEAR;
	brownyellow_params.term_crit=cvTermCriteria(CV_TERMCRIT_ITER,100,1e-6);
	CvSVM brownyellow_svm;
	brownyellow_svm.train(brownyellow_trainDataMat,brownyellow_lableMat,Mat(),Mat(),brownyellow_params);

	CvSVMParams browngreen_params;
	browngreen_params.svm_type=CvSVM::C_SVC;
	browngreen_params.kernel_type=CvSVM::LINEAR;
	browngreen_params.term_crit=cvTermCriteria(CV_TERMCRIT_ITER,100,1e-6);
	CvSVM browngreen_svm;
	browngreen_svm.train(browngreen_trainDataMat,browngreen_lableMat,Mat(),Mat(),browngreen_params);

	CvSVMParams brownpink_params;
	brownpink_params.svm_type=CvSVM::C_SVC;
	brownpink_params.kernel_type=CvSVM::LINEAR;
	brownpink_params.term_crit=cvTermCriteria(CV_TERMCRIT_ITER,100,1e-6);
	CvSVM brownpink_svm;
	brownpink_svm.train(brownpink_trainDataMat,brownpink_lableMat,Mat(),Mat(),brownpink_params);

	CvSVMParams browngray_params;
	browngray_params.svm_type=CvSVM::C_SVC;
	browngray_params.kernel_type=CvSVM::LINEAR;
	browngray_params.term_crit=cvTermCriteria(CV_TERMCRIT_ITER,100,1e-6);
	CvSVM browngray_svm;
	browngray_svm.train(browngray_trainDataMat,browngray_lableMat,Mat(),Mat(),browngray_params);

	CvSVMParams brownblue_params;
	brownblue_params.svm_type=CvSVM::C_SVC;
	brownblue_params.kernel_type=CvSVM::LINEAR;
	brownblue_params.term_crit=cvTermCriteria(CV_TERMCRIT_ITER,100,1e-6);
	CvSVM brownblue_svm;
	brownblue_svm.train(brownblue_trainDataMat,brownblue_lableMat,Mat(),Mat(),brownblue_params);

	CvSVMParams brownpurple_params;
	brownpurple_params.svm_type=CvSVM::C_SVC;
	brownpurple_params.kernel_type=CvSVM::LINEAR;
	brownpurple_params.term_crit=cvTermCriteria(CV_TERMCRIT_ITER,100,1e-6);
	CvSVM brownpurple_svm;
	brownpurple_svm.train(brownpurple_trainDataMat,brownpurple_lableMat,Mat(),Mat(),brownpurple_params);

	CvSVMParams yellowgreen_params;
	yellowgreen_params.svm_type=CvSVM::C_SVC;
	yellowgreen_params.kernel_type=CvSVM::LINEAR;
	yellowgreen_params.term_crit=cvTermCriteria(CV_TERMCRIT_ITER,100,1e-6);
	CvSVM yellowgreen_svm;
	yellowgreen_svm.train(yellowgreen_trainDataMat,yellowgreen_lableMat,Mat(),Mat(),yellowgreen_params);

	CvSVMParams yellowpink_params;
	yellowpink_params.svm_type=CvSVM::C_SVC;
	yellowpink_params.kernel_type=CvSVM::LINEAR;
	yellowpink_params.term_crit=cvTermCriteria(CV_TERMCRIT_ITER,100,1e-6);
	CvSVM yellowpink_svm;
	yellowpink_svm.train(yellowpink_trainDataMat,yellowpink_lableMat,Mat(),Mat(),yellowpink_params);

	CvSVMParams yellowgray_params;
	yellowgray_params.svm_type=CvSVM::C_SVC;
	yellowgray_params.kernel_type=CvSVM::LINEAR;
	yellowgray_params.term_crit=cvTermCriteria(CV_TERMCRIT_ITER,100,1e-6);
	CvSVM yellowgray_svm;
	yellowgray_svm.train(yellowgray_trainDataMat,yellowgray_lableMat,Mat(),Mat(),yellowgray_params);

	CvSVMParams greenpink_params;
	greenpink_params.svm_type=CvSVM::C_SVC;
	greenpink_params.kernel_type=CvSVM::LINEAR;
	greenpink_params.term_crit=cvTermCriteria(CV_TERMCRIT_ITER,100,1e-6);
	CvSVM greenpink_svm;
	greenpink_svm.train(greenpink_trainDataMat,greenpink_lableMat,Mat(),Mat(),greenpink_params);

	CvSVMParams greengray_params;
	greengray_params.svm_type=CvSVM::C_SVC;
	greengray_params.kernel_type=CvSVM::LINEAR;
	greengray_params.term_crit=cvTermCriteria(CV_TERMCRIT_ITER,100,1e-6);
	CvSVM greengray_svm;
	greengray_svm.train(greengray_trainDataMat,greengray_lableMat,Mat(),Mat(),greengray_params);

	CvSVMParams greenblue_params;
	greenblue_params.svm_type=CvSVM::C_SVC;
	greenblue_params.kernel_type=CvSVM::LINEAR;
	greenblue_params.term_crit=cvTermCriteria(CV_TERMCRIT_ITER,100,1e-6);
	CvSVM greenblue_svm;
	greenblue_svm.train(greenblue_trainDataMat,greenblue_lableMat,Mat(),Mat(),greenblue_params);

	CvSVMParams greenpurple_params;
	greenpurple_params.svm_type=CvSVM::C_SVC;
	greenpurple_params.kernel_type=CvSVM::LINEAR;
	greenpurple_params.term_crit=cvTermCriteria(CV_TERMCRIT_ITER,100,1e-6);
	CvSVM greenpurple_svm;
	greenpurple_svm.train(greenpurple_trainDataMat,greenpurple_lableMat,Mat(),Mat(),greenpurple_params);

	CvSVMParams purpleblue_params;
	purpleblue_params.svm_type=CvSVM::C_SVC;
	purpleblue_params.kernel_type=CvSVM::LINEAR;
	purpleblue_params.term_crit=cvTermCriteria(CV_TERMCRIT_ITER,100,1e-6);
	CvSVM purpleblue_svm;
	purpleblue_svm.train(purpleblue_trainDataMat,purpleblue_lableMat,Mat(),Mat(),purpleblue_params);

	CvSVMParams purplegray_params;
	purplegray_params.svm_type=CvSVM::C_SVC;
	purplegray_params.kernel_type=CvSVM::LINEAR;
	purplegray_params.term_crit=cvTermCriteria(CV_TERMCRIT_ITER,100,1e-6);
	CvSVM purplegray_svm;
	purplegray_svm.train(purplegray_trainDataMat,purplegray_lableMat,Mat(),Mat(),purplegray_params);

	CvSVMParams purplepink_params;
	purplepink_params.svm_type=CvSVM::C_SVC;
	purplepink_params.kernel_type=CvSVM::LINEAR;
	purplepink_params.term_crit=cvTermCriteria(CV_TERMCRIT_ITER,100,1e-6);
	CvSVM purplepink_svm;
	purplepink_svm.train(purplepink_trainDataMat,purplepink_lableMat,Mat(),Mat(),purplepink_params);

	CvSVMParams pinkgray_params;
	pinkgray_params.svm_type=CvSVM::C_SVC;
	pinkgray_params.kernel_type=CvSVM::LINEAR;
	pinkgray_params.term_crit=cvTermCriteria(CV_TERMCRIT_ITER,100,1e-6);
	CvSVM pinkgray_svm;
	pinkgray_svm.train(pinkgray_trainDataMat,pinkgray_lableMat,Mat(),Mat(),pinkgray_params);

	CvSVMParams pinkblue_params;
	pinkblue_params.svm_type=CvSVM::C_SVC;
	pinkblue_params.kernel_type=CvSVM::LINEAR;
	pinkblue_params.term_crit=cvTermCriteria(CV_TERMCRIT_ITER,100,1e-6);
	CvSVM pinkblue_svm;
	pinkblue_svm.train(pinkblue_trainDataMat,pinkblue_lableMat,Mat(),Mat(),pinkblue_params);

	CvSVMParams bluegray_params;
	bluegray_params.svm_type=CvSVM::C_SVC;
	bluegray_params.kernel_type=CvSVM::LINEAR;
	bluegray_params.term_crit=cvTermCriteria(CV_TERMCRIT_ITER,100,1e-6);
	CvSVM bluegray_svm;
	bluegray_svm.train(bluegray_trainDataMat,bluegray_lableMat,Mat(),Mat(),bluegray_params);

	CvSVMParams whitered_params;
	whitered_params.svm_type=CvSVM::C_SVC;
	whitered_params.kernel_type=CvSVM::LINEAR;
	whitered_params.term_crit=cvTermCriteria(CV_TERMCRIT_ITER,100,1e-6);
	CvSVM whitered_svm;
	whitered_svm.train(whitered_trainDataMat,whitered_lableMat,Mat(),Mat(),whitered_params);

	CvSVMParams whitebrown_params;
	whitebrown_params.svm_type=CvSVM::C_SVC;
	whitebrown_params.kernel_type=CvSVM::LINEAR;
	whitebrown_params.term_crit=cvTermCriteria(CV_TERMCRIT_ITER,100,1e-6);
	CvSVM whitebrown_svm;
	whitebrown_svm.train(whitebrown_trainDataMat,whitebrown_lableMat,Mat(),Mat(),whitebrown_params);

	CvSVMParams whiteyellow_params;
	whiteyellow_params.svm_type=CvSVM::C_SVC;
	whiteyellow_params.kernel_type=CvSVM::LINEAR;
	whiteyellow_params.term_crit=cvTermCriteria(CV_TERMCRIT_ITER,100,1e-6);
	CvSVM whiteyellow_svm;
	whiteyellow_svm.train(whiteyellow_trainDataMat,whiteyellow_lableMat,Mat(),Mat(),whiteyellow_params);

	CvSVMParams whitegreen_params;
	whitegreen_params.svm_type=CvSVM::C_SVC;
	whitegreen_params.kernel_type=CvSVM::LINEAR;
	whitegreen_params.term_crit=cvTermCriteria(CV_TERMCRIT_ITER,100,1e-6);
	CvSVM whitegreen_svm;
	whitegreen_svm.train(whitegreen_trainDataMat,whitegreen_lableMat,Mat(),Mat(),whitegreen_params);

	CvSVMParams whitepink_params;
	whitepink_params.svm_type=CvSVM::C_SVC;
	whitepink_params.kernel_type=CvSVM::LINEAR;
	whitepink_params.term_crit=cvTermCriteria(CV_TERMCRIT_ITER,100,1e-6);
	CvSVM whitepink_svm;
	whitepink_svm.train(whitepink_trainDataMat,whitepink_lableMat,Mat(),Mat(),whitepink_params);

	CvSVMParams whitegray_params;
	whitegray_params.svm_type=CvSVM::C_SVC;
	whitegray_params.kernel_type=CvSVM::LINEAR;
	whitegray_params.term_crit=cvTermCriteria(CV_TERMCRIT_ITER,100,1e-6);
	CvSVM whitegray_svm;
	whitegray_svm.train(whitegray_trainDataMat,whitegray_lableMat,Mat(),Mat(),whitegray_params);

	CvSVMParams whitepurple_params;
	whitepurple_params.svm_type=CvSVM::C_SVC;
	whitepurple_params.kernel_type=CvSVM::LINEAR;
	whitepurple_params.term_crit=cvTermCriteria(CV_TERMCRIT_ITER,100,1e-6);
	CvSVM whitepurple_svm;
	whitepurple_svm.train(whitepurple_trainDataMat,whitepurple_lableMat,Mat(),Mat(),whitepurple_params);

	CvSVMParams whiteblue_params;
	whiteblue_params.svm_type=CvSVM::C_SVC;
	whiteblue_params.kernel_type=CvSVM::LINEAR;
	whiteblue_params.term_crit=cvTermCriteria(CV_TERMCRIT_ITER,100,1e-6);
	CvSVM whiteblue_svm;
	whiteblue_svm.train(whiteblue_trainDataMat,whiteblue_lableMat,Mat(),Mat(),whiteblue_params);

	CvSVMParams whiteblack_params;
	whiteblack_params.svm_type=CvSVM::C_SVC;
	whiteblack_params.kernel_type=CvSVM::LINEAR;
	whiteblack_params.term_crit=cvTermCriteria(CV_TERMCRIT_ITER,100,1e-6);
	CvSVM whiteblack_svm;
	whiteblack_svm.train(whiteblack_trainDataMat,whiteblack_lableMat,Mat(),Mat(),whiteblack_params);

	CvSVMParams blackred_params;
	blackred_params.svm_type=CvSVM::C_SVC;
	blackred_params.kernel_type=CvSVM::LINEAR;
	blackred_params.term_crit=cvTermCriteria(CV_TERMCRIT_ITER,100,1e-6);
	CvSVM blackred_svm;
	blackred_svm.train(blackred_trainDataMat,blackred_lableMat,Mat(),Mat(),blackred_params);

	CvSVMParams blackbrown_params;
	blackbrown_params.svm_type=CvSVM::C_SVC;
	blackbrown_params.kernel_type=CvSVM::LINEAR;
	blackbrown_params.term_crit=cvTermCriteria(CV_TERMCRIT_ITER,100,1e-6);
	CvSVM blackbrown_svm;
	blackbrown_svm.train(blackbrown_trainDataMat,blackbrown_lableMat,Mat(),Mat(),blackbrown_params);

	CvSVMParams blackyellow_params;
	blackyellow_params.svm_type=CvSVM::C_SVC;
	blackyellow_params.kernel_type=CvSVM::LINEAR;
	blackyellow_params.term_crit=cvTermCriteria(CV_TERMCRIT_ITER,100,1e-6);
	CvSVM blackyellow_svm;
	blackyellow_svm.train(blackyellow_trainDataMat,blackyellow_lableMat,Mat(),Mat(),blackyellow_params);

	CvSVMParams blackgreen_params;
	blackgreen_params.svm_type=CvSVM::C_SVC;
	blackgreen_params.kernel_type=CvSVM::LINEAR;
	blackgreen_params.term_crit=cvTermCriteria(CV_TERMCRIT_ITER,100,1e-6);
	CvSVM blackgreen_svm;
	blackgreen_svm.train(blackgreen_trainDataMat,blackgreen_lableMat,Mat(),Mat(),blackgreen_params);

	CvSVMParams blackpink_params;
	blackpink_params.svm_type=CvSVM::C_SVC;
	blackpink_params.kernel_type=CvSVM::LINEAR;
	blackpink_params.term_crit=cvTermCriteria(CV_TERMCRIT_ITER,100,1e-6);
	CvSVM blackpink_svm;
	blackpink_svm.train(blackpink_trainDataMat,blackpink_lableMat,Mat(),Mat(),blackpink_params);

	CvSVMParams blackgray_params;
	blackgray_params.svm_type=CvSVM::C_SVC;
	blackgray_params.kernel_type=CvSVM::LINEAR;
	blackgray_params.term_crit=cvTermCriteria(CV_TERMCRIT_ITER,100,1e-6);
	CvSVM blackgray_svm;
	blackgray_svm.train(blackgray_trainDataMat,blackgray_lableMat,Mat(),Mat(),blackgray_params);

	CvSVMParams blackpurple_params;
	blackpurple_params.svm_type=CvSVM::C_SVC;
	blackpurple_params.kernel_type=CvSVM::LINEAR;
	blackpurple_params.term_crit=cvTermCriteria(CV_TERMCRIT_ITER,100,1e-6);
	CvSVM blackpurple_svm;
	blackpurple_svm.train(blackpurple_trainDataMat,blackpurple_lableMat,Mat(),Mat(),blackpurple_params);

	CvSVMParams blackblue_params;
	blackblue_params.svm_type=CvSVM::C_SVC;
	blackblue_params.kernel_type=CvSVM::LINEAR;
	blackblue_params.term_crit=cvTermCriteria(CV_TERMCRIT_ITER,100,1e-6);
	CvSVM blackblue_svm;
	blackblue_svm.train(blackblue_trainDataMat,blackblue_lableMat,Mat(),Mat(),blackblue_params);

	Mat converted;
	Mat hsv;
	int j;
	int count[11]={0,0,0,0,0,0,0,0,0,0,0};
	int colorcount[11]={0,0,0,0,0,0,0,0,0,0,0};
	converted.create(image.rows,image.cols,image.type());
	cvtColor(image,converted,CV_BGR2Lab);//转换到Lab空间
	hsv.create(image.rows,image.cols,image.type());
	cvtColor(image,hsv,CV_BGR2HSV);//转换到hsv空间

	//重建训练完毕的支持向量机来将输入的样本分类
	//根据h的范围训练样本
	for(i=0;i<image.cols;i++){
		for(j=0;j<image.rows;j++){
			if((image.at<Vec3b>(j,i)[0]!=255)||(image.at<Vec3b>(j,i)[1]!=255)||(image.at<Vec3b>(j,i)[2]!=255)){
				for(int k=0;k<11;k++)
 					count[k]=0;
				float h=hsv.at<Vec3b>(j,i)[0];
				float lab[3]={converted.at<Vec3b>(j,i)[0],converted.at<Vec3b>(j,i)[1],converted.at<Vec3b>(j,i)[2]};
				Mat sampleMat(3,1,CV_32FC1,lab);
				//cout<<sampleMat<<endl;
				if((h>=0)&&(h<30)){
					if(hsv.at<Vec3b>(j,i)[1]>=255*0.5){
						count[int(redbrown_svm.predict(sampleMat))]++;
						count[int(redyellow_svm.predict(sampleMat))]++;
						count[int(redgreen_svm.predict(sampleMat))]++;
						count[int(brownyellow_svm.predict(sampleMat))]++;
						count[int(browngreen_svm.predict(sampleMat))]++;
						count[int(yellowgreen_svm.predict(sampleMat))]++;
						colorcount[max(count)]++;
						continue;
					}
					else{
						count[int(redbrown_svm.predict(sampleMat))]++;
						count[int(redyellow_svm.predict(sampleMat))]++;
						count[int(redgreen_svm.predict(sampleMat))]++;
						count[int(redpink_svm.predict(sampleMat))]++;
						count[int(redgray_svm.predict(sampleMat))]++;
						count[int(brownyellow_svm.predict(sampleMat))]++;
						count[int(browngreen_svm.predict(sampleMat))]++;
						count[int(brownpink_svm.predict(sampleMat))]++;
						count[int(browngray_svm.predict(sampleMat))]++;
						count[int(yellowgreen_svm.predict(sampleMat))]++;
						count[int(yellowpink_svm.predict(sampleMat))]++;
						count[int(yellowgray_svm.predict(sampleMat))]++;
						count[int(greenpink_svm.predict(sampleMat))]++;
						count[int(greengray_svm.predict(sampleMat))]++;
						count[int(pinkgray_svm.predict(sampleMat))]++;
						count[int(whitered_svm.predict(sampleMat))]++;
						count[int(whitebrown_svm.predict(sampleMat))]++;
						count[int(whiteyellow_svm.predict(sampleMat))]++;
						count[int(whitegreen_svm.predict(sampleMat))]++;
						count[int(whitepink_svm.predict(sampleMat))]++;
						count[int(whitegray_svm.predict(sampleMat))]++;
						count[int(whiteblack_svm.predict(sampleMat))]++;
						count[int(blackred_svm.predict(sampleMat))]++;
						count[int(blackbrown_svm.predict(sampleMat))]++;
						count[int(blackyellow_svm.predict(sampleMat))]++;
						count[int(blackgreen_svm.predict(sampleMat))]++;
						count[int(blackpink_svm.predict(sampleMat))]++;
						count[int(blackgray_svm.predict(sampleMat))]++;
						colorcount[max(count)]++;
						continue;
					}
				}
				if((h>=30)&&(h<60)){
					count[int(yellowgreen_svm.predict(sampleMat))]++;
					count[int(yellowgray_svm.predict(sampleMat))]++;
					count[int(greengray_svm.predict(sampleMat))]++;
					count[int(whiteyellow_svm.predict(sampleMat))]++;
					count[int(whitegreen_svm.predict(sampleMat))]++;
					count[int(whitegray_svm.predict(sampleMat))]++;
					count[int(whiteblack_svm.predict(sampleMat))]++;
					count[int(blackyellow_svm.predict(sampleMat))]++;
					count[int(blackgreen_svm.predict(sampleMat))]++;
					count[int(blackgray_svm.predict(sampleMat))]++;
					colorcount[max(count)]++;
					continue;
				}
				if((h>=60)&&(h<90)){
					count[int(greenblue_svm.predict(sampleMat))]++;
					count[int(greengray_svm.predict(sampleMat))]++;
					count[int(bluegray_svm.predict(sampleMat))]++;
					count[int(whitegreen_svm.predict(sampleMat))]++;
					count[int(whiteblue_svm.predict(sampleMat))]++;
					count[int(whitegray_svm.predict(sampleMat))]++;
					count[int(whiteblack_svm.predict(sampleMat))]++;
					count[int(blackblue_svm.predict(sampleMat))]++;
					count[int(blackgreen_svm.predict(sampleMat))]++;
					count[int(blackgray_svm.predict(sampleMat))]++;
					colorcount[max(count)]++;
					continue;
				}
				if((h>=90)&&(h<120)){
					count[int(greengray_svm.predict(sampleMat))]++;
					count[int(greenblue_svm.predict(sampleMat))]++;
					count[int(greenpurple_svm.predict(sampleMat))]++;
					count[int(purpleblue_svm.predict(sampleMat))]++;
					count[int(purplegray_svm.predict(sampleMat))]++;
					count[int(bluegray_svm.predict(sampleMat))]++;
					count[int(whitegreen_svm.predict(sampleMat))]++;
					count[int(whiteblue_svm.predict(sampleMat))]++;
					count[int(whitepurple_svm.predict(sampleMat))]++;
					count[int(whitegray_svm.predict(sampleMat))]++;
					count[int(whiteblack_svm.predict(sampleMat))]++;
					count[int(blackgreen_svm.predict(sampleMat))]++;
					count[int(blackblue_svm.predict(sampleMat))]++;
					count[int(blackpurple_svm.predict(sampleMat))]++;
					count[int(blackgray_svm.predict(sampleMat))]++;
					colorcount[max(count)]++;
					continue;
				}
				if((h>=120)&&(h<150)){
					count[int(purpleblue_svm.predict(sampleMat))]++;
					count[int(purplegray_svm.predict(sampleMat))]++;
					count[int(bluegray_svm.predict(sampleMat))]++;
					count[int(whiteblue_svm.predict(sampleMat))]++;
					count[int(whitepurple_svm.predict(sampleMat))]++;
					count[int(whitegray_svm.predict(sampleMat))]++;
					count[int(whiteblack_svm.predict(sampleMat))]++;
					count[int(blackblue_svm.predict(sampleMat))]++;
					count[int(blackpurple_svm.predict(sampleMat))]++;
					count[int(blackgray_svm.predict(sampleMat))]++;
					colorcount[max(count)]++;
					continue;
				}
				if((h>=120)&&(h<=180)){
					count[int(redbrown_svm.predict(sampleMat))]++;
					count[int(redpink_svm.predict(sampleMat))]++;
					count[int(redblue_svm.predict(sampleMat))]++;
					count[int(redgray_svm.predict(sampleMat))]++;
					count[int(redpurple_svm.predict(sampleMat))]++;
					count[int(brownpink_svm.predict(sampleMat))]++;
					count[int(brownblue_svm.predict(sampleMat))]++;
					count[int(browngray_svm.predict(sampleMat))]++;
					count[int(brownpurple_svm.predict(sampleMat))]++;
					count[int(purpleblue_svm.predict(sampleMat))]++;
					count[int(purplegray_svm.predict(sampleMat))]++;
					count[int(purplepink_svm.predict(sampleMat))]++;
					count[int(pinkblue_svm.predict(sampleMat))]++;
					count[int(pinkgray_svm.predict(sampleMat))]++;
					count[int(bluegray_svm.predict(sampleMat))]++;
					count[int(whitered_svm.predict(sampleMat))]++;
					count[int(whitebrown_svm.predict(sampleMat))]++;
					count[int(whitepink_svm.predict(sampleMat))]++;
					count[int(whiteblue_svm.predict(sampleMat))]++;
					count[int(whitepurple_svm.predict(sampleMat))]++;
					count[int(whitegray_svm.predict(sampleMat))]++;
					count[int(whiteblack_svm.predict(sampleMat))]++;
					count[int(blackred_svm.predict(sampleMat))]++;
					count[int(blackbrown_svm.predict(sampleMat))]++;
					count[int(blackpink_svm.predict(sampleMat))]++;
					count[int(blackblue_svm.predict(sampleMat))]++;
					count[int(blackpurple_svm.predict(sampleMat))]++;
					count[int(blackgray_svm.predict(sampleMat))]++;
					colorcount[max(count)]++;
					continue;
				}
			}
		}
	}
	switch(max(colorcount)){
		case 1:
			Color="白色";
			break;
		case 2:
			Color="灰色";
			break;
		case 3:
			Color="黄色";
			break;
		case 4:
			Color="粉色";
			break;
		case 5:
			Color="红色";
			break;
		case 6:
			Color="紫色";
			break;
		case 7:
			Color="绿色";
			break;
		case 8:
			Color="蓝色";
			break;
		case 9:
			Color="棕色";
			break;
		case 10:
			Color="黑色";
			break;
	}
}
int CImageProcess::max(int count[],int num){//找出1—10中最大的下标，0号单元做哨兵
	int max_lable=1;
	count[0]=count[1];
	for(int i=1;i<=num;i++)
		if(count[i]>count[0]){
			count[0]=count[i];
			max_lable=i;
		}
	return max_lable;
}
void CImageProcess::NoBackgroudImageExtraction(IplImage *curFrame,IplImage *foregroundImg)
{
	CvScalar curScalar;
	uchar *foreData=(uchar *)foregroundImg->imageData;
	int h=curFrame->height;
	int step=foregroundImg->widthStep;
	cvZero(noBgImg);
	cvNot(noBgImg,noBgImg);
	for(int i=0;i<w;i++)
		for(int j=0;j<h;j++)
		{			
			if(!foreData[j*step+i])
			{
				curScalar=cvGet2D(curFrame,j,i);
				cvSet2D(noBgImg,j,i,curScalar);
			}
		}
}
BOOL CImageProcess::GetHistogram(unsigned char *pImageData,int nWidth,int nHeight,int nWidthStep,int *pHistogram)
{   
   int            i     = 0;   
   int            j     = 0;   
   unsigned char *pLine = NULL;   // 清空直方图      
   memset(pHistogram, 0, sizeof(int) * 256);   
   for (pLine = pImageData, j = 0; j < nHeight; j++, pLine += nWidthStep)   
   {   
        for (i = 0; i < nWidth; i++)   
       {   
            pHistogram[pLine[i]]++;   
        }   
  }   
    return TRUE;   
}   

int CImageProcess::Otsu(unsigned char *pImageData, int nWidth, int nHeight, int nWidthStep)
{
	int    i          = 0;   
	int    j          = 0;   
	int    nTotal     = 0;   
    int    nSum       = 0;   
    int    A          = 0;   
    int    B          = 0;   
    double u          = 0;   
    double v          = 0;   
	double dVariance  = 0;   

	double dMaximum   = 0;   
    int    nThreshold = 0;   
    int    nHistogram[256];   
    // 获取直方图   
    GetHistogram(pImageData, nWidth, nHeight, nWidthStep, nHistogram);   
    for (i = 0; i < 256; i++)   
    {   
        nTotal += nHistogram[i];   
        nSum   += (nHistogram[i] * i);   
    }   
   for (j = 0; j < 256; j++)   
    {   
        A = 0;   
       B = 0;   
       for (i = 0; i < j; i++)   
        {   
           A += nHistogram[i];   
            B += (nHistogram[i] * i);   
      }   
       if (A > 0)   
       {   
            u = B / A;   
        }   
        else  
        {   
           u = 0;   
       }   
       if (nTotal - A > 0)    
        {   
            v = (nSum - B) / (nTotal - A);   
      }   
       else  
        {   
            v = 0;   
       }   
       dVariance = A * (nTotal - A) * (u - v) * (u - v);   
        if (dVariance > dMaximum)   
       {   
           dMaximum = dVariance;   
         nThreshold = j;   
       }   
    }   
    return nThreshold;   

}
void CImageProcess::Laplace(IplImage *src,IplImage *workImg)
{
	IplImage * laplace = NULL;
    IplImage * plane = NULL;
	plane= cvCreateImage(cvSize(src->width,src->height),IPL_DEPTH_8U,1);
    laplace = cvCreateImage(cvSize(src->width,src->height),IPL_DEPTH_16S,1);

	cvCvtPixToPlane(src,plane,0,0,0);
	cvLaplace(plane,laplace,3);
    //使用线性变换转换输入函数元素成为无符号整形
	cvConvertScaleAbs(laplace,plane,1,0);

	cvCvtPlaneToPix(plane,0,0,0,workImg);
	//结构相同（- 顶—左结构,1 - 底—左结构）
	workImg->origin = src->origin;
	cvReleaseImage(&plane);
	cvReleaseImage(&laplace);
}
IplImage* CImageProcess::Robert( IplImage *src, int lWidth, int lHeight )
{
	 // the index of loop
   int x, y;

   // the value of pixel
   double result;
   uchar pixel[4];

   IplImage *dst = 0;
   dst = cvCreateImage( cvSize( src->width, src->height ), src->depth, src->nChannels );
   cvSet( dst,cvScalarAll( 255 ), 0 ); 

   for( y = 0; y < lHeight - 1; ++y )
   {
      for( x = 0; x < lWidth - 1; ++x )
      {
         pixel[0] = ((uchar*)( src->imageData + src->widthStep * y ))[ x ];
         pixel[1] = ((uchar*)( src->imageData + src->widthStep * y ))[ x + 1 ];
         pixel[2] = ((uchar*)( src->imageData + src->widthStep * ( y + 1 ) ))[ x ];
         pixel[3] = ((uchar*)( src->imageData + src->widthStep * ( y + 1 ) ))[ x + 1 ];

         // calculate the value of the pixel
         result = sqrt((double)(( pixel[0] - pixel[3] )*( pixel[0] - pixel[3] )) + (double)(( pixel[1] - pixel[2] )*( pixel[1] - pixel[2] )));
         // *lpDst = (unsigned char)result;
         ((uchar*)( dst->imageData + dst->widthStep * y ))[x] = (uchar)result;
      
      }
   }
   return dst;
}
int CImageProcess::OSTUthred(CvHistogram * MyHistogram)
{
	float histArray[256];
    for(int j=0;j<HistogramBins;j++)
	{
		histArray[j]=cvQueryHistValue_1D(MyHistogram,j);
	}
    histArray[0]=1;
    float u0,ul;//cO和c1组的均值
    float w0,wl;//cO和c1组产生的概率
    int count0;//cO组的像素数
    int t,maxT;//阈值t及记录方差最大时的最佳阈值maxT
    float devi,maxDevi=0;//方差及最大方差
    int i;
    int sum=0;//统计直方图中像素的个数，存放在sum中
    for(i=0;i<256;i++)
		sum = sum+histArray[i];
	for(t=-0;t<255;t++)
	{
		//计算阈值为t时，c0组的均值和产生的概率
		u0=0;
		count0=0;
        for(i=0;i<=t;i++)
		{
			u0+=i*histArray[i];
			count0+=histArray[i];
		}
		u0=u0/count0;
        w0=(float)count0/sum;
		//计算阈值为t时，c1组的均值和产生的概率
        ul=0;
		for(i=t+1;i<256;i++)
			ul+=i*histArray[i];
        //C0组像素数与C1组像素数之和为图像总像素数。
        ul=ul/(sum-count0);
        wl=1-w0;
        devi=w0*wl*(ul-u0)*(ul-u0);
		if(devi>maxDevi)//最大的方差及最佳阈值位置
		{
			maxDevi=devi;
			maxT=t;
		}
	}
	//返回最佳阈值
	return maxT;
}

int CImageProcess::itos(int num,char *str,int cur)
{
	int des=0;
	 if(num>9)
	 {
		 des=itos(num/10,str,cur+1);
		 str[des]=(num%10)+'0';
		 str[++des]='\0';
		 return des;
	 }else
	 {
		 str[des]=num+'0';
		 str[++des]='\0';
		 return des;
	 }
}

/**
*将rgb色彩空间转化为hsv色彩空间，通过与背景比较比较去除阴影
*/
void CImageProcess::rgb2hsv(const double R,const double G,const double B,double &H,double &S,double &V)
{

	double max,min;	
	max=R>G?R:G;
	max=max>B?max:B;
	min=R>G?G:R;
	min=min>B?B:min;
	if(max==min)
		H=0;
	else{
		if(max==R){
			H=60*(G-B)/(max-min);
			if(G<B)
				H+=360;
			}
		else if(max==G)
			H=60*(B-R)/(max-min)+120;
		else 
			H=60*(R-B)/(max-min)+240;	
	}
	S=0;
	if(max) 
		S+=1-min/max;
	V=max;	
}
void CImageProcess::ShadowRemove(double &R,double &G,double &B,const double bR,const double bG, const double bB)
{
	/**
	*将rgb色彩空间转化为hsv色彩空间，通过与背景比较比较去除阴影
	*/
	double H,S,V;
	double bH,bS,bV;
	double Vr,Ss,Hs;
	rgb2hsv(R,G,B,H,S,V);
	rgb2hsv(bR,bG,bB,bH,bS,bV);
	Vr=V/bV;
	Ss=fabs(S-bS);
	Hs=fabs(H-bH);

	if(Hs>=10&&Hs<=500&&Vr>=0.475)
		return;
	B=G=R=0;
	
}
void CImageProcess::ForegroundImageExtraction(IplImage *curFrame,IplImage*BackgroundImg,IplImage *foregroundImg)
{
	/**
	*输入为RGB当前帧图像，和RGB背景图像，输出为灰度前景图像
	*当前帧与背景图相差，差别大于阈值为前景图形，小于阈值的为背景图像，并按一定比率更新背景图像
	
	*/
	IplConvKernel* element = cvCreateStructuringElementEx(4,4,1,0,CV_SHAPE_RECT);
	IplImage *grayBkImg = cvCreateImage(cvGetSize(BackgroundImg),  IPL_DEPTH_8U,1);
	IplImage *grayCurFrame=cvCreateImage(cvGetSize(curFrame),IPL_DEPTH_8U,1);
	cvCvtColor(BackgroundImg,grayBkImg, CV_BGR2GRAY);
	cvCvtColor(curFrame,grayCurFrame,CV_BGR2GRAY);		
	cvAbsDiff(grayCurFrame, grayBkImg,foregroundImg);
		
	cvReleaseImage(&grayBkImg);
	cvReleaseImage(&grayCurFrame);

}
void CImageProcess::Sharpen(IplImage *OriginalImg,IplImage *SharpenImg)
{	//输入输出均为8位无符单通道图像
	IplImage *tempImg;
	tempImg=cvCreateImage(cvGetSize(OriginalImg),16,1);
	cvSobel(OriginalImg,tempImg,1,0,3);
	cvConvertScaleAbs(tempImg,SharpenImg);
}
void CImageProcess::BinaryZation(IplImage *OriginalImg,IplImage *BinaryImg)//输入图像为8位单通道灰度图像，输出为二值图像
{	//采用大津法获得阈值
	IplImage *tempImg;
	tempImg=cvCreateImage(cvGetSize(OriginalImg),8,1);
	unsigned int threshold = Otsu((unsigned char *)OriginalImg->imageData,OriginalImg->width,OriginalImg->height,OriginalImg->widthStep);		
	cvThreshold(OriginalImg,tempImg,threshold,255,CV_THRESH_BINARY);
	cvCopy(tempImg,BinaryImg);
}
void CImageProcess::ConvertGray(IplImage *OriginalImg,IplImage *GrayImg)
{
	IplImage *tempImg;		
	tempImg=cvCreateImage(cvGetSize(OriginalImg),8,1);
	cvCvtColor(OriginalImg,tempImg,CV_BGR2GRAY); 
	cvCopy(tempImg,GrayImg);
}
int CImageProcess::CarContursExtraction( IplImage *OriginalImg,IplImage **CarConturs)
{	//输入图像为处理后的二值化图像，第二参数为一个图形数组的引用
	int i=0;
	double tmpArea;
	cvWaitKey(1000);
	IplImage *temp=cvCreateImage(cvGetSize(OriginalImg),OriginalImg->depth,OriginalImg->nChannels);
	cvCopy(OriginalImg,temp);
	cvNot(temp,temp);
	CvMemStorage* stor=cvCreateMemStorage(0);
	CvSeq * cont=cvCreateSeq(CV_SEQ_ELTYPE_POINT,sizeof(CvSeq),sizeof(CvPoint),stor);
	cvFindContours(temp,stor,&cont,sizeof(CvContour),CV_RETR_TREE ,CV_CHAIN_APPROX_SIMPLE,cvPoint(0,0));	
	for(;cont;cont=cont->h_next)
	{			
		tmpArea=fabs(cvContourArea(cont,CV_WHOLE_SEQ));
		if (tmpArea>4000)
		{
			CarConturs[i]= cvCreateImage( cvGetSize(OriginalImg),8, 3);
			cvThreshold(CarConturs[i],CarConturs[i],0, 255, CV_THRESH_BINARY ); //在画轮廓前先把图像变成白色。 threshold=0，pixel>0,pixel = 0.
			cvDrawContours(CarConturs[i],cont,CV_RGB(0,0,0),CV_RGB(0,0,0),0,-1,4,cvPoint(0,0));//在图像上绘制外部和内部轮廓.
			i++;
			//【图像,第一个轮廓指针，外轮廓的颜色，内轮廓的颜色，画轮廓的最大层数（如果是0，只绘制contour），线条粗细，线条类型，按给定值移动所有点的坐标 】
		}
	}
	cvReleaseImage(&temp);
	return i;
}
void CImageProcess::SideFaceStandardization(IplImage *OriginalImg,IplImage *StdImg)
{
	double Dx=0;
	double Dy=0;
	double Dxy=0;
	double q=0;
	double ConturCenter_X=0;
	double ConturCenter_Y=0;
	int PixelNumber=0;
	int height=OriginalImg->height;
	int width=OriginalImg->width;
	int step=OriginalImg->widthStep;
	CvPoint2D32f center;
	CvMat* rot_mat=cvCreateMat(2,3,CV_32FC1);	
	uchar *OriginalData=(uchar *)OriginalImg->imageData;
	IplImage *temp=cvCreateImage(cvGetSize(OriginalImg),OriginalImg->depth,OriginalImg->nChannels);
	for(int i=0;i<height;i++)
	{
		for(int j=0;j<width;j++){
			if(!OriginalData[i*step+j])
			{	//白色是255，黑色是0
				PixelNumber++;
				ConturCenter_Y+=i;
				ConturCenter_X+=j;				
			}
		}
	}
	ConturCenter_X/=PixelNumber;
	ConturCenter_Y/=PixelNumber;
	for(int i=0;i<height;i++)
	{
		for(int j=0;j<width;j++)
			if(!OriginalData[i*step+j])
			{	//白色是255，黑色是0
				Dx+=(j-ConturCenter_X)*(j-ConturCenter_X);
				Dy+=(i-ConturCenter_Y)*(i-ConturCenter_Y);
				Dxy+=(i-ConturCenter_Y)*(j-ConturCenter_X);		
			}
	}
	Dx/=PixelNumber;
	Dy/=PixelNumber;
	Dxy=PixelNumber;
	q=atanl(2*Dxy/(Dx-Dy))/2.0;
	if(Dxy<0)
		q+=1.57;
	center.x=ConturCenter_X;
	center.y=ConturCenter_Y;
	cv2DRotationMatrix(center,q,1,rot_mat);
	cvWarpAffine(OriginalImg,temp,rot_mat);
	cvCopy(temp,StdImg);
}
double CImageProcess::SpaceOccupancyExtraction(IplImage *StdImg)
{
	double ConturArea;
	double RectArea;
	CvBox2D rect;
	CvMemStorage* stor=cvCreateMemStorage(0);
	IplImage *temp=cvCreateImage(cvGetSize(StdImg),StdImg->depth,StdImg->nChannels);
	cvCopy(StdImg,temp);
	cvNot(temp,temp);
	CvSeq * cont=cvCreateSeq(CV_SEQ_ELTYPE_POINT,sizeof(CvSeq),sizeof(CvPoint),stor);
	cvFindContours(temp,stor,&cont,sizeof(CvContour),CV_RETR_TREE,CV_CHAIN_APPROX_SIMPLE,cvPoint(0,0));
	ConturArea=fabs(cvContourArea(cont,CV_WHOLE_SEQ));	
	rect= cvMinAreaRect2(cont);
	RectArea=rect.size.width*rect.size.height;
	cvReleaseImage(&temp);
	return ConturArea/RectArea;
}
double CImageProcess::LengthHeightRatioExtration(IplImage *StdImg)
{
	CvBox2D rect;
	IplImage *temp=cvCreateImage(cvGetSize(StdImg),StdImg->depth,StdImg->nChannels);
	cvCopy(StdImg,temp);
	cvNot(temp,temp);
	CvMemStorage* stor=cvCreateMemStorage(0);
	CvSeq * cont=cvCreateSeq(CV_SEQ_ELTYPE_POINT,sizeof(CvSeq),sizeof(CvPoint),stor);
	cvFindContours(temp,stor,&cont,sizeof(CvContour),CV_RETR_TREE ,CV_CHAIN_APPROX_SIMPLE,cvPoint(0,0));	
	rect= cvMinAreaRect2(cont);
	cvReleaseImage(&temp);
	return rect.size.width/rect.size.height;
}
double CImageProcess::TailCarHeightRatioExtration(IplImage *StdImg)
{	//tailHeight可能小于实际值，
	int TailSign=0;
	int PixelLine=1;//标记当前是第几个有像素的列
	int tmpTail=-100;
	int mintop=1000,maxbottom=0;
	double carHeight=0;
	double tailHeight=0;
	int topPixel=0,bottomPixel=0;
	int PixelNumber=0;
	int height=StdImg->height;
	int width=StdImg->width;
	int step=StdImg->widthStep;
	char *StdData=StdImg->imageData;
	for(int j=width-1;j>=0;j--)		
	{
		PixelNumber=0;
		for(int i=0;i<height;i++)	
		{		
			if(!StdData[i*step+j])
			{	//白色是255，黑色是0
				if(!PixelNumber) topPixel=i;
				PixelNumber++;				
				bottomPixel=i;
			}
		}
		if(bottomPixel-topPixel)
		{
			TailSign++;
			if(TailSign==PixelLine)
			{
				if(abs(tmpTail-bottomPixel+topPixel)<2)
					tailHeight=tmpTail;
				else
				{
					tmpTail=bottomPixel-topPixel;
					PixelLine+=2;
				}
			}
			if(bottomPixel>maxbottom)	
				maxbottom=bottomPixel;
			if(mintop>topPixel)
				mintop=topPixel;
		}
			
	}
	carHeight=maxbottom-mintop;
	return tailHeight*1.0/carHeight;	
}
int CImageProcess::AxleNumberExtraion(IplImage *StdImg)
{
	//统计思想：一般而言，车轮处侧面轮廓的每一行上的像素点大于两个此处根据像素点的变化统计车轮数
	int axleNumber=0;
	int prePixelNumber=0;
	int PixelNumber=0;
	int height=StdImg->height;
	int width=StdImg->width;
	int step=StdImg->widthStep;
	char *StdData=StdImg->imageData;
	IplImage *temp=cvCreateImage(cvGetSize(StdImg),StdImg->depth,StdImg->nChannels);
	for(int j=0;j<width;j++)
	{
		PixelNumber=0;
		for(int i=0;i<height;i++)
		{		
			if(!StdData[i*step+j])
			{	//白色是255，黑色是0
				PixelNumber++;
			}
		}
		if(PixelNumber>prePixelNumber)
		{
			prePixelNumber=PixelNumber;
		}
		else
		{
			for(;prePixelNumber-PixelNumber>1;prePixelNumber-=2)
				axleNumber++;
		}
	}
	return axleNumber;
}
double CImageProcess::AxleDistanceCarLengthRatioExtration(IplImage *StdImg)
{
	double fontWheelPosition=0;
	double backWheelPosition=0;
	double axleDistance;
	double carLength;
	int PixelNumber=0;
	int height=StdImg->height;
	int width=StdImg->width;
	int step=StdImg->widthStep;
	char *StdData=StdImg->imageData;
	CvBox2D rect;
	rect= cvMinAreaRect2(StdImg);
	carLength=rect.size.width;
	for(int i=0;i<height;i++)	
	{
		PixelNumber=0;
		for(int j=0;j<width;j++)
		{		
			if(!StdData[i*step+j])
			{	//白色是255，黑色是0
				PixelNumber++;
			}
		}
		if(PixelNumber==4)
		{
			PixelNumber=0;
			for(int j=0;j<width;j++)
			{		
				if(!StdData[i*step+j])
				{	//白色是255，黑色是0
					PixelNumber++;
					if(PixelNumber<=2)
						fontWheelPosition+=j;
					else
						backWheelPosition+=j;
				}
			}
			break;
		}		
	}
	axleDistance=fabs(fontWheelPosition/2-backWheelPosition/2);
	return axleDistance/carLength;
}
void CImageProcess::CarTypeJudge(double Occupancy,double LengthHeightRatio,double TailCarHeightRatio)
{
	if(Occupancy>=0.78)
	{//客车
		if(LengthHeightRatio>=3)
		{
			CarType="大客车";
			return;
		}
		else
		{
			CarType="中小客车";
			return;
		}
	}
	if(Occupancy>0.4)
	{
		if(TailCarHeightRatio<0.3)
		{
			CarType="无法识别";
			return;
		}
		if(TailCarHeightRatio<0.55)
		{//轿车
			CarType="轿车";
			return;

		}
		if(TailCarHeightRatio<=0.60)
		{
			CarType="越野车";
			return;
		}
		if(TailCarHeightRatio<0.75)
		{
			CarType="货车";
			return ;
		}
		CarType="无法识别";
		return;
	}
	CarType="无法识别";
	return;
}
void CImageProcess::BackgroundInit(char * argv)
{
	CInitDialog *initDialog= new CInitDialog();
	initDialog->Create(IDD_INITIONG,NULL);
	initDialog->ShowWindow(SW_SHOW);
	IplImage* pFrame = NULL;
	IplImage * pFrimg=NULL;
	IplImage * temp32 = NULL;
	IplImage * preImg = NULL;
	IplImage * curImg = NULL;
	CvMat* pFrMat = NULL;
	int nFrmNum = 0;
	int time=1;
	int sum=0;
	int flag=1;
	int update=0;
	int m=1,t=0;
	int h;
	int ImgNum = 0,preNum = 1;
	double T=3.68;
	CvCapture* pCapture = NULL;
    //打开视频文件
    if(!(pCapture = cvCaptureFromFile(argv)))
        {
        	fprintf(stderr, "Can not open video file %s\n", argv);
	        exit(0);
        }
	initDialog->m_initProgress.SetRange(0,51);
	initDialog->m_initProgress.SetPos(0);
	while(pFrame = cvQueryFrame( pCapture ))
	{
		nFrmNum++;
	    if(nFrmNum == 1)
		{
			w = pFrame ->width ;
			h = pFrame ->height;
			num = new int*[w];    
			for (int i = 0; i != w; i++)
			{
				num[i] = new int[h];
				for(int j = 0; j != h;j++)
					num[i][j] = 0;
		    }
			temp32 = cvCreateImage(cvSize(w, h),  IPL_DEPTH_32S, 3);
			cvZero(temp32);
			curImg = cvCreateImage(cvSize(w, h),  IPL_DEPTH_8U, 3);
	    }
		if(nFrmNum == 1)
		{
		    pFrMat = cvCreateMat(pFrame->height, pFrame->width, CV_32FC3);
			pFrimg = cvCloneImage(pFrame);
		   	cvConvert(pFrimg, pFrMat);
	        cvResize(pFrimg, curImg, CV_INTER_LINEAR);
			preImg = cvCloneImage(curImg);
			cvReleaseImage(&pFrimg);
	        cvReleaseMat(&pFrMat);
			ImgNum++;
		}
		else
		{
			if(nFrmNum-preNum==2)
			{
			    pFrMat = cvCreateMat(pFrame->height, pFrame->width, CV_32FC3);
				pFrimg = cvCloneImage(pFrame);
		     	cvConvert(pFrimg, pFrMat);
	            cvResize(pFrimg, curImg, CV_INTER_LINEAR);
				preNum = nFrmNum;
				ImgNum++;
				t=1;
				cvReleaseImage(&pFrimg);
	            cvReleaseMat(&pFrMat);
			}
		}
		if(t)
		{
			CvScalar s1,s2,s3;
			for(int i=0;i<w;i++)
				for(int j=0;j<h;j++)
				{
					s1=cvGet2D(curImg,j,i);
					s2=cvGet2D(preImg,j,i);
					s3=cvGet2D(temp32,j,i);
					if(abs(s1.val[0]-s2.val[0])<T&&abs(s1.val[1]-s2.val[1])<T&&abs(s1.val[2]-s2.val[2])<T)
					{
						s3.val[0]+=s2.val[0];
						s3.val[1]+=s2.val[1];
						s3.val[2]+=s2.val[2];
						cvSet2D(temp32,j,i,s3);
						num[i][j]++;
					}
					else 
					{
						s3.val[0]+=0;
						s3.val[1]+=0;
						s3.val[2]+=0;
					    cvSet2D(temp32,j,i,s3);
					}
				}
			cvReleaseImage(&preImg);
			preImg = cvCloneImage(curImg);
			t=0;
		}
		initDialog->m_initProgress.SetPos(ImgNum);
		if(ImgNum==50) break;
	}   
	CvScalar s,s1;s.val[0]=0;s.val[1]=0;s.val[2]=0;
	BackgroundImg = cvCreateImage(cvSize(w,h),  IPL_DEPTH_8U,3);
	noBgImg=cvCreateImage(cvSize(w,h),IPL_DEPTH_8U,3);
	for(int i=0;i<BackgroundImg->width;i++)
		for(int j=0;j<BackgroundImg->height;j++)
		{
			s1=cvGet2D(temp32,j,i);
		    s.val[0]=s1.val[0]/ num[i][j];
			s.val[1]=s1.val[1]/ num[i][j];
			s.val[2]=s1.val[2]/ num[i][j];
		    cvSet2D(BackgroundImg,j,i,s);
		}
	initDialog->m_initProgress.SetPos(51);
	cvReleaseCapture(&pCapture);
	cvReleaseImage(&preImg);
	cvReleaseImage(&curImg);
	cvReleaseImage(&temp32);
	initDialog->DestroyWindow();

}
void CImageProcess::BackgroundUpdate(IplImage *curFrame,IplImage *preFrame)
{
	double T=3.68;
	CvScalar curScalar,bgScalar;
	IplImage *grayCur=cvCreateImage(cvGetSize(curFrame),IPL_DEPTH_8U,1);
	IplImage *grayPre=cvCreateImage(cvGetSize(curFrame),IPL_DEPTH_8U,1);
	ConvertGray(curFrame,grayCur);
	ConvertGray(preFrame,grayPre);
	uchar *curData=(uchar *)grayCur->imageData;
	uchar *preData=(uchar *)grayPre->imageData;
	int h=curFrame->height;
	int step=grayCur->widthStep;
	for(int i=0;i<w;i++)
		for(int j=0;j<h;j++)
		{
			if(num[i][j]>0)
				num[i][j]--;
			if(abs(curData[j*step+i]-preData[j*step+i])<T)
			{
				num[i][j]++;
				curScalar=cvGet2D(curFrame,j,i);
				bgScalar=cvGet2D(BackgroundImg,j,i);
				bgScalar.val[0]=(curScalar.val[0]+bgScalar.val[0]*(num[i][j]-1))/num[i][j];
				bgScalar.val[1]=(curScalar.val[1]+bgScalar.val[1]*(num[i][j]-1))/num[i][j];
				bgScalar.val[2]=(curScalar.val[2]+bgScalar.val[2]*(num[i][j]-1))/num[i][j];
				cvSet2D(BackgroundImg,j,i,bgScalar);
			}
		}
}
bool CImageProcess::IsNoise(IplImage *binaryImg,int x,int y,int row,int col)
{
	bool noiseSign=false;
	bool cornerSign=false;
	bool fullLine=false;
	bool conexSign=false;
	uchar *binaryData=(uchar *)binaryImg->imageData;
	int step=binaryImg->widthStep;
	int tmpPixels=0;
	int colPixels=0;
	int rowPixels=0;
	int totalPixels=0;
	for(int i=x;i<x+row;i+=row-1)
	{
		for(int j=y;j<y+col;j++)
			if(!binaryData[i*step+j])
				tmpPixels++;
		if(tmpPixels>rowPixels)
			rowPixels=tmpPixels;
		tmpPixels=0;
	}
	tmpPixels=0;
	for(int j=y;j<y+col;j+=col-1)
	{
		for(int i=x;i<x+row;i++)
			if(!binaryData[i*step+j])
				tmpPixels++;
		if(tmpPixels>colPixels)
			colPixels=tmpPixels;
		tmpPixels=0;
	}
	for(int i=x;i<x+row;i++)
		for(int j=y;j<y+col;j++)
			if(!binaryData[i*step+j])
				totalPixels++;
	if(!(binaryData[x*step+y]&&binaryData[(x+1)*step+y]&&binaryData[(x+2)*step+y]&&binaryData[x*step+y+1]&&binaryData[x*step+y+2]))
		cornerSign=true;
	if(!(binaryData[(x+row-1)*step+y]&&binaryData[((x+row-1)-1)*step+y]&&binaryData[((x+row-1)-2)*step+y]&&binaryData[(x+row-1)*step+y+1]&&binaryData[(x+row-1)*step+y+2]))
		cornerSign=true;
	if(!(binaryData[x*step+(y+col-1)]&&binaryData[(x+1)*step+(y+col-1)]&&binaryData[(x+2)*step+(y+col-1)]&&binaryData[x*step+(y+col-1)-1]&&binaryData[x*step+(y+col-1)-2]))
		cornerSign=true;
	if(!(binaryData[(x+row-1)*step+(y+col-1)]&&binaryData[(x+row-2)*step+(y+col-1)]&&binaryData[(x+row-3)*step+(y+col-1)]&&binaryData[(x+row-1)*step+(y+col-1)-1]&&binaryData[(x+row-1)*step+(y+col-1)-2]))
		cornerSign=true;
	if(row==rowPixels||col==colPixels)
		fullLine=true;
	if(!fullLine&&!cornerSign)
	{
		if(binaryData[x*step+y]&&binaryData[(x+1)*step+y]&&binaryData[(x+row-1)*step+y]&&binaryData[(x+row-2)*step+y])
			conexSign=true;		
		if(binaryData[x*step+(y+col-1)]&&binaryData[(x+1)*step+(y+col-1)]&&binaryData[(x+row-1)*step+(y+col-1)]&&binaryData[(x+row-2)*step+(y+col-1)])
			conexSign=true;
		if(binaryData[x*step+y]&&binaryData[x*step+y+1]&&binaryData[x*step+(y+col-1)]&&binaryData[x*step+(y+col-2)])
			conexSign=true;
		if(binaryData[(x+row-1)*step+y]&&binaryData[(x+row-1)*step+y+1]&&binaryData[(x+row-1)*step+y+col-1]&&binaryData[(x+row-1)*step+y+col-2])
			conexSign=true;
	}
	if(fullLine||cornerSign)
		return false;
	if(colPixels*rowPixels<totalPixels)
		return true;
	return conexSign;
}
void CImageProcess::RemoveNoise(IplImage*binaryImg,int row,int col,int Direction)
{
	int height=binaryImg->height;
	int width=binaryImg->width;
	int step=binaryImg->widthStep;
	uchar *binaryData=(uchar *)binaryImg->imageData;
	if(Direction)
	{//水平
		for(int j=0;j<height-row;j++)
			for(int i=0;i<width-col;i++)			
			{
				if(IsNoise(binaryImg,j,i,row,col))
				{
					for(int k=j;k<(j+row);k++)
						for(int is=i;is<(i+col);is++)						
							binaryData[k*step+is]=255;
				}
			}
	}else
	{//竖直
		for(int i=0;i<width-col;i++)
			for(int j=0;j<height-row;j++)						
			{
				if(IsNoise(binaryImg,j,i,row,col))
				{
					for(int k=j;k<(j+row);k++)
						for(int is=i;is<(i+col);is++)						
							binaryData[k*step+is]=255;
				}
			}
	}
}
void CImageProcess::LightUnitizing(IplImage *OriginalImg,IplImage *LightUnitizingImg)
{
	CvScalar HsvData;
	int height=OriginalImg->height;
	int width=OriginalImg->width;
	int step=OriginalImg->widthStep;
	IplImage *HsvImg=cvCreateImage(cvGetSize(OriginalImg),OriginalImg->depth,OriginalImg->nChannels);
	cvCvtColor(OriginalImg,HsvImg,CV_BGR2HSV);
	for(int i=0;i<height;i++)
	{
		for(int j=0;j<width;j++)
		{
			HsvData=cvGet2D(HsvImg,i,j);
			HsvData.val[1]=75;
			cvSet2D(HsvImg,i,j,HsvData);
		}
	}
	cvCvtColor(HsvImg,HsvImg,CV_HSV2BGR);
	cvCopy(HsvImg,LightUnitizingImg);
	cvReleaseImage(&HsvImg);
}
void CImageProcess::ImageStretchByHistogram(IplImage *OriginalImg,IplImage*StretchImg)
{
	/************************************************* 
	Function:      通过直方图变换进行图像增强，将图像灰度的域值拉伸到0-255 
	src1:               单通道灰度图像                   
	dst1:              同样大小的单通道灰度图像  
	*************************************************/  
    double p[256],p1[256],num[256];      
    memset(p,0,sizeof(p));  
    memset(p1,0,sizeof(p1));  
    memset(num,0,sizeof(num));  
    int height=OriginalImg->height;  
    int width=OriginalImg->width;  
	int step=OriginalImg->widthStep;
	IplImage *tempImg=cvCreateImage(cvGetSize(OriginalImg),OriginalImg->depth,OriginalImg->nChannels);
	uchar *OriginalData=(uchar *)OriginalImg->imageData;
	uchar *tempData=(uchar *)tempImg->imageData;
	uchar val;
    long wMulh = height * width;  
      
    //statistics  
    for(int x=0;x<width;x++)  
    {  
        for(int y=0;y<height;y++){  
			val=OriginalData[y*step+x];  
            num[val]++;  
        }  
    }  
    //calculate probability  
    for(int i=0;i<256;i++)  
    {  
        p[i]=num[i]/wMulh;  
    }  
     //p1[i]=sum(p[j]);  j<=i;  
    for(int i=0;i<256;i++)  
    {  
        for(int k=0;k<=i;k++)  
            p1[i]+=p[k];  
    }  
    // histogram transformation  
    for(int x=0;x<width;x++)  
    {  
        for(int y=0;y<height;y++){  
            val=OriginalData[y*step+x];  
           tempData[y*step+x]= p1[val]*255+0.5;              
        }  
    }  
   cvCopy(tempImg,StretchImg);
   cvReleaseImage(&tempImg);
}
bool CImageProcess::Judge(IplImage * foreground)
{  
	//cvNamedWindow("test");
    CvSeq *pContour = NULL;     
    CvSeq *pConInner = NULL;     
    CvMemStorage *pStorage = NULL;   
	IplImage * dst = NULL;
	IplImage * src = NULL;
	IplImage * temp = cvCloneImage(foreground);
	cvNot(temp,temp);
    CvSeq * seq = NULL;
    CvRect rect;
    if (temp)     
    { 
        // 查找所有轮廓     
        pStorage = cvCreateMemStorage(0);     
        cvFindContours(temp, pStorage, &pContour, sizeof(CvContour),CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);     
        for (; pContour != NULL; pContour = pContour->h_next)     
        {     
 
            rect = cvBoundingRect(pContour,0);
			//cvRectangle(temp, cvPoint( temp->width/5,temp->height/3.5), cvPoint(temp->width/5+500, temp->height/3.5+160),CV_RGB(255,255, 255), 1, 8, 0);  
			if((rect.y<temp->height/3.5||rect.x<temp->width/4)||(rect.x+rect.width>temp->width/4+450||rect.y+rect.height>temp->height/3.5+160)) return false;
			else
		     return true;
        } 
        cvReleaseMemStorage(&pStorage);     
        pStorage = NULL;     
    }
	//cvDestroyWindow("test");
	cvReleaseImage(&temp);
}
