#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include<iostream>

#include "linux_parser.h"
#include <unistd.h>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() {
  // Total used memory = (MemTotal - MemFree ) / MemTotal
  float total = -1.0, free =-1.0;
  string key;
  string val;
  string line;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line) && (total == -1.0 || free == -1.0) )
    {
    std::istringstream linestream(line);
    linestream >> key >> val;
    if (key  == "MemTotal:") 
      total = std::stof(val);
    else if (key == "MemFree:")
      free =  std::stof(val); 
  }
  }
     return (total - free) / total; 
   }

long LinuxParser::UpTime() {
  string suspend, idle;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> suspend >> idle;
  }
  return std::stoi(suspend) + std::stoi(idle);
  }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization(int pid) { 
  vector<string> items;
  string key;
  long int val=-1;
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream,line);
    std::istringstream linestream(line);
    for (int i=1;i <= 22;i++)
    {
      linestream >> key ;
    if(i == 14 || i == 15 || i == 16 || i == 17 || i == 22){ // the 22nd item in the line is number of clock for this process
       items.push_back(key);
    }
    }
    }
  
  return items;
}

int LinuxParser::TotalProcesses() {   
  string process = "-1",key;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line))
    {
    std::istringstream linestream(line);
    linestream >> key >> process;
    if (key == "processes") break;
    }
  }
  return std::stoi(process);
  }


int LinuxParser::RunningProcesses() {
  string process = "-1",key;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line))
    {
    std::istringstream linestream(line);
    linestream >> key >> process;
    if (key == "procs_running") break;
    }
  }
  return std::stoi(process);
}

string LinuxParser::Command(int pid) { 
  string Cmd = "No_Command_For_This_PID",key;
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream,line);
    std::istringstream linestream(line);
    linestream >> key;
    if (key != "") Cmd = key;
  }
  return Cmd;
  }

string LinuxParser::Ram(int pid) {
  string key;
  string val= "-1";
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)){
    std::istringstream linestream(line);
    linestream >> key >> val; // val is in KByte
    if (key  == "VmSize:") 
       break;
    }
  }
   int Mega = stoi(val) / 1024; // MG Byte 
   return to_string(Mega);
 }

string LinuxParser::Uid(int pid) {
  
  string key;
  string val= "-1";
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)){
    std::istringstream linestream(line);
    linestream >> key >> val;
    if (key  == "Uid:") 
       break;
    }
  }
   return val;
  }

// the format of each line is root:x:0:0:root:/root:/bin/bash
string LinuxParser::User(int pid) {
  string userId = Uid(pid); 
  string line,userName="Not_Found";
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)){
    auto one = line.find(':');
    auto two = line.find(':' , one+1);
    auto three = line.find(':' , two+1);
    string temp = line.substr(two+1,three - two -1);
    if (temp  == userId)
    {
       userName = line.substr(0,one); 
       break;
    }
    }
  }
  
  return userName;
  }

// i guess we have to calculate the currentTime - Starttime(22nd); the description says 
// 22nd is the start time, so for uptime we have to find how long it is UP, to now. 
long LinuxParser::UpTime(int pid) { 
  string key;
  long int val=-1;
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream,line);
    std::istringstream linestream(line);
    for (int i=0;i< 22;i++) // the 22nd item in the line is number of clock for this process
       linestream >> key;
    val = stoi(key);

    }
  
  val = val / sysconf(_SC_CLK_TCK);
  val = UpTime() - val; // no idea, just my understanding
  return val;
  }
