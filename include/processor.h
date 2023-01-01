#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  

 private:
  float user, nice,system,idle,iowait,irq,softirq,steal,guest,guest_nice;
};

#endif