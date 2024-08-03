#ifndef TUI_HPP
#define TUI_HPP

#include <ncurses.h>
#include <string>
#include <vector>
#include "Domain.hpp"

class TUI {
    public:
        TUI();
        ~TUI();

        void initialize();                  // Initialize ncurses and set up interface
        void run();                         // Main loop to handle interaction
        void terminate();                   // Clean up and close ncurses

    private:
        void draw_main();                   // Draw the main window
        void draw_menu();                   // Draw the main menu
        void handle_input();                // Handle user input
        void execute_command(int command);  // Execute user command
        void show_status(const std::string& message, int command); // Show status window when user selects an option
        void display_graph(const std::string& function); // Display graph in ASCII art
        void get_input_for_domain(const std::string& prompt, int& target);
        void handle_domain(int ch, std::string& message, bool& continue_interaction);

        WINDOW* main_window;                // Main ncurses window
        WINDOW* menu_window;                // Menu window
        WINDOW* status_window;              // Window for when user selects an option
        std::vector<std::string> menu_items;
        int highlighted_item;
        int menu_size = 8;
        Domain domain;
};

#endif // TUI_HPP
