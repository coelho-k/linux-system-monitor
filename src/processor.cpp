#include "processor.h"
#include "linux_parser.h"
using namespace LinuxParser;

// Return the aggregate CPU utilization
float Processor::Utilization() { 
    return LinuxParser::CpuUtilization();
 }