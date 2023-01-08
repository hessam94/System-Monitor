#include <string>

#include "format.h"

using std::string;


string Format::ElapsedTime(long seconds) {  
int se = seconds % 60;
int minute = seconds / 60;
int hour = minute / 60;
minute %= 60;

string sec = std::to_string(se);
sec.insert(0, 2 - sec.length(), '0');

string mins = std::to_string(minute);
mins.insert(0, 2 - mins.length(), '0');

string hrs = std::to_string(hour);
hrs.insert(0, 2 - hrs.length(), '0');

//return std::to_string(hour) + ":" + std::to_string(minute) + ":" + std::to_string(se);
return hrs+ ":" + mins + ":" + sec;
}