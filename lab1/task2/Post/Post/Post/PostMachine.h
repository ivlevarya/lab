#pragma once

#include "Tape.h"
#include "Command.h"

#include <vector>
#include<fstream>

/**
 * @brief Manages a Post machine program, tape, and execution state.
 *
 * Instructions use one-based numbering.
 * The tape API uses zero-based carriage positions.
 *
 * Execution can run continuously or one instruction at a time.
 */

class PostMachine {
private:
    Tape tape; ///< Current tape and carriage.
    std::vector<Command> program; ///< Loaded instructions.

    int currentCommand = 1; ///< One-based current instruction number.
    bool stopped = true; ///< Whether execution is stopped.

public:
    /**
     * @brief Loads tape data and instructions from a file.
     *
     * @param[in,out] file Open input file stream.
     * @param[out] pm Machine receiving the loaded data.
     * @return true on success; false if opening or parsing checks fail.
     *
     * Expected file layout:
     * - First line: initial tape, for example "00100".
     * - Second line: one-based carriage position.
     * - Remaining lines: instructions, for example "V-2" or "!".
     *
     * @warning A failed load may already have modified the destination
     * tape. Loading is not transactional.
     */
    bool load(std::ifstream& file, PostMachine& pm);
    /**
     * @brief Replaces the tape and carriage position.
     *
     * @param cells New tape contents.
     * @param carriage Zero-based carriage position.
     *
     * An empty vector is replaced with one blank cell.
     * Cell values and carriage bounds are not validated here.
     * The execution state is preserved.
     */
    void setTape(const std::vector<int>& cells, int carriage = 0);

    /**
      * @brief Replaces the program and stops execution.
      *
      * @param commands New instruction sequence.
      *
      * Selects instruction 1 without changing the tape or carriage.
      */
    void setProgram(const std::vector<Command>& commands);

    /**
     * @brief Runs the program from instruction 1 until it stops.
     *
     * Preserves the current tape and carriage before starting.
     * An empty program remains stopped.
     *
     * @warning A nonterminating program may run indefinitely.
     */
    void start();
    /**
     * @brief Executes one instruction if the machine is running.
     *
     * Does nothing if the machine is stopped.
     * An invalid instruction number stops execution.
     */
    void executeStep();
    /**
     * @brief Stops execution without resetting machine data.
     */
    void stop();

    /**
     * @brief Checks the execution state.
     * @return true if the machine is stopped; otherwise false.
     */
    bool isStopped() const;
    /**
    * @brief Returns the carriage position.
    * @return Zero-based index of the current tape cell.
    */
    int getCarriage() const;
    /**
     * @brief Provides read-only access to the tape.
     * @return Reference to the current tape cells.
     */
    const std::vector<int>& getCells() const;
    /**
     * @brief Appends an instruction and stops execution.
     *
     * @param command Instruction to append.
     *
     * Selects instruction 1 without changing the tape or carriage.
     */
    void addCommand(const Command& command);
    /**
     * @brief Deletes an instruction if other instructions do not reference it.
     *
     * @param number One-based instruction number.
     * @return true if deleted; false if the number is invalid or referenced.
     *
     * On success, destination numbers greater than the deleted number
     * are decreased by one. Execution stops and instruction 1 is selected.
     */
    bool deleteCommand(int number);
    /**
     * @brief Prints the numbered program to standard output.
     *
     * Prints a message if the program is empty.
     */
    void printProgram() const;
    /**
     * @brief Prepares execution from instruction 1.
     *
     * Enables execution if the program is nonempty.
     * Preserves the tape and carriage position.
     */
    void prepareExecution();
};