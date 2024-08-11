#include "TUI.hpp"
#include <cctype>
#include <cstring>
#include <fstream>
#include <iostream>
#include <ncurses.h>

constexpr int STATUS_WINDOW_WIDTH = 85;

TUI::TUI() : highlighted_item(0), parameters(-100, 100, 10000) {
    menu_window = nullptr;
    status_window = nullptr;
    menu_items = {"Run",
                  "Input Function",
                  "Change Domain",
                  "Change Independent Variable",
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
    nodelay(menu_window, FALSE);
    box(menu_window, 0, 0);

    wnoutrefresh(stdscr);
    wnoutrefresh(menu_window);
    doupdate();
}

void TUI::run() {
    bool running = true;
    while (running) {
        try {
            draw_main();
            draw_menu();
            handle_input();
        } catch (const std::exception &e) {
            std::cerr << "Exception caught in main loop: " << e.what()
                      << std::endl;
        }
    }
}

void TUI::terminate() {
    if (menu_window) {
        delwin(menu_window);
    }
    if (status_window) {
        delwin(status_window);
    }
    if (result_window) {
        delwin(result_window);
    }
    endwin();
}

void TUI::draw_main() {
    static bool first_draw = true;
    if (!first_draw)
        return;
    first_draw = false;

    std::vector<std::string> title = {
        " ______  __  __   __                                                  "
        "                     \n",
        "/\\__  _\\/\\ \\/\\ \\ /\\ \\                                         "
        "                            \n",
        "\\/_/\\ \\/\\ \\ \\_\\ \\\\ \\ \\                                     "
        "                               \n",
        "   \\ \\_\\ \\ \\_____\\\\ \\_\\                                      "
        "                             \n",
        "    \\/_/  \\/_____/ \\/_/                                            "
        "                       \n",
        "                                                                      "
        "                    \n",
        " ______   ______   ______   ______  __  __   __   __   __   ______    "
        "                    \n",
        "/\\  ___\\ /\\  == \\ /\\  __ \\ /\\  == \\/\\ \\_\\ \\ /\\ \\ /\\ "
        "\"-.\\ \\ /\\  ___\\                       \n",
        "\\ \\ \\__ \\\\ \\  __< \\ \\  __ \\\\ \\  _-/\\ \\  __ \\\\ \\ \\\\ "
        "\\ \\-.  \\\\ \\ \\__ \\                      \n",
        " \\ \\_____\\\\ \\_\\ \\_\\\\ \\_\\ \\_\\\\ \\_\\   \\ \\_\\ \\_\\\\ "
        "\\_\\\\ \\_\\\"\\_\\\\ \\_____\\                     \n",
        "  \\/_____/ \\/_/ /_/ \\/_/\\/_/ \\/_/    \\/_/\\/_/ \\/_/ \\/_/ "
        "\\/_/ \\/_____/                     \n",
        "                                                                      "
        "                    \n",
        " ______   ______   __       ______   __  __   __       ______   "
        "______  ______   ______   \n",
        "/\\  ___\\ /\\  __ \\ /\\ \\     /\\  ___\\ /\\ \\/\\ \\ /\\ \\     "
        "/\\  __ \\ /\\__  _\\/\\  __ \\ /\\  == \\  \n",
        "\\ \\ \\____\\ \\  __ \\\\ \\ \\____\\ \\ \\____\\ \\ \\_\\ \\\\ \\ "
        "\\____\\ \\  __ \\/_\\ \\_/\\ \\ \\/\\ \\\\ \\  __<   \n",
        " \\ \\_____\\\\ \\_\\ \\_\\\\ \\_____\\\\ \\_____\\\\ \\_____\\\\ "
        "\\_____\\\\ \\_\\ \\_\\  \\ \\_\\ \\ \\_____\\\\ \\_\\ \\_\\\n",
        "  \\/_____/ \\/_/\\/_/ \\/_____/ \\/_____/ \\/_____/ \\/_____/ "
        "\\/_/\\/_/   \\/_/  \\/_____/ \\/_/ /_/ \n"};

    int title_start_x = 6;
    int title_start_y = getmaxy(stdscr) / 10;

    for (int i = 0; i < title.size(); i++) {
        mvwprintw(stdscr, title_start_y + i, title_start_x, title[i].c_str());
    }

    wnoutrefresh(stdscr);
    doupdate();
}

void TUI::draw_menu() {
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
    case 0: // Run Function
        run_calculation();
        return;
    case 1: // Input Function
        message = parameters.display_expression();
        break;
    case 2: // Change Domain/Range
        message = parameters.display_domain();
        break;
    case 3: // Change Variables
        message = parameters.display_variable();
        break;
    case 4: // Change Number of Sample Points
        message = parameters.display_num_step();
        break;
    case 5: // Enable Output File
        message = parameters.display_output_status();
        break;
    case 6: // Set Export Directory
        message =
            parameters.display_output_directory_path(STATUS_WINDOW_WIDTH - 4);
        break;
    case 7: // Help
        message = "Help is not yet implemented.";
        break;
    case 8: // Quit
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
    int status_width = STATUS_WINDOW_WIDTH;
    int status_start_y = (max_y - status_height) / 2; // Center vertically
    int status_start_x = (max_x - status_width) / 2;  // Center horizontally

    status_window =
        newwin(status_height, status_width, status_start_y, status_start_x);
    keypad(status_window, TRUE);
    nodelay(status_window, FALSE);

    bool continue_interaction = true;
    std::string message = initial_message;

    while (continue_interaction) {
        werase(status_window);
        box(status_window, 0, 0);

        std::string displayed_message = message;

        mvwprintw(status_window, 1, 2, "%s", displayed_message.c_str());
        mvwprintw(status_window, 8, 2, "Press 'B' to go back");

        wnoutrefresh(status_window);
        doupdate();

        if (command == 1) {
            mvwprintw(status_window, 3, 2, "Press 'F' to change function");
            wnoutrefresh(status_window);
            doupdate();
        } else if (command == 2) {
            mvwprintw(status_window, 3, 2,
                      "Press 'S' to change start or 'E' to change end");
            wnoutrefresh(status_window);
            doupdate();
        } else if (command == 3) {
            mvwprintw(status_window, 3, 2,
                      "Press 'V' to change independant variable");
            wnoutrefresh(status_window);
            doupdate();
        } else if (command == 4) {
            mvwprintw(status_window, 3, 2,
                      "Press 'N' to change number of samples");
            wnoutrefresh(status_window);
            doupdate();
        } else if (command == 5) {
            mvwprintw(status_window, 3, 2,
                      "Press 'T' or 'F' to enable or disable output file");
        } else if (command == 6) {
            if (parameters.get_output_status()) {
                mvwprintw(status_window, 3, 2,
                          "Press 'D' to change the output directory");
            }
        }

        int ch = wgetch(status_window);
        switch (command) {
        case 1:
            handle_function(ch, message, continue_interaction);
            break;
        case 2:
            handle_domain(ch, message, continue_interaction);
            break;
        case 3:
            handle_variable(ch, message, continue_interaction);
            break;
        case 4:
            handle_sample_size(ch, message, continue_interaction);
            break;
        case 5:
            handle_output_status(ch, message, continue_interaction);
            break;
        case 6:
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
    mvwprintw(status_window, 1, 2, "%s", prompt.c_str());
    wnoutrefresh(status_window);
    doupdate();

    echo();
    curs_set(1);

    int input_area_width = getmaxx(status_window) - 4;
    int input_area_height = 3;
    WINDOW *input_win =
        derwin(status_window, input_area_height, input_area_width, 2, 2);
    keypad(input_win, TRUE);

    bool input_complete = false;

    while (!input_complete) {
        werase(input_win);
        box(input_win, 0, 0);

        int start_index = std::max(0, index - input_area_width + 3);
        mvwprintw(input_win, 1, 1, "%s", &input_str[start_index]);

        wnoutrefresh(input_win);
        doupdate();

        ch = wgetch(input_win);
        if (ch == '\n') {
            input_complete = true;
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

void TUI::get_char_input(const std::string &prompt, char &target) {
    char input_str[2];

    werase(status_window);
    box(status_window, 0, 0);

    mvwprintw(status_window, 3, 2, "%s", prompt.c_str());
    wnoutrefresh(status_window);
    doupdate();

    echo();
    curs_set(1);

    wgetnstr(status_window, input_str, 1);

    noecho();
    curs_set(0);

    try {
        if (!isalpha(input_str[0]) || strlen(input_str) != 1) {
            throw std::invalid_argument("Empty input or non-alphabetic");
        }
        target = input_str[0];
    } catch (const std::exception &e) {
        mvwprintw(status_window, 5, 2,
                  "Invalid input. Please enter a single alphabetic character.");
        wnoutrefresh(status_window);
        doupdate();
        wgetch(status_window);
    }
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
            parameters.set_num_samples(10000);
            message = parameters.display_num_step();
            mvwprintw(status_window, 8, 2, "Press any key to continue...");
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
            mvwprintw(status_window, 8, 2, "Press any key to continue...");
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
            parameters.set_end(parameters.get_start() + 10);
            message = parameters.display_domain();
            mvwprintw(status_window, 8, 2, "Press any key to continue...");
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
            parameters.set_output_directory_path(new_path);
            message = parameters.display_output_directory_path(
                STATUS_WINDOW_WIDTH - 4);
        } catch (const std::exception &e) {
            mvwprintw(status_window, 5, 2, e.what());
            parameters.set_output_directory_path(
                std::filesystem::current_path());
            message = parameters.display_output_directory_path(
                STATUS_WINDOW_WIDTH - 4);
            mvwprintw(status_window, 8, 2, "Press any key to continue...");
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

void TUI::handle_variable(int ch, std::string &message,
                          bool &continue_interaction) {
    switch (ch) {
    case 'v':
    case 'V': {
        char new_variable;
        get_char_input("Enter new independent variable character: ",
                       new_variable);

        try {
            parameters.set_variable(new_variable);
            message = parameters.display_variable();
        } catch (const std::exception &e) {
            mvwprintw(status_window, 5, 2, e.what());
            parameters.set_variable('x');
            message = parameters.display_variable();
            mvwprintw(status_window, 8, 2, "Press any key to continue...");
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

void TUI::handle_function(int ch, std::string &message,
                          bool &continue_interaction) {
    switch (ch) {
    case 'f':
    case 'F': {
        std::string new_expression;
        get_string_input("Enter new expression: ", new_expression);

        try {
            parameters.set_expression(new_expression);
            message = parameters.display_expression();
        } catch (const std::exception &e) {
            mvwprintw(status_window, 5, 2, e.what());
            parameters.set_expression(
                "sin(" + std::string(1, parameters.get_variable()) + ")");
            message = parameters.display_expression();
            mvwprintw(status_window, 8, 2, "Press any key to continue...");
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

void TUI::run_calculation() {
    double start = parameters.get_start();
    double end = parameters.get_end();
    double step = parameters.get_step();

    std::vector<std::pair<double, double>> results;

    results.clear();

    for (double x = start; x <= end; x += step) {
        parameters.set_variable_value(parameters.get_variable(), x);
        double y = parameters.evaluate_expression(x);
        results.emplace_back(x, y);
    }

    // TODO: Change this from using status_window
    if (parameters.get_output_status()) {
        // If output is enabled, write to file
        write_results_to_file(results);
    } else {
        show_results(results);
    }
}

void TUI::write_results_to_file(
    const std::vector<std::pair<double, double>> &results) {
    int max_x, max_y;
    getmaxyx(stdscr, max_y, max_x);

    int status_height = 10;
    int status_width = STATUS_WINDOW_WIDTH;
    int status_start_y = (max_y - status_height) / 2; // Center vertically
    int status_start_x = (max_x - status_width) / 2;  // Center horizontally

    status_window =
        newwin(status_height, status_width, status_start_y, status_start_x);
    keypad(status_window, TRUE);   // Enable keypad input
    nodelay(status_window, FALSE); // Disable non-blocking input

    std::string filename;
    get_string_input(
        "Enter filename to save to, output will save to <filename>.txt: ",
        filename);

    std::string filepath =
        (parameters.get_output_directory_path() / filename).string() + ".txt";

    std::ofstream outfile(filepath);

    werase(status_window);
    box(status_window, 0, 0);

    if (!outfile) {
        mvwprintw(status_window, 1, 2, "Error opening output file.");
    } else {
        for (const auto &[x, y] : results) {
            outfile << x << " " << y << "\n";
        }
        mvwprintw(status_window, 3, 2, "Results written to: %s.txt",
                  filename.c_str());
    }

    mvwprintw(status_window, 8, 2, "Press 'B' to go back");

    wnoutrefresh(status_window);
    doupdate();

    int ch;
    while ((ch = wgetch(status_window)) != 'b' && ch != 'B') {
        // Do nothing, just wait for 'B' or 'b'
    }

    delwin(status_window);
    status_window = nullptr;

    touchwin(stdscr);
    refresh();
}

void TUI::show_results(const std::vector<std::pair<double, double>> &results) {
    int max_x, max_y;
    getmaxyx(stdscr, max_y, max_x);

    int max_length = 0;
    for (const auto &[x, y] : results) {
        std::string x_str = std::to_string(x);
        std::string y_str = std::to_string(y);
        // Truncate or adjust the precision manually if needed
        if (x_str.length() > max_length)
            max_length = x_str.length();
        if (y_str.length() > max_length)
            max_length = y_str.length();
    }

    int result_height = static_cast<int>(max_y * 2 / 3.0) + 1;
    int padding = 4;
    int column_width = max_length + 4 * padding;
    int result_width = 2 * column_width;
    int result_start_y = (max_y - result_height) / 2;
    int result_start_x = (max_x - result_width) / 2;

    result_window =
        newwin(result_height, result_width, result_start_y, result_start_x);
    keypad(result_window, TRUE);
    nodelay(result_window, FALSE);

    int items_per_page = (result_height - 5);
    int total_pages = (results.size() + items_per_page * 2 - 1) /
                      (items_per_page * 2); // Total number of pages
    int current_page = 0;

    bool continue_interaction = true;

    while (continue_interaction) {
        werase(result_window);
        box(result_window, 0, 0);

        int start_index = current_page * items_per_page * 2;
        int end_index = std::min(start_index + items_per_page * 2,
                                 static_cast<int>(results.size()));

        for (int i = start_index; i < end_index; ++i) {
            int row = (i - start_index) % items_per_page;
            int col = (i - start_index) / items_per_page;

            mvwprintw(result_window, row + 1, col * (result_width / 2) + 3,
                      "%-10.3f %-10.3f", results[i].first, results[i].second);
        }

        std::string page_info = "Page " + std::to_string(current_page + 1) +
                                " of " + std::to_string(total_pages);
        mvwprintw(result_window, result_height - 3,
                  (result_width - page_info.size()) / 2, "%s",
                  page_info.c_str());
        mvwprintw(result_window, result_height - 2, (result_width - 21) / 2,
                  "Press 'B' to go back");

        wnoutrefresh(result_window);
        doupdate();

        int ch = wgetch(result_window);
        switch (ch) {
        case KEY_LEFT:
            if (current_page > 0) {
                --current_page;
            }
            break;
        case KEY_RIGHT:
            if (current_page < total_pages - 1) {
                ++current_page;
            }
            break;
        case 'b':
        case 'B':
            continue_interaction = false;
            break;
        default:
            break;
        }
    }

    delwin(result_window);
    result_window = nullptr;

    touchwin(stdscr);
    refresh();
}