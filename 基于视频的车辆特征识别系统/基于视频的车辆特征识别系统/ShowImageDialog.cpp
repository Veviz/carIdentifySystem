//// ShowImageDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ShowImageDialog.h"
#include "afxdialogex.h"


// CShowImageDialog �Ի���

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
	CDC* pDC=GetDlgItem(IDC_SHOWBIGIMG)->GetDC();			// �����ʾ�ؼ��� DC
    HDC hDC = pDC ->GetSafeHdc();									// ��ȡ HDC(�豸���) �����л�ͼ����

    CRect rect;
	GetDlgItem(IDC_SHOWBIGIMG)->GetClientRect(&rect);
    int rw = rect.right - rect.left;								// ���ͼƬ�ؼ��Ŀ�͸�
    int rh = rect.bottom - rect.top;
	IplImage *reSizeImg=cvCreateImage(cvSize(rw,rh),BigImg->depth,BigImg->nChannels);
	cvResize(BigImg,reSizeImg);
	SetRect(rect,0,0,rw,rh );
	CvvImage cvvImage;
	cvvImage.CopyOf(reSizeImg);										// ��ֵ������ͼ���Աָ��
    cvvImage.DrawToHDC(hDC,&rect );										// ���ø����ͼ��������ͼƬ
	cvReleaseImage(&reSizeImg);
    ReleaseDC(pDC);
}
void CShowImageDialog::PostNcDestroy()
{
	delete this;
}

BEGIN_MESSAGE_MAP(CShowImageDialog, CDialogEx)
END_MESSAGE_MAP()


// CShowImageDialog ��Ϣ�������
