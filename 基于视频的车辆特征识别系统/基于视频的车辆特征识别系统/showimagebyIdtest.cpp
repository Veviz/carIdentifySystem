#include "stdafx.h"
#define IMAGE_WIDTH 199
#define IMAGE_HEIGHT 112
#define IMAGE_CHANNELS 3
//void CmymfcDlg::ShowImage( IplImage* img, UINT ID )    // ID 是Picture Control控件的ID号
//{
//    CDC* pDC = GetDlgItem( ID ) ->GetDC();        // 获得显示控件的 DC
//    HDC hDC = pDC ->GetSafeHdc();                // 获取 HDC(设备句柄) 来进行绘图操作
//
//    CRect rect;
//    GetDlgItem(ID) ->GetClientRect( &rect );
//    int rw = rect.right - rect.left;            // 求出图片控件的宽和高
//    int rh = rect.bottom - rect.top;
//    int iw = img->width;                        // 读取图片的宽和高
//    int ih = img->height;
//    int tx = (int)(rw - iw)/2;                    // 使图片的显示位置正好在控件的正中
//    int ty = (int)(rh - ih)/2;
//    SetRect( rect, tx, ty, tx+iw, ty+ih );
//
//    CvvImage cimg;
//    cimg.CopyOf( img );                            // 复制图片
//    cimg.DrawToHDC( hDC, &rect );                // 将图片绘制到显示控件的指定区域内
//
//    ReleaseDC( pDC );
//}
