#include "analysis_parameters.hpp"
#include <iostream>

// Constructor definition
AnalysisParameters::AnalysisParameters(int start, int end, int num_samples)
    : start_(start), end_(end), num_samples_(num_samples) {
    if (!is_valid()) {
        throw std::invalid_argument("Invalid Parameters: Start must be less than End");
    }
    update_step();
}

// Getter for start_
int AnalysisParameters::get_start() const {
    return start_;
}

// Getter for end_
int AnalysisParameters::get_end() const {
    return end_;
}

// Getter for num_samples_
int AnalysisParameters::get_num_samples() const {
    return num_samples_;
}

// Getter for step_
double AnalysisParameters::get_step() const {
    return step_;
}

// Display the domain as a string
std::string AnalysisParameters::display_domain() const {
    return "Domain: [" + std::to_string(start_) + ", " + std::to_string(end_) + "]";
}

// Display the number of samples and step size
std::string AnalysisParameters::display_num_step() const {
    return "Number of samples: " + std::to_string(num_samples_) + ", Step size: " + std::to_string(step_);
}

// Setter for start_
void AnalysisParameters::set_start(int new_start) {
    start_ = new_start;
    if (!is_valid()) {
        throw std::invalid_argument("Invalid Parameters: Start must be less than End");
    }
    update_step();
}

// Setter for end_
void AnalysisParameters::set_end(int new_end) {
    end_ = new_end;
    if (!is_valid()) {
        throw std::invalid_argument("Invalid Parameters: Start must be less than End");
    }
    update_step();
}

// Setter for num_samples_
void AnalysisParameters::set_num_samples(int new_samples) {
    num_samples_ = new_samples;
    update_step();
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
bool AnalysisParameters::is_valid() const {
    return start_ < end_ && end_ - start_ >= 10;
}
