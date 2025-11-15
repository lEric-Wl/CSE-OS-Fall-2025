#include <iostream>
#include <string>
#include <limits>
#include "MemoryManager.h"

MemoryManager mm;

bool running = true;

std::string listOptions() {
    return "1. Allocate a new page\n2. Delete a page at an address\n3. Write to an address\n4. Read from an address\n5. Print information about the page at an address\n6. [ADVANCED] Reinitialize MemoryManager\n7. Exit\n";
}

int hexStringToInt(std::string string) {
    // validate that string is valid hex
    if (string.compare(0, 2, "0x") == 0 || string.compare(0, 2, "0X") == 0) {
        string.erase(0, 2);
    }
    for (auto character : string){
        if (!isxdigit(character)) return -1;
    }

    int integer;
    try {
        integer = std::stoi(string, nullptr, 16);
    } catch (const std::exception& e) {
        std::cerr << "Caught an exception: " << e.what() << " (probably too big of a number)" << std::endl;
        return -1;
    }

    return integer;
}

void allocateAPage() {
    int newPage;
    try {
        newPage = mm.allocateAnyPage();
    }
    catch (const std::exception& e) {
        std::cerr << "Caught an exception: " << e.what() << std::endl;
        return;
    }

    std::string zeroBase = ""; if (newPage == 0) zeroBase = "0x";
    std::cout << "Your new page is located at virtual memory address: " << zeroBase << std::showbase << std::hex << newPage << std::endl;
    std::cout << "It can be written to from addresses: [" << zeroBase << newPage << ":" << newPage + 4095 << "]" << std::endl;
}

void deleteAPage() {
    std::string input;
    int address;

    std::cout << "Enter address of page you would like deleted (enter -1 to return to menu): ";
    std::cin >> input; std::cout << std::endl;

    address = hexStringToInt(input);
    if (address < 0) {std::cout << "Please enter a valid address!" << std::endl; return;}

    try {
        mm.deletePageTableEntry(address);
    } catch (const std::exception& e) {
        std::cerr << "Caught an exception: " << e.what() << std::endl;
        return;
    }

    std::cout << "Entry successfully deleted!" << std::endl;
}

void writeToAnAddress() {
    std::string input;
    int address;
    int data;

    std::cout << "Enter address you would like to write to (enter -1 to return to menu): ";
    std::cin >> input; std::cout << std::endl;

    address = hexStringToInt(input);
    std::string zeroBaseAddr = ""; if (address == 0) zeroBaseAddr = "0x";
    if (address < 0) {std::cout << "Please enter a valid address!" << std::endl; return;}

    std::cout << "Enter data you would like to write. \nData must be DEC 0-255 or HEX 0x0-0xff (please include the '0x' base!) (enter -1 to return to menu): ";
    std::cin >> input; std::cout << std::endl;
    std::string zeroBaseData = "";
    if (input.compare(0, 2, "0x") == 0 || input.compare(0, 2, "0X") == 0) {
        data = hexStringToInt(input);
        std::cout << std::hex << std::showbase;
        if (data == 0) zeroBaseData = "0x";
    } else {
        for (auto character : input){
            if (!isdigit(character)) data = -1;
        }
        data = std::stoi(input, nullptr, 10);
        if (data > 255) {std::cout << "Please enter valid data!" << std::endl; return;}
    }

    if (data < 0) {std::cout << "Please enter valid data!" << std::endl; return;}

    try {
        mm.writeVirtualMemory(address, data);
    } catch (const std::exception& e) {
        std::cerr << "Caught an exception: " << e.what() << std::endl;
        return;
    }

    std::cout << "Data [" << zeroBaseData << data << "] successfully written to address " << std::hex << std::showbase << zeroBaseAddr << address << "!" << std::endl;
}

void readFromAnAddress() {
    std::string input;
    int address;
    int data;

    std::cout << "Enter address you would like to write to (enter -1 to return to menu): ";
    std::cin >> input; std::cout << std::endl;

    address = hexStringToInt(input);
    if (address < 0) {std::cout << "Please enter a valid address!" << std::endl; return;}
    std::string zeroBaseAddr = ""; if (address == 0) zeroBaseAddr = "0x";


    try {
        data = mm.readVirtualMemory(address);
    } catch (const std::exception& e) {
        std::cerr << "Caught an exception: " << e.what() << std::endl;
        return;
    }

    std::string zeroBaseData = ""; if (data == 0) zeroBaseData = "0x";

    std::cout << "Data successfully read!" << std::endl;
    std::cout << "Value at address " << std::hex << std::showbase << zeroBaseAddr << address << ": " << zeroBaseData << data <<std::endl;
}

void printPageInfo() {
    std::string input;
    int address;

    std::cout << "Enter address you would like to print page info of (enter -1 to return to menu): ";
    std::cin >> input; std::cout << std::endl;

    address = hexStringToInt(input);
    if (address < 0) {std::cout << "Please enter a valid address!" << std::endl; return;}
    std::string zeroBaseAddr = ""; if (address == 0) zeroBaseAddr = "0x";

    std::cout << "Page data at address " << zeroBaseAddr << address << ": " << std::endl;

    try {
        mm.printPageTableEntry(address);
    } catch (const std::exception& e) {
        std::cerr << "Caught an exception: " << e.what() << std::endl;
        return;
    }

    std::cout << std::endl;
}

void reinitializeMemory() {
    std::string input;
    int page_size = 4096;
    int num_pages = 1024;
    int num_frames = 1024;


    std::cout << "WARNING! This will reset all data entered. Enter 1 to continue, -1 to return: ";
    std::cin >> input; std::cout << std::endl;
    int confirm;

    try {
        confirm = std::stoi(input, nullptr, 10);
    } catch (...) {
        confirm = -1;
    }

    if(confirm != 1) return;

    std::cout << "Enter size of each page in bytes (multiple of 2) (enter -1 to return to menu): ";
    std::cin >> input; std::cout << std::endl;
    try {page_size = std::stoi(input, nullptr, 10);} catch (...) {page_size = -1;} if(page_size < 0) return;

    std::cout << "Enter number of pages (enter -1 to return to menu): ";
    std::cin >> input; std::cout << std::endl;
    try {num_pages = std::stoi(input, nullptr, 10);} catch (...) {num_pages = -1;} if(num_pages < 0) return;

    std::cout << "Enter number of physical memory frames (enter -1 to return to menu): ";
    std::cin >> input; std::cout << std::endl;
    try {num_frames = std::stoi(input, nullptr, 10);} catch (...) {num_frames = -1;} if(num_frames < 0) return;

    mm = MemoryManager(page_size, num_pages, num_frames);

    std::cout << "MemoryManager reinitialized with:\n" << num_pages << " " << page_size << "B pages\nNumber of physical memory frames: " << num_frames << std::endl;
}

void exitProgram() {
     running = false;

     std::cout << "Goodbye!" << std::endl;
}

void handleOptions(int choice) {
    switch (choice) {
        case 1:
            allocateAPage();
            break;
        case 2:
            deleteAPage();
            break;
        case 3:
            writeToAnAddress();
            break;
        case 4:
            readFromAnAddress();
            break;
        case 5:
            printPageInfo();
            break;
        case 6:
            reinitializeMemory();
            break;
        case 7:
            exitProgram();
            break;
    }
    if (choice != 7) {
        std::cout << "Press enter to continue...";
        while (std::cin.get() != '\n');
        while (std::cin.get() != '\n');
        std::cout << std::endl;
    }
}

int main() {
    std::cout << "\033[2J\033[1;1H" << std::flush;
    while (running) {
        std::cout << " --=--= Virtual Memory Simulation =--=--" << std::endl;
        std::cout << listOptions() << std::endl;

        std::string selection;
        int choice = -1;
        std::cout << "Please make a selection: ";

        std::cin >> selection;
        std::cout << std::endl;

        if (selection.length() == 1 && isdigit(selection[0])) {
            choice = selection[0] - '0';
            if (choice >= 1 && choice <= 7) {
                handleOptions(choice);
                continue;
            }
        }

        std::cout << "!!! Please pick from the choices presented! !!!" << std::endl << std::endl;
    }
    return 0;
}
