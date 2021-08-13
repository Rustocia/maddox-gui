#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_internal.h"


class maddox
{
public:
	void text(const char* fmt, ...);
	bool button(const char* label, ImVec2 size_arg);
};

