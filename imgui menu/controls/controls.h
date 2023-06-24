#include "animator.h"
#include "../imgui/imgui_impl_dx9.h"
#include "../imgui/imgui_impl_win32.h"
#include <map>
#include <d3d9.h>
#include <tchar.h>

class c {
public:
    enum Icons
    {
        LOGO,
        AIMBOT,
        USER,
        WORLD,
        VISUALS,
        DASHBOARD,
        SEARCH
    };
    static bool tabbutton(const char* label, const ImVec2& size_arg = ImVec2(0,0));
    static bool button(const char* label, const ImVec2& size_arg = ImVec2(0, 0));
    static bool checkbox(const char* label, bool* v);
    static void groupbox(const char* title, ImVec2 size);
    static void endgroupbox();
    static bool begintab(const char* unique_id, Icons Icon, ImFont* font);
    static bool slider(const char* label, int *v, int v_min, int v_max);

    static const char* GetIconString(Icons icons)
    {
        switch (icons)
        {
        case LOGO:
            return "A";
        case AIMBOT:
            return "B";
        case USER:
            return "C";
        case WORLD:
            return "D";
        case VISUALS:
            return "E";
        case DASHBOARD:
            return "F";
        case SEARCH:
            return "G";
        default:
            return "B";
        }
    }
};
