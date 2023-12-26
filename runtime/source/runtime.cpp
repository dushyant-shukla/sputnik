#include "runtime.hpp"
#include <graphics/api/renderer.h>

#include <glad/glad.h>
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
    // dll_path             = dll_path.substr(0, dll_path.find_last_of("\\/")) + "\\editor.dll";
    dll_path = dll_path.substr(0, dll_path.find_last_of("\\/")) + "\\graphics-sandbox.dll";
    std::cout << "Loading DLL: " << dll_path << "\n";

    dll = LoadLibraryA("graphics-sandbox.dll");
    if(dll == NULL)
    {
        std::cout << "Failed to load DLL\n";
        return;
    }

    // Get the function pointers
    initialize = (InitializeFunction)GetProcAddress(dll, "initialize");
    shutdown   = (ShutdownFunction)GetProcAddress(dll, "shutdown");
    update     = (UpdateFunction)GetProcAddress(dll, "update");

    // Initialize the DLL plugin
    initialize(this);

    //m_animated_model = Model::LoadModel("../data/assets/Woman.gltf");
    m_diff_texture   = std::make_shared<OglTexture2D>("../data/assets/Woman.png", false);

    m_shader_program = std::make_shared<OglShaderProgram>();
    m_shader_program->addShaderStage("../data/shaders/simple.vert");
    m_shader_program->addShaderStage("../data/shaders/simple.frag");
    m_shader_program->configure();
}

//void Runtime::Run()
//{
//    while(m_is_running)
//    {
//        float          time      = (float)GetTime();
//        core::TimeStep time_step = time - m_last_frame_time;
//        m_last_frame_time        = time;
//
//        graphics::api::Renderer::SetClearColor(0.16f, 0.16f, 0.16f, 1.00f);
//        graphics::api::Renderer::Clear();
//
//        if(!m_is_minimized)
//        {
//            graphics::api::Renderer::Update(time_step);
//
//            m_editor->BeginFrame(); // Todo:: Editor dll
//            for(const std::shared_ptr<core::Layer>& layer : m_application_layer_stack)
//            {
//                layer->OnPreUpdate(time_step);
//            }
//
//            for(const std::shared_ptr<core::Layer>& layer : m_application_layer_stack)
//            {
//                layer->OnUpdate(time_step);
//            }
//
//            update(time_step); // Todo:: Editor dll
//
//            for(const std::shared_ptr<core::Layer>& layer : m_application_layer_stack)
//            {
//                layer->OnPostUpdate(time_step);
//            }
//            m_editor->EndFrame(); // Todo:: Editor dll
//
//            // m_editor->Update(time_step); // Todo:: Editor dll
//        }
//
//        m_input_manager->LateUpdate(time_step);
//        graphics::api::Renderer::LateUpdate(time_step);
//    }
//}

Runtime ::~Runtime()
{
    // Shutdown the DLL plugin
    shutdown();
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

void Runtime::createShaderProgram()
{
    // m_static_program = std::make_shared<OglShaderProgram>();
    // m_static_program->addShaderStage("../data/shaders/glsl/blinn_phong.vert");
    // m_static_program->addShaderStage("../data/shaders/glsl/blinn_phong.frag");
    // m_static_program->configure();

    // ENGINE_INFO("Shader program created through runtime.");

    glEnable(GL_DEPTH_TEST);
    const auto& editor_camera = sputnik::graphics::api::EditorCamera::GetInstance();
    mat4        projection    = editor_camera->GetCameraPerspective();
    mat4        view          = editor_camera->GetCameraView();
    mat4        model{};
    model = model.rotate({0.0, 1.0, 0.0}, -90.0f * kDegToRad);

    m_shader_program->bind();
    m_shader_program->setMat4("projection", projection);
    m_shader_program->setMat4("model", model);
    m_shader_program->setMat4("view", view);

    m_diff_texture->bind(0);
    m_shader_program->setInt("diffuse", 0);

    m_shader_program->setFloat3("light", {0.0f, 5.0f, 5.0f});

    m_animated_model->Draw();

    m_shader_program->unbind();
    glDisable(GL_DEPTH_TEST);
}

void Runtime::DrawModel(sputnik::graphics::api::Model* model)
{
    glEnable(GL_DEPTH_TEST);
    const auto& editor_camera = sputnik::graphics::api::EditorCamera::GetInstance();
    mat4        projection    = editor_camera->GetCameraPerspective();
    mat4        view          = editor_camera->GetCameraView();
    mat4        model_mat{};
    model_mat = model_mat.rotate({0.0, 1.0, 0.0}, -90.0f * kDegToRad);

    m_shader_program->bind();
    m_shader_program->setMat4("projection", projection);
    m_shader_program->setMat4("model", model_mat);
    m_shader_program->setMat4("view", view);

    m_diff_texture->bind(0);
    m_shader_program->setInt("diffuse", 0);

    m_shader_program->setFloat3("light", {0.0f, 5.0f, 5.0f});

    model->Draw();

    m_shader_program->unbind();
    glDisable(GL_DEPTH_TEST);
}

void Runtime::DrawModel()
{
    glEnable(GL_DEPTH_TEST);
    const auto& editor_camera = sputnik::graphics::api::EditorCamera::GetInstance();
    mat4        projection    = editor_camera->GetCameraPerspective();
    mat4        view          = editor_camera->GetCameraView();
    mat4        model{};
    model = model.rotate({0.0, 1.0, 0.0}, -90.0f * kDegToRad);

    m_shader_program->bind();
    m_shader_program->setMat4("projection", projection);
    m_shader_program->setMat4("model", model);
    m_shader_program->setMat4("view", view);

    m_diff_texture->bind(0);
    m_shader_program->setInt("diffuse", 0);

    m_shader_program->setFloat3("light", {0.0f, 5.0f, 5.0f});

    m_animated_model->Draw();

    m_shader_program->unbind();
    glDisable(GL_DEPTH_TEST);
}

void Runtime::LoadModel(const std::string& path)
{
    m_animated_model = Model::LoadModel(path);
}

} // namespace sputnik::runtime
