#define IMGUI_DEFINE_MATH_OPERATORS
#include "controls.h"

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
    this_act->second = ImClamp(this_act->second + g.IO.DeltaTime * 4.f * (currentTab->second || hovered ? 1.f : -1.f), 0.f, 1.0f);

    ImColor textColorTransition = ImLerp(idleText, hoverText, this_act->second);

    drawlist->AddRectFilled(bb.Min, bb.Max, color, 3.0f);
    drawlist->AddText(ImVec2(pos.x + (size.x / 2) - label_size.x * 0.5f, pos.y + (size.y / 2) - label_size.y * 0.5f), textColorTransition, label);

    return currentTab->second;
}

bool c::button(const char* label, const ImVec2& size_arg)
{
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
    ImVec4 hoverText = ImColor(25, 25, 25, 255);
    ImVec4 idleText = ImColor(30, 30, 30, 255);
    static std::map<ImGuiID, float> active_anim;
    auto this_act = active_anim.find(id);
    if (this_act == active_anim.end())
    {
        active_anim.insert({ id, 0.f });
        this_act = active_anim.find(id);
    }
    this_act->second = ImClamp(this_act->second + g.IO.DeltaTime * 4.f * (pressed || hovered ? 1.f : -1.f), 0.f, 1.0f);
    ImColor textColorTransition = ImLerp(idleText, hoverText, this_act->second);

    drawlist->AddRectFilled(bb.Min, bb.Max, textColorTransition, 3.0f);
    drawlist->AddText(ImVec2(pos.x + (size.x / 2) - label_size.x * 0.5f, pos.y + (size.y / 2) - label_size.y * 0.5f), ImColor(203, 119, 180, 255), label);

    return pressed;
}

bool c::checkbox(const char* label, bool* v)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    const ImGuiID id = window->GetID(label);
    const ImRect bb = ImRect(window->DC.CursorPos, window->DC.CursorPos + ImVec2(20, 20));

    ImGui::ItemSize(bb);
    if (!ImGui::ItemAdd(bb, id))
        return false;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);

    if (pressed)
        *v = !(*v);

    static Animator pressAnim(0.f, 3.f, v, 8.f);
    float pressValue = pressAnim.Update(id);

    static Animator hoverAnim(20.f, 0.f, &hovered, 4.f);
    float hoverValue = hoverAnim.Update(id);
    window->DrawList->AddRectFilled(bb.Min + ImVec2(0, hoverValue), bb.Max, ImColor(30, 30, 30), 0.f);


    if (*v || pressValue != 0.f)
    {
        window->DrawList->AddRectFilled(bb.Min + ImVec2(pressValue, pressValue), bb.Max - ImVec2(pressValue, pressValue), ImColor(203, 119, 180), 0.f);
    }

    window->DrawList->AddRect(bb.Min, bb.Max, ImColor(30, 30, 30), 0.f);
    return true;
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
    ImVec4 LineIdle = ImColor(20, 20, 20, 255);

    //tab icon fade anim
    static std::map<ImGuiID, float> active_anim;
    auto this_act = active_anim.find(id);
    if (this_act == active_anim.end())
    {
        active_anim.insert({ id, 0.f });
        this_act = active_anim.find(id);
    }
    this_act->second = ImClamp(this_act->second + g.IO.DeltaTime * 4.f * (currentTab->second || hovered ? 1.f : -1.f), 0.f, 1.0f);

    //line fade anim
    static std::map<ImGuiID, float> new_anim;
    auto new_act = new_anim.find(id);
    if (new_act == new_anim.end())
    {
        new_anim.insert({ id, 0.f });
        new_act = new_anim.find(id);
    }
    new_act->second = ImClamp(new_act->second + g.IO.DeltaTime * 4.f * (currentTab->second ? 1.f : -1.f), 0.f, 1.0f);

    ImColor textColorTransition = ImLerp(idleText, hoverText, this_act->second);
    ImColor lineColorTransition = ImLerp(LineIdle, hoverText, new_act->second);

    window->DrawList->AddText(font, 35.f, pos, textColorTransition , GetIconString(icon));
    window->DrawList->AddLine(ImVec2(window->Pos.x + 58, pos.y + 5), ImVec2(window->Pos.x + 58, pos.y + icon_size.y + 5), lineColorTransition, 2.0F);

    return currentTab->second;
}

bool c::slider(const char* label, int* v, int v_min, int v_max)
{

}