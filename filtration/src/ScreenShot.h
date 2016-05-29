#ifndef SCREENSHOT_H
#define SCREENSHOT_H

class ScreenShot
{
public:
	ScreenShot();
	ScreenShot(const ScreenShot* oldScreenShot);
	virtual ~ScreenShot();
	void takeShot();
	unsigned char* getPixelArray();
	int getWidth();
	int getHeight();

	int getRedAt(int x, int y);
	int getGreenAt(int x, int y);
	int getBlueAt(int x, int y);
	int getAlphaAt(int x, int y);

	void setAt(int x, int y, int r, int g, int b);
	void setAt(int x, int y, int r, int g, int b, int a);
	void setRedAt(int x, int y, int val);
	void setGreenAt(int x, int y, int val);
	void setBlueAt(int x, int y, int val);
	void setAlphaAt(int x, int y, int val);

	static void copyScreenShot(ScreenShot* source, ScreenShot* target);

private:
	unsigned char* m_pixels;
	int m_width, m_height;
};

#endif