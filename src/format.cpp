#include <string>

#include "format.h"

using std::string;


string Format::ElapsedTime(long seconds) {  
int sec = seconds % 60;
int minute = seconds / 60;
int hour = minute / 60;
minute %= 60;
return std::to_string(hour) + ":" + std::to_string(minute) + ":" + std::to_string(seconds);
}