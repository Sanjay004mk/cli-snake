#pragma once
#include "app.h"

int main(int argc, char** argv)
{
	auto app = Core::CreateApplication({ argc, argv });
	app->Run();
	delete app;
	return 0;
}