#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"
#include "../imgui/imgui_impl_dx9.h"
#include "../imgui/imgui_impl_win32.h"
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
    static bool button(const char* label, ImVec2 size);
    static bool checkbox(const char* label, bool* v);
    static void groupbox(const char* title, ImVec2 size);
    static void endgroupbox();
    static bool begintab(const char* unique_id, Icons Icon, ImFont* font);

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
