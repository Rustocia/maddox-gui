#include "maddox_gui.hpp"

void maddox::text(const char* fmt, ...) {
    using namespace ImGui;
    ImGuiWindow* window = GetCurrentWindow();

    const ImVec2 text_pos(window->DC.CursorPos.x, window->DC.CursorPos.y + window->DC.CurrentLineTextBaseOffset);

    const float wrap_pos_x = window->DC.TextWrapPos;
    const bool wrap_enabled = (wrap_pos_x >= 0.0f);
    const float wrap_width = wrap_enabled ? CalcWrapWidthForPos(window->DC.CursorPos, wrap_pos_x) : 0.0f;

    const char* text_begin = fmt;

    const ImVec2 text_size = CalcTextSize(text_begin, NULL, false, wrap_width);

    ImRect bb(text_pos, text_pos + text_size);
 
    ItemSize(text_size + ImVec2(0, -2), 0.0f);
    if (!ItemAdd(bb, 0))
        return;

    // Рендер текста                                   
    window->DrawList->AddText(bb.Min + ImVec2(1, 1), ImColor(0, 0, 0, 200), fmt);
    window->DrawList->AddText(bb.Min, ImColor(208, 208, 208, 255), fmt);
}

bool maddox::button(const char* label, ImVec2 size_arg) {
    using namespace ImGui;
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);
    ImGuiButtonFlags flags = ImGuiButtonFlags_None;

    ImVec2 pos = window->DC.CursorPos;
    if ((flags & ImGuiButtonFlags_AlignTextBaseLine) && style.FramePadding.y < window->DC.CurrentLineTextBaseOffset) // Try to vertically align buttons that are smaller/have no padding so that text baseline matches (bit hacky, since it shouldn't be a flag)
        pos.y += window->DC.CurrentLineTextBaseOffset - style.FramePadding.y;
    ImVec2 size = CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

    const ImRect bb(pos, pos + size);
    ItemSize(size, style.FramePadding.y);
    if (!ItemAdd(bb, id))
        return false;

    if (window->DC.ItemFlags & ImGuiItemFlags_ButtonRepeat)
        flags |= ImGuiButtonFlags_Repeat;
    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held, flags);
    if (pressed)
        MarkItemEdited(id);

    // Render
    if(hovered)
        window->DrawList->AddRectFilledMultiColor(
            bb.Min,
            bb.Max,
            ImColor(32, 32, 32, 255),
            ImColor(32, 32, 32, 255),
            ImColor(47, 47, 47, 255),
            ImColor(47, 47, 47, 255)
        );  
    else
        window->DrawList->AddRectFilledMultiColor(
            bb.Min,
            bb.Max,
            ImColor(25, 25, 25, 255),
            ImColor(25, 25, 25, 255),
            ImColor(31, 31, 31, 255),
            ImColor(31, 31, 31, 255)
        );

    window->DrawList->AddRect(
        bb.Min /*+ ImVec2(2, 2)*/,
        bb.Max /*- ImVec2(2, 2)*/,
        ImColor(42, 42, 42, 255),
        0.f,
        ImDrawCornerFlags_All,
        2.5f
    );
    window->DrawList->AddRect(
        bb.Min - ImVec2(1, 1),
        bb.Max + ImVec2(1, 1),
        ImColor(0, 0, 0, 150),
        0.f,
        ImDrawCornerFlags_All,
        0.5f
    );

    PushStyleColor(ImGuiCol_Text, ImVec4(0 / 255.f, 0 / 255.f, 0 / 255.f, 200 / 255.f));
    RenderTextClipped(bb.Min + style.FramePadding + ImVec2(1, 1), bb.Max - style.FramePadding + ImVec2(1, 1),  label, NULL, &label_size, style.ButtonTextAlign, &bb);
    PopStyleColor();

    PushStyleColor(ImGuiCol_Text, ImVec4(208 / 255.f, 208 / 255.f, 208 / 255.f, 1.f));
    RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb);
    PopStyleColor();  

    return pressed;
}