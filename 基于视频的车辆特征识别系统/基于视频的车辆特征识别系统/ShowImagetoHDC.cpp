#include "StdAfx.h"
#include "ShowImagetoHDC.h"
ShowImagetoHDC::ShowImagetoHDC()
{

}
ShowImagetoHDC::~ShowImagetoHDC()
{

}
void ShowImagetoHDC::ShowImage( IplImage* img, UINT ID)			// ID ��Picture Control�ؼ���ID��
{
	;
	CDC* pDC = AfxGetMainWnd()->GetDlgItem(ID)->GetDC();			// �����ʾ�ؼ��� DC
    HDC hDC = pDC ->GetSafeHdc();									// ��ȡ HDC(�豸���) �����л�ͼ����

    CRect rect;
	AfxGetMainWnd()->GetDlgItem(ID)->GetClientRect(&rect);
    int rw = rect.right - rect.left;								// ���ͼƬ�ؼ��Ŀ�͸�
    int rh = rect.bottom - rect.top;
	IplImage *reSizeImg=cvCreateImage(cvSize(rw,rh),img->depth,img->nChannels);
	cvResize(img,reSizeImg);
	SetRect(rect,0,0,rw,rh );
    m_img=reSizeImg;												// ��ֵ������ͼ���Աָ��
    DrawToHDC(hDC,&rect );										// ���ø����ͼ��������ͼƬ
	cvReleaseImage(&reSizeImg);
    AfxGetMainWnd()->ReleaseDC(pDC);
}