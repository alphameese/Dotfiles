#include "Filter.h"

#include "ScreenShot.h"
#include <math.h>
#include <algorithm>
#include <iostream>

using std::cout; using std::endl;

const int Filter::NUM_PASSES = 3;

Filter::Filter() : m_origScreen(0), m_newScreen(0), m_boxSizes(0),
                   m_stampArray(0), m_stampWidth(0), m_stampHeight(0)
{
	m_loader = PNGHandler();
}

Filter::~Filter()
{
	// Filter never makes m_origScreen, so it shouldn't delete it
	delete m_newScreen;
	delete [] m_boxSizes;
	delete [] m_stampArray;
}

void Filter::setScreenShot(ScreenShot* screen)
{
	m_origScreen = screen;
	m_newScreen = new ScreenShot(screen);
}

ScreenShot* Filter::getFilteredScreenShot()
{
	return m_newScreen;
}


/*
 * Blurring algorithm from : http://blog.ivank.net/fastest-gaussian-blur.html
 */

void Filter::calculateBoxSizes(int radius)
{

	m_boxSizes = new int[NUM_PASSES];

	if(NUM_PASSES == 1) {
		m_boxSizes[0] = radius;
		return;
	}

	double wIdeal = sqrt((12*radius*radius/NUM_PASSES)+1);

	int wl = floor(wIdeal);
	if(!(wl%2)) wl--;		// wl needs to be odd
	int wu = wl + 2;

	double mIdeal = (12*radius*radius - NUM_PASSES*wl*wl - 4*NUM_PASSES*wl - 3*NUM_PASSES)/(-4*wl - 4);
	int m = round(mIdeal);

	for(int i = 0; i < NUM_PASSES; i++) {
		if(i < m) {
			m_boxSizes[i] = wl;
		} else {
			m_boxSizes[i] = wu;
		}
	}
}

void Filter::applyBlur(int radius)
{
	calculateBoxSizes(radius);
	boxBlur(m_origScreen, m_newScreen, (m_boxSizes[0]-1)/2);
	boxBlur(m_newScreen, m_origScreen, (m_boxSizes[1]-1)/2);
	boxBlur(m_origScreen, m_newScreen, (m_boxSizes[2]-1)/2);
}

void Filter::boxBlur(ScreenShot* source, ScreenShot* target, int radius)
{
	ScreenShot::copyScreenShot(source, target);

	boxBlurH(target, source, radius);
	boxBlurV(source, target, radius);
}

void Filter::boxBlurH(ScreenShot* source, ScreenShot* target, int radius)
{
	double weight = (double) 1 / (2*radius + 1);
	int width = target->getWidth();
	int height = target->getHeight();

	for(int row = 0; row < height; row++) {
		int px = 0;
		int leftpx = px;
		int rightpx = px+radius;

		int firstR = source->getRedAt(px, row);
		int firstG = source->getGreenAt(px, row);
		int firstB = source->getBlueAt(px, row);

		int lastR = source->getRedAt(px+width-1, row);
		int lastG = source->getGreenAt(px+width-1, row);
		int lastB = source->getBlueAt(px+width-1, row);

		int valR = (radius+1)*firstR;
		int valG = (radius+1)*firstG;
		int valB = (radius+1)*firstB;

		// First accumulator setup
		for(int x = 0; x < radius; x++) {
			valR += source->getRedAt(px+x, row);
			valG += source->getGreenAt(px+x, row);
			valB += source->getBlueAt(px+x, row);
		}

		// Color the beginning of the image
		for(int x = 0; x <= radius; x++) {
			valR += source->getRedAt(rightpx, row) - firstR;
			valG += source->getGreenAt(rightpx, row) - firstG;
			valB += source->getBlueAt(rightpx, row) - firstB;

			target->setAt(px, row, valR*weight, valG*weight, valB*weight);

			rightpx++;
			px++;
		}

		// Color the whole center of the image
		for(int x = radius+1; x < width-radius; x++) {
			valR += source->getRedAt(rightpx, row) - source->getRedAt(leftpx, row);
			valG += source->getGreenAt(rightpx, row) - source->getGreenAt(leftpx, row);
			valB += source->getBlueAt(rightpx, row) - source->getBlueAt(leftpx, row);

			target->setAt(px, row, valR*weight, valG*weight, valB*weight);

			rightpx++;
			leftpx++;
			px++;
		}

		// Color the end of the image
		for(int x = width-radius; x < width; x++) {
			valR += lastR - source->getRedAt(leftpx, row);
			valG += lastG - source->getGreenAt(leftpx, row);
			valB += lastB - source->getBlueAt(leftpx, row);

			target->setAt(px, row, valR*weight, valG*weight, valB*weight);

			leftpx++;
			px++;
		}
	}
}

void Filter::boxBlurV(ScreenShot* source, ScreenShot* target, int radius)
{
	double weight = (double) 1 / (2*radius + 1);
	int width = target->getWidth();
	int height = target->getHeight();

	for(int col = 0; col < width; col++) {
		int px = 0;
		int upperpx = px;
		int lowerpx = px+radius;

		int firstR = source->getRedAt(col, px);
		int firstG = source->getGreenAt(col, px);
		int firstB = source->getBlueAt(col, px);

		int lastR = source->getRedAt(col, px+height-1);
		int lastG = source->getGreenAt(col, px+height-1);
		int lastB = source->getBlueAt(col, px+height-1);

		int valR = (radius+1)*firstR;
		int valG = (radius+1)*firstG;
		int valB = (radius+1)*firstB;

		// First accumulator setup
		for(int y = 0; y < radius; y++) {
			valR += source->getRedAt(col, px+y);
			valG += source->getGreenAt(col, px+y);
			valB += source->getBlueAt(col, px+y);
		}

		// Color the beginning of the image
		for(int y = 0; y <= radius; y++) {
			valR += source->getRedAt(col, lowerpx) - firstR;
			valG += source->getGreenAt(col, lowerpx) - firstG;
			valB += source->getBlueAt(col, lowerpx) - firstB;

			target->setAt(col, px, valR*weight, valG*weight, valB*weight);

			lowerpx++;
			px++;
		}

		// Color the whole center of the image
		for(int y = radius+1; y < height-radius; y++) {
			valR += source->getRedAt(col, lowerpx) - source->getRedAt(col, upperpx);
			valG += source->getGreenAt(col, lowerpx) - source->getGreenAt(col, upperpx);
			valB += source->getBlueAt(col, lowerpx) - source->getBlueAt(col, upperpx);

			target->setAt(col, px, valR*weight, valG*weight, valB*weight);

			lowerpx++;
			upperpx++;
			px++;
		}

		// Color the end of the image
		for(int y = height-radius; y < height; y++) {
			valR += lastR - source->getRedAt(col, upperpx);
			valG += lastG - source->getGreenAt(col, upperpx);
			valB += lastB - source->getBlueAt(col, upperpx);

			target->setAt(col, px, valR*weight, valG*weight, valB*weight);

			upperpx++;
			px++;
		}
	}
}

// void Filter::applyMotionBlur(int radius, int direction) { }

void Filter::addStamp(std::string location) 
{
	m_stampArray = m_loader.load(location);
	m_stampWidth = m_loader.getLoadedWidth();
	m_stampHeight = m_loader.getLoadedHeight();

	// m_loader.save(m_stampArray, m_stampWidth, m_stampHeight, "stamp.png");

	int width = m_newScreen->getWidth();
	int height = m_newScreen->getHeight();

	if(m_stampArray) {
		int topLeftX = width/2 - m_stampWidth/2;
		int topLeftY = height/2 - m_stampHeight/2;

		for(int sy = 0; sy < m_stampHeight; sy++) {
			for(int sx = 0; sx < m_stampWidth; sx++) {
				if((topLeftX+sx >= 0 && topLeftX+sx < width) && (topLeftY+sy >= 0 && topLeftY+sy < height)) {
				   	int sr = (int)m_stampArray[(sx + m_stampWidth * sy) * 4];
				   	int sg = (int)m_stampArray[(sx + m_stampWidth * sy) * 4 + 1];
				   	int sb = (int)m_stampArray[(sx + m_stampWidth * sy) * 4 + 2];
				   	int sa = (int)m_stampArray[(sx + m_stampWidth * sy) * 4 + 3];

				   	double weight = (double)sa/255;

					int oldR = m_newScreen->getRedAt(topLeftX+sx, topLeftY+sy);
					int oldG = m_newScreen->getGreenAt(topLeftX+sx, topLeftY+sy);
					int oldB = m_newScreen->getBlueAt(topLeftX+sx, topLeftY+sy);

					m_newScreen->setAt(topLeftX+sx, topLeftY+sy, 
						(int)(oldR*(1-weight) + sr*weight),
						(int)(oldG*(1-weight) + sg*weight),
						(int)(oldB*(1-weight) + sb*weight));
				}
			}
		}
	}
}