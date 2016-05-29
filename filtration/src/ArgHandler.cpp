#include "ArgHandler.h"

#include <stdexcept>

using std::cout; using std::endl;

const int ArgHandler::NUM_CMD = 5;

const int ArgHandler::BLUR     = 0;
const int ArgHandler::HELP     = 1;
const int ArgHandler::PIXELATE = 2;
const int ArgHandler::STAMP    = 3;
const int ArgHandler::TARGET   = 4;

ArgHandler::ArgHandler()
{
	cmdList = new Command[NUM_CMD]{{"-b", "--blur", "<int>", "apply blur, with optional amount specified;", "5"},
	                               {"-h", "--help", "", "print this help message", ""},
	                               {"-p", "--pixelate", "<int>", "pixelate, where SIZE is percentage of original;", "10%%"},
	                               {"-s", "--stamp", "<string>", "load an image into center of screen", ""},
	                               {"-t", "--target", "<string>", "set target save file", "screenshot.png"}};
	filter = 0;
	blurAmount = 0;
	pixelateAmount = 0;
	saveLocation = cmdList[TARGET].defaultVal;
	loadLocation = "";
}

ArgHandler::~ArgHandler()
{
	delete [] cmdList;
}

void ArgHandler::parse(int count, char* array[])
{
	for(int i = 1; i < count; i++) {
		int arg = findArg(array[i]);

		string nextArg = "";
		if(i+1 < count && array[i+1][0] != '-') {
			nextArg = array[i+1];
			i++;
		}

		switch(arg) {
			case BLUR : 
				blur(nextArg);
				break;
			case HELP : 
				help();
				break;
			case PIXELATE : 
				pixelate(nextArg);
				break;
			case STAMP:
				stamp(nextArg);
				break;
			case TARGET : 
				target(nextArg);
				break;
			default :
				string* err = new string("Invalid command \'");
				*err = *err + array[i]+ "\'";
				throw err;
		};
	}
}


void ArgHandler::setFilter(Filter* newFilter)
{
	filter = newFilter;
}

void ArgHandler::applyFilters()
{
	if(filter) {
		if(blurAmount) {
			cout << "Starting blur with value " << blurAmount << endl;
			filter->applyBlur(blurAmount);
		}

		if(pixelateAmount) {
			cout << "Starting pixelate with value " << pixelateAmount << endl;
		}

		if(loadLocation != "") {
			cout << "Loading stamp image from " << loadLocation << endl;
			filter->addStamp(loadLocation);
		}
	} else {
		throw new string("Handler's filter was not set");
	}
}

int ArgHandler::findArg(string input) {
	for(int i = 0; i < NUM_CMD; i++) {
		if(startsWith(input, cmdList[i].shortForm) || startsWith(input, cmdList[i].longForm)) {
			return i;
		}
	}
	return -1;
}

bool ArgHandler::startsWith(string input, string prefix) {
	if(prefix.length() > input.length()) {
		return false;
	}

	return input.substr(0,prefix.length()) == prefix;
}

void ArgHandler::blur(string nextArg)
{
	if(nextArg != "") {
		try {
			blurAmount = std::stoi(nextArg);
			// cout << "Prepping blur : " << blurAmount << endl;
		} catch(std::invalid_argument &e) {
			throw new string("Invalid argument passed to blur");
		}
	}
}

void ArgHandler::help()
{
	cout << "This is a help message" << endl;
}

void ArgHandler::pixelate(string nextArg)
{
	if(nextArg != "") {
		try {
			pixelateAmount = std::stoi(nextArg);
			// cout << "Prepping pixelate : " << pixelateAmount << endl;
		} catch(std::invalid_argument &e) {
			throw new string("Invalid argument passed to pixelate");
		}
	}
}

void ArgHandler::stamp(string nextArg)
{
	if(nextArg != "") {
		loadLocation = nextArg;
	}
}

void ArgHandler::target(string nextArg)
{
	if(nextArg != "") {
		saveLocation = nextArg;
	}
}

string ArgHandler::getSaveLocation()
{
	return saveLocation;
}

string ArgHandler::getLoadLocation()
{
	return loadLocation;
}

// void printHelp()
// {
// 	char const* name = "filterer";
// 	char const* format = "%7s, %-22s    %s\n";
// 	char const* blankformat = "%29s        %s\n";

// 	printf("Usage: %s [OPTIONS]... [FILES]...\n", name);
// 	printf("Arguments specified below can be applied with short or long options\n");
// 	printf("If a location is not specified, shot will be saved in the current directory\n");

// 	printf("\nNOTE: Only PNG format is currently supported\n");

// 	printf("\nOptions:\n");
// 	printf(format, "-b", "--blur(=AMOUNT)", "apply blur, with optional amount specified;");
// 	printf(blankformat, "", "default = 5");
// 	printf(format, "-h", "--help", "print this help message");
// 	printf(format, "-m", "--motionblur(=AMOUNT)", "motion blur, with specified amount;");
// 	printf(blankformat, "", "default = 5");
// 	printf(format, "-p", "--pixelate(=SIZE)", "pixelate, where SIZE is percentage of original;");
// 	printf(blankformat, "", "default = 10%");
// 	printf(format, "-t", "--target=NAME", "set target save file");
// 	printf(blankformat, "", "default = ~/Pictures/screenshot.png");
// 	printf(blankformat, "", "If above default fails, image saves to current directory");

// }