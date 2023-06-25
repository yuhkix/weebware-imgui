#include "menu.h"
#include "utilities.h"
#include "controls/controls.h"
#include "tabs/aimbot/aimbot.h"
#include "tabs/user/user.h"
#include "tabs/world/world.h"
#include "tabs/visuals/visuals.h"
#include "tabs/dashboard/dashboard.h"
#include "tabs/search/search.h"
#include <Windows.h>
#include <map>
#define flags ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar

void menu::rendermenu()
{
    ImGuiContext& g = *GImGui;
    ImGui::SetNextWindowSize(ImVec2(650, 500), ImGuiCond_Always);
    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y / 2), ImGuiCond_Once, ImVec2(0.5f, 0.5f));
    ImGui::Begin(util::title, &util::isOpen, flags);
    {
        ImDrawList* draw = ImGui::GetWindowDrawList();
        ImVec2 pos = ImGui::GetWindowPos();

        //rendering side and top bar
        draw->AddRectFilled(ImVec2(pos.x, pos.y), ImVec2(pos.x + 60, pos.y + ImGui::GetWindowSize().y), ImColor(20, 20, 20), 0.f);
        draw->AddRectFilled(ImVec2(pos.x + 60, pos.y), ImVec2(pos.x + ImGui::GetWindowSize().x, pos.y + 45), ImColor(24, 24, 24), 0.f);

        //rendering logo
        #pragma region Logo
        ImGui::SetCursorPos(ImVec2(15, 5));
        ImGui::PushFont(util::LogoIconFont);
        ImGui::TextColored(ImColor(203, 119, 180), "A");
        ImGui::PopFont();
        #pragma endregion

        //rendering tab icons
        #pragma region Tabs
        ImGui::SetCursorPosY(30);
        if (c::begintab("Aimbot", c::AIMBOT, util::TabIconFont))
        {
            util::currentTab = c::AIMBOT;
        }
        ImGui::SetCursorPosY(70);
        if (c::begintab("User", c::USER, util::TabIconFont))
        {
            util::currentTab = c::USER;
        }
        ImGui::SetCursorPosY(110);
        if (c::begintab("World", c::WORLD, util::TabIconFont))
        {
            util::currentTab = c::WORLD;
        }
        ImGui::SetCursorPosY(150);
        if (c::begintab("Visuals", c::VISUALS, util::TabIconFont))
        {
            util::currentTab = c::VISUALS;
        }
        ImGui::SetCursorPosY(190);
        if (c::begintab("Dashboard", c::DASHBOARD, util::TabIconFont))
        {
            util::currentTab = c::DASHBOARD;
        }
        ImGui::SetCursorPosY(230);
        if (c::begintab("Search", c::SEARCH, util::TabIconFont))
        {
            util::currentTab = c::SEARCH;
        }
        #pragma endregion

        //switching between tabs
        #pragma region TabSwitching
        switch (util::currentTab)
        {
        case c::AIMBOT:
            aimbot::renderaimbot();
            break;

        case c::USER:
            user::renderuser();
            break;

        case c::WORLD:
            world::renderworld();
            break;

        case c::VISUALS:
            visuals::rendervisuals();
            break;

        case c::DASHBOARD:
            dashboard::renderdashboard();
            break;

        case c::SEARCH:
            search::rendersearch();
            break;

        default:

            break;
        }
        #pragma endregion
    }
    ImGui::End();
}