#include "TUI.hpp"
#include <iostream>
#include <ncurses.h>

TUI::TUI() : highlighted_item(0) {
    menu_window = nullptr;
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

    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_WHITE, COLOR_BLACK);
    } else {
        terminate();
        std::cerr << "Your terminal does not support color\n";
        exit(1);
    }

    int max_x, max_y;
    getmaxyx(stdscr, max_y, max_x);

    // Menu window setup
    int menu_height = menu_size + 2;
    int menu_width = 35;
    int menu_start_y = max_y - menu_height - 4;
    int menu_start_x = 6;

    menu_window = newwin(menu_height, menu_width, menu_start_y, menu_start_x);
    keypad(menu_window, TRUE);
    box(menu_window, 0, 0);

    wnoutrefresh(stdscr);
    wnoutrefresh(menu_window);
    doupdate();
}

void TUI::run() {
    bool running = true;
    while (running) {
        draw_main();
        draw_menu();
        handle_input();
    }
}

void TUI::terminate() {
    if (menu_window) {
        delwin(menu_window);
    }
    endwin();
}

void TUI::draw_main() {
    clear();
    
    const char* title_1 = " ________  ______  _____              __   _             _____     __         __     __          \n";
    const char* title_2 = "/_  __/ / / /  _/ / ___/______ ____  / /  (_)__  ___ _  / ___/__ _/ /_____ __/ /__ _/ /____  ____\n";
    const char* title_3 = " / / / /_/ // /  / (_ / __/ _ `/ _ \\/ _ \\/ / _ \\/ _ `/ / /__/ _ `/ / __/ // / / _ `/ __/ _ \\/ __/\n";
    const char* title_4 = "/_/  \\____/___/  \\___/_/  \\_,_/ .__/_//_/_/_//_/\\_, /  \\___/\\_,_/_/\\__/\\_,_/_/\\_,_/\\__/\\___/_/   \n";
    const char* title_5 = "                             /_/               /___/                                             \n";









    int title_start_x = 6;
    int title_start_y = getmaxy(stdscr) / 3;

    mvwprintw(stdscr, title_start_y, title_start_x, title_1);
    mvwprintw(stdscr, title_start_y + 1, title_start_x, title_2);
    mvwprintw(stdscr, title_start_y + 2, title_start_x, title_3);
    mvwprintw(stdscr, title_start_y + 3, title_start_x, title_4);
    mvwprintw(stdscr, title_start_y + 4, title_start_x, title_5);
    refresh();
}

void TUI::draw_menu() {
    // Clear and redraw the menu window
    wclear(menu_window);
    box(menu_window, 0, 0);

    for (int i = 0; i < menu_size; ++i) {
        if (i == highlighted_item) {
            wattron(menu_window, A_REVERSE | COLOR_PAIR(1));
            mvwprintw(menu_window, i + 1, 1, menu_items[i].c_str());
            wattroff(menu_window, A_REVERSE | COLOR_PAIR(1));
        } else {
            mvwprintw(menu_window, i + 1, 1, menu_items[i].c_str());
        }
    }

    wnoutrefresh(menu_window);
    doupdate();
}

void TUI::handle_input() {
    int ch = wgetch(menu_window);
    switch (ch) {
        case KEY_UP:
            // Move up in the menu
            --highlighted_item;
            if(highlighted_item < 0) highlighted_item++;
            break;
        case KEY_DOWN:
            // Move down in the menu
            ++highlighted_item;
            if(highlighted_item >= menu_size) --highlighted_item;
            break;
        case '\n':  // Enter key
            execute_command(highlighted_item);
            break;
        default:
            break;
    }
}

void TUI::execute_command(int command) {
    // Perform actions based on the selected command
    switch (command) {
        case 0: // Input Function
            mvwprintw(menu_window, 3, 2, "Function input is not yet implemented.");
            break;
        case 1: // Change Domain/Range
            mvwprintw(menu_window, 3, 2, "Change Domain/Range is not yet implemented.");
            break;
        case 2: // Change Variables
            mvwprintw(menu_window, 3, 2, "Change Variables is not yet implemented.");
            break;
        case 3: // Change Interval
            mvwprintw(menu_window, 3, 2, "Change Interval is not yet implemented.");
            break;
        case 4: // Enable Output File
            mvwprintw(menu_window, 3, 2, "Enable Output File is not yet implemented.");
            break;
        case 5: // Set Export Directory
            mvwprintw(menu_window, 3, 2, "Set Export Directory is not yet implemented.");
            break;
        case 6: // Help
            mvwprintw(menu_window, 3, 2, "Help is not yet implemented.");
            break;
        case 7: // Quit
            // Exit the program
            terminate();
            exit(0);
        default:
            break;
    }

    wnoutrefresh(menu_window);
    doupdate();
    wgetch(menu_window);
}

void TUI::display_graph(const std::string& function) {
    // Not yet implemented
}
