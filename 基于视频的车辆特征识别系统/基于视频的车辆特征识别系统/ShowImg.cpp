#include "StdAfx.h"
#include "ShowImg.h"

#include <vector>  
#include <opencv2/opencv.hpp>  

#include <queue>
using namespace std;

#include <highgui.h>
#include <cv.h>
using namespace cv;
#include <stdio.h>

ShowImg::ShowImg(void)
{
}


ShowImg::~ShowImg(void)
{
}

#define HG_IS_IMAGE(img) ((img)!=0&&((const IplImage*)(img))->nSize == sizeof(IplImage)&&((IplImage*)img)->imageData!=0)

CV_INLINE RECT NormalizeRect( RECT r )
{
		int t;
	 if( r.left > r.right )
	 {
		  t = r.left;
		  r.left = r.right;
		  r.right = t;
	 }
	 if( r.top > r.bottom )
	 {
		  t = r.top;
		  r.top = r.bottom;
		  r.bottom = t;
	 }
	 return r;
}
CV_INLINE CvRect RectToCvRect( RECT sr ){sr = NormalizeRect( sr );return cvRect( sr.left, sr.top, sr.right - sr.left, sr.bottom - sr.top );}
bool  ShowImg::Create( int w, int h, int bpp, int origin )
{
	 const unsigned max_img_size = 10000;
	 if((bpp!=8&&bpp!=24&&bpp!=32)||(unsigned)w>=max_img_size||(unsigned)h >=max_img_size||(origin!=IPL_ORIGIN_TL&&origin!=IPL_ORIGIN_BL))
	 {
		  assert(0); // most probably, it is a programming error
		  return false;
	 }
	 if( !m_img || Bpp() != bpp || m_img->width != w || m_img->height != h )
	 {
		  if( m_img && m_img->nSize == sizeof(IplImage))
			   Destroy(); 
		  m_img = cvCreateImage( cvSize( w, h ), IPL_DEPTH_8U, bpp/8 );
	 }
	 if( m_img )
		  m_img->origin = origin == 0 ? IPL_ORIGIN_TL : IPL_ORIGIN_BL;
	 return m_img != 0;
}
void  FillBitmapInfo( BITMAPINFO* bmi, int width, int height, int bpp, int origin )
{
	 assert( bmi && width >= 0 && height >= 0 && (bpp == 8 || bpp == 24 || bpp == 32));
	 BITMAPINFOHEADER* bmih = &(bmi->bmiHeader);
	 memset( bmih, 0, sizeof(*bmih));
	 bmih->biSize = sizeof(BITMAPINFOHEADER);
	 bmih->biWidth = width;
	 bmih->biHeight = origin ? abs(height) : -abs(height);
	 bmih->biPlanes = 1;
	 bmih->biBitCount = (unsigned short)bpp;
	 bmih->biCompression = BI_RGB;
	 if( bpp == 8 )
	 {
		  RGBQUAD* palette = bmi->bmiColors;
		  int i;
		  for( i = 0; i < 256; i++ )
		  {
			   palette[i].rgbBlue = palette[i].rgbGreen = palette[i].rgbRed = (BYTE)i;
			   palette[i].rgbReserved = 0;
		  }
	 }
}
void  ShowImg::Show( HDC dc, int x, int y, int w, int h, int from_x, int from_y )
{
	 if( m_img && m_img->depth == IPL_DEPTH_8U )
	 {
		  uchar buffer[sizeof(BITMAPINFOHEADER) + 1024];
		  BITMAPINFO* bmi = (BITMAPINFO*)buffer;
		  int bmp_w = m_img->width, bmp_h = m_img->height;
		  FillBitmapInfo( bmi, bmp_w, bmp_h, Bpp(), m_img->origin );
		  from_x = MIN( MAX( from_x, 0 ), bmp_w - 1 );
		  from_y = MIN( MAX( from_y, 0 ), bmp_h - 1 );
		  int sw = MAX( MIN( bmp_w - from_x, w ), 0 );
		  int sh = MAX( MIN( bmp_h - from_y, h ), 0 );
		  SetDIBitsToDevice(
		   dc, x, y, sw, sh, from_x, from_y, from_y, sh,
		   m_img->imageData + from_y*m_img->widthStep,
		   bmi, DIB_RGB_COLORS );
	 }
}
void  ShowImg::DrawToHDC( HDC hDCDst, RECT* pDstRect )
{
	 if( pDstRect && m_img && m_img->depth == IPL_DEPTH_8U && m_img->imageData )
	 {
		  uchar buffer[sizeof(BITMAPINFOHEADER) + 1024];
		  BITMAPINFO* bmi = (BITMAPINFO*)buffer;
		  int bmp_w = m_img->width, bmp_h = m_img->height;
		  CvRect roi = cvGetImageROI( m_img );
		  CvRect dst = RectToCvRect( *pDstRect );
		  if( roi.width == dst.width && roi.height == dst.height )
		  {
			   Show( hDCDst, dst.x, dst.y, dst.width, dst.height, roi.x, roi.y );
			   return;
		  }
		  if( roi.width > dst.width )
		  {
			   SetStretchBltMode(hDCDst,HALFTONE ); 	
		  }
		  else
		  {
			   SetStretchBltMode(hDCDst,COLORONCOLOR );
		  }
		  FillBitmapInfo( bmi, bmp_w, bmp_h, Bpp(), m_img->origin );
		  ::StretchDIBits(hDCDst,dst.x, dst.y, dst.width, dst.height,roi.x, roi.y, roi.width, roi.height,m_img->imageData, bmi, DIB_RGB_COLORS, SRCCOPY );
	 }
}
void ShowImg::DrawPicToHDC(IplImage* img , UINT ID)
{
	
	int desired_color =  -1;
	CDC* pDC = GetDlgItem( ID ) ->GetDC();        // 获得显示控件的 DC
    HDC hDC = pDC ->GetSafeHdc();                // 获取 HDC(设备句柄) 来进行绘图操作

    CRect rect;
    GetDlgItem(ID) ->GetClientRect( &rect );
    int rw = rect.right - rect.left;            // 求出图片控件的宽和高
    int rh = rect.bottom - rect.top;
    int iw = img->width;                        // 读取图片的宽和高
    int ih = img->height;
    int tx = (int)(rw - iw)/2;                    // 使图片的显示位置正好在控件的正中
    int ty = (int)(rh - ih)/2;
    SetRect( rect, tx, ty, tx+iw, ty+ih );
	if( HG_IS_IMAGE(img) )
	 {
		  int color = desired_color;
		  CvSize size = cvGetSize( img );
		  if( color < 0 )
			   color = img->nChannels > 1;
		  if( Create(size.width,size.height,(!color?1:img->nChannels>1?img->nChannels:3)*8,img->origin))
		  {
			   cvConvertImage( img, m_img, 0 );
		  }
	 }
   // CvvImage cimg;
   // cimg.CopyOf( img );                            // 复制图片
  //  cimg.DrawToHDC( hDC, &rect );                // 将图片绘制到显示控件的指定区域内

    ReleaseDC( pDC );
}