#include "StdAfx.h"
#include "ShowImagetoHDC.h"
ShowImagetoHDC::ShowImagetoHDC()
{

}
ShowImagetoHDC::~ShowImagetoHDC()
{

}
void ShowImagetoHDC::ShowImage( IplImage* img, UINT ID)			// ID 是Picture Control控件的ID号
{
	;
	CDC* pDC = AfxGetMainWnd()->GetDlgItem(ID)->GetDC();			// 获得显示控件的 DC
    HDC hDC = pDC ->GetSafeHdc();									// 获取 HDC(设备句柄) 来进行绘图操作

    CRect rect;
	AfxGetMainWnd()->GetDlgItem(ID)->GetClientRect(&rect);
    int rw = rect.right - rect.left;								// 求出图片控件的宽和高
    int rh = rect.bottom - rect.top;
	IplImage *reSizeImg=cvCreateImage(cvSize(rw,rh),img->depth,img->nChannels);
	cvResize(img,reSizeImg);
	SetRect(rect,0,0,rw,rh );
    m_img=reSizeImg;												// 赋值给父类图像成员指针
    DrawToHDC(hDC,&rect );										// 调用父类绘图函数绘制图片
	cvReleaseImage(&reSizeImg);
    AfxGetMainWnd()->ReleaseDC(pDC);
}