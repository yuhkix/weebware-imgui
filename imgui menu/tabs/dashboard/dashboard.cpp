#include "dashboard.h"
#include "../../controls/controls.h"
#include "../../utilities.h"

void dashboard::renderdashboard()
{
    ImGui::SetCursorPos(ImVec2(75, 10));
    if (c::button("Helpers", ImVec2(55, 25)))
    {
        ImGui::SetCursorPos(ImVec2(65, 50));
        c::groupbox("General", ImVec2(285, 225));
        {
            ImGui::SetCursorPos(ImVec2(10, 5));
            ImGui::PushFont(util::groupboxTitle);
            ImGui::TextColored(ImColor(203, 119, 180, 255), "GENERAL");
            ImGui::PopFont();
        }
        c::endgroupbox();
        ImGui::SetCursorPos(ImVec2(359, 50));
        c::groupbox("Indicators", ImVec2(285, 225));
        {
            ImGui::SetCursorPos(ImVec2(10, 5));
            ImGui::PushFont(util::groupboxTitle);
            ImGui::TextColored(ImColor(203, 119, 180, 255), "INDICATORS");
            ImGui::PopFont();
        }
        c::endgroupbox();
    }
    ImGui::SetCursorPos(ImVec2(140, 10));
    c::button("Movement", ImVec2(80, 25));
}