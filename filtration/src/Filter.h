#ifndef FILTER_H
#define FILTER_H

#include "PNGHandler.h"
#include <string>
class ScreenShot;

class Filter
{
public:
	Filter();
	virtual ~Filter();

	void setScreenShot(ScreenShot* screen);
	ScreenShot* getFilteredScreenShot();

	void applyBlur(int radius);
	// void applyMotionBlur(int radius, int direction);
	void addStamp(std::string location);

private:
	PNGHandler m_loader;
	ScreenShot* m_origScreen;
	ScreenShot* m_newScreen;
	int* m_boxSizes;

	unsigned char* m_stampArray;
	int m_stampWidth, m_stampHeight;

	static const int NUM_PASSES;

	void calculateBoxSizes(int radius);
	void boxBlur(ScreenShot* source, ScreenShot* target, int radius);
	void boxBlurH(ScreenShot* source, ScreenShot* target, int radius);
	void boxBlurV(ScreenShot* source, ScreenShot* target, int radius);
};

#endif