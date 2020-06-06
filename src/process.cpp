#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <linux_parser.h>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

using namespace LinuxParser; 

// TODO: Return this process's ID
int Process::Pid() { 
    return pid_; 
}

Process::Process(int pid) : pid_(pid){
    CpuUtilization();
};

// Return this process's CPU utilization
float Process::CpuUtilization() { 
    // [ utime, stime, cutime, cstime]
    vector<long> time_info = LinuxParser::GetCpuUtilData(pid_);
    long int up_time = LinuxParser::ProcessUpTime(pid_);
    long hertz = sysconf(_SC_CLK_TCK);
    float total_time = (time_info[0] + time_info[1] + time_info[2] + time_info[3]) / hertz;
    cpu_util = ((total_time / up_time) );
    return cpu_util;
 }

// TODO: Return the command that generated this process
string Process::Command() { 
    return LinuxParser::Command(pid_); 
    //return "";
}

// TODO: Return this process's memory utilization
string Process::Ram() { 
    return LinuxParser::Ram(pid_); 
    //return "";
}

// TODO: Return the user (name) that generated this process
string Process::User() { 
    return LinuxParser::User(pid_);
    //return ""; 
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { 
    return LinuxParser::ProcessUpTime(pid_);    
}

// TODO: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const { 
    return (cpu_util > a.cpu_util);    
}