#include "TUI.hpp"
#include <iostream>
#include <ncurses.h>
#include <map>
#include <limits>

TUI::TUI() : highlighted_item(0), parameters(-100, 100, 10000) {
    menu_window = nullptr;
    status_window = nullptr;
    menu_items = {
        "Input Function",
        "Change Domain",
        "Change Variables",
        "Change amount of Sample Points",
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

    for (int i = 0; i < title.size(); i++) {
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
            if (highlighted_item < 0) highlighted_item++;
            break;
        case KEY_DOWN:
            // Move down in the menu
            ++highlighted_item;
            if (highlighted_item >= menu_size) --highlighted_item;
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
            message = parameters.display_domain(); // Use parameters to display the domain
            break;
        case 2: // Change Variables
            message = "Change Variables is not yet implemented.";
            break;
        case 3: // Change Number of Sample Points
            message = "Using " + std::to_string(parameters.get_num_samples()) + " samples";
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
                break;

            case 1: { // Change Domain/Range
                mvwprintw(status_window, 3, 2, "Press S to change start or E to change end");

                wrefresh(status_window);
                ch = wgetch(status_window);
                handle_domain(ch, message, continue_interaction);

                break;
            }

            case 2: // Change Variables
                break;

            case 3: // Change Interval
                mvwprintw(status_window, 3, 2, "Press N to change number of samples");

                wrefresh(status_window);
                ch = wgetch(status_window);
                handle_sample_size(ch, message, continue_interaction);

                break;

            case 4: // Enable Output File
                break;

            case 5: // Set Export Directory
                break;

            case 6: // Help
                break;

            default:
                mvwprintw(status_window, 3, 2, "Invalid selection.");
                break;
        }

        mvwprintw(status_window, 7, 2, "Press B to go back");
        wrefresh(status_window);

        if (command != 1) { 
            ch = wgetch(status_window);
            if (ch == 'b' || ch == 'B') {
                continue_interaction = false;
            }
        }
    }

    delwin(status_window);
    status_window = nullptr;

    touchwin(stdscr);
    refresh();
}

void TUI::get_single_number_input(const std::string& prompt, int& target) {
    char input_str[10];
    int new_value;

    wclear(status_window);
    box(status_window, 0, 0);

    mvwprintw(status_window, 3, 2, "%s", prompt.c_str());
    wrefresh(status_window);

    echo();
    curs_set(1);

    wgetnstr(status_window, input_str, sizeof(input_str) - 1);

    noecho();
    curs_set(0);

    std::string input = input_str;
    size_t pos;

    try {
        new_value = std::stoi(input, &pos);
        if(pos != input.length()) {
            throw std::invalid_argument("Trailing characters");
        }
        target = new_value;
    } catch (const std::exception& e) {
        mvwprintw(status_window, 5, 2, "Invalid input. Please enter a number.");
        wrefresh(status_window);
        wgetch(status_window);
    }
}

void TUI::handle_sample_size(int ch, std::string& message, bool& continue_interaction) {
    const double min_step = std::numeric_limits<double>::epsilon() * 10;
    double epsilon = std::numeric_limits<double>::epsilon();

    switch (ch) {
        case 'n':
        case 'N':
            int new_num_samples;
            get_single_number_input("Enter new number of samples: ", new_num_samples);
            parameters.set_num_samples(new_num_samples);
            parameters.update_step();

            if (parameters.get_step() < min_step) {
                parameters.set_num_samples(static_cast<int>(parameters.get_end() - parameters.get_start()) / min_step);
                parameters.update_step();
                mvwprintw(status_window, 5, 2, "Sample size too large. Adjusted to %d samples.", parameters.get_num_samples());
                wrefresh(status_window);
                wgetch(status_window);
            }

            mvwprintw(status_window, 4, 2, "Interval step size is %f", parameters.get_step());
            break;

        case 'b':
        case 'B':
            continue_interaction = false;
            break;

        default:
            break;
    }

    wrefresh(status_window);
}

void TUI::handle_domain(int ch, std::string& message, bool& continue_interaction) {
    switch (ch) {
        case 's':
        case 'S': {
            int new_start;
            get_single_number_input("Enter new start value: ", new_start);
            try {
                parameters.set_start(new_start);
                message = parameters.display_domain();
            } catch (const std::exception& e) {
                mvwprintw(status_window, 5, 2, e.what());
                parameters.set_start(parameters.get_end() - 10); // Revert to a valid state
                message = parameters.display_domain();
                mvwprintw(status_window, 6, 2, "Press any key to continue...");
                wrefresh(status_window);
                wgetch(status_window);
            }
            break;
        }

        case 'e':
        case 'E': {
            int new_end;
            get_single_number_input("Enter new end value: ", new_end);
            try {
                parameters.set_end(new_end);
                message = parameters.display_domain();
            } catch (const std::exception& e) {
                mvwprintw(status_window, 5, 2, e.what());
                parameters.set_end(parameters.get_start() + 10); // Revert to a valid state
                message = parameters.display_domain();
                mvwprintw(status_window, 6, 2, "Press any key to continue...");
                wrefresh(status_window);
                wgetch(status_window);
            }
            break;
        }

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
