#ifndef GSTELEMENTLIST_H
#define GSTELEMENTLIST_H
#include <gst/gst.h>
#include <string>
typedef struct gstElementList
{
  GstElement *element;
  string elementName;
  string elementType;
  node *next;
};

