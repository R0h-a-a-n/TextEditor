#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>

class TextEditor {
private:
    std::vector<std::string> lines;
    std::string filename;
    bool modified;
    std::stack<std::vector<std::string>> undoStack;  // Stack to store previous states for undo

    void ensureTxtExtension(std::string &name) {
        if (name.find('.') == std::string::npos) {
            name += ".txt";
        }
    }

    bool fileExists(const std::string &name) {
        std::ifstream f(name.c_str());
        return f.good();
    }

    void saveCurrentState() {
        undoStack.push(lines);  // Save current state to the stack
    }

public:
    TextEditor() : modified(false) {}

    void createFile() {
        std::cout << "Enter the filename: ";
        std::cin >> filename;
        ensureTxtExtension(filename);
        std::ofstream file(filename);
        if (file.is_open()) {
            std::cout << "New file created successfully.\n";
            modified = false;
            lines.clear();
            while (!undoStack.empty()) undoStack.pop();  // Clear undo stack
        } else {
            std::cout << "Error creating file.\n";
        }
    }

    void openFile() {
        std::cout << "Enter the filename to open: ";
        std::cin >> filename;
        ensureTxtExtension(filename);
        if (!fileExists(filename)) {
            std::cout << "Error: File does not exist.\n";
            return;
        }
        std::ifstream file(filename);
        if (file.is_open()) {
            lines.clear();
            std::string line;
            while (getline(file, line)) {
                lines.push_back(line);
            }
            file.close();
            std::cout << "File opened successfully.\n";
            displayContent();  // Automatically display content after opening the file
            modified = false;
            while (!undoStack.empty()) undoStack.pop();  // Clear undo stack
        } else {
            std::cout << "Error opening file.\n";
        }
    }

    void saveFile() {
        if (filename.empty()) {
            std::cout << "No file is currently open. Please create or open a file first.\n";
            return;
        }
        std::ofstream file(filename);
        if (file.is_open()) {
            for (const auto &line : lines) {
                file << line << "\n";
            }
            file.close();
            std::cout << "File saved successfully.\n";
            modified = false;
        } else {
            std::cout << "Error saving file.\n";
        }
    }

    void closeFile() {
        if (modified) {
            std::cout << "You have unsaved changes. Do you want to save the file? (y/n): ";
            char choice;
            std::cin >> choice;
            if (choice == 'y' || choice == 'Y') {
                saveFile();
            }
        }
        filename.clear();
        lines.clear();
        modified = false;
        while (!undoStack.empty()) undoStack.pop();  // Clear undo stack
        std::cout << "File closed successfully.\n";
    }

    void displayContent() const {
        if (lines.empty()) {
            std::cout << "File is empty.\n";
        } else {
            for (const auto &line : lines) {
                std::cout << line << "\n";
            }
        }
    }

    void insertText() {
        saveCurrentState();  // Save the current state before making changes
        std::cout << "Enter the line number to insert text at: ";
        int lineNumber;
        std::cin >> lineNumber;
        std::cin.ignore();  // Ignore the newline character left by std::cin
        if (lineNumber < 1 || lineNumber > lines.size() + 1) {
            std::cout << "Invalid line number.\n";
            return;
        }
        std::cout << "Enter the text to insert: ";
        std::string text;
        getline(std::cin, text);
        lines.insert(lines.begin() + lineNumber - 1, text);
        modified = true;
    }

    void searchAndReplace() {
        saveCurrentState();  // Save the current state before making changes
        std::cout << "Enter the word to search: ";
        std::string searchWord, replaceWord;
        std::cin >> searchWord;
        std::cout << "Enter the word to replace with: ";
        std::cin >> replaceWord;
        for (auto &line : lines) {
            size_t pos;
            while ((pos = line.find(searchWord)) != std::string::npos) {
                line.replace(pos, searchWord.length(), replaceWord);
            }
        }
        modified = true;
        std::cout << "Search and replace completed.\n";
    }

    void undoLastChange() {
        if (!undoStack.empty()) {
            lines = undoStack.top();  // Revert to the last saved state
            undoStack.pop();
            modified = true;
            std::cout << "Last change undone.\n";
        } else {
            std::cout << "No changes to undo.\n";
        }
    }

    void run() {
        int choice;
        do {
            std::cout << "\nText Editor Menu\n";
            std::cout << "1. Create New File\n";
            std::cout << "2. Open File\n";
            std::cout << "3. Save File\n";
            std::cout << "4. Close File\n";
            std::cout << "5. Display File Content\n";
            std::cout << "6. Insert Text\n";
            std::cout << "7. Search and Replace\n";
            std::cout << "8. Undo Last Change\n";
            std::cout << "9. Exit\n";
            std::cout << "Enter your choice: ";
            std::cin >> choice;

            switch (choice) {
                case 1:
                    createFile();
                    break;
                case 2:
                    openFile();
                    break;
                case 3:
                    saveFile();
                    break;
                case 4:
                    closeFile();
                    break;
                case 5:
                    displayContent();
                    break;
                case 6:
                    insertText();
                    break;
                case 7:
                    searchAndReplace();
                    break;
                case 8:
                    undoLastChange();
                    break;
                case 9:
                    if (modified) {
                        std::cout << "You have unsaved changes. Do you want to save before exiting? (y/n): ";
                        char exitChoice;
                        std::cin >> exitChoice;
                        if (exitChoice == 'y' || exitChoice == 'Y') {
                            saveFile();
                        }
                    }
                    std::cout << "Exiting the editor.\n";
                    break;
                default:
                    std::cout << "Invalid choice. Please try again.\n";
            }
        } while (choice != 9);
    }
};

int main() {
    TextEditor editor;
    editor.run();
    return 0;
}
