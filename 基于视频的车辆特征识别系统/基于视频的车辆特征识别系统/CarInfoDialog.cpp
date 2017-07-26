//// CarInfoDialog.cpp : 实现文件
////
//
//#include "stdafx.h"
//#include "CarInfoDialog.h"
//#include "afxdialogex.h"
//
//
//// CCarInfoDialog 对话框
//
#include "stdafx.h"
#include "CarInfoDialog.h"
#include "afxdialogex.h"
#include "resource.h"


// CCarInfoDialog 对话框

IMPLEMENT_DYNAMIC(CCarInfoDialog, CDialogEx)

CCarInfoDialog::CCarInfoDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCarInfoDialog::IDD, pParent)
{

	m_color = _T("");
	m_cartype = _T("");
	m_lengthheight = 0.0;
	m_spaceoccupy = 0.0;
	m_tailheight = 0.0;
}
CCarInfoDialog::CCarInfoDialog(IplImage * img,CString color,CString cartype,double lengthheight,double spaceoccupy,double tailheight,CWnd* pParent): CDialogEx(CCarInfoDialog::IDD, pParent)
{
	m_color = color;
	m_cartype = cartype;
	m_lengthheight = lengthheight;
	m_spaceoccupy = spaceoccupy;
	m_tailheight = tailheight;
	CurImg=cvCreateImage(cvSize(309,173),img->depth,img->nChannels);
	cvResize(img,CurImg,CV_INTER_LINEAR);
}
CCarInfoDialog::~CCarInfoDialog()
{
	cvReleaseImage(&CurImg);
}

void CCarInfoDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_COLOREDIT, m_color);
	DDX_Text(pDX, IDC_CARTYPEEDIT, m_cartype);
	DDX_Text(pDX, IDC_LENGTHHEIGHTRATIOEDIT, m_lengthheight);
	DDX_Text(pDX, IDC_SPACEOCCUPYEDIT, m_spaceoccupy);
	DDX_Text(pDX,IDC_TAILHEIGHTRATIOEDIT, m_tailheight);
}

void CCarInfoDialog::showImg()
{
	CDC* pDC=GetDlgItem(IDC_CARIMG)->GetDC();			// 获得显示控件的 DC
    HDC hDC = pDC ->GetSafeHdc();									// 获取 HDC(设备句柄) 来进行绘图操作

    CRect rect;
	GetDlgItem(IDC_CARIMG)->GetClientRect(&rect);
    int rw = rect.right - rect.left;								// 求出图片控件的宽和高
    int rh = rect.bottom - rect.top;
	IplImage *reSizeImg=cvCreateImage(cvSize(rw,rh),CurImg->depth,CurImg->nChannels);
	cvResize(CurImg,reSizeImg);
	SetRect(rect,0,0,rw,rh );
	CvvImage cvvImage;
	cvvImage.CopyOf(reSizeImg);										// 赋值给父类图像成员指针
    cvvImage.DrawToHDC(hDC,&rect );										// 调用父类绘图函数绘制图片
	cvReleaseImage(&reSizeImg);
    ReleaseDC(pDC);
}
void CCarInfoDialog::PostNcDestroy()
{
	delete this;
}
BEGIN_MESSAGE_MAP(CCarInfoDialog, CDialogEx)
END_MESSAGE_MAP()


// CCarInfoDialog 消息处理程序

