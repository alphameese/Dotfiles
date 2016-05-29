#include "ArgHandler.h"
#include "ScreenShot.h"
#include "Filter.h"
#include "PNGHandler.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <ctime>
#include <exception>

using std::cout; using std::endl;
using std::string;

int main(int argc, char* argv[])
{
	int exitcode = 0;

	ArgHandler* handler = new ArgHandler();
	ScreenShot* ss = new ScreenShot();
	Filter* filter = new Filter();

	PNGHandler saver;
	clock_t time1, time2, timediff;
	ScreenShot* filteredShot;

	try {
		handler->parse(argc, argv);
		handler->setFilter(filter);
	} catch(string *e) {
		cout << "Error: " << *e << endl;
		exitcode = 1;
		goto cleanup;
	}


	// Do and time work
	time1 = clock();
	ss->takeShot();

	filter->setScreenShot(ss);
	try {
		handler->applyFilters();
	} catch(string *e) {
		cout << "Error: " << *e << endl;
		exitcode = 1;
		goto cleanup;
	}

	time2 = clock();
	timediff = time2 - time1;
	cout << "Time: " << ((float)timediff) / CLOCKS_PER_SEC << " seconds" << endl;

	// Save screenshot
	filteredShot = filter->getFilteredScreenShot();
	saver.save(filteredShot->getPixelArray(), filteredShot->getWidth(), 
	           filteredShot->getHeight(), handler->getSaveLocation());

	cout << "Image saved to " << handler->getSaveLocation() << endl;

	// Exit safely to avoid segfault
cleanup:
	delete handler;
	delete filter;
	delete ss;

	cout << " - Exiting with exit status " << exitcode << endl;

	return exitcode;
}