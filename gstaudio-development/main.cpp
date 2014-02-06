#include "main.h"
#include "gstaudio.h"
using namespace std;

int main (int argc, char *argv[])
{
  GstAudio aplayer;
  gst_init(NULL, NULL);
  aplayer.printVersion();
  aplayer.createElement("audiotestsrc","source", 1);
  aplayer.createElement("audioconvert","convert", 2);
  aplayer.createElement("audioresample","resample", 3);
  aplayer.createElement("alsasink","sink", 4);
  
  aplayer.printElementList();

  aplayer.playbackAuto();
}
