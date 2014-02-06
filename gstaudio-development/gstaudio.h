#ifndef GSTAUDIO_H
#define GSTAUDIO_H
#include "gstcommon.h"
#include <string>
#include <iostream>
class GstAudio 
{
 private:
  int maxElements;
  struct gstver version;
  gstElementList *elementHead;
  //Private functions used in implementation, 
  void addElementList(GstElement*, std::string, std::string, int);
 public:
  GstAudio(void);
  ~GstAudio(void);
  //public methods that show an action in abstract.
  void printVersion();
  void createElement(std::string, std::string, int);
  void printElementList();
  GstElement *findE(std::string);
  void playbackAuto();
  void linkAllElements();
  void addBin(GstElement*);
  void addToArray(GstElement**);
};

#endif
