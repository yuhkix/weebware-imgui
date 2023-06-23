#include "imgui/imgui.h"

class util
{
public:
    inline static const char* title = "weebware";
    inline static int currentTab;
    inline static ImFont* segoeuiSemibold;
    inline static ImFont* groupboxTitle;
    inline static ImFont* TabIconFont;
    inline static ImFont* LogoIconFont;
    inline static bool isOpen = true;
    static constexpr auto frequency = 1 / 0.5f;
    inline static float fadeinAlpha;
    inline static float fadeoutAlpha;
};