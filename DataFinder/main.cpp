#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <chrono>
#include <iomanip>

int main() {

    std::wcout.imbue(std::locale(""));

    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);

    std::tm tm_time;
    localtime_s(&tm_time, &time); // ���������� localtime_s ������ localtime // ok
    std::ostringstream oss;
    oss << std::put_time(&tm_time, "%Y-%m-%d--%H-%M-%S");
    std::string formatted_datetime = oss.str();



    std::filesystem::path folderSavedPath = "./Saved";  // saved folder

    // ���������, ���������� �� �����
    if (!std::filesystem::exists(folderSavedPath)) {
        // �������� ������� �����
        if (std::filesystem::create_directory(folderSavedPath)) {
            std::cout << "Folder Created ;)" << std::endl;
        }
        else {
            std::cerr << "Cant create a folder" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(2));
            return 1;
        }
    }

    // create file for saving entres
    std::filesystem::path fullFilePath = folderSavedPath / (formatted_datetime + ".txt");

    std::ofstream output_file(fullFilePath);

    // �������� ������ �� ����
    if (!output_file.is_open()) {
        std::cerr << "Cant open file to save entries" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
        return 1;
    }

    std::string Name;
    std::cout << "Enter folder name: ";
    std::cin >> Name;

    //std::filesystem::path folder_path = std::filesystem::path("./") / Name / "/";

    std::filesystem::path folder_path = "./" + Name;

    std::string String;
    std::cout << "Enter text for search: ";
    std::cin >> String;

    if (!std::filesystem::exists(folder_path) || !std::filesystem::is_directory(folder_path)) {
        std::cerr << "Entered folder doesnt exist." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    try {
        for (const auto& entry : std::filesystem::directory_iterator(std::filesystem::absolute(folder_path))) {

            std::ifstream input_file(entry.path());

            std::cout << "Checking file:" << entry.path() << std::endl;

            if (!input_file.is_open()) {
                std::cerr << "Error with opening, file will be skipped: " << entry.path() << std::endl;
                continue;
            }

            // rEAD FILE
            std::string line;
            while (std::getline(input_file, line)) {
                // check if contains
                if (line.find(String) != std::string::npos) {
                    output_file << line << std::endl;
                }
            }

            input_file.close();
        }

        // clossing saving file
        output_file.close();

        std::cout << "Search complited. Results saved in /Saved" << std::endl;
        system("pause");
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
        return 1;
    }

    return 0;
}
