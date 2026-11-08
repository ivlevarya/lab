#pragma once

#include "Tape.h"
#include <map>

class PostMachine;

/**
 * @brief Represents a single Post machine instruction.
 *
 * Supported symbols:
 * - '<': move the carriage left.
 * - '>': move the carriage right.
 * - 'V': mark the current cell.
 * - 'X': erase the current mark.
 * - '?': choose a destination based on the current cell.
 * - '!': stop execution.
 *
 * Instruction destinations use one-based numbering.
 */

class Command {
private:
    char symbol; ///< Instruction symbol.
    int nextCommand; ///< Ordinary destination or blank-cell branch.
    int markedCommand; ///< Marked-cell branch of a conditional instruction.

   
    using Handler = void (Command::*)(Tape&, int&, bool&) const;

    std::map<char, Handler> actions;

    /**
     * @brief Moves left, extending the tape if necessary.
     */
    void moveLeft(Tape& tape, int& current, bool& stopped) const;
    /**
     * @brief Moves right, extending the tape if necessary.
     */
    void moveRight(Tape& tape, int& current, bool& stopped) const;
    /**
     * @brief Marks a blank cell.
     *
     * Stops execution if the cell is not blank.
     */
    void setMark(Tape& tape, int& current, bool& stopped) const;
    /**
     * @brief Erases a marked cell.
     *
     * Stops execution if the cell does not contain a mark.
     */
    void eraseMark(Tape& tape, int& current, bool& stopped) const;
    /**
     * @brief Selects a destination according to the current cell.
     *
     * A blank cell selects nextCommand; a marked cell selects
     * markedCommand. Any other cell value stops execution.
     */
    void check(Tape& tape, int& current, bool& stopped) const;
    /**
    * @brief Stops execution.
    */
    void halt(Tape& tape, int& current, bool& stopped) const;

public:
    /**
     * @brief Constructs an instruction.
     *
     * @param symbol Instruction symbol.
     * @param nextCommand Destination for an ordinary instruction,
     * or the blank-cell branch of '?'.
     * @param markedCommand Destination for the marked-cell branch of '?'.
     *
     * Destination arguments are ignored by the halt instruction.
     */
    Command(char symbol, int nextCommand = 0, int markedCommand = 0);

    /**
     * @brief Executes the instruction.
     *
     * @param[in,out] tape Tape to operate on.
     * @param[in,out] currentCommand Current instruction number.
     * @param[in,out] stopped Execution stop flag.
     *
     * Does nothing if execution is already stopped.
     * An unsupported symbol, invalid carriage position, or instruction
     * error stops execution.
     */
    void execute(Tape& tape, int& currentCommand, bool& stopped) const;
    friend class PostMachine;
};
