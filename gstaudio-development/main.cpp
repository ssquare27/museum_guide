#include "main.h"
#include "gstaudio.h"
using namespace std;

int main (int argc, char *argv[])
{
  GstAudio aplayer;
  gst_init(NULL, NULL);
  aplayer.printVersion();
  // aplayer.createElement("audiotestsrc","source", 1);
  // aplayer.createElement("audioconvert","convert", 2);
  // aplayer.createElement("audioresample","resample", 3);
  aplayer.createElement("filesrc", "source", 1);
  aplayer.setOptions("filesrc", "location", "/home/ssquare/test.mp3");
  aplayer.createElement("mad", "convert", 2);
  aplayer.createElement("alsasink","sink", 3);
  
  aplayer.printElementList();

  //aplayer.seek(100000000000);
  
  aplayer.playbackAuto();
}
