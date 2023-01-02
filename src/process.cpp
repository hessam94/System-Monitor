#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;


Process::Process(int pid)
{
    Pid_ = pid;
}

int Process::Pid() { return Pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
     vector<string> items = LinuxParser::CpuUtilization(Pid());
     string utime = items[0];
     string stime = items[1];
     string cutime = items[2];
     string cstime  = items[3];
     string starttime = items[4];
     long uptime = LinuxParser::UpTime();
     auto total_time = stol(utime) + stol(stime);   
     total_time = total_time + stol(cutime) + stol(cstime);
     auto Hertz = sysconf(_SC_CLK_TCK);
     auto seconds = uptime - (stol(starttime) / Hertz) ;  
     auto cpu_usage = 100 * ((total_time / Hertz) / seconds);
     return cpu_usage;
    }

string Process::Command() { return LinuxParser::Command(Pid()); }

string Process::Ram() { return LinuxParser::Ram(Pid()); }

string Process::User() { return LinuxParser::User(Pid()); }

long int Process::UpTime() { 
    return LinuxParser::UpTime(Pid());
    }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const { return true; }