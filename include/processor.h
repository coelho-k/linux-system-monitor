#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  

  
 private:
 // TODO: Store previous values for CPU Utilization
  std::vector<float> previous_cpu_elements = {};
    

};

#endif