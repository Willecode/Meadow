#include "application.h"
#include <memory>

int main()
{
	Application app = Application();
	if(!app.appFailed)
		app.run();
	return 0;
}