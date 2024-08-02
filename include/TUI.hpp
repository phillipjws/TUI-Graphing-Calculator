#ifndef TUI_HPP
#define TUI_HPP

#include <ncurses.h>
#include <string>
#include <vector>

class TUI {
    public:
        TUI();
        ~TUI();

        void initialize();                  // Initialize ncurses and set up interface
        void run();                         // Main loop to handle interaction
        void terminate();                   // Clean up and close ncurses

    private:
        void draw_menu();                   // Draw the main menu
        void handle_input();                // Handle user input
        void execute_command(int command);  // Execute user command
        void display_graph(const std::string& function); // Display graph in ASCII art

        WINDOW* main_window;                // Main ncurses window
        std::vector<std::string> menu_items;
        int highlighted_item;
        int menu_size;
};

#endif // TUI_HPP
