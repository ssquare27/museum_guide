#include "main.h"
#include "gstaudio.h"
using namespace std;

int main (int argc, char *argv[])
{
  GstAudio aplayer;
  gst_init(NULL, NULL);
  aplayer.printVersion();
  aplayer.createElement("filesrc","source");
  aplayer.createElement("filesink","sink");
  aplayer.printElementList();
}
