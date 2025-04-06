#pragma once

namespace sputnik::editor
{

class EditorStyleUtility
{
public:
    enum class Theme
    {
        DARK,
        LIGHTER_DARK,
        DEEP_DARK,
        LIGHT,
        CHERRY,
        GOLDEN_BLACK,
        STREAMING_LIFE,
        SOFT_LIFE,
        WHITE,
        MAYA,

        END_OF_LIST
    };

    static std::unordered_map<Theme, std::string> sThemes;
    static std::unordered_map<Theme, void (*)()>  sThemeFunctions;
    static Theme                                  sActiveTheme;

private:
    static void setDarkThemeColors();
    static void setLighterDarkThemeColors();
    static void setDeepDarkThemeColors();
    static void setLightThemeColors();
    static void setCherryThemeColors();
    static void setGoldenBlackThemeColors();
    static void setStreamingLifeThemeColors();
    static void setSoftLifeThemeColors();
    static void setWhiteThemeColors();
    static void setMayaThemecolors();

    // unused
    static void setGreenThemeColors();
    static void setGreenBlueThemeColors();
};

} // namespace sputnik::editor