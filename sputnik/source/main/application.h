#pragma once

namespace sputnik::main
{
class Application
{

public:
    inline Application(){};
    inline virtual ~Application() {}
    inline virtual void Initialize() {}
    inline virtual void Update(float delta_time) {}
    inline virtual void Render(float aspect_ratio) {}
    inline virtual void RenderUI(float aspect_ratio) {}
    inline virtual void Shutdown() {}

private:
    Application(const Application& other);
    Application& operator=(const Application& other);
};

Application* CreateApplication(); // Must be defined by classes inheriting from this class
} // namespace sputnik::main
