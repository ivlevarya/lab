#include "Command.h"

Command::Command(char symbol, int nextCommand, int markedCommand) : symbol(symbol), nextCommand(nextCommand), markedCommand(markedCommand)
{
    actions['<'] = &Command::moveLeft;
    actions['>'] = &Command::moveRight;
    actions['V'] = &Command::setMark;
    actions['X'] = &Command::eraseMark;
    actions['?'] = &Command::check;
    actions['!'] = &Command::halt;
}

void Command::execute(Tape& tape, int& currentCommand, bool& stopped) const
{
    if (stopped) {
        return;
    }

    auto it = actions.find(symbol);

    if (it == actions.end()) {
        stopped = true;
        return;
    }

    
    if (tape.carriage < 0 || tape.carriage >= tape.cells.size()) {
        stopped = true;
        return;
    }

    Handler handler = it->second;
    (this->*handler)(tape, currentCommand, stopped);
}


void Command::moveLeft(Tape& tape, int& currentCommand, bool&) const {
    if (tape.carriage == 0) {
        tape.cells.insert(tape.cells.begin(), 0);
    } else {
        --tape.carriage;
    }

    currentCommand = nextCommand;
}

void Command::moveRight(Tape& tape, int& currentCommand, bool&) const {
    if (tape.carriage == tape.cells.size() - 1) {
        tape.cells.push_back(0);
    }

    ++tape.carriage;
    currentCommand = nextCommand;
}

void Command::setMark(Tape& tape, int& currentCommand, bool& stopped) const {
    if (tape.cells[tape.carriage] != 0) {
        stopped = true;
        return;
    }

    tape.cells[tape.carriage] = 1;
    currentCommand = nextCommand;
}

void Command::eraseMark(Tape& tape, int& currentCommand, bool& stopped) const {
    if (tape.cells[tape.carriage] != 1) {
        stopped = true;
        return;
    }

    tape.cells[tape.carriage] = 0;
    currentCommand = nextCommand;
}

void Command::check(Tape& tape, int& currentCommand, bool& stopped) const {
    if (tape.cells[tape.carriage] == 0) {
        currentCommand = nextCommand;
    } else if (tape.cells[tape.carriage] == 1) {
        currentCommand = markedCommand;
    } else {
        stopped = true;
    }
}

// Остановить выполнение
void Command::halt(Tape&, int&, bool& stopped) const
{
    stopped = true;
}