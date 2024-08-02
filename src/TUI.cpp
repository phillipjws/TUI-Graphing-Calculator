#include "TUI.hpp"
#include <iostream>
#include <ncurses.h>

TUI::TUI() : highlighted_item(0) {
    menu_window = nullptr;
    status_window = nullptr;
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
    if (status_window) {
        delwin(status_window);
    }
    endwin();
}

void TUI::draw_main() {
    clear();
    
    std::vector<std::string> title = {
        " ________  ______\n",
        "/_  __/ / / /  _/\n",
        " / / / /_/ // /  \n",
        "/_/  \\____/___/  \n",
        "  _____              __   _          \n",
        " / ___/______ ____  / /  (_)__  ___ _\n",
        "/ (_ / __/ _ `/ _ \\/ _ \\/ / _ \\/ _ `/\n",
        "\\___/_/  \\_,_/ .__/_//_/_/_//_/\\_, / \n",
        "            /_/               /___/  \n",
        "  _____     __         __     __          \n",
        " / ___/__ _/ /_____ __/ /__ _/ /____  ____\n",
        "/ /__/ _ `/ / __/ // / / _ `/ __/ _ \\/ __/\n",
        "\\___/\\_,_/_/\\__/\\_,_/_/\\_,_/\\__/\\___/_/   \n"

    };



    int title_start_x = 6;
    int title_start_y = getmaxy(stdscr) / 10;

    for(int i = 0; i < title.size(); i++) {
        mvwprintw(stdscr, title_start_y + i, title_start_x, title[i].c_str());
    }
    
    
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
    std::string message;
    switch (command) {
        case 0: // Input Function
            message = "Function input is not yet implemented.";
            break;
        case 1: // Change Domain/Range
            message = "Change Domain/Range is not yet implemented.";
            break;
        case 2: // Change Variables
            message = "Change Variables is not yet implemented.";
            break;
        case 3: // Change Interval
            message = "Change Interval is not yet implemented.";
            break;
        case 4: // Enable Output File
            message = "Enable Output File is not yet implemented.";
            break;
        case 5: // Set Export Directory
            message = "Set Export Directory is not yet implemented.";
            break;
        case 6: // Help
            message = "Help is not yet implemented.";
            break;
        case 7: // Quit
            // Exit the program
            terminate();
            exit(0);
        default:
            message = "Invalid selection.";
            break;
    }

    show_status(message); // Show status window with message
}

void TUI::show_status(const std::string& message) {
    int max_x, max_y;
    getmaxyx(stdscr, max_y, max_x);

    int status_height = 5;
    int status_width = 50;
    int status_start_y = (max_y - status_height) / 2; // Center vertically
    int status_start_x = (max_x - status_width) / 2; // Center horizontally

    status_window = newwin(status_height, status_width, status_start_y, status_start_x);
    keypad(status_window, TRUE);

    bool continue_interaction = true;
    while (continue_interaction) {
        wclear(status_window);
        box(status_window, 0, 0);

        mvwprintw(status_window, 2, 2, "%s", message.c_str());
        mvwprintw(status_window, 3, 2, "Press Enter to continue, or Q to quit");

        wrefresh(status_window);

        int ch = wgetch(status_window);
        switch (ch) {
            case 'q':
            case 'Q':
                continue_interaction = false; // Exit the loop on 'q' or 'Q'
                break;
            case '\n': // Enter key
                continue_interaction = false; // Exit the loop on Enter (for now)
                break;
            default:
                break;
        }
    }

    delwin(status_window);
    status_window = nullptr;

    touchwin(stdscr);
    refresh();
}

void TUI::display_graph(const std::string& function) {
    // Not yet implemented
}
