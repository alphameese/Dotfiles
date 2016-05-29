#include "PNGHandler.h"
#include <stdexcept>
#include <stdio.h>
#include <png.h>

using std::cout; using std::endl;

PNGHandler::PNGHandler() : loadedWidth(0), loadedHeight(0) { }

unsigned char* PNGHandler::load(std::string fileName)
{
	FILE* filePointer = fopen(fileName.c_str(), "rb");

	if(filePointer == NULL) {
		throw std::invalid_argument("Could not load image: Invalid image name");
	}

	// Read image and information structures
	png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	png_infop info = png_create_info_struct(png);

	png_init_io(png, filePointer);
	png_read_info(png, info);

	loadedWidth = png_get_image_width(png, info);
	loadedHeight = png_get_image_height(png, info);
	png_byte colorType = png_get_color_type(png, info);
	png_byte bitDepth = png_get_bit_depth(png, info);

	if(bitDepth == 16) {
		png_set_strip_16(png);
	}

	if(colorType == PNG_COLOR_TYPE_PALETTE) {
		png_set_palette_to_rgb(png);
	}

	// PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
	if(colorType == PNG_COLOR_TYPE_GRAY && bitDepth < 8) {
		png_set_expand_gray_1_2_4_to_8(png);
	}

	if(png_get_valid(png, info, PNG_INFO_tRNS)) {
		png_set_tRNS_to_alpha(png);
	}

	if (colorType == PNG_COLOR_TYPE_GRAY ||
	  colorType == PNG_COLOR_TYPE_GRAY_ALPHA) {
		png_set_gray_to_rgb(png);
	}

	// These don't have alpha so fill with 0xff
	if(colorType == PNG_COLOR_TYPE_RGB ||
	  colorType == PNG_COLOR_TYPE_GRAY ||
	  colorType == PNG_COLOR_TYPE_PALETTE) {
		png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
	}

	png_read_update_info(png, info);


	// Set up rows for reading into
	png_bytep *rowPointers = new png_bytep[loadedHeight];
	for(int y = 0; y < loadedHeight; y++) {
		rowPointers[y] = new png_byte[png_get_rowbytes(png,info)];
	}

	png_read_image(png, rowPointers);

	// Copy information to PixelBuffer
	// PixelBuffer* newBuffer = new PixelBuffer(width, loadedHeight, ColorData(0,0,0,0));
	unsigned char* newArray = new unsigned char[loadedHeight * (4 * loadedWidth)];

	for(int y = 0; y < loadedHeight; y++) {
		for(int x = 0; x < loadedWidth; x++) {
			png_bytep pixel = &(rowPointers[y][x*4]);
			// newBuffer->setPixel(x, y, ColorData((float)pixel[0] / 255, (float)pixel[1] / 255, 
			// 	(float)pixel[2] / 255, (float)pixel[3] / 255));

			newArray[(x + loadedWidth * y) * 4] = (unsigned char)pixel[0];
			newArray[(x + loadedWidth * y) * 4 + 1] = (unsigned char)pixel[1];
			newArray[(x + loadedWidth * y) * 4 + 2] = (unsigned char)pixel[2];
			newArray[(x + loadedWidth * y) * 4 + 3] = (unsigned char)pixel[3];
		}
	}

	delete [] rowPointers;
	png_destroy_read_struct(&png, &info, NULL);
	fclose(filePointer);

	// return newBuffer;
	return newArray;
}

void PNGHandler::save(unsigned char* array, int width, int height, std::string fileName)
{
	FILE* filePointer = fopen(fileName.c_str(), "wb");

	png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	png_infop info = png_create_info_struct(png);

	png_init_io(png, filePointer);

	// 8-bit depth, RGBA
	png_set_IHDR(png, info, width, height, 8,
		PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_DEFAULT,
		PNG_FILTER_TYPE_DEFAULT);

	png_write_info(png, info);


	// Set up rows for writing from
	png_bytep *rowPointers = new png_bytep[height];
	for(int y = 0; y < height; y++) {
		rowPointers[y] = new png_byte[png_get_rowbytes(png,info)];
	}

	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			rowPointers[y][x*4]   = (int)(array[(x + width * y) * 4]);
			rowPointers[y][x*4+1] = (int)(array[(x + width * y) * 4 + 1]);
			rowPointers[y][x*4+2] = (int)(array[(x + width * y) * 4 + 2]);
			rowPointers[y][x*4+3] = (int)(array[(x + width * y) * 4 + 3]);
		}
	}

	png_write_image(png, rowPointers);
	png_write_end(png, info);

	delete [] rowPointers;
	png_destroy_write_struct(&png, &info);
	fclose(filePointer);
}

int PNGHandler::getLoadedWidth() {
	return loadedWidth;
}

int PNGHandler::getLoadedHeight() {
	return loadedHeight;
}
