#include "stdafx.h"
#define IMAGE_WIDTH 199
#define IMAGE_HEIGHT 112
#define IMAGE_CHANNELS 3
//void CmymfcDlg::ShowImage( IplImage* img, UINT ID )    // ID ��Picture Control�ؼ���ID��
//{
//    CDC* pDC = GetDlgItem( ID ) ->GetDC();        // �����ʾ�ؼ��� DC
//    HDC hDC = pDC ->GetSafeHdc();                // ��ȡ HDC(�豸���) �����л�ͼ����
//
//    CRect rect;
//    GetDlgItem(ID) ->GetClientRect( &rect );
//    int rw = rect.right - rect.left;            // ���ͼƬ�ؼ��Ŀ�͸�
//    int rh = rect.bottom - rect.top;
//    int iw = img->width;                        // ��ȡͼƬ�Ŀ�͸�
//    int ih = img->height;
//    int tx = (int)(rw - iw)/2;                    // ʹͼƬ����ʾλ�������ڿؼ�������
//    int ty = (int)(rh - ih)/2;
//    SetRect( rect, tx, ty, tx+iw, ty+ih );
//
//    CvvImage cimg;
//    cimg.CopyOf( img );                            // ����ͼƬ
//    cimg.DrawToHDC( hDC, &rect );                // ��ͼƬ���Ƶ���ʾ�ؼ���ָ��������
//
//    ReleaseDC( pDC );
//}
