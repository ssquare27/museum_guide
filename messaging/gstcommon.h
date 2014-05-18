/*****************************************************************************
 * File: gstcommon.h
 * Author: Steve Square
 * Created: 11th January 2014
 * Updated: 30th April 2014
 * Description
 ****************************************************************************/
#ifndef GSTCOMMON_H
#define GSTCOMMON_H

#include <gst/gst.h>
#include <string>

//Struct for holding version info.
struct gstver {
    guint major;
    guint minor;
    guint micro;
    guint nano;
};

/*****************************************************************************
 * Struct gstElementList
 * Description: Holds the elements linked list. Contains element name, 
 *				element type and the element order number.
 ****************************************************************************/
struct gstElementList
{
  GstElement *element;
  std::string elementName;
  std::string elementType;
  int elementOrder;
  gstElementList *next;
};

#endif
