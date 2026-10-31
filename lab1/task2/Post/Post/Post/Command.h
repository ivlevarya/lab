#pragma once

#include "Tape.h"
#include <map>

class Command {
private:
    char symbol;
    int nextCommand;
    int markedCommand;

    // Тип указателя на обычный метод класса Command
    using Handler = void (Command::*)(Tape&, int&, bool&) const;

    std::map<char, Handler> actions;

    void moveLeft(Tape& tape, int& current, bool& stopped) const;
    void moveRight(Tape& tape, int& current, bool& stopped) const;
    void setMark(Tape& tape, int& current, bool& stopped) const;
    void eraseMark(Tape& tape, int& current, bool& stopped) const;
    void check(Tape& tape, int& current, bool& stopped) const;
    void halt(Tape& tape, int& current, bool& stopped) const;

public:
    Command(char symbol,
        int nextCommand = 0,
        int markedCommand = 0);

    void execute(Tape& tape,
        int& currentCommand,
        bool& stopped) const;
    friend class PostMachine;
};
