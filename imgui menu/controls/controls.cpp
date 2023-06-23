#define IMGUI_DEFINE_MATH_OPERATORS
#include "controls.h"
#include <map>
#include <unordered_map>

bool c::button(const char* label, ImVec2 size)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    ImGuiID id = window->GetID(label);
    ImVec2 label_size = ImGui::CalcTextSize(label);
    ImVec2 pos = window->DC.CursorPos;
    ImRect bb = ImRect(pos, pos + size);

    ImGui::ItemSize(bb);
    if (!ImGui::ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);

    ImDrawList* drawlist = ImGui::GetWindowDrawList();
    ImColor color = (hovered && held) ? ImColor(30, 30, 30, 255) : hovered ? ImColor(30, 30, 30, 255) : ImColor(30, 30, 30, 255);
    ImColor textColor = (hovered && held) ? ImColor(54, 54, 54, 255) : hovered ? ImColor(54, 54, 54, 255) : ImColor(203, 119, 180, 255);

    static std::unordered_map<ImGuiID, bool> Tabs;
    auto currentTab = Tabs.find(id);

    if (currentTab == Tabs.end())
    {
        bool firstOne = Tabs.empty();
        Tabs.insert({ id, firstOne ? true : false });
        currentTab = Tabs.find(id);
    }
    if (pressed)
    {
        for (auto& tab : Tabs)
            tab.first != currentTab->first ? tab.second = false : tab.second = true;
    }

    ImVec4 hoverText = ImColor(203, 119, 180, 255);
    ImVec4 idleText = ImColor(54, 54, 54, 255);

    static std::map<ImGuiID, float> active_anim;
    auto this_act = active_anim.find(id);
    if (this_act == active_anim.end())
    {
        active_anim.insert({ id, 0.f });
        this_act = active_anim.find(id);
    }
    this_act->second = ImClamp(this_act->second + g.IO.DeltaTime * 5.f * (currentTab->second || hovered ? 1.f : -1.f), 0.f, 1.0f);

    ImColor textColorTransition = ImLerp(idleText, hoverText, this_act->second);

    drawlist->AddRectFilled(bb.Min, bb.Max, color, 2.0f);
    drawlist->AddText(ImVec2(pos.x + (size.x / 2) - label_size.x * 0.5f, pos.y + (size.y / 2) - label_size.y * 0.5f), textColorTransition, label);

    return currentTab->second;
}

bool c::checkbox(const char* label, bool* v)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

    const float square_sz = ImGui::GetFrameHeight();
    const ImVec2 pos = window->DC.CursorPos;
    const ImRect total_bb(pos, pos + ImVec2(square_sz + (label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f), label_size.y + style.FramePadding.y * 2.0f));
    ImGui::ItemSize(total_bb, style.FramePadding.y);
    if (!ImGui::ItemAdd(total_bb, id))
    {
        IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags | ImGuiItemStatusFlags_Checkable | (*v ? ImGuiItemStatusFlags_Checked : 0));
        return false;
    }

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(total_bb, id, &hovered, &held);
    if (pressed)
    {
        *v = !(*v);
        ImGui::MarkItemEdited(id);
    }

    const ImRect check_bb(pos, pos + ImVec2(square_sz, square_sz));
    ImGui::RenderNavHighlight(total_bb, id);
    //ImGui::RenderFrame(check_bb.Min, check_bb.Max, ImGui::GetColorU32((held && hovered) ? ImGuiCol_FrameBgActive : hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg), true, style.FrameRounding);

    ImVec4 idleColor = ImColor(30, 30, 30, 255);
    ImVec4 hoverColor = ImColor(151, 89, 134, 255);
    ImVec4 activeColor = ImColor(203, 119, 180, 255);
    static std::map<ImGuiID, float> active_anim;
    auto this_act = active_anim.find(id);
    if (this_act == active_anim.end())
    {
        active_anim.insert({ id, 0.f });
        this_act = active_anim.find(id);
    }
    this_act->second = ImClamp(this_act->second + g.IO.DeltaTime * 5.f * (hovered ? 1.f : -1.f), 0.f, 1.0f);

    ImColor textColorTransition = ImLerp(idleColor, hoverColor, this_act->second);

    window->DrawList->AddRectFilled(check_bb.Min, check_bb.Max, textColorTransition, 5.0f);


    ImU32 check_col = ImColor(255, 255, 255, 255);
    bool mixed_value = (g.LastItemData.InFlags & ImGuiItemFlags_MixedValue) != 0;
    if (mixed_value)
    {
        ImVec2 pad(ImMax(1.0f, IM_FLOOR(square_sz / 3.6f)), ImMax(1.0f, IM_FLOOR(square_sz / 3.6f)));
        window->DrawList->AddRectFilled(check_bb.Min + pad, check_bb.Max - pad, check_col, style.FrameRounding);
    }
    else if (*v)
    {
        const float pad = ImMax(1.0f, IM_FLOOR(square_sz / 6.0f));
        ImGui::RenderCheckMark(window->DrawList, check_bb.Min + ImVec2(pad, pad), check_col, square_sz - pad * 2.0f);
    }

    ImVec2 label_pos = ImVec2(check_bb.Max.x + style.ItemInnerSpacing.x, check_bb.Min.y + style.FramePadding.y);
    if (g.LogEnabled)
        ImGui::LogRenderedText(&label_pos, mixed_value ? "[~]" : *v ? "[x]" : "[ ]");
    if (label_size.x > 0.0f)
        ImGui::RenderText(label_pos, label);

    IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags | ImGuiItemStatusFlags_Checkable | (*v ? ImGuiItemStatusFlags_Checked : 0));
    return pressed;
}

void c::groupbox(const char* title, ImVec2 size)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    const ImGuiID id = window->GetID(title);
    ImVec2 label_size = ImGui::CalcTextSize(title);
    ImVec2 pos = window->DC.CursorPos;
    ImRect bb = ImRect(pos, pos + size);

    ImGui::BeginChild(title, size);
}

void c::endgroupbox()
{
    ImGui::EndChild();
}

bool c::begintab(const char* unique_id, Icons icon, ImFont* font)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;
    ImGuiContext& g = *GImGui;
    ImGuiIO& io = ImGui::GetIO();
    const ImGuiID id = window->GetID(unique_id);
    ImVec2 icon_size = font->CalcTextSizeA(35.f, FLT_MAX, 0.f, GetIconString(icon));
    ImVec2 pos = window->DC.CursorPos + ImVec2((60 / 2) - icon_size.x * 0.5f, 20);
    ImRect bb = ImRect(pos, pos + icon_size);

    ImGui::ItemSize(bb);
    if (!ImGui::ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);

    static std::unordered_map<ImGuiID, bool> Tabs;
    auto currentTab = Tabs.find(id);

    if (currentTab == Tabs.end())
    {
        bool firstOne = Tabs.empty();
        Tabs.insert({ id, firstOne ? true : false });
        currentTab = Tabs.find(id);
    }
    if (pressed)
    {
        for (auto& tab : Tabs)
            tab.first != currentTab->first ? tab.second = false : tab.second = true;
    }

    ImVec4 hoverText = ImColor(203, 119, 180, 255);
    ImVec4 idleText = ImColor(70, 70, 70, 255);

    static std::map<ImGuiID, float> active_anim;
    auto this_act = active_anim.find(id);
    if (this_act == active_anim.end())
    {
        active_anim.insert({ id, 0.f });
        this_act = active_anim.find(id);
    }
    this_act->second = ImClamp(this_act->second + g.IO.DeltaTime * 5.f * (hovered ? 1.f : -1.f), 0.f, 1.0f);

    ImColor textColorTransition = ImLerp(idleText, hoverText, this_act->second);

    window->DrawList->AddText(font, 35.f, pos, currentTab->second ? ImColor(203.f / 255.f, 119.f / 255.f, 180.f / 255.f) : textColorTransition , GetIconString(icon));
    if (currentTab->second)
    {
        window->DrawList->AddLine(ImVec2(window->Pos.x + 58, pos.y + 5), ImVec2(window->Pos.x + 58, pos.y + icon_size.y + 5), ImColor(203, 119, 180, 255), 2.0F);
    }

    return currentTab->second;
}