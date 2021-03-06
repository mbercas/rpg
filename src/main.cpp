#include <SFML/Window/Joystick.hpp>
#include <bits/c++config.h>
#include <iostream>
#include <array>

#include <spdlog/spdlog.h>
#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include <docopt/docopt.h>
#include <type_traits>


#include "Input.hpp"
#include "ImGuiHelpers.hpp"

static constexpr auto USAGE =
    R"(game.

    Usage:
          game [options]

     Options:
          -h --help          Show this screen.
          --width=WIDTH      Screen width in pixels [default: 1024]
          --height=HEIGHT    Screen height in pixels [default: 768]
          --scale=SCALE      Scaling factor [default: 1]
)";


int main([[maybe_unused]] int argc, [[maybe_unused]]const char **argv)
{
    std::map<std::string, docopt::value> args = docopt::docopt(USAGE,
                                                               { std::next(argv), std::next(argv, argc) },
                                                               true,          // show help if requested
                                                               "game 0.3");   // version string


    //Use the default logger (stdout, multi-threaded, colored)
    spdlog::set_level(spdlog::level::debug);
    spdlog::info("Starting ImGUI + SFML");

    fmt::print("Hello, from {}\n", "{fmt}");

    
    const auto width = args["--width"].asLong();
    const auto height = args["--height"].asLong();
    const auto scale = args["--scale"].asLong();

    if (width < 0 || height < 0 || scale < 1 || scale > 5)
    {
        spdlog::error("Command line options out of reasoble range.");
        for (auto const & arg : args )
        {
            if (arg.second.isString()) {
                spdlog::info("Parameter set {}='{}'", arg.first, arg.second.asString());
            }
        }
        abort();
    }


    sf::RenderWindow window(sf::VideoMode(static_cast<unsigned int>(width), 
                                          static_cast<unsigned int>(height)
                                ), 
                            "ImGui + SFML = <3");
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);

    const auto scale_factor = static_cast<float>(scale);
    ImGui::GetStyle().ScaleAllSizes(scale_factor);
    ImGui::GetIO().FontGlobalScale = scale_factor;


    // requires c++-17
    constexpr std::array steps = {
        "The Plan",
        "Getting Started",
        "Finding Errors as soon as possible",
        "Handling Command Line Paramenters",
        "Reading SFML Joystick States",
        "Displaying SFML Joystick States",
        "Dealing With Game Event",
        "Reading SFML Keyboard States",
        "Reading SFML Mouse States",
        "Reading SFML Touchscreen States",
        "C++ 20 So Far",
        "Managing Game State",
        "Making Our Game Testable",
        "Making Game State Allocator Aware",
        "Add logging To Game Engine",
        "Draw A Game Map",
        "Dialog Trees",
        "Porting From SFML to SDL"};

    std::array<bool, steps.size()> states{};

    
    sf::Clock deltaClock;

    std::vector<Game::Joystick> joysticks;
    bool joystickEvent = false;
    
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);

            switch (event.type)
            {
            case sf::Event::Closed:
            {
                window.close();
                break;
            }
            case sf::Event::JoystickConnected:
            {
                joystickEvent = true;
                break;
            }
            case sf::Event::JoystickDisconnected:
            {
                joystickEvent = true;                
                break;
            }
            case sf::Event::JoystickButtonPressed: 
            {
                joystickEvent = true;

                auto &js = joystickById(joysticks, event.joystickButton.joystickId);
                js.buttonState[event.joystickButton.button] = true;

                break;
            }
            case sf::Event::JoystickButtonReleased: 
            {
                joystickEvent = true;

                auto &js = joystickById(joysticks, event.joystickButton.joystickId);
                js.buttonState[event.joystickButton.button] = false;

                break;
            }
            case sf::Event::JoystickMoved: 
            {
                joystickEvent = true;

                auto &js = joystickById(joysticks, event.joystickMove.joystickId);
                js.axisPosition[event.joystickMove.axis] = event.joystickMove.position; 

                break;
            }
            default:
                spdlog::trace("Undhandled Event Type");
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

//    ImGui::ShowDemoWindow();

        ImGui::Begin("The Plan");

        // for_init this will only work in C++20
        for (std::size_t index = 0; const auto &step : steps)
        {
            ImGui::Checkbox(fmt::format("{} : {}", index, step).c_str(),&states.at(index));
            ++index;
        }

        ImGui::End();

        ImGui::Begin("Joystick");
        
        ImGui::TextUnformatted(fmt::format("JS Event: {}", joystickEvent).c_str());

        if (!joysticks.empty())
        {
            for (std::size_t button = 0; button < joysticks[0].buttonCount; ++button)
            {
                ImGuiHelper::Text("{}: {}", button, joysticks[0].buttonState[button]);
            }

            for( std::size_t axis = 0; axis < sf::Joystick::AxisCount; ++axis )
            {
                ImGuiHelper::Text("{}: {}",
                                  Game::toString(static_cast<sf::Joystick::Axis>(axis)),
                                  joysticks[0].axisPosition[axis]);

            }
            
        }
        ImGui::End();
        
        window.clear();
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();

    return 0;
}
