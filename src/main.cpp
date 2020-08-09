#include <iostream>

#include <spdlog/spdlog.h>
#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include <docopt/docopt.h>


static constexpr auto USAGE =
    R"(Naval Fate.

    Usage:
          naval_fate ship new <name>...
          naval_fate ship <name> move <x> <y> [--speed=<kn>]
          naval_fate ship shoot <x> <y>
          naval_fate mine (set|remove) <x> <y> [--moored | --drifting]
          naval_fate (-h | --help)
          naval_fate --version
 Options:
          -h --help     Show this screen.
          --version     Show version.
          --speed=<kn>  Speed in knots [default: 10].
          --moored      Moored (anchored) mine.
          --drifting    Drifting mine.
)";

int main([[maybe_unused]] int argc, [[maybe_unused]]const char **argv)
{
    // std::map<std::string, docopt::value> args = docopt::docopt(USAGE,
    //                                                            { std::next(argv), std::next(argv, argc) },
    //                                                            true,// show help if requested
    //                                                            "Naval Fate 2.0");// version string

    // for (auto const &arg :args) {
    //     std::cout << arg.first << arg.second << std::endl;
    // }


    //Use the default logger (stdout, multi-threaded, colored)
    spdlog::info("Starting ImGUI + SFML");

    fmt::print("Hello, from {}\n", "{fmt}");

    sf::RenderWindow window(sf::VideoMode(1024, 760), "ImGui + SFML = <3");
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);

    constexpr double scale_factor = 1.2f;
    ImGui::GetStyle().ScaleAllSizes(scale_factor);
    ImGui::GetIO().FontGlobalScale = scale_factor;


    // game status - keep outside of the main loop
    bool state0(false);
    bool state1(false);
    bool state2(false);
    bool state3(false);
    bool state4(false);
    bool state5(false);
    bool state6(false);
    bool state7(false);
    bool state8(false);
    bool state9(false);
    bool state10(false);
    bool state11(false);    
    bool state12(false);
    
    sf::Clock deltaClock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

//    ImGui::ShowDemoWindow();

        ImGui::Begin("The Plan");
        ImGui::Button("Look at this pretty button");


        ImGui::Checkbox(" 0: The Plan", &state0);
     
        ImGui::Checkbox(" 1: Getting Started", &state1);
        ImGui::Checkbox(" 2: Finding Errors as soon as possible", &state2);
        ImGui::Checkbox(" 3: Handling Command Line Paramenters", &state3);
        ImGui::Checkbox(" 4: C++ 20 So Far", &state4);
        ImGui::Checkbox(" 5: Reading SFML Input States", &state5);
        ImGui::Checkbox(" 6: Managing Game State", &state6);
        ImGui::Checkbox(" 7: Making Our Game Testable", &state7);
        ImGui::Checkbox(" 8: Making Game State Allocator Aware", &state8);
        ImGui::Checkbox(" 9: Add logging To Game Engine", &state9);
        ImGui::Checkbox("10: Draw A Game Map", &state10);
        ImGui::Checkbox("11: Dialog Trees", &state11);
        ImGui::Checkbox("12: Porting From SFML to SDL", &state12);

        ImGui::End();

        window.clear();
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();

    return 0;
}
