#include <memory>
#include "application.h"

int main()
{
	Application app = Application();
	if(!app.appFailed)
		//app.run();
		app.runGPUBenchmarked();
	return 0;
}