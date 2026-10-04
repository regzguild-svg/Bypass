#include "imgui.h"
#include <string>
#include "imgui_internal.h"
namespace ImStyle {

	inline ImVec4 general_color = ImColor(255, 255, 255, 255);

	namespace window {
		inline ImVec4 background_pic = ImColor(255, 255, 255, 255);

		inline ImVec4 background = ImColor(16, 16, 16, 200);
		inline ImVec4 border = ImColor(50, 50, 50, 255);

		inline ImVec4 shadow_text = ImColor(42, 200, 250, 255);
		inline ImVec4 shadow_panel = ImColor(42, 200, 250, 255);

		inline ImVec2 size = ImVec2(450, 510);
		inline float triangle = 15.f;
	}

	namespace button {

		inline ImVec4 background_active = ImColor(0, 150, 150, 150);
		inline ImVec4 background_hov = ImColor(0, 150, 150, 150);
		inline ImVec4 background = ImColor(0, 150, 150, 150);

		inline ImVec4 background_shadow = ImColor(45, 45, 45, 205);

		inline float triangle = 5.f;
	}

	namespace input {

		inline ImVec4 text_selected = ImColor(255, 255, 255, 255);

		inline ImVec4 background_active = ImColor(255, 255, 255, 255);
		inline ImVec4 background_hov = ImColor(255, 255, 255, 255);
		inline ImVec4 background = ImColor(255, 255, 255, 255);

		inline float triangle = 5.f;
	}

	namespace text {

		inline ImVec4 hint_text = ImColor(0, 0, 0, 0);
		inline ImVec4 have_account = ImColor(255, 255, 255, 255);

		inline ImVec4 text_active = ImColor(255, 255, 255, 255);
		inline ImVec4 text_hov = ImColor(255, 255, 255, 255);
		inline ImVec4 text = ImColor(255, 255, 255, 255);

	}

	namespace selector {

		inline ImVec4 icon = ImColor(47, 50, 53, 230);

		inline ImVec4 background_active = ImColor(47, 50, 53, 150);
		inline ImVec4 background_hov = ImColor(41, 44, 47, 150);
		inline ImVec4 background = ImColor(35, 38, 41, 150);

	}

}
