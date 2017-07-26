#pragma once
#include "stdafx.h"
#include "CvvImage.h"

class ShowImagetoHDC :public CvvImage
{
public:
	ShowImagetoHDC();
	~ShowImagetoHDC();
public:
	void ShowImage( IplImage* img, UINT ID ); 

 
 };