#include "PostMachine.h"

#include <cstddef>
#include<string>
#include<sstream>
#include<iostream>


void PostMachine::setTape(const std::vector<int>& cells, int carriage)
{
    tape.cells = cells;

    
    if (tape.cells.empty()) {
        tape.cells.push_back(0);
    }

    tape.carriage = carriage;
}


void PostMachine::setProgram(const std::vector<Command>& commands)
{
    program = commands;
    currentCommand = 1;
    stopped = true;
}


void PostMachine::start()
{
    if (program.empty()) {
        stopped = true;
        return;
    }

    currentCommand = 1;
    stopped = false;

    while (!stopped) {
        executeStep();
    }
}

void PostMachine::executeStep()
{
    if (stopped) {
        return;
    }

    if (currentCommand < 1 || currentCommand > program.size()) {
        stopped = true;
        return;
    }

    program[currentCommand - 1].execute(tape, currentCommand, stopped);
}


void PostMachine::stop()
{
    stopped = true;
}


bool PostMachine::isStopped() const
{
    return stopped;
}


int PostMachine::getCarriage() const
{
    return tape.carriage;
}

const std::vector<int>& PostMachine::getCells() const
{
    return tape.cells;
}
bool PostMachine::load(std::ifstream& file, PostMachine& pm) {
    if (!file.is_open()) {
        return false;
    }

    std::string tape_;
    std::vector<int> cells;
    getline(file, tape_);
    std::stringstream ss(tape_);
    if (tape_.empty() || tape_.find_first_not_of("\t\r") == std::string::npos) {
        cells = { 0 };
    }
    else {
        for (char num : tape_) {
            cells.push_back(num - '0');
       }
    }

    std::string car;
    getline(file, car);
    ss.str(car);
    ss.clear();
    int carriage = 0;
    if (car.empty() || car.find_first_not_of("\t\r") == std::string::npos) {
        carriage = 1;
    }

    if (!(ss >> carriage)) { return false; }

    pm.setTape(cells, carriage - 1);

    std::vector<Command> comands;
    std::string com;
    while (getline(file, com)) {
        if (com.empty() || com.find_first_not_of("\t\r") == std::string::npos) {
            continue;
        }
        ss.str(com);
        ss.clear();

        char symbol;
        char separator;

        int nextCommand = 0;
        int markedCommand = 0;

        if (!(ss >> symbol)) {
            return false;
        }

        if (symbol == '!') {
        
        }
        else if (symbol == '<' || symbol == '>' || symbol == 'V' || symbol == 'X' || symbol == '?') {

            if (!(ss >> separator >> nextCommand) || separator != '-' || nextCommand < 1) {
                return false;
            }

            if (symbol == '?') {
                if (!(ss >> separator >> markedCommand) || separator != '-' || markedCommand < 1) {
                    return false;
                }
            }
        }
        else {
            
            return false;
        }
        
        comands.push_back(Command(symbol, nextCommand, markedCommand));
    }
    pm.setProgram(comands);
    return true;
}

void PostMachine::addCommand(const Command& command)
{
    program.push_back(command);

    currentCommand = 1;
    stopped = true;
}

void PostMachine::printProgram() const
{
    if (program.empty()) {
        std::cout << "Программа не содержит правил.\n";
        return;
    }

    for (std::size_t i = 0; i < program.size(); ++i) {
        const Command& command = program[i];

        std::cout << i + 1 << ": " << command.symbol;

        if (command.symbol == '?') {
            std::cout << '-' << command.nextCommand
                << '-' << command.markedCommand;
        }
        else if (command.symbol != '!') {
            std::cout << '-' << command.nextCommand;
        }

        std::cout << '\n';
    }
}

bool PostMachine::deleteCommand(int number)
{
    if (number < 1 || number > program.size()) {
        return false;
    }

    int index = number - 1;

   
    for (int i = 0; i < program.size(); ++i) {
        if (i == index) {
            continue;
        }

        const Command& command = program[i];

        if (command.symbol == '!') {
            continue;
        }

        if (command.nextCommand == number) {
            return false;
        }

        if (command.symbol == '?' &&
            command.markedCommand == number) {
            return false;
        }
    }

    program.erase(program.begin() + index);

    for (Command& command : program) {
        if (command.symbol == '!') {
            continue;
        }

        if (command.nextCommand > number) {
            --command.nextCommand;
        }

        if (command.symbol == '?' &&
            command.markedCommand > number) {
            --command.markedCommand;
        }
    }

    currentCommand = 1;
    stopped = true;

    return true;
}
void PostMachine::prepareExecution()
{
    currentCommand = 1;
    stopped = program.empty();
}
