#include "imgui.h"
#include <string>
#include "imgui_internal.h"

namespace ImStyle {

	// ============================================================================
	// MODERN DARK RED THEME - Visual UI Overhaul Only
	// All logic and app functionality remain untouched.
	// ============================================================================

	inline ImVec4 general_color = ImColor(255, 255, 255, 255);

	namespace window {
		inline ImVec4 background_pic = ImColor(20, 20, 25, 255);
		inline ImVec4 background = ImColor(21, 21, 28, 210);
		inline ImVec4 border = ImColor(255, 80, 90, 140);
		inline ImVec4 border_shadow = ImColor(0, 0, 0, 140);
		inline ImVec4 shadow_text = ImColor(255, 95, 110, 220);
		inline ImVec4 shadow_panel = ImColor(255, 70, 90, 80);
		inline ImVec2 size = ImVec2(450, 510);
		inline float triangle = 15.f;
		inline float rounding = 12.f;
		inline float rounding_small = 6.f;
	}

	namespace button {
		inline ImVec4 background = ImColor(145, 18, 24, 210);
		inline ImVec4 background_hov = ImColor(210, 35, 48, 230);
		inline ImVec4 background_active = ImColor(255, 95, 110, 255);
		inline ImVec4 background_shadow = ImColor(0, 0, 0, 180);
		inline ImVec4 text = ImColor(255, 255, 255, 255);
		inline ImVec4 text_hov = ImColor(255, 255, 255, 255);
		inline ImVec4 text_active = ImColor(255, 255, 255, 255);
		inline float triangle = 5.f;
		inline float rounding = 8.f;
		inline float padding = 6.f;
	}

	namespace input {
		inline ImVec4 text_selected = ImColor(255, 255, 255, 255);
		inline ImVec4 background_active = ImColor(40, 40, 52, 255);
		inline ImVec4 background_hov = ImColor(38, 38, 48, 220);
		inline ImVec4 background = ImColor(34, 34, 44, 210);
		inline ImVec4 border = ImColor(255, 80, 90, 110);
		inline ImVec4 border_active = ImColor(255, 115, 120, 200);
		inline ImVec4 hint_text = ImColor(135, 135, 145, 160);
		inline float triangle = 5.f;
		inline float rounding = 7.f;
	}

	namespace text {
		inline ImVec4 hint_text = ImColor(140, 140, 150, 160);
		inline ImVec4 have_account = ImColor(200, 200, 210, 255);
		inline ImVec4 text_active = ImColor(255, 190, 110, 255);
		inline ImVec4 text_hov = ImColor(255, 255, 255, 255);
		inline ImVec4 text = ImColor(240, 240, 245, 255);
	}

	namespace selector {
		inline ImVec4 icon = ImColor(255, 110, 120, 220);
		inline ImVec4 background_active = ImColor(200, 30, 42, 180);
		inline ImVec4 background_hov = ImColor(70, 70, 80, 150);
		inline ImVec4 background = ImColor(33, 33, 40, 150);
	}

	namespace panel {
		inline ImVec4 background = ImColor(29, 29, 36, 220);
		inline ImVec4 background_hov = ImColor(35, 35, 45, 240);
		inline ImVec4 border = ImColor(255, 90, 100, 100);
		inline ImVec4 shadow = ImColor(0, 0, 0, 120);
		inline float rounding = 10.f;
		inline float border_thickness = 1.0f;
		inline float shadow_offset = 4.f;
	}

	namespace animation {
		inline float hover_speed = 0.14f;
		inline float press_speed = 0.08f;
		inline float fade_speed = 0.12f;
	}

	inline void ApplyModernDarkRedTheme() {
		ImGuiStyle& style = ImGui::GetStyle();

		style.Colors[ImGuiCol_WindowBg] = window::background;
		style.Colors[ImGuiCol_FrameBg] = input::background;
		style.Colors[ImGuiCol_FrameBgHovered] = input::background_hov;
		style.Colors[ImGuiCol_FrameBgActive] = input::background_active;
		style.Colors[ImGuiCol_Button] = button::background;
		style.Colors[ImGuiCol_ButtonHovered] = button::background_hov;
		style.Colors[ImGuiCol_ButtonActive] = button::background_active;
		style.Colors[ImGuiCol_SliderGrab] = ImColor(220, 35, 50, 255);
		style.Colors[ImGuiCol_SliderGrabActive] = ImColor(255, 95, 110, 255);
		style.Colors[ImGuiCol_CheckMark] = ImColor(255, 110, 120, 255);
		style.Colors[ImGuiCol_Text] = text::text;
		style.Colors[ImGuiCol_TextDisabled] = ImColor(130, 130, 140, 255);
		style.Colors[ImGuiCol_Header] = selector::background_active;
		style.Colors[ImGuiCol_HeaderHovered] = selector::background_hov;
		style.Colors[ImGuiCol_HeaderActive] = selector::background_active;
		style.Colors[ImGuiCol_Separator] = ImColor(120, 120, 130, 100);
		style.Colors[ImGuiCol_SeparatorHovered] = ImColor(150, 150, 160, 140);
		style.Colors[ImGuiCol_SeparatorActive] = ImColor(255, 80, 90, 200);
		style.Colors[ImGuiCol_Border] = window::border;
		style.Colors[ImGuiCol_BorderShadow] = window::border_shadow;

		style.WindowPadding = ImVec2(16.f, 16.f);
		style.FramePadding = ImVec2(10.f, 8.f);
		style.ItemSpacing = ImVec2(12.f, 10.f);
		style.ItemInnerSpacing = ImVec2(8.f, 6.f);
		style.IndentSpacing = 20.f;
		style.ScrollbarSize = 14.f;
		style.WindowRounding = window::rounding;
		style.FrameRounding = input::rounding;
		style.GrabRounding = 7.f;
		style.PopupRounding = 10.f;
		style.ScrollbarRounding = 8.f;
		style.TabRounding = 8.f;
		style.WindowBorderSize = 1.f;
		style.FrameBorderSize = 0.5f;
		style.PopupBorderSize = 1.f;
		style.Alpha = 1.0f;
		style.WindowAlpha = 0.96f;
		style.ShadowColor = ImColor(0, 0, 0, 120);
		style.HoverStateDuration = animation::hover_speed;
	}

	inline void DrawShadowRect(ImDrawList* draw_list, ImVec2 min, ImVec2 max,
		ImVec4 color, float rounding, int shadow_spread) {
		ImVec2 shadow_min(min.x + 2, min.y + 2);
		ImVec2 shadow_max(max.x + shadow_spread, max.y + shadow_spread);
		draw_list->AddRectFilled(shadow_min, shadow_max, ImGui::GetColorU32(window::border_shadow), rounding);
		draw_list->AddRectFilled(min, max, ImGui::GetColorU32(color), rounding);
		draw_list->AddRect(min, max, ImGui::GetColorU32(window::border), rounding, 0, 1.5f);
	}

	inline void DrawGlowRect(ImDrawList* draw_list, ImVec2 min, ImVec2 max,
		ImVec4 glow_color, float rounding, int glow_width) {
		for (int i = glow_width; i > 0; --i) {
			ImVec4 fade_color = glow_color;
			fade_color.w = glow_color.w * (float)i / glow_width * 0.45f;
			float offset = (float)(glow_width - i) * 0.5f;
			draw_list->AddRect(ImVec2(min.x - offset, min.y - offset),
				ImVec2(max.x + offset, max.y + offset),
				ImGui::GetColorU32(fade_color), rounding);
		}
		draw_list->AddRect(min, max, ImGui::GetColorU32(glow_color), rounding, 0, 2.f);
	}

}
