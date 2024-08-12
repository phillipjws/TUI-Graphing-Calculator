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
        void get_single_number_input(const std::string &prompt,
                                     int &target) const;
        void get_string_input(const std::string &prompt,
                              std::string &target) const;
        void get_char_input(const std::string &prompt, char &target) const;

        void show_status(const std::string &message, int command);

        void handle_sample_size(int ch, std::string &message,
                                bool &continue_interaction);
        void handle_domain(int ch, std::string &message,
                           bool &continue_interaction);
        void handle_output_directory(int ch, std::string &message,
                                     bool &continue_interaction);
        void handle_variable(int ch, std::string &message,
                             bool &continue_interaction);
        void handle_function(int ch, std::string &message,
                             bool &continue_interaction);
        void handle_running(int ch, std::string &message,
                            bool &continue_interaction);

        void run_calculation();

        void display_graph();
        void adjust_graph_domain_range();

        WINDOW *main_window;
        WINDOW *menu_window;
        WINDOW *status_window;
        WINDOW *result_window;
        WINDOW *graph_window;
        std::vector<std::string> menu_items;
        std::vector<std::pair<double, double>> results_;
        int highlighted_item;
        int menu_size = 8;
        AnalysisParameters parameters;

        int user_min_x_ = -10;
        int user_max_x_ = 10;
        int user_min_y_ = -5;
        int user_max_y_ = 5;
};

#endif // TUI_HPP
