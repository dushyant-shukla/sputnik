#include "pch.h"

#include "input_manager.h"
#include "core/core.h"

namespace sputnik::engine::api
{
InputManager* InputManager::GetInstance()
{
    static InputManager input;
    return &input;
}

InputManager::InputManager()
    : m_cursor_position(ramanujan::Vector2(0.0f, 0.0f))
    , m_cursor_max_position(ramanujan::Vector2(0.0f, 0.0f))
{
    memset(m_previous_keyboard_state, false, sizeof(bool) * (KEY_LAST + 1));
    memset(m_current_keyboard_state, false, sizeof(bool) * (KEY_LAST + 1));

    memset(m_previous_mouse_button_state, false, sizeof(bool) * (MOUSE_BUTTON_LAST + 1));
    memset(m_current_mouse_button_state, false, sizeof(bool) * (MOUSE_BUTTON_LAST + 1));
}

InputManager::~InputManager() {}

void InputManager::Update(const sputnik::core::TimeStep& time_step) {}

void InputManager::LateUpdate(const sputnik::core::TimeStep& time_step)
{
    for(int i = 0; i < KEY_LAST + 1; ++i)
    {
        m_previous_keyboard_state[i] = m_current_keyboard_state[i];
    }
    for(int i = 0; i < MOUSE_BUTTON_LAST + 1; ++i)
    {
        m_previous_mouse_button_state[i] = m_current_mouse_button_state[i];
    }
}

void InputManager::UpdateKeyboardState(const int& key, const bool& state, const bool& repeat)
{
    m_current_keyboard_state[key] = state;
}

void InputManager::UpdateMouseButtonState(const int& button, const bool& state)
{
    m_current_mouse_button_state[button] = state;
}

bool InputManager::IsKeyPressed(const int& keyCode)
{
    SPUTNIK_ASSERT(keyCode < KEY_LAST, "Invalid key-code.");
    return m_current_keyboard_state[keyCode];
}

bool InputManager::IsKeyTriggered(const int& keyCode)
{
    SPUTNIK_ASSERT(keyCode < KEY_LAST, "Invalid key-code.");

    if(m_current_keyboard_state[keyCode] && !m_previous_keyboard_state[keyCode])
    {
        return true;
    }
    return false;
}

bool InputManager::IsKeyReleased(const int& keyCode)
{
    SPUTNIK_ASSERT(keyCode < KEY_LAST, "Invalid key-code.");

    if(!m_current_keyboard_state[keyCode] && m_previous_keyboard_state[keyCode])
    {
        return true;
    }
    return false;
}

bool InputManager::IsMouseButtonTriggered(const int& mouse_button)
{
    SPUTNIK_ASSERT(mouse_button < MOUSE_BUTTON_LAST, "Invalid mouse-button.");
    if(m_current_mouse_button_state[mouse_button] && !m_previous_mouse_button_state[mouse_button])
    {
        return true;
    }
    return false;
}

bool InputManager::IsMouseButtonPressed(const int& mouse_button)
{
    SPUTNIK_ASSERT(mouse_button < MOUSE_BUTTON_LAST, "Invalid mouse-button.");
    return m_current_mouse_button_state[mouse_button];
}

bool InputManager::IsMouseButtonReleased(const int& mouse_button)
{
    SPUTNIK_ASSERT(mouse_button < MOUSE_BUTTON_LAST, "Invalid mouse-button.");

    if(!m_current_mouse_button_state[mouse_button] && m_previous_mouse_button_state[mouse_button])
    {
        return true;
    }
    return false;
}

void InputManager::UpdateCursorPosition(const double& position_x, const double& position_y)
{
    m_cursor_position.x = (float)position_x;
    m_cursor_position.y = (float)position_y;
}

void InputManager::UpdateScrollOffset(const double& offset_x, const double& offset_y)
{
    m_scroll_offset.x = (float)offset_x;
    m_scroll_offset.y = (float)offset_y;
}

const ramanujan::Vector2& InputManager::GetCursorPosition() const
{
    return m_cursor_position;
}

ramanujan::Vector2 InputManager::GetScrollOffset()
{
    // TODO: This is only a quick fix for endless zoom problem. Think of a better solution.
    ramanujan::Vector2 temp = ramanujan::Vector2(0.0f);
    std::swap(temp, m_scroll_offset);
    return temp;
}

const double InputManager::GetMousePositionX()
{
    return m_cursor_position.x;
}

const double InputManager::GetMousePositionY()
{
    return m_cursor_position.y;
}
void InputManager::SetMouseMaxPositions(double x, double y)
{
    m_cursor_max_position.x = (float)x;
    m_cursor_max_position.y = (float)y;
}
const ramanujan::Vector2& InputManager::GetCursorMaxPosition() const
{
    return m_cursor_max_position;
}
const double InputManager::GetMouseMaxPositionX()
{
    return m_cursor_max_position.x;
}
const double InputManager::GetMouseMaxPositionY()
{
    return m_cursor_max_position.y;
}

const ramanujan::Vector2 InputManager::GetCursorViewPosition(float _x, float _y)
{
    float a = -m_cursor_max_position.x / m_cursor_max_position.y;
    float b = -a;

    float x = lerp(a, b, _x / m_cursor_max_position.x);
    float y = lerp(1.0f, -1.0f, _y / m_cursor_max_position.y);
    return ramanujan::Vector2(x, y);
}

const ramanujan::Vector2 InputManager::GetCursorViewPosition()
{
    float a = -m_cursor_max_position.x / m_cursor_max_position.y;
    float b = -a;

    float x = lerp(a, b, m_cursor_position.x / m_cursor_max_position.x);
    float y = lerp(1.0f, -1.0f, m_cursor_position.y / m_cursor_max_position.y);
    return ramanujan::Vector2(x, y);
}

float InputManager::lerp(float x, float y, float t)
{
    return x * (1.f - t) + y * t;
}

const ramanujan::experimental::vec2 InputManager::GetCursorNDCPosition()
{
    float x = ((2.0f * m_cursor_position.x) / m_cursor_max_position.x) - 1.0f;
    float y = 1.0f - (2.0f * m_cursor_position.y) / m_cursor_max_position.y;
    // float y = ((2.0f * m_cursor_position.y) / m_cursor_max_position.y) - 1.0f;
    return ramanujan::experimental::vec2(x, y);
}

} // namespace sputnik::engine::api