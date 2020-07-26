#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <chrono>
#include "GPIO/GPIOManager.h"
#include "GPIO/GPIOConst.h"


double HC_SR04() {
  double distance;
  GPIO::GPIOManager* gp = GPIO::GPIOManager::getInstance();
  
  int pin1 = GPIO::GPIOConst::getInstance()->getGpioByKey("P8_10");
  int pin2 = GPIO::GPIOConst::getInstance()->getGpioByKey("P8_9");
  
  gp->exportPin(pin1);
  gp->exportPin(pin2);
  
  gp->setDirection(pin1, GPIO::OUTPUT);
  gp->setDirection(pin2, GPIO::INPUT);
  
 
///Disparo do sensor
  gp->setValue(pin1, GPIO::LOW);
  usleep(2000);//microsegundos
  gp->setValue(pin1, GPIO::HIGH);
  usleep(10000);
  gp->setValue(pin1, GPIO::LOW);
      
  std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
      
  while(!gp->getValue(pin1)){
  	
  }
  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	  
  std::chrono::duration<double> elapsed = (end - begin).count()/1000000;
  
  gp->~GPIOManager();
	  
  distance = (elapsed/2)*340.29;
  return distance;
}
      
  

  

