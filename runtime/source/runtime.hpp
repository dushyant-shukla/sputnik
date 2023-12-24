#pragma once

#include <main/entry_point.h>
#include <main/application.h>
#include <core/time_step.h>

namespace sputnik::runtime
{

class Runtime : public sputnik::main::Application
{

public:
    Runtime(const std::string& name);
    ~Runtime();
};

} // namespace sputnik::runtime

sputnik::main::Application* sputnik::main::CreateApplication()
{
    return new sputnik::runtime::Runtime("Sputnik Engine Runtime");
}
