#ifndef GSTCOMMON_H
#define GSTCOMMON_H

#include <gst/gst.h>
#include <string>

struct gstver {
    guint major;
    guint minor;
    guint micro;
    guint nano;
};

struct gstElementList
{
  GstElement *element;
  std::string elementName;
  std::string elementType;
  int elementOrder;
  gstElementList *next;
};

#endif
