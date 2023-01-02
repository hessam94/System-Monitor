#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;
/*You need to complete the mentioned TODOs in order to satisfy the rubric criteria "The student will be able to extract and display basic data about the system."

You need to properly format the uptime. Refer to the comments mentioned in format. cpp for formatting the uptime.*/


// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// This is not completely correct, what happens if a process get killed, so we need to create a new processes every time and fill it
// but for this project i guess it works.
vector<Process>& System::Processes() {
    vector<int> Pids = LinuxParser::Pids();
    set<int> PidsSet(Pids.begin(), Pids.end());
    for (auto& pr : processes_)
    {
        auto it = PidsSet.find(pr.Pid());
        if (it != PidsSet.end())
            PidsSet.erase(it);
    } 

    // rest of the processes, the new ones that was not among them
    for (auto& pr : PidsSet)
    {
        Process p(pr);
        processes_.push_back(p);
    }

    // this will set the cpuUtil property for all live processes , so there will not be any process in the list
    // with empty cpuutil_. because we overloaded the operator < on Cpuutil_  

    // the problem is here, having thsi code gives me a runtime error, if I comment this two lines then there is no error
    // but no meaningful soritng later we have
    for (auto& pr: processes_)
       pr.CpuUtilization(); 


    // simple sort for proceses to have bigger cpuUtil in the beginning 
     for (int i=0;i< processes_.size();i++)
        for(int j= i+1;j< processes_.size();j++)
            if (processes_[i].CpuUtil_ < processes_[j].CpuUtil_)
                std::swap(processes_[i] , processes_[j]);

     //sort(processes_.begin() , processes_.end());
     //reverse(processes_.begin() , processes_.end());
     //sort(processes_.begin() , processes_.end() , std::greater<Process>()) ;

     return processes_; 
     }

std::string System::Kernel() { return LinuxParser::Kernel(); }

float System::MemoryUtilization() { return LinuxParser::MemoryUtilization() ; }

std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() { LinuxParser::TotalProcesses(); }

long int System::UpTime() { return LinuxParser::UpTime(); }
