#include "analysis_parameters.hpp"
#include <iostream>
#include <stdexcept>
#include <format>

// Constructor definition
AnalysisParameters::AnalysisParameters(int start, int end, int num_samples)
    : start_(start), end_(end), num_samples_(num_samples), min_step_(0.000001) {
    if (!is_valid_domain()) {
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

// Getter for min_step_
double AnalysisParameters::get_min_step() const {
    return min_step_;
}

// Display the domain as a string
std::string AnalysisParameters::display_domain() const {
    return "Domain: [" + std::to_string(start_) + ", " + std::to_string(end_) + "]";
}

// Display the number of samples and step size
std::string AnalysisParameters::display_num_step() const {
    return std::format("Number of samples: {}, Step size: {:.6f}", num_samples_, step_);
}

// Setter for start_
void AnalysisParameters::set_start(int new_start) {
    start_ = new_start;
    if (!is_valid_domain()) {
        throw std::invalid_argument("Invalid Parameters: Start must be less than End");
    }
    update_step();
}

// Setter for end_
void AnalysisParameters::set_end(int new_end) {
    end_ = new_end;
    if (!is_valid_domain()) {
        throw std::invalid_argument("Invalid Parameters: Start must be less than End");
    }
    update_step();
}

// Setter for num_samples_
void AnalysisParameters::set_num_samples(int new_samples) {
    num_samples_ = new_samples;
    update_step();
    if(!is_valid_samples()) {
        throw std::invalid_argument("Invalid Parameters: Step size too small. Increase the number of samples.");
    }
}

// Update step based on current parameters
void AnalysisParameters::update_step() {
    if (num_samples_ > 0) {
        step_ = static_cast<double>(end_ - start_) / num_samples_;
    } else {
        step_ = std::numeric_limits<double>::infinity(); // Avoid division by zero
    }
}

// Check if the current domain parameters are valid
bool AnalysisParameters::is_valid_domain() const {
    return start_ < end_ && end_ - start_ >= 10;
}

bool AnalysisParameters::is_valid_samples() const {
    return step_ >= min_step_ && num_samples_ > 0;
}
