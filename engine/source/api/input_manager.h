#pragma once

#include "core/core.h"
#include "mouse_button.h"
#include "keyboard_keys.h"
#include "core/time_step.h"

#include <vector2.h>

namespace sputnik::engine::api
{
class InputManager
{
public:
    NON_COPYABLE(InputManager);

    static InputManager* GetInstance();
    ~InputManager();

    void Update(const sputnik::core::TimeStep& time_step);
    void LateUpdate(const sputnik::core::TimeStep& time_step);

    void UpdateKeyboardState(const int& key, const bool& state, const bool& repeat);
    bool IsKeyPressed(const int& key_code);
    bool IsKeyTriggered(const int& key_code);
    bool IsKeyReleased(const int& key_code);

    template <class... Args>
    bool IsAllKeyNotPressed(const Args&... args)
    {
        bool             ret = false;
        std::vector<int> vec = {args...};
        for(unsigned i = 0; i < vec.size(); ++i)
        {
            ret = ret || IsKeyPressed(vec[i]);
        }
        return !ret;
    }

    void                      UpdateMouseButtonState(const int& button, const bool& state);
    bool                      IsMouseButtonTriggered(const int& mouse_button);
    bool                      IsMouseButtonPressed(const int& mouse_button);
    bool                      IsMouseButtonReleased(const int& mouse_button);
    void                      UpdateScrollOffset(const double& offset_x, const double& offset_y);
    void                      UpdateCursorPosition(const double& position_x, const double& position_y);
    ramanujan::Vector2        GetScrollOffset();
    const ramanujan::Vector2& GetCursorPosition() const;
    const double              GetMousePositionX();
    const double              GetMousePositionY();
    void                      SetMouseMaxPositions(double x, double y);
    const ramanujan::Vector2& GetCursorMaxPosition() const;
    const double              GetMouseMaxPositionX();
    const double              GetMouseMaxPositionY();
    const ramanujan::Vector2  GetCursorViewPosition(float _x, float _y);
    const ramanujan::Vector2  GetCursorViewPosition();
    float                     lerp(float x, float y, float t);

private:
    InputManager();

private:
    bool m_previous_keyboard_state[KEY_LAST + 1];
    bool m_current_keyboard_state[KEY_LAST + 1];

    bool m_previous_mouse_button_state[MOUSE_BUTTON_LAST + 1];
    bool m_current_mouse_button_state[MOUSE_BUTTON_LAST + 1];

    ramanujan::Vector2 m_cursor_position;
    ramanujan::Vector2 m_cursor_max_position;
    ramanujan::Vector2 m_scroll_offset;
};
} // namespace sputnik::engine::api
