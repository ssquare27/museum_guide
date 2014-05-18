/*****************************************************************************
 * File: gstaudio.cpp
 * Author: Steve Square
 * Created: 11th January 2014
 * Updated: 30th April 2014
 * Description
 ****************************************************************************/
#include "gstaudio.h"
#include "stdio.h"
using namespace std;

/*****************************************************************************
 * GstAudio::GstAudio
 * Parameters:  void
 * return: 		void
 *Description:  Constructor that initialises linked list
 ****************************************************************************/
GstAudio::GstAudio(void)
{
  elementHead = new(gstElementList);
  maxElements = 0;
  elementHead->elementName.clear();
  elementHead->elementType.clear();
  elementHead->elementOrder = 0;
  elementHead->element = NULL;
}

/*****************************************************************************
 * GstAudio::~GstAudio
 * Parameters:  void
 * return: 		void
 *Description:  Destructor for the GstAudio class.
 ****************************************************************************/
GstAudio::~GstAudio(void)
{

}
/*****************************************************************************
 * GstAudio::printVersion
 * Parameters:  void
 * return: 		void
 *Description:  Prints the current version of gstreamer
 ****************************************************************************/
void GstAudio::printVersion()
{
  gst_version (&version.major, &version.minor, &version.micro, &version.nano);
  printf ("This program is linked against GStreamer %d.%d.%d %d\n",
	  version.major, version.minor, version.micro, version.nano);
}

/*****************************************************************************
 * GstAudio::addElementList(string elementName, 
 *							string elementType, int elementOrder)
 * Parameters: 	string elementName, 
 *				string elementType, 
 *				int elementOrder
 * return: 		void
 *Description:  creates an GstElement and inserts this to the head linked list
 ****************************************************************************/
void GstAudio::createElement(string elementName, string elementType, int elementOrder)
{
  GstElement *newElement;
  
  // create a new gstreamer element can be a source, bin, decoder, sink
  newElement = gst_element_factory_make(elementName.c_str(), 
  					elementType.c_str());
  addElementList(newElement, elementName, elementType, elementOrder);
}
/*****************************************************************************
 * GstAudio::addElementList(GstElement *element, string elementName, 
 *							string elementType, int elementOrder)
 * Parameters: 	GstElement *element, 
 *				string elementName, 
 *				string elementType, 
 *				int elementOrder
 * return: void
 * Description: Adds a Gstreamer element to the objects linked list.
 ****************************************************************************/
void GstAudio::addElementList(GstElement *element, string elementName, string elementType, int elementOrder)
{
  //needed to hold information prior to copy.
  gstElementList *temp; 
  temp = new(gstElementList);
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
 * GstAudio::printElementList()
 * Parameters: void
 * return: void
 * Description: Simple print function that prints each element currently 
 *				created.
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

/*****************************************************************************
 * GstAudio::playbackAuto
 * Parameters: void
 * return: void
 * Description: Links all playback objects in the linked list and starts 
 *   			playback
 ****************************************************************************/
void GstAudio::playbackAuto()
{
  GstElement *pipeline = gst_pipeline_new("audio-player");
  GMainLoop *loop = g_main_loop_new (NULL, FALSE);;
  GstElement **elementArray = new(GstElement*[maxElements]);

  curPipe = pipeline;
  
  addBin(pipeline, elementArray);
  
  gst_element_set_state (pipeline, GST_STATE_PLAYING);

  seek(100000000000);
  g_print ("Running...\n");
  g_main_loop_run (loop);
}

/*****************************************************************************
 * GstAudio::addBin(GstElement *pipeline, GstElement **elementArray)
 * Parameters: 	GstElement *pipeline, 
 * 				GstElement **elementArray
 * return: void
 * Description: Adds a specialist playback bin, usually a pipeline
 ****************************************************************************/
void GstAudio::addBin(GstElement *pipeline, GstElement **elementArray) 
{
  int i;
  elementArray[0] = pipeline;
  
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

/*****************************************************************************
 * GstAudio::addBin(GstElement **eArray)
 * Parameters: 	GstElement *eArray
 * return: void
 * Description: 
 ****************************************************************************/
void GstAudio::addToArray(GstElement **eArray)
{
  gstElementList *currentElement;
  gstElementList *innerElement;
  int elementCount;
  currentElement = new(gstElementList);
  innerElement = new(gstElementList);
  //iterate through each element in the linked list till null (finished).
  currentElement = elementHead;
  innerElement = elementHead;
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
		innerElement = elementHead;
		}
    }
}

int GstAudio::seek(gint64 nanoTime)
{
  if(!gst_element_seek(curPipe, 1.0, GST_FORMAT_TIME, GST_SEEK_FLAG_FLUSH,
                         GST_SEEK_TYPE_SET, nanoTime,
                         GST_SEEK_TYPE_NONE, GST_CLOCK_TIME_NONE)) {
    g_print ("Seek failed!\n");
  }
}

int GstAudio::setOptions(string elementName, string option, string argument)
{
  GstElement* optionElement;
  optionElement = findE(elementName);
  if(optionElement != NULL)
    {
      cout << "adding file: " << argument << endl;
      g_object_set(G_OBJECT(optionElement), option.c_str(), argument.c_str(), NULL);
      return 0;
    }
  else
    {
      cout << "Failure to find " << elementName << endl;
    }
}
