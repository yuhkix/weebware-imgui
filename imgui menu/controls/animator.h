#include <unordered_map>
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"
class Animator {
public:
    Animator(float a, float b, bool* when, float speed = 2.f) noexcept
    {
        this->a = a; // the point A
        this->b = b; // the point B
        this->when = when; // when shall it go from A to B, if true it will go from A to B and if it's false then from B to A
        this->speed = speed; // the speed this animation shall happen
    }

    auto GetCurrentAnimation(ImGuiID id) noexcept
    {
        auto currentAnimation = animatorMap.find(id);
        if (currentAnimation == animatorMap.end())
        {
            animatorMap.insert({ id, a });
            currentAnimation = animatorMap.find(id);
        }

        return currentAnimation;
    }

    float Update(ImGuiID id) noexcept
    {
        auto currentAnimation = GetCurrentAnimation(id);
        currentAnimation->second = ImClamp(currentAnimation->second + ImGui::GetIO().DeltaTime * speed * (*when ? 1.f : -1.f), 0.f, 1.0f);
        return ImLerp(a, b, currentAnimation->second);
    }

private:
    std::unordered_map<ImGuiID, float> animatorMap;
    float
        a = 0.f,
        b = 1.f,
        speed;
    bool* when;
};