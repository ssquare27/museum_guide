//#include "main.h"
#include "gstaudio.h"
using namespace std;

GstAudio::GstAudio(void)
{
  elementHead = new(gstElementList);
  maxElements = 0;
  elementHead->elementName.clear();
  elementHead->elementType.clear();
  elementHead->elementOrder = 0;
  elementHead->element = NULL;
}

GstAudio::~GstAudio(void)
{
  /*while (elementHead->elementName != "")
    {
      gstElementList temp;
      //Remove all entrys up to the placeholder
      temp = elementHead;
      elementHead = elementHead->next;
      delete temp;
    }
    delete elementHead;*/
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
void GstAudio::createElement(string elementName, string elementType, int elementOrder)
{
  GstElement *newElement;
  
  // create a new gstreamer element can be a source, bin, decoder, sink
  newElement = gst_element_factory_make(elementName.c_str(), 
  					elementType.c_str());
  addElementList(newElement, elementName, elementType, elementOrder);
}

void GstAudio::addElementList(GstElement *element, string elementName, string elementType, int elementOrder)
{
  gstElementList *temp; //needed to hold information proir to copy.
  temp = new(gstElementList);
  //temp = (gstElementList*)malloc(sizeof(gstElementList));
  //assign data to the temp node in linked list
  temp->elementName = elementName;
  temp->elementType = elementType;
  temp->elementOrder = elementOrder;
  temp->element = element;
  temp->next = NULL;
  
  temp->next = elementHead;
  //add this node to the linked list
  GstAudio::elementHead = temp;
  if (elementOrder > maxElements) 
    {
      maxElements = elementOrder;
    }
}
/*****************************************************************************
 *Simple print function that prints each element currently created.
 ****************************************************************************/
void GstAudio::printElementList()
{
  gstElementList *currentElement;
  currentElement = new(gstElementList);
  //iterate through each element in the linked list till null (finished).
  currentElement = elementHead;
  while (currentElement->elementName != "")
    {
      if (currentElement != NULL)
	{
	  cout << currentElement->elementName << endl;
	  currentElement = currentElement->next;
	}
    }
}
/*****************************************************************************
 * GstAudio::FindE(std::string elementName)
 * Parameters: std::string elementName;
 * return: GstElement*
 * Description: Find algorithm for searching the linked list for elements.
 ****************************************************************************/
GstElement *GstAudio::findE(string elementName) 
{
  gstElementList *currentElement;
  currentElement = new(gstElementList);
  currentElement = elementHead;
  while (currentElement->elementName != "")
    {
    if (currentElement != NULL)
      {
	if (currentElement->elementName.compare(elementName) == 0)
	  {
	  return currentElement->element;
	  } 
	else 
	  {
	    currentElement = currentElement->next;
	  }
	  
      }
    }
  return NULL;
}
/*
 * play all auto
 */
void GstAudio::playbackAuto()
{
  GstElement *pipeline = gst_pipeline_new("audio-player");
  GMainLoop *loop = g_main_loop_new (NULL, FALSE);;

  
  
  // gst_bin_add_many (GST_BIN(pipeline), findE("audiotestsrc"), findE("audioconvert"),
  //		    findE("audioresample"), findE("alsasink"), NULL);
  addBin(pipeline);
  //gst_element_link (findE("audiotestsrc"), findE("alsasink"));
  //gst_element_link_many (findE("audioconvert"), findE("audioresample"), NULL);
  
  gst_element_set_state (pipeline, GST_STATE_PLAYING);

  g_print ("Running...\n");
  g_main_loop_run (loop);
}

void GstAudio::linkAllElements() 
{
  
}

void GstAudio::addBin(GstElement *pipeline) 
{
  GstElement **elementArray = new(GstElement*[maxElements]);
  int i;
  //GstElement *pipeline  = gst_pipeline_new("audio-player");
  elementArray[0] = pipeline;
  //gst_bin_add_many (GST_BIN
  addToArray(elementArray);
  for(i = 1; i <= maxElements; i++) 
    {
      gst_bin_add(GST_BIN(elementArray[0]), elementArray[i]);
    }
  for(i = 1; i < (maxElements); i++)
    {
      gst_element_link (elementArray[i], elementArray[i+1]);
    }
}

void GstAudio::addToArray(GstElement **eArray)
{
  gstElementList *currentElement;
  gstElementList *innerElement;
  int elementCount;
  currentElement = new(gstElementList);
  innerElement = new(gstElementList);
  //iterate through each element in the linked list till null (finished).
  currentElement = elementHead;
  elementCount = 1;
  
  while (currentElement->elementName != "")
    {
      if (currentElement != NULL)
	{
	  while (innerElement->elementName != "")
	    {
	      if (innerElement != NULL)
		{
		  if (elementCount == innerElement->elementOrder)
		    {
		      eArray[elementCount] = innerElement->element;
		      break;
		    }
		  else
		    {
		      innerElement = innerElement->next;
		    }
		}
	    }
	  elementCount++;
	  currentElement = currentElement->next;
	}
    }
}
