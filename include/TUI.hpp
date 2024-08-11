#ifndef TUI_HPP
#define TUI_HPP

#include "analysis_parameters.hpp"
#include <ncurses.h>
#include <string>
#include <vector>

class TUI {
    public:
        TUI();
        ~TUI();

        void initialize();
        void run();
        void terminate();

    private:
        void draw_main();
        void draw_menu();
        void handle_input();
        void execute_command(int command);
        void get_single_number_input(const std::string &prompt, int &target);
        void get_string_input(const std::string &prompt, std::string &target);
        void get_char_input(const std::string &prompt, char &target);

        void show_status(const std::string &message, int command);

        void handle_sample_size(int ch, std::string &message,
                                bool &continue_interaction);
        void handle_domain(int ch, std::string &message,
                           bool &continue_interaction);
        void handle_output_status(int ch, std::string &message,
                                  bool &continue_interaction);
        void handle_output_directory(int ch, std::string &message,
                                     bool &continue_interaction);
        void handle_variable(int ch, std::string &message,
                             bool &continue_interaction);
        void handle_function(int ch, std::string &message,
                             bool &continue_interaction);

        void run_calculation();
        void write_results_to_file(
            const std::vector<std::pair<double, double>> &results);
        void
        show_results(const std::vector<std::pair<double, double>> &results);

        void display_graph(const std::string &function);

        WINDOW *main_window;
        WINDOW *menu_window;
        WINDOW *status_window;
        WINDOW *result_window;
        std::vector<std::string> menu_items;
        int highlighted_item;
        int menu_size = 9;
        AnalysisParameters parameters;
};

#endif // TUI_HPP
