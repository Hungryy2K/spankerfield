#pragma once
#include "../common.h"
#include <ImGui/imgui.h>

/**
 * @file Tooltip.h
 * @brief Enhanced ImGui tooltips with formatting support
 * @details Provides extended tooltip functionality for ImGui with support for
 * multiline text, color formatting, and special warning tooltips.
 * 
 * Color formatting supports any valid 6-digit hex color code (#RRGGBB).
 * For example: [color=#8fe222] for custom green shade,
 *             [color=#ff6b47] for custom orange, etc.
 */

namespace ImGui
{
	// Modern UI Components
	inline void ModernProgressBar(float fraction, const ImVec2& size_arg = ImVec2(0, 0), const char* overlay = nullptr)
	{
		// Use a simpler approach that doesn't rely on complex ImGui internals
		ImGui::ProgressBar(fraction, size_arg, overlay);
	}

	inline void StatusIndicator(const char* label, bool is_active, const ImVec4& active_color = ImVec4(0.2f, 0.8f, 0.4f, 1.0f), const ImVec4& inactive_color = ImVec4(0.5f, 0.5f, 0.5f, 1.0f))
	{
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Text, is_active ? active_color : inactive_color);
		ImGui::Text(is_active ? xorstr_("●") : xorstr_("○"));
		ImGui::PopStyleColor();
		
		if (label)
		{
			ImGui::SameLine();
			ImGui::Text(xorstr_("%s"), label);
		}
	}

	inline void ModernSeparator(const char* label = nullptr)
	{
		ImGui::Spacing();
		if (label)
		{
			ImGui::Text(xorstr_("%s"), label);
			ImGui::SameLine();
		}
		ImGui::Separator();
		ImGui::Spacing();
	}

	inline void ModernButton(const char* label, const ImVec2& size = ImVec2(0, 0))
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(12.0f, 8.0f));
		ImGui::Button(label, size);
		ImGui::PopStyleVar(2);
	}

	inline void ProcessFormattedText(const std::string_view& text)
	{
		const char* start = text.data();
		const char* end = start + text.size();
		
		while (start < end)
		{
			const char* line_end = std::strchr(start, '\n');
			if (!line_end) line_end = end;
			
			// Process color tags if they exist
			std::string_view line(start, line_end - start);
			if (line.length() >= 7 && line.substr(0, 7) == "[color=")
			{
				size_t close_bracket = line.find(']');
				if (close_bracket != std::string_view::npos)
				{
					std::string_view color_str = line.substr(7, close_bracket - 7);
					ImVec4 color;

					// Parse hex color (#RRGGBB or #RRGGBBAA)
					if (color_str.length() > 0 && color_str[0] == '#')
					{
						unsigned int c;
						if (sscanf_s(color_str.data(), "#%x", &c) == 1)
						{
							color = ImVec4(
								((c >> 16) & 0xFF) / 255.0f,
								((c >> 8) & 0xFF) / 255.0f,
								(c & 0xFF) / 255.0f,
								1.0f
							);
						}
					}

					ImGui::PushStyleColor(ImGuiCol_Text, color);
					ImGui::TextUnformatted(line.data() + close_bracket + 1, line_end);
					ImGui::PopStyleColor();
				}
			}
			else
				ImGui::TextUnformatted(start, line_end);
			
			if (line_end < end)
				start = line_end + 1;
			else
				break;
		}
	}

	inline void Tooltip(const std::string& description)
	{
		ImGui::SameLine();
        ImGui::Text(xorstr_("( ? )"));
		
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			
			ProcessFormattedText(description);
			
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}

	inline void Tooltip(const std::string& label, const std::string& description)
	{
		ImGui::SameLine();
		ImGui::Text(xorstr_("( ? )"));    

		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			
			ImGui::Text(xorstr_("%s"), label.c_str());
			ProcessFormattedText(description);
			
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}

	inline void WarningTooltip(const std::string& description)
	{
		ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.8f, 0.0f, 1.0f));
        ImGui::Text(xorstr_("[ ! ]"));
        ImGui::PopStyleColor();

		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			
			ProcessFormattedText(description);
			
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}

	inline void WarningTooltip(const std::string& label, const std::string& description)
	{
		ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.8f, 0.0f, 1.0f));
        ImGui::Text(xorstr_("[ ! ]"));
        ImGui::PopStyleColor();

		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			
			ImGui::Text(xorstr_("%s"), label.c_str());
			ProcessFormattedText(description);
			
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}
}