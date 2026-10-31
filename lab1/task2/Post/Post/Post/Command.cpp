#include "Command.h"

Command::Command(char symbol, int nextCommand, int markedCommand)
    : symbol(symbol),
      nextCommand(nextCommand),
      markedCommand(markedCommand)
{
    actions['<'] = &Command::moveLeft;
    actions['>'] = &Command::moveRight;
    actions['V'] = &Command::setMark;
    actions['X'] = &Command::eraseMark;
    actions['?'] = &Command::check;
    actions['!'] = &Command::halt;
}

// Выполнение одной команды
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

    // Проверка положения каретки
    if (tape.carriage < 0 ||
        static_cast<std::size_t>(tape.carriage) >= tape.cells.size()) {
        stopped = true;
        return;
    }

    Handler handler = it->second;
    (this->*handler)(tape, currentCommand, stopped);
}

// Сдвиг влево
void Command::moveLeft(Tape& tape,
                       int& currentCommand,
                       bool&) const
{
    if (tape.carriage == 0) {
        // Новая пустая клетка становится текущей
        tape.cells.insert(tape.cells.begin(), 0);
    } else {
        --tape.carriage;
    }

    currentCommand = nextCommand;
}

// Сдвиг вправо
void Command::moveRight(Tape& tape,
                        int& currentCommand,
                        bool&) const
{
    if (static_cast<std::size_t>(tape.carriage) ==
        tape.cells.size() - 1) {
        tape.cells.push_back(0);
    }

    ++tape.carriage;
    currentCommand = nextCommand;
}

// Поставить метку: заменить 0 на 1
void Command::setMark(Tape& tape,
                      int& currentCommand,
                      bool& stopped) const
{
    if (tape.cells[tape.carriage] != 0) {
        stopped = true;
        return;
    }

    tape.cells[tape.carriage] = 1;
    currentCommand = nextCommand;
}

// Стереть метку: заменить 1 на 0
void Command::eraseMark(Tape& tape,
                        int& currentCommand,
                        bool& stopped) const
{
    if (tape.cells[tape.carriage] != 1) {
        stopped = true;
        return;
    }

    tape.cells[tape.carriage] = 0;
    currentCommand = nextCommand;
}

// Проверить значение текущей клетки
void Command::check(Tape& tape,
                    int& currentCommand,
                    bool& stopped) const
{
    if (tape.cells[tape.carriage] == 0) {
        currentCommand = nextCommand;
    } else if (tape.cells[tape.carriage] == 1) {
        currentCommand = markedCommand;
    } else {
        // В ленте допустимы только 0 и 1
        stopped = true;
    }
}

// Остановить выполнение
void Command::halt(Tape&, int&, bool& stopped) const
{
    stopped = true;
}