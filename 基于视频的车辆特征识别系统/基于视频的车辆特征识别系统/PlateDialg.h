#pragma once
#include "resource.h"
#include "ShowImagetoHDC.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <highgui.h>
#include <cv.h>

// CPlateDialg 对话框

class CPlateDialg : public CDialogEx
{
	DECLARE_DYNAMIC(CPlateDialg)

public:
	int Model;//确定使用的是那种方式打开
	CImage m_Image;
	CvvImage m_curCvImage;
	int lBorder,rBorder;
	int top,bottom;
	IplImage *chara1;
	IplImage *chara2;
	IplImage *chara3;
	IplImage *chara4;
	IplImage *chara5;
	IplImage *chara6;
	IplImage *chara7;

public:
	CPlateDialg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPlateDialg();

// 对话框数据
	enum { IDD = IDD_CAR_PLATE };
protected:
	IplImage *CurImg;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOpenimage();
	afx_msg void OnBnClickedPlateFind();
	afx_msg void OnBnClickedPlateDiv();
	CString m_result;
	char chara;
public:
	void CPlateDialg::showImg();
	void CPlateDialg::PostNcDestroy();
	void CPlateDialg::getTemplateFeature();
	void CPlateDialg::getCharaFeature(IplImage *src,int n);
	afx_msg void OnBnClickedResult();
};
