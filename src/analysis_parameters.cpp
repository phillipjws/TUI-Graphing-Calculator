#include "analysis_parameters.hpp"
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <stdexcept>

// Constructor definition
AnalysisParameters::AnalysisParameters(int start, int end, int num_samples)
    : start_(start), end_(end), num_samples_(num_samples), min_step_(0.000001),
      output_status_(false),
      output_directory_path_(std::filesystem::current_path()) {
    if (!is_valid_domain()) {
        throw std::invalid_argument(
            "Invalid Parameters: Start must be less than End");
    }
    update_step();
}

// Getter for start_
int AnalysisParameters::get_start() const { return start_; }

// Getter for end_
int AnalysisParameters::get_end() const { return end_; }

// Getter for num_samples_
int AnalysisParameters::get_num_samples() const { return num_samples_; }

// Getter for step_
double AnalysisParameters::get_step() const { return step_; }

// Getter for min_step_
double AnalysisParameters::get_min_step() const { return min_step_; }

// Getter for output_status_
bool AnalysisParameters::get_output_status() const { return output_status_; }

// Getter for output_directory_path_
std::filesystem::path AnalysisParameters::get_output_directory_path() const {
    return output_directory_path_;
}

// Display the domain as a string
std::string AnalysisParameters::display_domain() const {
    return std::format("Domain: [{}, {}]", start_, end_);
}

// Display the number of samples and step size
std::string AnalysisParameters::display_num_step() const {
    return std::format("Number of samples: {}, Step size: {:.6f}", num_samples_,
                       step_);
}

// Display the output status as a string
std::string AnalysisParameters::display_output_status() const {
    if (output_status_) {
        return "Enable output file is: On";
    }
    return "Enable output file is: Off";
}

// Display the current output directory as a string
std::string AnalysisParameters::display_output_directory_path() const {
    if (output_status_) {
        return std::format("Current output directory is: {}",
                           output_directory_path_.string());
    }
    return "Enable output file is off, will not save any output";
}

// Setter for start_
void AnalysisParameters::set_start(int new_start) {
    start_ = new_start;
    if (!is_valid_domain()) {
        throw std::invalid_argument(
            "Invalid Parameters: Start must be less than End");
    }
    update_step();
}

// Setter for end_
void AnalysisParameters::set_end(int new_end) {
    end_ = new_end;
    if (!is_valid_domain()) {
        throw std::invalid_argument(
            "Invalid Parameters: Start must be less than End");
    }
    update_step();
}

// Setter for num_samples_
void AnalysisParameters::set_num_samples(int new_samples) {
    num_samples_ = new_samples;
    update_step();
    if (!is_valid_samples()) {
        throw std::invalid_argument("Invalid Parameters: Step size too small. "
                                    "Increase the number of samples.");
    }
}

void AnalysisParameters::set_output_status(bool choice) {
    output_status_ = choice;
}

void AnalysisParameters::set_ouput_directory_path(std::string new_dir) {
    output_directory_path_ = new_dir;
    if (!is_valid_output_path()) {
        throw std::invalid_argument(
            "Invalid Parameters: Must be an existing directory path.");
    }
}

// Update step based on current parameters
void AnalysisParameters::update_step() {
    if (num_samples_ > 0) {
        step_ = static_cast<double>(end_ - start_) / num_samples_;
    } else {
        step_ = std::numeric_limits<double>::infinity();
    }
}

// Check if the current domain parameters are valid
bool AnalysisParameters::is_valid_domain() const {
    return start_ < end_ && end_ - start_ >= 10;
}

// Check if current sample size parameters are valid
bool AnalysisParameters::is_valid_samples() const {
    return step_ >= min_step_ && num_samples_ > 0;
}

// Check if output diresctory path exists and is a directory
bool AnalysisParameters::is_valid_output_path() const {
    if (std::filesystem::exists(output_directory_path_) &&
        std::filesystem::is_directory(output_directory_path_)) {
        return true;
    }
    return false;
}
