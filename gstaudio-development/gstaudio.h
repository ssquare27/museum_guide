#ifndef GSTAUDIO_H
#define GSTAUDIO_H
#include "gstcommon.h"
#include <string>

class GstAudio 
{
 private:
  struct gstver version;
  gstElementList *elementHead;
 public:
  GstAudio(void);
  void printVersion();
  void createElement(std::string, std::string);
  void addElementList(GstElement*, std::string, std::string);
};

#endif
