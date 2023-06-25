#include "dashboard.h"
#include "../../controls/controls.h"
#include "../../utilities.h"

void dashboard::renderdashboard()
{
    static bool test = false;
    ImGui::SetCursorPos(ImVec2(75, 10));
    if (c::tabbutton("Helpers", ImVec2(55, 25)))
    {
        ImGui::SetCursorPos(ImVec2(65, 50));
        c::groupbox("GENERAL", ImVec2(285, 225));
        {
            ImGui::PushFont(util::segoeuiSemibold);
            c::button("Button");
            c::checkbox("Checkbox", &util::checkTest);
            ImGui::PopFont();
        }
        c::endgroupbox();
        ImGui::SetCursorPos(ImVec2(359, 50));
        c::groupbox("INDICATORS", ImVec2(285, 225));
        {

        }
        c::endgroupbox();
    }
    ImGui::SetCursorPos(ImVec2(140, 10));
    c::tabbutton("Movement", ImVec2(80, 25));
}