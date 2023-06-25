#include "aimbot.h"
#include "../../controls/controls.h"
#include "../../utilities.h"

void aimbot::renderaimbot()
{
    ImGui::SetCursorPos(ImVec2(75, 10));
    if (c::tabbutton("SubTab1"))
    {
        ImGui::SetCursorPos(ImVec2(65, 50));
        c::groupbox("GROUPBOX1", ImVec2(285, 225));
        {
            ImGui::PushFont(util::segoeuiSemibold);
            c::button("Button");
            c::checkbox("Checkbox", &util::checkTest);
            ImGui::PopFont();
        }
        c::endgroupbox();
        ImGui::SetCursorPos(ImVec2(359, 50));
        c::groupbox("GROUPBOX2", ImVec2(285, 225));
        {

        }
        c::endgroupbox();
    }
    ImGui::SetCursorPos(ImVec2(140, 10));
    if (c::tabbutton("SubTab2"))
    {

    }
}