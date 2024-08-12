#include "TUI.hpp"
#include <cctype>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <ncurses.h>
#include <sstream>
#include <string>

constexpr int STATUS_WINDOW_WIDTH = 85;

TUI::TUI() : highlighted_item(0), parameters(-100, 100, 10000), help_page(0) {
    menu_window = nullptr;
    status_window = nullptr;
    graph_window = nullptr;
    help_menu_window = nullptr;
    help_total_pages = 3;
    help_pages = {
        "\n"
        "Welcome to the TUI Graphing Calculator help menu.\n"
        "Use the arrow keys to navigate between pages.\n"
        "\n"
        "Menu:\n"
        "   The menu is the main form of interaction for users within\n"
        "   the TUI Graphing Calculator program. It allows the user to\n"
        "   change the parameters that will be used to create the graph.\n"
        "   The user can navigate the menu with the arrow keys, or using\n"
        "   the corresponding numbers next to each title. The current option\n"
        "   remains highlighted, giving some inclination to what option\n"
        "   you have selected. You can then choose that option with the\n"
        "   enter key, which will bring you to the options for that specific\n"
        "   menu option. Any time the user is prompted to make a choice using\n"
        "   a specific key on the keyboard, either capital or lowercase will "
        "be\n"
        "   accepted. (If the option prompts the user to press 'B', either "
        "'B'\n"
        "   or 'b' will ensure the desired outcome).\n",

        "\n"
        "1. Run:\n"
        "\n"
        "   The run option allows the user to see the output of the current\n"
        "   function. Gives the user the choice to save the output to a file,\n"
        "   with the input and output values separated by whitespace. If "
        "there\n"
        "   is no output value at the provided input, the output file will "
        "show\n"
        "   the input, the 'nan' where the output would be. This occurs for "
        "cases\n"
        "   like f(x) = ln(x), where x < 0. There are certain cases that cause "
        "the\n"
        "   output to be undefined, in the case of f(x) = 1/x, f(0) is "
        "undefined\n"
        "   due to the division by 0, in the output file here, the user sees "
        "the\n"
        "   input, and 'inf' in the output column.\n"
        "\n"
        "   Upon pressing 'O' to write output to a file, the user will be "
        "prompted\n"
        "   to enter a name for the file in which the output will be written.\n"
        "   If a file with the name already exists, the file will be saved "
        "with\n"
        "   a number appended. Meaning if output.txt exists, and the user "
        "enters\n"
        "   'output' as a name to save the output to, it will be saved as\n"
        "   'output1.txt'. This number will increase by 1 until it reaches a "
        "name\n"
        "   that doesn't already exist in the output directory.\n"
        "\n"
        "   If the user pressed 'G', they will be taken to a window showing a\n"
        "   graphical representation of the expression. The default window "
        "size\n"
        "   for the graph is [-10, 10] horizontally, and [-5, 5] vertically.\n"
        "   The user can press 'W' to change the window size, they will be "
        "prompted\n"
        "   to enter new values for the window size. These are limited to "
        "being\n"
        "   inside the domain for which the expression is evaluated, and the\n"
        "   minimums must be less than the maximums. If the user enters "
        "incorrect\n"
        "   parameters, they will be prompted to re-enter correct parameters.\n"
        "   When in the graph view, the user can press 'B' where they will "
        "return\n"
        "   back to the initial menu that appears when the users selects run\n",

        "\n"
        "2. Input Function:\n"
        "\n"
        "   The input function option allows the user to enter the expression "
        "that\n"
        "   will be evaluated by the program. It is defaulted to f(x) = "
        "sin(x), and\n"
        "   will go back to this value anytime the user enters an incorrect "
        "or\n"
        "   invalid expression. This will adjust to whatever the current\n"
        "   independent variable is set to. The user is limited in what "
        "functions\n"
        "   they can enter, this list is limited to:\n"
        "     - cos(x)\n"
        "     - sin(x)\n"
        "     - tan(x)\n"
        "     - arccos(x)\n"
        "     - arcsin(x)\n"
        "     - arctan(x)\n"
        "     - ln(x)\n"
        "     - log(x)\n"
        "     - sqrt(x)\n"
        "     - Addition: x + 3\n"
        "     - Subtraction: x - 3\n"
        "     - Exponents: x ^ 3\n"
        "     - Multiplication: x * 3\n"
        "     - Division: x / 3\n"
        "   * Note: these examples use the case that the independent variable "
        "is\n"
        "   set to x, although this can be changed, and functions will only "
        "be\n"
        "   validated and marked as correct if the correct variable is used. "
        "This\n"
        "   variable is case sensitive, if the independent variable is 'x',\n"
        "   'X' will throw an error\n"
        "\n"
        "   There are a number of mathematical/scientific constants available "
        "to\n"
        "   the user allowing for various applications. This list includes:\n"
        "     - e: Euler's Number\n"
        "     - c: Speed of light in a vacuum (m/s)\n"
        "     - g: Acceleration due to gravity (m/s^2)\n"
        "     - h: Planck's Constant (J*s)\n"
        "     - k: Boltzmann Constant (J/K)\n"
        "     - G: Gravitational Constant (m^3*kg^-1*s^-2)\n"
        "     - R: Universal Gas Constant (J/(mol*K))\n"
        "     - pi\n"
        "\n"
        "   The functions can take a number of different combinations, "
        "allowing\n"
        "   the user to enter something like:\n"
        "     - sin(pi) + 2 * ln(3 / x) - sqrt(682) + tan(g - x)\n"};
    menu_items = {" 1. Run ",
                  " 2. Input Function ",
                  " 3. Change Domain ",
                  " 4. Change Independent Variable ",
                  " 5. Change amount of Sample Points ",
                  " 6. Set Export Directory ",
                  " 7. Help ",
                  " 8. Quit "};
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
    int menu_width = 39;
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
        draw_main();
        draw_menu();
        handle_input();
    }
}

void TUI::terminate() {
    if (menu_window)
        delwin(menu_window);
    if (status_window)
        delwin(status_window);
    if (result_window)
        delwin(result_window);
    if (graph_window)
        delwin(graph_window);
    if (help_menu_window)
        delwin(help_menu_window);
    endwin();
}

void TUI::draw_main() {

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
        --highlighted_item;
        if (highlighted_item < 0)
            highlighted_item++;
        break;
    case KEY_DOWN:
        ++highlighted_item;
        if (highlighted_item >= menu_size)
            --highlighted_item;
        break;
    case '1': // Handle number input
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
        highlighted_item = ch - '1';
        break;
    case '\n':
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
        // run_calculation();
        message = "Ran calculation: " + parameters.display_expression();
        break;
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
    case 5: // Set Export Directory
        message =
            parameters.display_output_directory_path(STATUS_WINDOW_WIDTH - 4);
        break;
    case 6: // Help
        handle_help();
        return;
    case 7: // Quit
        terminate();
        exit(0);
    default:
        message = "Invalid selection.";
        break;
    }

    show_status(message, command);
}

void TUI::get_single_number_input(const std::string &prompt,
                                  int &target) const {
    char input_str[10];
    int new_value;
    bool valid_input = false;

    while (!valid_input) {
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
            valid_input = true; // Input is valid, exit loop
        } catch (const std::exception &e) {
            mvwprintw(status_window, 5, 2,
                      "Invalid input. Please enter a number.");
            wnoutrefresh(status_window);
            doupdate();
            wgetch(status_window); // Wait for user to press a key before
                                   // re-prompting
        }
    }
}

void TUI::get_string_input(const std::string &prompt,
                           std::string &target) const {
    const int input_max_length = 256;
    char input_str[input_max_length] = {0};
    int ch;
    int index = 0;
    bool valid_input = false;

    while (!valid_input) {
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
        index = 0; // Reset index for new input attempt

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
        if (!target.empty()) {
            valid_input = true;
        } else {
            mvwprintw(status_window, 5, 2,
                      "Invalid input. Please enter a non-empty string.");
            wnoutrefresh(status_window);
            doupdate();
            wgetch(
                status_window); // Wait for user to acknowledge before retrying
        }
    }
}

void TUI::get_char_input(const std::string &prompt, char &target) const {
    char input_str[2];
    bool valid_input = false;

    while (!valid_input) {
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
            valid_input = true;
        } catch (const std::exception &e) {
            mvwprintw(
                status_window, 5, 2,
                "Invalid input. Please enter a single alphabetic character.");
            wnoutrefresh(status_window);
            doupdate();
            wgetch(status_window);
        }
    }
}

void TUI::show_status(const std::string &initial_message, int command) {
    int max_x, max_y;
    getmaxyx(stdscr, max_y, max_x);

    int status_height = 10;
    int status_width = STATUS_WINDOW_WIDTH;
    int status_start_y = (max_y - status_height) / 2;
    int status_start_x = (max_x - status_width) / 2;

    status_window =
        newwin(status_height, status_width, status_start_y, status_start_x);
    keypad(status_window, TRUE);
    nodelay(status_window, FALSE);

    bool continue_interaction = true;
    std::string message = initial_message;

    while (continue_interaction) {
        werase(status_window);
        box(status_window, 0, 0);

        mvwprintw(status_window, 1, 2, "%s", message.c_str());
        mvwprintw(status_window, 8, 2, "Press 'B' to go back");

        if (command == 0) {
            mvwprintw(status_window, 3, 2,
                      "Press 'O' to save output to file or 'G' to view graph");
        } else if (command == 1) {
            mvwprintw(status_window, 3, 2, "Press 'C' to change function");
        } else if (command == 2) {
            mvwprintw(status_window, 3, 2,
                      "Press 'S' to change start or 'E' to change end");
        } else if (command == 3) {
            mvwprintw(status_window, 3, 2,
                      "Press 'V' to change independent variable");
        } else if (command == 4) {
            mvwprintw(status_window, 3, 2,
                      "Press 'N' to change number of samples");
        } else if (command == 5) {
            mvwprintw(status_window, 3, 2,
                      "Press 'D' to change the output directory");
        }

        wnoutrefresh(status_window);
        doupdate();

        int ch = wgetch(status_window);
        switch (ch) {
        case 'o':
        case 'O':
        case 'g':
        case 'G':
            if (command == 0)
                handle_running(ch, message, continue_interaction);
            break;
        case 'c':
        case 'C':
            if (command == 1)
                handle_function(ch, message, continue_interaction);
            break;
        case 's':
        case 'S':
        case 'e':
        case 'E':
            if (command == 2)
                handle_domain(ch, message, continue_interaction);
            break;
        case 'v':
        case 'V':
            if (command == 3)
                handle_variable(ch, message, continue_interaction);
            break;
        case 'n':
        case 'N':
            if (command == 4)
                handle_sample_size(ch, message, continue_interaction);
            break;
        case 'd':
        case 'D':
            if (command == 6)
                handle_output_directory(ch, message, continue_interaction);
            break;
        case 'b':
        case 'B':
            continue_interaction = false;
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

void TUI::handle_sample_size(int ch, std::string &message,
                             bool &continue_interaction) {
    if (ch == 'n' || ch == 'N') {
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
    }
}

void TUI::handle_domain(int ch, std::string &message,
                        bool &continue_interaction) {
    if (ch == 's' || ch == 'S') {
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
    } else if (ch == 'e' || ch == 'E') {
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
    }
}

void TUI::handle_output_directory(int ch, std::string &message,
                                  bool &continue_interaction) {
    if (ch == 'd' || ch == 'D') {
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
    }
}

void TUI::handle_variable(int ch, std::string &message,
                          bool &continue_interaction) {
    if (ch == 'v' || ch == 'V') {
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
    }
}

void TUI::handle_function(int ch, std::string &message,
                          bool &continue_interaction) {
    if (ch == 'c' || ch == 'C') {
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
    }
}

void TUI::handle_running(int ch, std::string &message,
                         bool &continue_interaction) {
    run_calculation();
    if (ch == 'o' || ch == 'O') {
        std::string filename;
        get_string_input(
            "Enter filename to save to, output will save to <filename>.txt: ",
            filename);

        std::string filepath =
            (parameters.get_output_directory_path() / filename).string();

        std::string full_filepath = filepath + ".txt";

        int file_index = 0;
        while (std::ifstream(full_filepath)) {
            file_index++;
            full_filepath = filepath + std::to_string(file_index) + ".txt";
        }

        std::ofstream outfile(full_filepath);

        werase(status_window);
        box(status_window, 0, 0);

        if (!outfile) {
            mvwprintw(status_window, 1, 2, "Error opening output file.");
        } else {
            for (const auto &[x, y] : results_) {
                outfile << x << " " << y << "\n";
            }
            std::string display_filename =
                full_filepath.substr(full_filepath.find_last_of("/") + 1);
            mvwprintw(status_window, 3, 2, "Results written to: %s",
                      display_filename.c_str());
            mvwprintw(status_window, 8, 2, "Press any key to continue...");
            wnoutrefresh(status_window);
            doupdate();
            wgetch(status_window);
        }

    } else if (ch == 'g' || ch == 'G') {
        display_graph();
    }
}

void TUI::run_calculation() {
    double start = parameters.get_start();
    double end = parameters.get_end();
    double step = parameters.get_step();

    results_.clear();
    for (double x = start; x <= end; x += step) {
        parameters.set_variable_value(parameters.get_variable(), x);
        double y = parameters.evaluate_expression(x);
        results_.emplace_back(x, y);
    }
}

void TUI::display_graph() {
    int terminal_max_x, terminal_max_y;
    getmaxyx(stdscr, terminal_max_y, terminal_max_x);

    // Determine the boundaries of the graph window
    int graph_width = terminal_max_x - 10; // leave some padding
    int graph_height = terminal_max_y - 10;

    // Create the graph window
    graph_window = newwin(graph_height + 4, graph_width + 4, 3, 3);
    box(graph_window, 0, 0);

    // Define the dimensions and position for the inner box
    int inner_start_y = 3;
    int inner_start_x = 3;
    int inner_width = graph_width - 3;   // Adjust for padding
    int inner_height = graph_height - 2; // Adjust for padding

    // Draw the inner box
    mvwhline(graph_window, inner_start_y, inner_start_x, 0, inner_width);
    mvwhline(graph_window, inner_start_y + inner_height, inner_start_x, 0,
             inner_width);
    mvwvline(graph_window, inner_start_y, inner_start_x, 0, inner_height);
    mvwvline(graph_window, inner_start_y, inner_start_x + inner_width, 0,
             inner_height);

    // Draw corners of the inner box
    mvwaddch(graph_window, inner_start_y, inner_start_x, ACS_ULCORNER);
    mvwaddch(graph_window, inner_start_y, inner_start_x + inner_width,
             ACS_URCORNER);
    mvwaddch(graph_window, inner_start_y + inner_height, inner_start_x,
             ACS_LLCORNER);
    mvwaddch(graph_window, inner_start_y + inner_height,
             inner_start_x + inner_width, ACS_LRCORNER);

    // User-specified or default domain/range
    double graph_min_x = user_min_x_;
    double graph_max_x = user_max_x_;
    double graph_min_y = user_min_y_;
    double graph_max_y = user_max_y_;

    // Ensure the graph's range covers the data range
    if (graph_min_x > graph_max_x)
        std::swap(graph_min_x, graph_max_x);
    if (graph_min_y > graph_max_y)
        std::swap(graph_min_y, graph_max_y);

    // Avoid division by zero in scaling
    double x_range =
        (graph_max_x - graph_min_x) != 0 ? (graph_max_x - graph_min_x) : 1;
    double y_range =
        (graph_max_y - graph_min_y) != 0 ? (graph_max_y - graph_min_y) : 1;

    // Scale the results to fit within the inner box dimensions
    auto scale_x = [&](double x) {
        return inner_start_x +
               static_cast<int>(((x - graph_min_x) / x_range) * inner_width);
    };
    auto scale_y = [&](double y) {
        return inner_start_y +
               static_cast<int>(((y - graph_min_y) / y_range) * inner_height);
    };

    // Determine positions for the axes
    int y_axis_pos = (graph_min_x <= 0 && graph_max_x >= 0) ? scale_x(0) : -1;
    int x_axis_pos = (graph_min_y <= 0 && graph_max_y >= 0)
                         ? inner_height - scale_y(0) + inner_start_y
                         : -1;

    // Draw X and Y axes if they exist within the range
    if (x_axis_pos != -1) {
        for (int i = inner_start_x + 1; i < inner_start_x + inner_width; ++i) {
            mvwaddch(graph_window, x_axis_pos, i, '-'); // X-axis
        }
    }
    if (y_axis_pos != -1) {
        for (int i = inner_start_y + 1; i < inner_start_y + inner_height; ++i) {
            mvwaddch(graph_window, i, y_axis_pos, '|'); // Y-axis
        }
        if (x_axis_pos != -1) {
            mvwaddch(graph_window, x_axis_pos, y_axis_pos, '+'); // Origin
        }
    }

    // Plot the points, skipping NaN values
    for (const auto &[x, y] : results_) {
        if (std::isnan(y))
            continue; // Skip NaN values

        // Ensure the points are within the user-defined domain and range
        if (x < graph_min_x || x > graph_max_x || y < graph_min_y ||
            y > graph_max_y) {
            continue;
        }

        int scaled_x = scale_x(x);
        int scaled_y = inner_start_y + inner_height - scale_y(y);

        // Ensure points are within graph boundaries
        if (scaled_x > inner_start_x &&
            scaled_x < inner_start_x + inner_width &&
            scaled_y > inner_start_y &&
            scaled_y < inner_start_y + inner_height) {
            mvwaddch(graph_window, scaled_y, scaled_x, '*');
        }
    }

    mvwprintw(graph_window, 2,
              (graph_width + 4 - parameters.display_expression().length()) / 2,
              parameters.display_expression().c_str());

    // Display domain and range information at the top, outside the inner box
    mvwprintw(graph_window, 1, 2, "Domain: [%d, %d]", user_min_x_, user_max_x_);
    mvwprintw(graph_window, 2, 2, "Range: [%d, %d]", user_min_y_, user_max_y_);

    // Display command instructions at the bottom, outside the inner box
    mvwprintw(graph_window, graph_height + 2, 2,
              "Press 'B' to go back or 'W' to change the window settings");

    wnoutrefresh(graph_window);
    doupdate();

    // Wait for user input to return
    int ch;
    while ((ch = wgetch(graph_window)) != 'b' && ch != 'B') {
        if (ch == 'w' || ch == 'W') {
            adjust_graph_domain_range();
            display_graph();
            return; // Redraw the graph with the new domain/range
        }
        // Wait for 'B' to go back
    }

    delwin(graph_window);
    graph_window = nullptr;

    // Clear the entire screen and force refresh
    clear();
    refresh();

    // Clear and redraw the entire screen
    draw_main(); // Redraw the title and main elements
    draw_menu(); // Redraw the menu window

    wnoutrefresh(menu_window);
    wnoutrefresh(stdscr);

    doupdate();
}

void TUI::adjust_graph_domain_range() {
    int status_height = 10;
    int status_width = STATUS_WINDOW_WIDTH;
    int max_x, max_y;
    getmaxyx(stdscr, max_y, max_x);

    int status_start_y = (max_y - status_height) / 2;
    int status_start_x = (max_x - status_width) / 2;

    status_window =
        newwin(status_height, status_width, status_start_y, status_start_x);
    keypad(status_window, TRUE);
    nodelay(status_window, FALSE);
    box(status_window, 0, 0);
    wnoutrefresh(status_window);
    doupdate();

    int min_x, max_x_value, min_y, max_y_value;
    bool valid_input = false;

    // Loop for X values
    while (!valid_input) {
        get_single_number_input("Enter minimum X value: ", min_x);
        get_single_number_input("Enter maximum X value: ", max_x_value);

        if (min_x >= parameters.get_start() &&
            max_x_value <= parameters.get_end() && min_x < max_x_value) {
            valid_input = true;
        } else {
            if (min_x >= max_x_value) {
                mvwprintw(status_window, 5, 2,
                          "Invalid Window Settings: Minimum X must be less "
                          "than Maximum X.");
            } else {
                mvwprintw(status_window, 5, 2,
                          "Invalid Window Setting: Window cannot be wider than "
                          "domain.");
            }
            wnoutrefresh(status_window);
            doupdate();
            wgetch(status_window); // Wait for keypress
        }
    }

    valid_input = false;

    // Loop for Y values
    while (!valid_input) {
        get_single_number_input("Enter minimum Y value: ", min_y);
        get_single_number_input("Enter maximum Y value: ", max_y_value);

        if (min_y < max_y_value) {
            valid_input = true;
        } else {
            mvwprintw(status_window, 5, 2,
                      "Invalid Window Settings: Minimum Y must be less than "
                      "Maximum Y.");
            wnoutrefresh(status_window);
            doupdate();
            wgetch(status_window); // Wait for keypress
        }
    }

    // Assign valid values to the class members
    user_min_x_ = min_x;
    user_max_x_ = max_x_value;
    user_min_y_ = min_y;
    user_max_y_ = max_y_value;

    delwin(status_window);
    status_window = nullptr;

    touchwin(stdscr);
    wnoutrefresh(graph_window);
    doupdate();
}

void TUI::handle_help() {
    int max_x, max_y;
    getmaxyx(stdscr, max_y, max_x);

    int help_height = max_y - 10;
    int help_width = static_cast<int>(max_x / 3);
    int help_start_y = (max_y - help_height) / 2;
    int help_start_x = (max_x - help_width) / 2;

    help_menu_window =
        newwin(help_height, help_width, help_start_y, help_start_x);
    keypad(help_menu_window, TRUE);
    nodelay(help_menu_window, FALSE);
    box(help_menu_window, 0, 0);

    help_page = 0;
    draw_help_menu();
    handle_help_input();

    delwin(help_menu_window);
    help_menu_window = nullptr;

    // Clear the screen and redraw the entire interface
    clear();
    refresh();

    // Redraw the title and main elements
    draw_main(); // Redraw the title and main elements
    draw_menu(); // Redraw the menu window

    // Ensure that status_window is not refreshed or drawn
    if (status_window) {
        werase(status_window);       // Erase any content from the status window
        wnoutrefresh(status_window); // Refresh to clear the content
    }

    wnoutrefresh(menu_window);
    wnoutrefresh(stdscr);
    doupdate();
}

void TUI::draw_help_menu() {
    werase(help_menu_window);
    box(help_menu_window, 0, 0);

    int line_y = 1; // Start at line 1
    const std::string &current_page = help_pages[help_page];
    std::istringstream stream(current_page);
    std::string line;

    while (std::getline(stream, line)) {
        mvwprintw(help_menu_window, line_y++, 2, "%s", line.c_str());
    }

    // Display page information
    mvwprintw(help_menu_window, getmaxy(help_menu_window) - 2, 2, "Page %d/%d",
              help_page + 1, help_total_pages);
    mvwprintw(help_menu_window, getmaxy(help_menu_window) - 2,
              getmaxx(help_menu_window) - 20, "Press 'Q' to quit");

    wnoutrefresh(help_menu_window);
    doupdate();
}

void TUI::handle_help_input() {
    int ch;
    bool in_help_menu = true;

    while (in_help_menu) {
        ch = wgetch(help_menu_window);
        switch (ch) {
        case KEY_RIGHT:
        case KEY_DOWN:
            if (help_page < help_total_pages - 1) {
                ++help_page;
            }
            break;
        case KEY_LEFT:
        case KEY_UP:
            if (help_page > 0) {
                --help_page;
            }
            break;
        case 'q':
        case 'Q':
            in_help_menu = false;
            break;
        default:
            break;
        }
        draw_help_menu();
    }
}
