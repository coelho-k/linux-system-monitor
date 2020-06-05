#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
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

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
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

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string line;
  string key;
  string value;
  float total = 0;
  float free = 0;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), ':', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
          if (key == "MemTotal") {
            std::replace(value.begin(), value.end(), '_', ' ');
            total = std::stof(value);
          }
          if (key == "MemFree") {
            std::replace(value.begin(), value.end(), '_', ' ');
            free = std::stof(value);
          }
          if (total != 0 && free != 0){
            return (total - free) / total;
          }
      }
    }
  }
  return 0.0;
}

// Read and return the system uptime
long int LinuxParser::UpTime() { 
  string line;
  string up_time = "";
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()){
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> up_time;
    return (std::stol(up_time));
  }
  return 0.0;
 }


// Read and return CPU utilization
float LinuxParser::CpuUtilization() { 
  float cpu_util = 0;
  float idle=0, iowait=0, user=0,nice=0,system=0,irq=0,softirq=0,steal=0;
  string line;
  string cpu_first;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()){
    std::getline(filestream, line);
    std::replace(line.begin(), line.end(), '"', ' ');
    std::istringstream linestream(line);
    linestream >>cpu_first>>user>>nice>>system>>idle>>iowait>>irq>>softirq>>steal;
    float idle_t = idle + iowait;
    float nonidle_t = user+system+irq+softirq+steal;
    float total_t = idle_t + nonidle_t;
    cpu_util = nonidle_t / total_t;
    return cpu_util;
  }
  return cpu_util; 

}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()){
    while (std::getline(filestream, line)){
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "processes"){
        return std::stoi(value);
      }
    }
  }
  return 0;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()){
    while (std::getline(filestream, line)){
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "procs_running"){
        return std::stoi(value);
      }
    }
  }
  return 0;
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  string line;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()){
    std::getline(filestream, line);
    return line;
  } 
  return ""; 
}

// Read and return the memory used by a process
// Returning in KB right now ## CHECK FOR LATER
string LinuxParser::Ram(int pid) { 
  string line;
  string key;
  string value; 
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while(std::getline(filestream, line)){
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "VmSize:"){
        int tmp= (100 * std::stof(value)/1024+0.5); 
        return to_string(tmp/100)+"."+((tmp%100 <10) ? "0"+to_string(tmp%100):to_string(tmp%100));
      }
    }
  }
  return "";
}

// TODO: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  string line;
  string key;
  string value; 
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while(std::getline(filestream, line)){
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "Uid:"){
        return value;
      }
    }
  }
  return "";
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) { 
  string line;
  std::string username, x, user_id;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()){
    while (std::getline(filestream, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream lineStream(line);
      lineStream >> username >> x >> user_id; 
      if (user_id == LinuxParser::Uid(pid)) {
        return username;
      }
    }
  }
  return ""; 
}

// Read and return the uptime of a process
long int LinuxParser::ProcessUpTime(int pid) { 
  string line;
  string u_time;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (filestream.is_open()){
    std::getline(filestream, line);
    std::istringstream linestream(line);

    for (int i=0; i<14; i++){
      linestream >> u_time;
    }
    return (std::stol(u_time) / sysconf(_SC_CLK_TCK));
  }
  return 0.0;
}

vector<long> LinuxParser::GetCpuUtilData(int pid){
    string line;
    string stime="", cutime="", cstime="", utime="";
    vector<long> data = {};
    std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
    if (filestream.is_open()){
    std::getline(filestream, line);
    std::replace(line.begin(), line.end(), '"', ' ');
    std::istringstream linestream(line);
    
    for (int i=0; i<15; i++)
      linestream >> utime;
    
    linestream >> stime >> cutime >> cstime;
    data.push_back(std::stol(utime));
    data.push_back(std::stol(stime));
    data.push_back(std::stol(cutime));
    data.push_back(std::stol(cstime));

    /*
    // TEMP FIX FIND BETTER WAY TO DO THIS !!!!!!!!
    std::istringstream lstream(line);
    for (int i=0; i<22; i++)
      lstream >> starttime;
    data.push_back(std::stol(starttime));
    */
    return data;
  }
  return data;
}