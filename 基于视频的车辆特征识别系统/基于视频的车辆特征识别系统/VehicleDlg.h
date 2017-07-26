
// VehicleDlg.h : ͷ�ļ�
//

#pragma once

#include "CImageProcess.h"
#include "highgui.h"
#include <cv.h>
#include "ShowImagetoHDC.h"
using namespace cv;
// CVehicleDlg �Ի���
class CVehicleDlg : public CDialogEx
{
// ����
public:
	CVehicleDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CVehicleDlg();
// �Ի�������
	enum { IDD = IDD_MY_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
protected:
	ShowImagetoHDC showImagetoHDC;	//����ͼ����
protected:							//����ͼ���ָ���Ϊ856*480
	IplImage *OriginalImg;			//3ͨ��
	IplImage *BackgroundImg;		//3ͨ��
	IplImage *SubImg;				//��ͨ��
	IplImage *ConturImg;			//��ͨ��
	IplImage *NoBgImg;				//3ͨ��
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
// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
