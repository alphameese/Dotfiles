#ifndef ArgHandler_H
#define ArgHandler_H

#include "Filter.h"
#include <vector>
#include <iostream>
#include <string>
using std::vector; using std::string;

class ArgHandler
{
public:
	ArgHandler();
	virtual ~ArgHandler();
	void parse(int count, char* array[]);
	void setFilter(Filter* newFilter);
	void applyFilters();
	string getSaveLocation();
	string getLoadLocation();

private:
	int findArg(string input);
	bool startsWith(string input, string prefix);

	void blur(string nextArg);
	void help();
	void pixelate(string nextArg);
	void stamp(string nextArg);
	void target(string nextArg);

	static const int NUM_CMD;

	static const int BLUR;
	static const int HELP;
	static const int PIXELATE;
	static const int STAMP;
	static const int TARGET;

	int blurAmount, pixelateAmount;
	string saveLocation, loadLocation;

	struct Command {
		string shortForm;
		string longForm;
		string inputForm;
		string description;
		string defaultVal;
	};

	Command* cmdList;
	Filter* filter;
};

#endif
