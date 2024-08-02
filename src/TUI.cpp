#include "TUI.hpp"
#include <iostream>
#include <ncurses.h>

TUI::TUI() {
    main_window = nullptr;
}

TUI::~TUI() {
    terminate();
}

void TUI::initialize() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

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

    // Display menu options
    mvwprintw(main_window, 1, 1, "Terminal Based Graphing Calculator");
    mvwprintw(main_window, 3, 1, "1. Input Function");
    mvwprintw(main_window, 4, 1, "2. Change Domain/Range");
    mvwprintw(main_window, 5, 1, "3. Change Variables");
    mvwprintw(main_window, 6, 1, "4. Change Interval");
    mvwprintw(main_window, 7, 1, "5. Set Export Directory");
    mvwprintw(main_window, 8, 1, "6. Help");
    mvwprintw(main_window, 9, 1, "7. Quit");

    // Refresh window to show changes
    wrefresh(main_window);
}

void TUI::handle_input() {
    int ch = wgetch(main_window);
    execute_command(ch);
}
