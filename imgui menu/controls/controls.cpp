#define IMGUI_DEFINE_MATH_OPERATORS
#include "controls.h"
#include "../utilities.h"

#pragma region tab button
bool c::tabbutton(const char* label, const ImVec2& size_arg)
{

    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    ImGuiID id = window->GetID(label);
    ImVec2 label_size = ImGui::CalcTextSize(label);
    ImVec2 size = ImGui::CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);
    ImVec2 pos = window->DC.CursorPos;
    ImRect bb = ImRect(pos, pos + size);

    ImGui::ItemSize(size, style.FramePadding.y);
    if (!ImGui::ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);

    ImDrawList* drawlist = ImGui::GetWindowDrawList();

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

    bool textcons = currentTab->second || hovered;
    static Animator<ImColor> textAnim(ImColor(54, 54, 54), ImColor(203, 119, 180), 4.f);
    ImColor textColor = textAnim.Update(id, &textcons);

    drawlist->AddRectFilled(bb.Min, bb.Max, ImColor(30, 30, 30), 2.0f);
    drawlist->AddText(ImVec2(pos.x + (size.x / 2) - label_size.x * 0.5f, pos.y + (size.y / 2) - label_size.y * 0.5f), textColor, label);

    return currentTab->second;
}
#pragma endregion

#pragma region button
bool c::button(const char* label, const ImVec2& size_arg)
{
    ImGui::SetCursorPosX(10);
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    ImGuiID id = window->GetID(label);
    ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
    ImVec2 size = ImGui::CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);
    ImVec2 pos = window->DC.CursorPos;
    ImRect bb = ImRect(pos, pos + size);

    ImGui::ItemSize(size, style.FramePadding.y);
    if (!ImGui::ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);
    ImDrawList* drawlist = ImGui::GetWindowDrawList();
    
    static Animator<ImColor> hoverAnim(ImColor(30, 30, 30), ImColor(25, 25, 25), 4.f);
    ImColor hoverColor = hoverAnim.Update(id, &hovered);

    drawlist->AddRectFilled(bb.Min, bb.Max, hoverColor, 2.0f);
    drawlist->AddText(ImVec2(pos.x + (size.x / 2) - label_size.x * 0.5f, pos.y + (size.y / 2) - label_size.y * 0.5f), ImColor(203, 119, 180), label);

    return pressed;
}
#pragma endregion

#pragma region checkbox
bool c::checkbox(const char* label, bool* v)
{
    ImGui::SetCursorPosX(10);
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImRect bb = ImRect(window->DC.CursorPos, window->DC.CursorPos + ImVec2(20, 20));
    const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
    const float square_sz = ImGui::GetFrameHeight();
    const ImVec2 pos = window->DC.CursorPos;

    ImGui::ItemSize(bb);
    if (!ImGui::ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);

    if (pressed)
        *v = !(*v);

    static Animator<float> pressAnimator(0.f, 3.f, 8.f);
    float pressValue = pressAnimator.Update(id, v);

    static Animator<float> hoverAnimator(20.f, 0.f, 4.f);
    float hoverValue = hoverAnimator.Update(id, &hovered);

    window->DrawList->AddRectFilled(bb.Min + ImVec2(0, hoverValue), bb.Max, ImColor(30, 30, 30), 0.f);

    if (*v || pressValue != 0.f)
    {
        window->DrawList->AddRectFilled(bb.Min + ImVec2(pressValue, pressValue), bb.Max - ImVec2(pressValue, pressValue), ImColor(203, 119, 180), 0.f);
    }

    static Animator<ImColor> textAnim(ImColor(54, 54, 54), ImColor(203, 119, 180), 4.f);
    ImColor textColor = textAnim.Update(id, v);

    const ImRect check_bb(pos, pos + ImVec2(square_sz, square_sz));
    ImVec2 label_pos = ImVec2(check_bb.Max.x + style.ItemInnerSpacing.x, check_bb.Min.y + style.FramePadding.y - 1);
    if (label_size.x > 0.0f)
        window->DrawList->AddText(label_pos, textColor, label);
    window->DrawList->AddRect(bb.Min, bb.Max, ImColor(30, 30, 30), 0.f);


    return true;
}
#pragma endregion

#pragma region groupbox
void c::groupbox(const char* title, ImVec2 size)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    const ImGuiID id = window->GetID(title);
    ImVec2 label_size = ImGui::CalcTextSize(title);
    ImVec2 pos = window->DC.CursorPos;
    ImRect bb = ImRect(pos, pos + size);
    ImGui::BeginChild(title, size);
    ImGui::SetCursorPos(ImVec2(10, 5));
    ImGui::PushFont(util::groupboxTitle);
    ImGui::TextColored(ImColor(203, 119, 180), title);
    ImGui::PopFont();

}

void c::endgroupbox()
{
    ImGui::EndChild();
}
#pragma endregion

#pragma region iconbuttons
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

    bool iconcons = currentTab->second || hovered;
    static Animator<ImColor> iconAnim(ImColor(70, 70, 70), ImColor(203, 119, 180), 4.f);
    ImColor iconColor = iconAnim.Update(id, &iconcons);

    static Animator<ImColor> lineAnim(ImColor(20, 20, 20), ImColor(203, 119, 180), 4.f);
    ImColor lineColor = lineAnim.Update(id, &currentTab->second);

    window->DrawList->AddText(font, 35.f, pos, iconColor, GetIconString(icon));
    window->DrawList->AddLine(ImVec2(window->Pos.x + 58, pos.y + 5), ImVec2(window->Pos.x + 58, pos.y + icon_size.y + 5), lineColor, 2.0F);

    return currentTab->second;
}
#pragma endregion

#pragma region slider
bool c::slider(const char* label, int* v, int v_min, int v_max, const char* format, ImGuiSliderFlags flags)
{
    return NULL;
}
#pragma endregion