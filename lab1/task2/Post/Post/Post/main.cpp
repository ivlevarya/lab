#include "PostMachine.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

int main(int argc, char* argv[])
{
    std::string fileName;
    bool log = false;

    // Читаем путь к файлу и необязательный флаг -log
    for (int i = 1; i < argc; ++i) {
        std::string argument = argv[i];

        if (argument == "-log") {
            log = true;
        }
        else if (fileName.empty()) {
            fileName = argument;
        }
        else {
            std::cout << "Extra argument: " << argument << '\n';
            return 1;
        }
    }

    if (fileName.empty()) {
        std::cout << "Launch: Post.exe file_name.txt [-log]\n";
        return 1;
    }

    std::ifstream file(fileName);

    if (!file.is_open()) {
        std::cerr << "Cannot open the file: " << fileName << '\n';
        return 1;
    }

    PostMachine machine;

    if (!machine.load(file, machine)) {
        std::cerr << "File upload failed.\n";
        return 1;
    }

    file.close();

    bool executionPrepared = false;
    bool exit = false;

    while (!exit) {
        // Показываем состояние перед выбором действия
        const std::vector<int>& cells = machine.getCells();
        int carriage = machine.getCarriage();

        std::cout << "\nTape: ";

        for (std::size_t i = 0; i < cells.size(); ++i) {
            if (static_cast<int>(i) == carriage) {
                std::cout << '[' << cells[i] << ']';
            }
            else {
                std::cout << cells[i];
            }
        }

        std::cout << "\nCarriage posotion: " << carriage + 1
            << "\nLogging: " << (log ? "on" : "off")
            << "\n\n";

        std::cout
            << "1. Review the program\n"
            << "2. Add comand at the end\n"
            << "3. Delete a comand\n"
            << "4. Set or change tape and carriage\n"
            << "5. Implement one step\n"
            << "6. Implement remaining program\n"
            << "7. Start implementation from first comand\n"
            << "8. Upload another file\n"
            << "9. Turn on or turn off logging\n"
            << "0. Enter\n"
            << "Choose the option: ";

        std::string line;

        if (!std::getline(std::cin, line)) {
            break;
        }

        std::stringstream menuInput(line);
        int choice;

        if (!(menuInput >> choice)) {
            std::cout << "Enter the number of the option.\n";
            continue;
        }
        switch (choice) {
        case 1: {
            machine.printProgram();
            break;
        }

        case 2: {
            std::cout
                << "Enter the comand.\n"
                << "Examles: V-4, >-2, ?-3-5, !\n"
                << "Comand: ";

            if (!std::getline(std::cin, line)) {
                exit = true;
                break;
            }

            std::stringstream commandInput(line);

            char symbol;
            char separator;
            int nextCommand = 0;
            int markedCommand = 0;
            bool valid = true;

            if (!(commandInput >> symbol)) {
                valid = false;
            }
            else if (symbol == '!') {

            }
            else if (symbol == '<' || symbol == '>' || symbol == 'V' || symbol == 'X' || symbol == '?') {

                if (!(commandInput >> separator >> nextCommand) ||
                    separator != '-' || nextCommand < 1) {
                    valid = false;
                }

                if (valid && symbol == '?') {
                    if (!(commandInput >> separator >> markedCommand) ||
                        separator != '-' || markedCommand < 1) {
                        valid = false;
                    }
                }
            }
            else {
                valid = false;
            }

            if (!valid) {
                std::cout << "Wrong form of comand.\n";
                break;
            }

            machine.addCommand(
                Command(symbol, nextCommand, markedCommand)
            );

            executionPrepared = false;
            std::cout << "Comand was added at the and.\n";
            break;
        }

        case 3: {
            std::cout << "Enter the number of the comand: ";

            if (!std::getline(std::cin, line)) {
                exit = true;
                break;
            }

            std::stringstream numberInput(line);
            int number;

            if (!(numberInput >> number)) {
                std::cout << "You should enter integer number.\n";
                break;
            }

            if (machine.deleteCommand(number)) {
                executionPrepared = false;
                std::cout << "Comand was deleted.\n";
            }
            else {
                std::cout
                    << "Cannot delete this comand: there is no such comand "
                    << "or referenced by other comand.\n";
            }

            break;
        }

        case 4: {
            std::cout << "Enter the tape of 0 and 1, for example 01110: ";

            std::string tapeLine;

            if (!std::getline(std::cin, tapeLine)) {
                exit = true;
                break;
            }

            std::vector<int> newCells;
            bool valid = !tapeLine.empty();

            for (char value : tapeLine) {
                if (value != '0' && value != '1') {
                    valid = false;
                    break;
                }

                newCells.push_back(value - '0');
            }

            if (!valid) {
                std::cout << "Tape should contains only 0 and 1.\n";
                break;
            }

            std::cout << "Enter carriage position, strats with 1: ";

            if (!std::getline(std::cin, line)) {
                exit = true;
                break;
            }

            std::stringstream positionInput(line);
            int position;

            if (!(positionInput >> position)) {
                std::cout << "Wrong carriage position.\n";
                break;
            }

            positionInput >> std::ws;

            if (!positionInput.eof() ||
                position < 1 ||
                static_cast<std::size_t>(position) > newCells.size()) {
                std::cout << "The postion goes beyond tape boundaries.\n";
                break;
            }

            machine.setTape(newCells, position - 1);
            machine.stop();
            executionPrepared = false;

            std::cout << "Tape was changed.\n";
            break;
        }

        case 5:
        case 6: {
            // Подготовка нужна перед первым шагом после загрузки
            // или редактирования программы.
            if (!executionPrepared) {
                machine.prepareExecution();
                executionPrepared = true;
            }

            if (machine.isStopped()) {
                std::cout
                    << "Machine was stopped. To restart"
                    << "choose option 7.\n";
                break;
            }

            do {
                machine.executeStep();

                // При -log выводим состояние после каждого правила
                if (log) {
                    const std::vector<int>& state = machine.getCells();
                    int currentPosition = machine.getCarriage();

                    std::cout << "\nAfter comand execution:\nTape: ";

                    for (std::size_t i = 0; i < state.size(); ++i) {
                        if (static_cast<int>(i) == currentPosition) {
                            std::cout << '[' << state[i] << ']';
                        }
                        else {
                            std::cout << state[i];
                        }
                    }

                    std::cout
                        << "\nCarriage postion: " << currentPosition + 1
                        << "\nMachine was stopped: "
                        << (machine.isStopped() ? "да" : "нет")
                        << '\n';
                }

                // Для действия 5 выполняется только один шаг.
                // Для действия 6 цикл продолжается до остановки.
            } while (choice == 6 && !machine.isStopped());

            if (machine.isStopped()) {
                std::cout << "Execution was stopped.\n";
            }

            break;
        }

        case 7: {
            machine.prepareExecution();
            executionPrepared = true;

            std::cout
                << "Execution is prepared from the first comand.\n"
                << "Тhe current content of the tape has been saved.\n";
            break;
        }

        case 8: {
            std::cout << "Enter file name: ";

            if (!std::getline(std::cin, line)) {
                exit = true;
                break;
            }

            std::ifstream newFile(line);

            if (!newFile.is_open()) {
                std::cout << "Cannot open the file.\n";
                break;
            }

            // Загружаем во временный объект, чтобы при ошибке
            // сохранить прежнюю машину.
            PostMachine newMachine;

            if (!newMachine.load(newFile, newMachine)) {
                std::cout << "Error loading file.\n";
                break;
            }

            machine = newMachine;
            executionPrepared = false;

            std::cout << "File loaded.\n";
            break;
        }

        case 9: {
            log = !log;

            std::cout << "Logging "
                << (log ? "on.\n" : "off.\n");
            break;
        }

        case 0: {
            exit = true;
            break;
        }

        default: {
            std::cout << "There is no such option.\n";
            break;
        }
        }
    }

    return 0;
}