//// ShowImageDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "ShowImageDialog.h"
#include "afxdialogex.h"


// CShowImageDialog 对话框

IMPLEMENT_DYNAMIC(CShowImageDialog, CDialogEx)

CShowImageDialog::CShowImageDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CShowImageDialog::IDD, pParent)
{

}
CShowImageDialog::CShowImageDialog(IplImage * img,CWnd* pParent): CDialogEx(CShowImageDialog::IDD, pParent)
{
	BigImg = cvCreateImage(cvSize(640,360),img->depth,img->nChannels);
	cvResize(img, BigImg, CV_INTER_LINEAR);
}

CShowImageDialog::~CShowImageDialog()
{
	cvReleaseImage(&BigImg);
}

void CShowImageDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

void CShowImageDialog::ShowImg()
{
	CDC* pDC=GetDlgItem(IDC_SHOWBIGIMG)->GetDC();			// 获得显示控件的 DC
    HDC hDC = pDC ->GetSafeHdc();									// 获取 HDC(设备句柄) 来进行绘图操作

    CRect rect;
	GetDlgItem(IDC_SHOWBIGIMG)->GetClientRect(&rect);
    int rw = rect.right - rect.left;								// 求出图片控件的宽和高
    int rh = rect.bottom - rect.top;
	IplImage *reSizeImg=cvCreateImage(cvSize(rw,rh),BigImg->depth,BigImg->nChannels);
	cvResize(BigImg,reSizeImg);
	SetRect(rect,0,0,rw,rh );
	CvvImage cvvImage;
	cvvImage.CopyOf(reSizeImg);										// 赋值给父类图像成员指针
    cvvImage.DrawToHDC(hDC,&rect );										// 调用父类绘图函数绘制图片
	cvReleaseImage(&reSizeImg);
    ReleaseDC(pDC);
}
void CShowImageDialog::PostNcDestroy()
{
	delete this;
}

BEGIN_MESSAGE_MAP(CShowImageDialog, CDialogEx)
END_MESSAGE_MAP()


// CShowImageDialog 消息处理程序
