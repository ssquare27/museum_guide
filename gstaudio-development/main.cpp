#include "main.h"
#include "gstaudio.h"
using namespace std;

int main (int argc, char *argv[])
{
  GstAudio aplayer;
  gst_init(NULL, NULL);
  aplayer.printVersion();
}
