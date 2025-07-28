#pragma once
#include "../common.h"
#include <ImGui/imgui.h>

namespace big
{
	class gui
	{
	public:
		void dx_init();
		void dx_on_tick();
		
		// Modern UI enhancement functions
		void setup_modern_colors();
		void draw_enhanced_tooltip(const char* text, const char* details);
		void draw_modern_status_indicator(const char* label, bool is_active, const ImVec4& active_color);
		void draw_modern_separator(const char* label);
		bool draw_modern_button(const char* label, const ImVec2& size);
		
	public:
		bool m_opened{};
	};

	inline gui g_gui;
}
