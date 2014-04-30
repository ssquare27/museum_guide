/*****************************************************************************
 * File: gstaudio.h
 * Author: Steve Square
 * Created: 11th January 2014
 * Updated: 30th April 2014
 * Description
 ****************************************************************************/
#ifndef GSTAUDIO_H
#define GSTAUDIO_H
#include "gstcommon.h"
#include <string>
#include <iostream>

/*****************************************************************************
 * class GstAudio
 * Description: Main gstreamer audio class to provide streaming capabilities.
 *				This class is designed as a framework for the Museum Guide
 *				project server implementation.
 ****************************************************************************/
class GstAudio 
{
 //Private class methods.
 private:
  int maxElements;
  struct gstver version;
  gstElementList *elementHead;
  //Private functions used in implementation, 
  void addElementList(GstElement*, std::string, std::string, int);
 //Public Class Methods.
 public:
  GstAudio(void);
  ~GstAudio(void);
  void printVersion();
  void createElement(std::string, std::string, int);
  void printElementList();
  GstElement *findE(std::string);
  void playbackAuto();
  void linkAllElements();
  void addBin(GstElement*, GstElement**);
  void addToArray(GstElement**);
};

#endif
