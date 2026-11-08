#pragma once

#pragma once

#include <vector>

class Command;
class PostMachine;
/**
 * @brief Stores the tape and carriage position of a Post machine.
 *
 * A cell contains 0 for a blank or 1 for a mark.
 * Initially, the tape contains one blank cell and the carriage is at
 * position zero.
 *
 * Command and PostMachine access the private state through friendship.
 */
class Tape {
private:
    int carriage = 0; ///< Zero-based carriage
    std::vector<int> cells{ 0 }; ///< Tape cells, initially one blank cell.

    friend class Command;
    friend class PostMachine;
};