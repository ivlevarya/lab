#pragma once

#include "Tape.h"
#include "Command.h"

#include <vector>
#include<fstream>

class PostMachine {
private:
    Tape tape;
    std::vector<Command> program;

    int currentCommand = 1; // Нумерация команд начинается с 1
    bool stopped = true;

public:
    bool load(std::ifstream& file, PostMachine& pm);
    // Задать исходное содержимое ленты и положение каретки
    void setTape(const std::vector<int>& cells, int carriage = 0);

    // Задать программу машины
    void setProgram(const std::vector<Command>& commands);

    // Запустить программу или выполнить один шаг
    void start();
    void executeStep();
    void stop();

    // Получить состояние машины и ленты
    bool isStopped() const;
    int getCarriage() const;
    const std::vector<int>& getCells() const;
    void addCommand(const Command& command);
    bool deleteCommand(int number);
    void printProgram() const;
    void prepareExecution();
};