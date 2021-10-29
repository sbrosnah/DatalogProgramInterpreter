#include "Tuple.h"

void Tuple::SetValues(std::vector<std::string> values) {
    this->values = values;
}

std::string Tuple::GetValue(int index) {
    std::string value = values.at(index);
    return value;
}

std::vector<std::string> Tuple::GetValues(){
    std::vector<std::string> valuesCopy = values;
    return valuesCopy;
}

void Tuple::AddValue(std::string value) {
    this->values.push_back(value);
}