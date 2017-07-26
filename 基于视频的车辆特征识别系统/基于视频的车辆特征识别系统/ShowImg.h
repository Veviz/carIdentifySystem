#pragma once
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgproc/imgproc_c.h"
class ShowImg : public CView
{
public:
	ShowImg(void);
	~ShowImg(void);
public:
	IplImage * m_img ;
public:
	bool  Create( int w, int h, int bpp, int origin );
	int Bpp() { return m_img ? (m_img->depth & 255)*m_img->nChannels : 0; };
	void Destroy(){cvReleaseImage( &m_img );};
	void DrawPicToHDC(IplImage* img , UINT ID);
	void  DrawToHDC( HDC hDCDst, RECT* pDstRect );
	void  Show( HDC dc, int x, int y, int w, int h, int from_x, int from_y );
};

