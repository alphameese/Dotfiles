#include "ScreenShot.h"

#include <iostream>
#include <cstring>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

using namespace std;

ScreenShot::ScreenShot() : m_pixels(0), m_width(0), m_height(0) { }

ScreenShot::ScreenShot(const ScreenShot* oldScreenShot) : m_width(oldScreenShot->m_width), m_height(oldScreenShot->m_height)
{
	m_pixels = new unsigned char[m_height * (4 * m_width)];
	memcpy(m_pixels, oldScreenShot->m_pixels, 4*sizeof(unsigned char)*m_height*m_width);
}

ScreenShot::~ScreenShot()
{
	delete [] m_pixels;
}

void ScreenShot::takeShot()
{
	Display *disp = XOpenDisplay(NULL);
	Window root = DefaultRootWindow(disp);

	XWindowAttributes info;
	XGetWindowAttributes(disp, DefaultRootWindow(disp), &info);
	m_width = info.width;
	m_height = info.height;

	XImage *image = XGetImage(disp, root, 0, 0, m_width, m_height, AllPlanes, ZPixmap);

	m_pixels = new unsigned char[m_height * (4 * m_width)];

	unsigned long red_mask   = image->red_mask;
	unsigned long green_mask = image->green_mask;
	unsigned long blue_mask  = image->blue_mask;

	for(int y = 0; y < m_height; y++) {
		for(int x = 0; x < m_width; x++) {

			unsigned long pixel = XGetPixel(image, x, y);

			m_pixels[(x + m_width * y) * 4]     = (pixel & red_mask) >> 16;
			m_pixels[(x + m_width * y) * 4 + 1] = (pixel & green_mask) >> 8;
			m_pixels[(x + m_width * y) * 4 + 2] = (pixel & blue_mask);
			m_pixels[(x + m_width * y) * 4 + 3] = 255;
		}
	}

	XFree(image);
	XCloseDisplay(disp);
}

unsigned char* ScreenShot::getPixelArray()
{
	return m_pixels;
}

int ScreenShot::getWidth()
{
	return m_width;
}

int ScreenShot::getHeight()
{
	return m_height;
}

int ScreenShot::getRedAt(int x, int y)
{
	return m_pixels[(x + m_width * y) * 4];
}

int ScreenShot::getGreenAt(int x, int y)
{
	return m_pixels[(x + m_width * y) * 4 + 1];
}

int ScreenShot::getBlueAt(int x, int y)
{
	return m_pixels[(x + m_width * y) * 4 + 2];
}

int ScreenShot::getAlphaAt(int x, int y)
{
	return m_pixels[(x + m_width * y) * 4 + 3];
}

void ScreenShot::setAt(int x, int y, int r, int g, int b)
{
	m_pixels[(x + m_width * y) * 4]     = (unsigned char) r;
	m_pixels[(x + m_width * y) * 4 + 1] = (unsigned char) g;
	m_pixels[(x + m_width * y) * 4 + 2] = (unsigned char) b;
	m_pixels[(x + m_width * y) * 4 + 3] = (unsigned char) 255;
}

void ScreenShot::setAt(int x, int y, int r, int g, int b, int a)
{
	m_pixels[(x + m_width * y) * 4]     = (unsigned char) r;
	m_pixels[(x + m_width * y) * 4 + 1] = (unsigned char) g;
	m_pixels[(x + m_width * y) * 4 + 2] = (unsigned char) b;
	m_pixels[(x + m_width * y) * 4 + 3] = (unsigned char) a;
}

void ScreenShot::setRedAt(int x, int y, int val)
{
	m_pixels[(x + m_width * y) * 4] = (unsigned char) val;
}

void ScreenShot::setGreenAt(int x, int y, int val)
{
	m_pixels[(x + m_width * y) * 4 + 1] = (unsigned char) val;
}

void ScreenShot::setBlueAt(int x, int y, int val)
{
	m_pixels[(x + m_width * y) * 4 + 2] = (unsigned char) val;
}

void ScreenShot::setAlphaAt(int x, int y, int val)
{
	m_pixels[(x + m_width * y) * 4 + 3] = (unsigned char) val;
}

void ScreenShot::copyScreenShot(ScreenShot* source, ScreenShot* target)
{
	if (target->getWidth() != source->getWidth() || target->getHeight() != source->getHeight()) {
		cerr << "Error: dimension mismatch" << endl;
	} else {
		memcpy(target->m_pixels, source->m_pixels, 4*sizeof(unsigned char) * target->m_height * target->m_width);
	}
}