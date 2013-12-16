#include "main.h"
using namespace std;

void gstaudio::print_version()
{
  gst_version (&version.major, &version.minor, &version.micro, &version.nano);
  printf ("This program is linked against GStreamer %d.%d.%d %d\n",
	  version.major, version.minor, version.micro, version.nano);
}
