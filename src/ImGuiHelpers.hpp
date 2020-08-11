#ifndef __IMGUI_HELPERS_HPP__
#define __IMGUI_HELPERS_HPP__

#include <imgui.h>
#include <string_view>
#include <fmt/format.h>


namespace ImGuiHelper
{
    template<typename... Param> static void Text(std::string_view format, Param && ... param)
    {
        ImGui::TextUnformatted(fmt::format(format,  std::forward<Param>(param)...).c_str());
    }
} // namespace ImGuiHelper

#endif // __IMGUI_HELPERS_HPP__
