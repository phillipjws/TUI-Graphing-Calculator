#ifndef ANALYSIS_PARAMETERS_HPP
#define ANALYSIS_PARAMETERS_HPP

#include <filesystem>
#include <string>

class AnalysisParameters {
    public:
        AnalysisParameters(int start = -100, int end = 100,
                           int num_samples = 10000);

        int get_start() const;
        int get_end() const;
        int get_num_samples() const;
        double get_step() const;
        double get_min_step() const;
        bool get_output_status() const;
        std::filesystem::path get_output_directory_path() const;

        std::string display_domain() const;
        std::string display_num_step() const;
        std::string display_output_status() const;
        std::string display_output_directory_path() const;

        void set_start(int new_start);
        void set_end(int new_end);
        void set_num_samples(int new_samples);
        void set_output_status(bool choice);
        void set_ouput_directory_path(std::string new_dir);
        bool is_valid_domain() const;
        bool is_valid_samples() const;
        void update_step();

    private:
        int start_;
        int end_;
        int num_samples_;
        double step_;
        double min_step_;
        bool output_status_;
        std::filesystem::path output_directory_path_;
};

#endif // ANALYSIS_PARAMETERS_HPP
