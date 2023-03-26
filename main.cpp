#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <filesystem>

struct Account {
    std::string name;
    std::string surname;
    int cash;
    std::string date;
};

bool isValidDate(Account&);
void writeData(Account&, std::ofstream&);
void loadData(Account&, std::ifstream&);


int main() {
    Account person;
    std::string input;

    std::ifstream inFile("../account.bin", std::ios::binary);
    std::ofstream outFile("../account.bin", std::ios::binary | std::ios::app);

    std::cout << "What operation you want to perform? List or Add?: ";
    std::cin >> input;
    if (input == "List" || input == "list") {
        loadData(person, inFile);
    } else if (input == "Add" || input == "add") {
        writeData(person, outFile);
    }

}


void writeData(Account &person, std::ofstream& outFile) {
    std::cout << "Enter prompt: Name Surname Cash MM.DD.YYYY\n";
    std::cin >> person.name >> person.surname >> person.cash >> person.date;

    if (isValidDate(person) && person.cash > 0) {
        int lengthName = person.name.length();
        int lengthSurname = person.surname.length();
        int lengthDate = person.date.length();

        outFile.write((char*)&lengthName, sizeof(lengthName));
        outFile.write((char*)person.name.c_str(), lengthName);
        outFile.write((char*)&lengthSurname, sizeof(lengthSurname));
        outFile.write((char*)person.surname.c_str(), lengthSurname);
        outFile.write((char*)&person.cash, sizeof(person.cash));
        outFile.write((char*)&lengthDate, sizeof(lengthDate));
        outFile.write((char*)person.date.c_str(), lengthDate);
    } else {
        std::cout << "Input isn't valid. Try again\n";
        std::cerr << "Error: Invalid prompt.\n";
    }
    outFile.close();
}

void loadData(Account &person, std::ifstream& inFile) {
    int lengthName;
    int lengthSurname;
    int lengthDate;

    if (std::filesystem::file_size("../account.bin") == 0) {
        std::cout << "File is empty. First of all add some data.\n";
        return;
    }

    while (true) {
        inFile.read((char *) &lengthName, sizeof(lengthName));
        person.name.resize(lengthName);
        inFile.read((char *) person.name.c_str(), lengthName);

        inFile.read((char *) &lengthSurname, sizeof(lengthSurname));
        person.surname.resize(lengthSurname);
        inFile.read((char *) person.surname.c_str(), lengthSurname);

        inFile.read((char *) &person.cash, sizeof(person.cash));

        inFile.read((char *) &lengthDate, sizeof(lengthDate));
        person.date.resize(lengthDate);
        inFile.read((char *) person.date.c_str(), lengthDate);

        if (inFile.eof()) break;
        std::cout << person.name << " " << person.surname << " " << person.cash << " " << person.date << "\n";
    }

    inFile.close();
}

bool isValidDate(Account &person) {
    // Length check
    if (person.date.length() != 10) {
        return false;
    }
    // dots check
    if (person.date.substr(2, 1) != "." || person.date.substr(5, 1) != ".") {
        return false;
    }
    // day check
    int day = stoi(person.date.substr(0, 2));
    if (day < 1 || day > 31) {
        return false;
    }
    // month check
    int month = stoi(person.date.substr(3, 2));
    if (month < 1 || month > 12) {
        return false;
    }
    // year check
    int year = stoi(person.date.substr(6, 4));
    if (year < 1900 || year > 9999) {
        return false;
    }

    return true;
}