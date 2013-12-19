#ifndef GSTAUDIO_H
#define GSTAUDIO_H
#include "gstcommon.h"
#include <string>
#include <iostream>
class GstAudio 
{
 private:
  struct gstver version;
  gstElementList *elementHead;
  //Private functions used in implementation, 
  void addElementList(GstElement*, std::string, std::string);
 public:
  GstAudio(void);
  //public methods that show an action in abstract.
  void printVersion();
  void createElement(std::string, std::string);
  void printElementList();
};

#endif
