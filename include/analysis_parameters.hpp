#ifndef ANALYSIS_PARAMETERS_HPP
#define ANALYSIS_PARAMETERS_HPP

#include <string>
#include <stdexcept>
#include <limits>

class AnalysisParameters {
public:
    AnalysisParameters(int start = -100, int end = 100, int num_samples = 10000);

    int get_start() const;
    int get_end() const;
    int get_num_samples() const;
    double get_step() const;

    std::string display_domain() const;
    std::string display_num_step() const;

    void set_start(int new_start);
    void set_end(int new_end);
    void set_num_samples(int new_samples);
    bool is_valid() const;
    void update_step();

private:
    int start_;
    int end_;
    int num_samples_;
    double step_;
};

#endif // ANALYSIS_PARAMETERS_HPP
