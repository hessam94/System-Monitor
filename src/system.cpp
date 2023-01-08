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
vector<Process>& System::Processes()
{
  vector<int> pids = LinuxParser::Pids();
  processes_.clear();
  for (long unsigned int i = 0; i < pids.size(); i++) {
    if (LinuxParser::Ram(pids[i]) != "0") {
      Process process(pids[i]);
      processes_.push_back(process);
    }
  }

   for (auto& p: processes_)
     p.CpuUtilization();

  std::sort(processes_.rbegin(), processes_.rend());
  return processes_;

}

std::string System::Kernel() { return LinuxParser::Kernel(); }

float System::MemoryUtilization() { return LinuxParser::MemoryUtilization() ; }

std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() { LinuxParser::TotalProcesses(); }

long int System::UpTime() { return LinuxParser::UpTime(); }
