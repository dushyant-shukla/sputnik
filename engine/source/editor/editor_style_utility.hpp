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
    static void SetDarkThemeColors();
    static void SetLighterDarkThemeColors();
    static void SetDeepDarkThemeColors();
    static void SetLightThemeColors();
    static void SetCherryThemeColors();
    static void SetGoldenBlackThemeColors();
    static void SetStreamingLifeThemeColors();
    static void SetSoftLifeThemeColors();
    static void SetWhiteThemeColors();
    static void SetMayaThemecolors();

    // unused
    static void SetGreenThemeColors();
    static void SetGreenBlueThemeColors();
};

} // namespace sputnik::editor