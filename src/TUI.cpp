#include "TUI.hpp"
#include <iostream>
#include <limits>
#include <map>
#include <ncurses.h>

TUI::TUI() : highlighted_item(0), parameters(-100, 100, 10000) {
    menu_window = nullptr;
    status_window = nullptr;
    menu_items = {"Input Function",
                  "Change Domain",
                  "Change Variables",
                  "Change amount of Sample Points",
                  "Enable Output File",
                  "Set Export Directory",
                  "Help",
                  "Quit"};
}

TUI::~TUI() { terminate(); }

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
    nodelay(menu_window, TRUE);
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
    static bool first_draw = true;
    if (!first_draw) return;
    first_draw = false;

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
        "\\___/\\_,_/_/\\__/\\_,_/_/\\_,_/\\__/\\___/_/   \n"};

    int title_start_x = 6;
    int title_start_y = getmaxy(stdscr) / 10;

    for (int i = 0; i < title.size(); i++) {
        mvwprintw(stdscr, title_start_y + i, title_start_x, title[i].c_str());
    }

    wnoutrefresh(stdscr);
    doupdate();
}

void TUI::draw_menu() {
    // Clear and redraw the menu window
    werase(menu_window);
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
        if (highlighted_item < 0)
            highlighted_item++;
        break;
    case KEY_DOWN:
        // Move down in the menu
        ++highlighted_item;
        if (highlighted_item >= menu_size)
            --highlighted_item;
        break;
    case '\n': // Enter key
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
        message = parameters.display_domain();
        break;
    case 2: // Change Variables
        message = "Change Variables is not yet implemented.";
        break;
    case 3: // Change Number of Sample Points
        message = parameters.display_num_step();
        break;
    case 4: // Enable Output File
        message = parameters.display_output_status();
        break;
    case 5: // Set Export Directory
        message = parameters.display_output_directory_path();
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

void TUI::show_status(const std::string &initial_message, int command) {
    int max_x, max_y;
    getmaxyx(stdscr, max_y, max_x);

    int status_height = 10;
    int status_width = 65;
    int status_start_y = (max_y - status_height) / 2; // Center vertically
    int status_start_x = (max_x - status_width) / 2;  // Center horizontally

    status_window =
        newwin(status_height, status_width, status_start_y, status_start_x);
    keypad(status_window, TRUE);
    nodelay(status_window, TRUE);

    bool continue_interaction = true;
    std::string message = initial_message;

    while (continue_interaction) {
        werase(status_window);
        box(status_window, 0, 0);

        mvwprintw(status_window, 1, 2, "%s", message.c_str());
        mvwprintw(status_window, 7, 2, "Press 'B' to go back");

        wnoutrefresh(status_window);
        doupdate();

        if (command == 1) {
            mvwprintw(status_window, 3, 2,
                      "Press 'S' to change start or 'E' to change end");
            wnoutrefresh(status_window);
            doupdate();
        } else if (command == 3) {
            mvwprintw(status_window, 3, 2,
                      "Press 'N' to change number of samples");
            wnoutrefresh(status_window);
            doupdate();
        } else if (command == 4) {
            mvwprintw(status_window, 3, 2,
                      "Press 'T' or 'F' to enable or disable output file");
        } else if (command == 5) {
            if (parameters.get_output_status()) {
                mvwprintw(status_window, 3, 2,
                          "Press 'D' to change the output directory");
            }
        }

        int ch = wgetch(status_window);
        switch (command) {
        case 1:
            handle_domain(ch, message, continue_interaction);
            break;
        case 3:
            handle_sample_size(ch, message, continue_interaction);
            break;
        case 4:
            handle_output_status(ch, message, continue_interaction);
            break;
        case 5:
            if (parameters.get_output_status()) {
                handle_output_directory(ch, message, continue_interaction);
            }
            if (ch == 'b' || ch == 'B') {
                continue_interaction = false;
            }
            break;
        default:
            if (ch == 'b' || ch == 'B') {
                continue_interaction = false;
            }
            break;
        }
    }

    delwin(status_window);
    status_window = nullptr;

    touchwin(stdscr);
    refresh();
}

void TUI::get_single_number_input(const std::string &prompt, int &target) {
    char input_str[10];
    int new_value;

    werase(status_window);
    box(status_window, 0, 0);

    mvwprintw(status_window, 3, 2, "%s", prompt.c_str());
    wnoutrefresh(status_window);
    doupdate();

    echo();
    curs_set(1);

    wgetnstr(status_window, input_str, sizeof(input_str) - 1);

    noecho();
    curs_set(0);

    std::string input = input_str;
    size_t pos;

    try {
        new_value = std::stoi(input, &pos);
        if (pos != input.length()) {
            throw std::invalid_argument("Trailing characters");
        }
        target = new_value;
    } catch (const std::exception &e) {
        mvwprintw(status_window, 5, 2, "Invalid input. Please enter a number.");
        wnoutrefresh(status_window);
        doupdate();
        wgetch(status_window);
    }
}

void TUI::get_string_input(const std::string &prompt, std::string &target) {
    const int input_max_length = 256;
    char input_str[input_max_length] = {0};
    int ch;
    int index = 0;

    werase(status_window);
    box(status_window, 0, 0);
    mvwprintw(status_window, 3, 2, "%s", prompt.c_str());
    wnoutrefresh(status_window);
    doupdate();

    echo();
    curs_set(1);

    int input_area_width = getmaxx(status_window) - 4;
    int input_area_height = 3;
    WINDOW *input_win =
        derwin(status_window, input_area_height, input_area_width, 5, 2);
    keypad(input_win, TRUE);

    while (true) {
        werase(input_win);
        box(input_win, 0, 0);

        int start_index = std::max(0, index - input_area_width + 3);
        mvwprintw(input_win, 1, 1, "%s", &input_str[start_index]);

        wnoutrefresh(status_window);
        doupdate();

        ch = wgetch(input_win);
        if (ch == '\n') {
            break;
        } else if (ch == KEY_BACKSPACE || ch == 127 || ch == 8) {
            if (index > 0) {
                input_str[--index] = '\0';
            }
        } else if (index < input_max_length - 1) {
            input_str[index++] = ch;
            input_str[index] = '\0';
        }
    }

    noecho();
    curs_set(0);
    delwin(input_win);

    target = std::string(input_str);
}

void TUI::handle_sample_size(int ch, std::string &message,
                             bool &continue_interaction) {
    switch (ch) {
    case 'n':
    case 'N': {
        int new_num_samples;
        get_single_number_input("Enter new number of samples: ",
                                new_num_samples);
        try {
            parameters.set_num_samples(new_num_samples);
            message = parameters.display_num_step();
        } catch (const std::exception &e) {
            mvwprintw(status_window, 5, 2, e.what());
            parameters.set_num_samples(static_cast<int>(
                (parameters.get_end() - parameters.get_start()) /
                parameters.get_min_step()));
            message = parameters.display_num_step();
            mvwprintw(status_window, 6, 2, "Press any key to continue...");
            wnoutrefresh(status_window);
            doupdate();
            wgetch(status_window);
        }
        break;
    }

    case 'b':
    case 'B':
        continue_interaction = false;
        return;

    default:
        break;
    }
}

void TUI::handle_domain(int ch, std::string &message,
                        bool &continue_interaction) {
    switch (ch) {
    case 's':
    case 'S': {
        int new_start;
        get_single_number_input("Enter new start value: ", new_start);
        try {
            parameters.set_start(new_start);
            message = parameters.display_domain();
        } catch (const std::exception &e) {
            mvwprintw(status_window, 5, 2, e.what());
            parameters.set_start(parameters.get_end() - 10);
            message = parameters.display_domain();
            mvwprintw(status_window, 6, 2, "Press any key to continue...");
            wnoutrefresh(status_window);
            doupdate();
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
        } catch (const std::exception &e) {
            mvwprintw(status_window, 5, 2, e.what());
            parameters.set_end(parameters.get_start() +
                               10); // Revert to a valid state
            message = parameters.display_domain();
            mvwprintw(status_window, 6, 2, "Press any key to continue...");
            wnoutrefresh(status_window);
            doupdate();
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

void TUI::handle_output_status(int ch, std::string &message,
                               bool &continue_interaction) {
    switch (ch) {
    case 't':
    case 'T': {
        parameters.set_output_status(true);
        message = parameters.display_output_status();
        break;
    }
    case 'f':
    case 'F': {
        parameters.set_output_status(false);
        message = parameters.display_output_status();
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

void TUI::handle_output_directory(int ch, std::string &message,
                                  bool &continue_interaction) {
    switch (ch) {
    case 'd':
    case 'D': {
        std::string new_path;
        get_string_input("Enter new output file directory path: ", new_path);

        try {
            parameters.set_ouput_directory_path(new_path);
            message = parameters.display_output_directory_path();
        } catch (const std::exception &e) {
            mvwprintw(status_window, 5, 2, e.what());
            parameters.set_ouput_directory_path(std::filesystem::current_path());
            message = parameters.display_output_directory_path();
            mvwprintw(status_window, 6, 2, "Press any key to continue...");
            wnoutrefresh(status_window);
            doupdate();
            wgetch(status_window);
        }
        break;
    }

    case 'b':
    case 'B':
        continue_interaction = false;
        return;

    default:
        break;
    }
}

void TUI::display_graph(const std::string &function) {
    // Not yet implemented
}
