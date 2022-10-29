#pragma once

#ifndef _DEBUG
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#else
#pragma comment(linker, "/SUBSYSTEM:console")
#endif

#include "application.h"

extern sputnik::main::Application* sputnik::main::CreateApplication();

int main(int argc, char** argv)
{
    auto app = sputnik::main::CreateApplication();
    app->Run();
    delete app;
}
