#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include<iostream>
#include<thread>
#include<chrono>

#include "processor.h"
#include "linux_parser.h"
using std::string;
using std::vector;
// TODO: Return the aggregate CPU utilization
vector<long long> cpuUtil() 
{   
    string user, nice,system,idle,iowait,irq,softirq,steal,guest,guest_nice;
    string key, line;
    long long CPU_Percentage = 0;
    vector<long long> info;
    std::ifstream stream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
    if (stream.is_open()) {
    while(std::getline(stream, line))
    {
    std::istringstream linestream(line);
    linestream >> key;
    if (key == "cpu")
    { 
        linestream >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;

        auto Idle = stoi(idle) + stoi(iowait);
        auto NonIdle = stoi(user) + stoi(nice) + stoi(system) + stoi(irq) + stoi(softirq) + stoi(steal);
        auto Total = Idle + NonIdle;
        info.push_back(Total);
        info.push_back(Idle);
        break;
    }
    }
    }
    return info;
}


float Processor::Utilization() {

    auto infoPre = cpuUtil();
    float CPU_Percentage;
    std::this_thread::sleep_for(std::chrono::milliseconds(500)); // sleep for one second
    auto info = cpuUtil();
    auto totalcur = info[0] , totalPre = infoPre[0];
    auto idlecur = info[1] , idlePre = infoPre[1];
    auto total = totalcur - totalPre;
    auto idle = idlecur - idlePre;
    if (total == idle) CPU_Percentage  = Utilization(); // to avoid 0 / 0 and also not to return 0;
    else
       CPU_Percentage = (float) ( total - idle  ) / total ;
    return CPU_Percentage;
  }
