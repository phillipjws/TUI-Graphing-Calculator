#ifndef DOMAIN_HPP
#define DOMAIN_HPP

#include <string>
#include <stdexcept>

struct Domain {
    int start;
    int end;

Domain(int s = -100, int e = 100) : start(s), end(e) {
    if(!is_valid()) {
        throw std::invalid_argument("Invalid Domain: Start must be greater than end");
    }
}    

bool is_valid() {
    return start < end;
}

void set_start(int s) {
    start = s;
}

void set_end(int e) {
    end = e;
}

std::string display() const {
    return "Domain: [" + std::to_string(start) + ", " + std::to_string(end) + "]";
}

};

#endif // DOMAIN_HPP