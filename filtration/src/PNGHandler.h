#ifndef PNGHANDLER_H
#define PNGHANDLER_H

#include <iostream>

class PNGHandler
{
public:
	PNGHandler();

	unsigned char* load(std::string fileName);
	void save(unsigned char* array, int width, int height, std::string fileName);

	int getLoadedWidth();
	int getLoadedHeight();

private:
	int loadedWidth, loadedHeight;
};

#endif