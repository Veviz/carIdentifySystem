
// VehicleDlg.h : 头文件
//

#pragma once

#include "CImageProcess.h"
#include "highgui.h"
#include <cv.h>
#include "ShowImagetoHDC.h"
using namespace cv;
// CVehicleDlg 对话框
class CVehicleDlg : public CDialogEx
{
// 构造
public:
	CVehicleDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CVehicleDlg();
// 对话框数据
	enum { IDD = IDD_MY_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
protected:
	ShowImagetoHDC showImagetoHDC;	//绘制图像类
protected:							//五种图像的指针均为856*480
	IplImage *OriginalImg;			//3通道
	IplImage *BackgroundImg;		//3通道
	IplImage *SubImg;				//单通道
	IplImage *ConturImg;			//单通道
	IplImage *NoBgImg;				//3通道
protected:
	double spaceOccupy;
	double lenghtHeightRatio;
	double tailHeightRatio;
protected:
	CString m_color;
	CString m_carType;
//
protected:
	IplImage **conturSeq;
	CImageProcess imageProcess;
	CvCapture *capture;
	CvCapture *capture1;
	IplImage *curFrame;
	IplImage *preFrame;
	int Threshold;
	IplImage *foregroundImg;
	IplImage *grayBgImg;
	IplImage *laplaceImg;
	IplImage *RobertImg;	
	IplConvKernel* element;
	int conturNumber;	
	int j;
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOkstart();	
	afx_msg void OnStnDblclickOriginalimg();
	afx_msg void OnStnDblclickBackgroundimg();
	afx_msg void OnStnDblclickSubimg();
	afx_msg void OnStnDblclickConturimg();
	afx_msg void OnStnDblclickNobgimg();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedPlay();
	afx_msg void OnBnClickedPlate();
};
