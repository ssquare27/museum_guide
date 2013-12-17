//#include "main.h"
#include "gstaudio.h"
using namespace std;

GstAudio::GstAudio(void)
{

}

void GstAudio::printVersion()
{
  gst_version (&version.major, &version.minor, &version.micro, &version.nano);
  printf ("This program is linked against GStreamer %d.%d.%d %d\n",
	  version.major, version.minor, version.micro, version.nano);
}
/*****************************************************************************
 *creates an GstElement and insertes this to the head linked list
 ****************************************************************************/
void GstAudio::createElement(string elementName, string elementType)
{
  GstElement *newElement;
  
  //create a new gstreamer element can be a source, bin, decoder, sink
  newElement = gst_element_factory_make(elementName.c_str(), 
					elementType.c_str());
  addElementList(newElement, elementName, elementType);
}

void GstAudio::addElementList(GstElement *element, string elementName, string elementType)
{
  gstElementList *temp; //needed to hold information proir to copy.
  temp = (gstElementList*)malloc(sizeof(gstElementList));
  //assign data to the temp node in linked list
  temp->elementName = elementName;
  temp->elementType = elementType;
  temp->element = element;
  //add this node to the linked list
  GstAudio::elementHead = temp;
}
