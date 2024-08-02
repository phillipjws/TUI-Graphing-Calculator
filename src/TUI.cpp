#include "TUI.hpp"
#include <iostream>
#include <ncurses.h>

TUI::TUI() : highlighted_item(0) {
    main_window = nullptr;
    menu_items = {
        "Input Function",
        "Change Domain",
        "Change Variables",
        "Change amount of sample points",
        "Enable Output File",
        "Set Export Directory",
        "Help",
        "Quit"
    };
}

TUI::~TUI() {
    terminate();
}

void TUI::initialize() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    if(has_colors()) {
        start_color();
        init_pair(1, COLOR_BLACK, COLOR_WHITE);
    }

    main_window = newwin(24, 80, 0, 0);
    refresh();
}

void TUI::run() {
    bool running = true;
    while (running) {
        draw_menu();
        handle_input();
    }
}

void TUI::terminate() {
    if(main_window) {
        delwin(main_window);
    }
    endwin();
}

void TUI::draw_menu() {
    // Clear the screen
    wclear(main_window);

    for(int i = 0; i < menu_size; ++i) {
        if(i == highlighted_item) {
            wattron(main_window, A_REVERSE | COLOR_PAIR(1));
            mvwprintw(main_window, i + 1, 1, menu_items[i].c_str());
            wattroff(main_window, A_REVERSE | COLOR_PAIR(1));
        } else {
            mvwprintw(main_window, i + 1, 1, menu_items[i].c_str());
        }
    }

    // Refresh window to show changes
    wrefresh(main_window);
}

void TUI::handle_input() {
    int ch = wgetch(main_window);
    switch (ch) {
        case KEY_UP:
            highlighted_item = (highlighted_item - 1 + menu_size) % menu_size;
            break;
        case KEY_DOWN:
            highlighted_item = (highlighted_item + 1) % menu_size;
            break;
        case '\n':  // Enter key
            execute_command(highlighted_item);
            break;
        default:
            break;
    }
}

void TUI::execute_command(int command) {
    switch (command) {
        case 0: // Input Function
            mvwprintw(main_window, 10, 1, "Function input is not yet implemented.");
            break;
        case 1: // Change Domain/Range
            mvwprintw(main_window, 10, 1, "Change Domain/Range is not yet implemented.");
            break;
        case 2: // Change Variables
            mvwprintw(main_window, 10, 1, "Change Variables is not yet implemented.");
            break;
        case 3: // Change Interval
            mvwprintw(main_window, 10, 1, "Change Interval is not yet implemented.");
            break;
        case 4: // Set Export Directory
            mvwprintw(main_window, 10, 1, "Set Export Directory is not yet implemented.");
            break;
        case 5: // Help
            mvwprintw(main_window, 10, 1, "Help is not yet implemented.");
            break;
        case 6: // Quit
            terminate();
            exit(0);
        default:
            break;
    }
    wrefresh(main_window);
    wgetch(main_window); // Pause to let user read the message
}

void TUI::display_graph(const std::string& function) {

}
