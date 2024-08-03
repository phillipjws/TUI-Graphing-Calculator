#include "TUI.hpp"
#include <iostream>
#include <ncurses.h>
#include <map>

TUI::TUI() : highlighted_item(0), domain(-100, 100) {
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
            message = domain.display();
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

    show_status(message, command);
}

void TUI::show_status(const std::string& initial_message, int command) {
    int max_x, max_y;
    getmaxyx(stdscr, max_y, max_x);

    int status_height = 10;
    int status_width = 50;
    int status_start_y = (max_y - status_height) / 2; // Center vertically
    int status_start_x = (max_x - status_width) / 2; // Center horizontally

    status_window = newwin(status_height, status_width, status_start_y, status_start_x);
    keypad(status_window, TRUE);

    bool continue_interaction = true;
    std::string message = initial_message;

    while (continue_interaction) {
        wclear(status_window);
        box(status_window, 0, 0);

        mvwprintw(status_window, 1, 2, "%s", message.c_str());
        mvwprintw(status_window, 7, 2, "Press B to go back");

        int ch;
        switch (command) {
            case 0: // Input Function
                mvwprintw(status_window, 3, 2, "Function input not yet implemented.");
                break;

            case 1: { // Change Domain/Range
                mvwprintw(status_window, 3, 2, "Press S to change start or E to change end");

                wrefresh(status_window);
                ch = wgetch(status_window);
                handle_domain(ch, message, continue_interaction);

                break; // Ensure we exit this block properly
            }

            case 2: // Change Variables
                mvwprintw(status_window, 3, 2, "Change Variables not yet implemented.");
                break;

            case 3: // Change Interval
                mvwprintw(status_window, 3, 2, "Change Interval not yet implemented.");
                break;

            case 4: // Enable Output File
                mvwprintw(status_window, 3, 2, "Enable Output File not yet implemented.");
                break;

            case 5: // Set Export Directory
                mvwprintw(status_window, 3, 2, "Set Export Directory not yet implemented.");
                break;

            case 6: // Help
                mvwprintw(status_window, 3, 2, "Help not yet implemented.");
                break;

            default:
                mvwprintw(status_window, 3, 2, "Invalid selection.");
                break;
        }

        mvwprintw(status_window, 7, 2, "Press B to go back");
        wrefresh(status_window); // Refresh to display all updates

        // Wait for user input to go back only if not in the middle of domain change
        if (command != 1) { 
            int ch = wgetch(status_window);
            if (ch == 'b' || ch == 'B') {
                continue_interaction = false; // Exit the loop
            }
        }
    }

    delwin(status_window);
    status_window = nullptr;

    touchwin(stdscr);
    refresh();
}

void TUI::get_input_for_domain(const std::string& prompt, int& target) {
    char input_str[10]; // Buffer for input
    int new_value; // To hold the new integer value

    wclear(status_window); // Clear the window for fresh input
    box(status_window, 0, 0); // Draw a box around the window

    mvwprintw(status_window, 3, 2, "%s", prompt.c_str());
    wrefresh(status_window);

    echo(); // Enable echoing of typed characters
    curs_set(1); // Show the cursor

    wgetnstr(status_window, input_str, sizeof(input_str) - 1); // Read input

    noecho(); // Disable echoing
    curs_set(0); // Hide the cursor

    // Try to convert the input string to an integer
    try {
        new_value = std::stoi(input_str); // Convert string to integer
        target = new_value; // Update the target value
    } catch (const std::exception& e) {
        mvwprintw(status_window, 5, 2, "Invalid input. Please enter a number.");
        wrefresh(status_window);
        wgetch(status_window); // Wait for user input to acknowledge error
    }
}

void TUI::handle_domain(int ch, std::string& message, bool& continue_interaction) {
    switch (ch) {
    case 's':
    case 'S':
        get_input_for_domain("Enter new start value: ", domain.start);
        if (!domain.is_valid()) {
            mvwprintw(status_window, 5, 2, "Start must be less than end.");
            domain.start = domain.end - 1;
            mvwprintw(status_window, 6, 2, "Press any key to continue...");
            wrefresh(status_window);
            wgetch(status_window);
        }
        message = domain.display();
        break;

    case 'e':
    case 'E':
        get_input_for_domain("Enter new end value: ", domain.end);
        if (!domain.is_valid()) {
            mvwprintw(status_window, 5, 2, "End must be greater than start.");
            domain.end = domain.start + 1;
            mvwprintw(status_window, 6, 2, "Press any key to continue...");
            wrefresh(status_window);
            wgetch(status_window);
        }
        message = domain.display();
        break;

    case 'b':
    case 'B':
        continue_interaction = false;
        break;

    default:
        break;
    }
}

void TUI::display_graph(const std::string& function) {
    // Not yet implemented
}
