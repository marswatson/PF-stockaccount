#include "myFunction_SiyuanZhong.h"
#include <stdio.h>
#include <time.h>
#include <string>

using namespace std; 

string myDate(){
	time_t rawtime;
	struct tm* timeinfo;
	char timE[80];
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(timE, 80, "%x", timeinfo);
	return timE;
}

string myTime(){
	time_t rawtime;
	struct tm* timeinfo;
	char timE[80];
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(timE, 80, "%X", timeinfo);
	return timE;
};
