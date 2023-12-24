#include "runtime.hpp"

#include <Windows.h>

namespace sputnik::runtime
{

HINSTANCE dll = NULL;

typedef void (*InitializeFunction)(sputnik::main::Application* const);
InitializeFunction initialize = NULL;

typedef void (*ShutdownFunction)();
ShutdownFunction shutdown = NULL;

typedef void (*UpdateFunction)(float);
UpdateFunction update = NULL;

Runtime::Runtime(const std::string& name) : sputnik::main::Application(name)
{
    std::cout << "Runtime initializing...\n";

    // Load the DLL
    char path[MAX_PATH];
    std::memset(path, 0, MAX_PATH);
    DWORD result = GetModuleFileNameA(NULL, path, MAX_PATH);
    if(result == 0)
    {
        std::cout << "Failed to get module file name\n";
        return;
    }
    std::string dll_path = std::string(path);
    dll_path             = dll_path.substr(0, dll_path.find_last_of("\\/")) + "\\editor.dll";
    std::cout << "Loading DLL: " << dll_path << "\n";

    dll = LoadLibraryA("editor.dll");
    if(dll == NULL)
    {
        std::cout << "Failed to load DLL\n";
        return;
    }

    // Get the function pointers
    initialize = (InitializeFunction)GetProcAddress(dll, "initialize");
    shutdown   = (ShutdownFunction)GetProcAddress(dll, "shutdown");
    update     = (UpdateFunction)GetProcAddress(dll, "update");

    // Call the functions
    initialize(this);
    shutdown();
    update(0.0f);
}

Runtime ::~Runtime()
{

    // TODO: Unload the DLL
    BOOL result = FreeLibrary(dll);
    if(result == 0)
    {
        std::cout << "Failed to unload DLL\n";
        return;
    }
    std::cout << "DLL unloaded\n";
    std::cout << "Runtime shutting down...\n";
}

} // namespace sputnik::runtime
