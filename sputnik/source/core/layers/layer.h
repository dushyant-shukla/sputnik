#pragma once

namespace sputnik::core::layer
{

class Layer
{

public:
    Layer(const std::string& name);
    virtual ~Layer();

    virtual void OnAttach();
    virtual void OnDetach();
    virtual void OnUpdate();
    virtual void OnEvent();
    virtual void OnUpdateUI();

protected:
    const std::string& GetName() const;

protected:
    std::string m_name;
};

} // namespace sputnik::core::layer