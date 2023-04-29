#ifndef ANALYZER_H
#define ANALYZER_H

#include <cstdint>
#include <vector>

class Analyzer{
public:
    static uint8_t calcAmplitude(const std::vector<uint16_t>& points, uint8_t levelPos);
};

#endif // ANALYZER_H
