#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<stack>
#include<regex>
#include<sstream>

class TextEditor {
private:
    std::vector<std::string> lines;
    std::string filename;
    bool modified;

    std::stack<std::vector<std::string>> undoStack;
    std::stack<std::vector<std::string>> redoStack;

    std::string clipboard;

    void ensureTxtExtension(std::string &name){
        if(name.find('.') == std::string::npos){
            name += ".txt";
        }
    }

    bool fileExists(const std::string &name){
        std::ifstream f(name.c_str());
        return f.good();
    }

    void saveCurrentState(){
        undoStack.push(lines);
        while(!redoStack.empty()) redoStack.pop();
    }

public:
    TextEditor() : modified(false) {}

    void createFile() {
        std::cout << "Enter the filename: ";
        std::cin >> filename;
        ensureTxtExtension(filename);
        std::ofstream file(filename);
        if(file.is_open()) {
            std::cout <<"New file created.\n";
            modified = false;
            lines.clear();
            while(!undoStack.empty()) undoStack.pop();
            while(!redoStack.empty()) redoStack.pop();
        } else std::cout<<"Error creating file.\n";
    }

    void openFile(){
        std::cout<<"Enter filename: ";
        std::cin>>filename;
        ensureTxtExtension(filename);
        if(!fileExists(filename)){
            std::cout << "Error: File doesn't exist.\n";
            return;
        }
        std::ifstream file(filename);
        if(file.is_open()){
            lines.clear();
            std::string line;
            while(getline(file, line)){
                lines.push_back(line);
            }
            file.close();
            std::cout <<"File loaded.\n";
            displayContent();
            modified = false;
            while(!undoStack.empty()) undoStack.pop();
            while(!redoStack.empty()) redoStack.pop();
        } else {
            std::cout << "Could not open.\n";
        }
    }

    void saveFile(){
        if(filename.empty()){
            std::cout<<"Open or create a file first.\n";
            return;
        }
        std::ofstream file(filename);
        if(file){
            for(const auto &line : lines){
                file << line << "\n";
            }
            file.close();
            std::cout << "Saved.\n";
            modified = false;
        } else std::cout << "Failed to save.\n";
    }

    void closeFile(){
        if(modified){
            std::cout << "Unsaved changes. Save? (y/n): ";
            char c; std::cin>>c;
            if(c=='y'||c=='Y') saveFile();
        }
        filename.clear();
        lines.clear();
        modified = false;
        while(!undoStack.empty()) undoStack.pop();
        while(!redoStack.empty()) redoStack.pop();
        std::cout<<"File closed.\n";
    }

    void displayContent() const{
        if(lines.empty()){
            std::cout<<"Empty file.\n";
        } else {
            for(size_t i=0; i<lines.size(); ++i)
                std::cout<< i+1 <<": " << lines[i] << "\n";
        }
    }

    void insertText(){
        saveCurrentState();
        std::cout<<"Line to insert at: ";
        int ln; std::cin>>ln; std::cin.ignore();
        if(ln < 1 || ln > lines.size()+1){
            std::cout<<"Invalid index.\n";
            return;
        }
        std::cout << "Enter text: ";
        std::string text;
        getline(std::cin, text);
        lines.insert(lines.begin() + ln - 1, text);
        modified = true;
    }

    void deleteLine(){
        saveCurrentState();
        std::cout<<"Line to delete: ";
        int l; std::cin>>l;
        if(l<1||l>lines.size()){
            std::cout<<"Invalid.\n";
            return;
        }
        lines.erase(lines.begin()+l-1);
        modified = true;
    }

    void editLine(){
        saveCurrentState();
        std::cout << "Line number: ";
        int l; std::cin>>l; std::cin.ignore();
        if(l<1 || l>lines.size()){
            std::cout << "Invalid.\n";
            return;
        }
        std::cout<<"New text: ";
        std::string t;
        getline(std::cin, t);
        lines[l-1]=t;
        modified = true;
    }

    void cutLine(){
        saveCurrentState();
        std::cout << "Line to cut: ";
        int l; std::cin>>l;
        if(l<1||l>lines.size()){
            std::cout<<"Invalid.\n";
            return;
        }
        clipboard = lines[l-1];
        lines.erase(lines.begin()+l-1);
        modified=true;
        std::cout << "Cut.\n";
    }

    void copyLine(){
        std::cout << "Line to copy: ";
        int l; std::cin>>l;
        if(l<1||l>lines.size()){
            std::cout<<"Invalid.\n";
            return;
        }
        clipboard = lines[l-1];
        std::cout<<"Copied.\n";
    }

    void pasteLine(){
        saveCurrentState();
        std::cout<<"Paste at line: ";
        int l; std::cin>>l;
        if(l<1||l>lines.size()+1){
            std::cout<<"Invalid.\n";
            return;
        }
        lines.insert(lines.begin()+l-1, clipboard);
        modified = true;
        std::cout<<"Pasted.\n";
    }

    void searchAndReplace(){
        saveCurrentState();
        std::cout<<"Pattern: ";
        std::string p, r;
        std::cin>>p;
        std::cout<<"Replace with: ";
        std::cin>>r;
        std::regex reg(p);
        for(auto &line: lines){
            line = std::regex_replace(line, reg, r);
        }
        modified=true;
        std::cout<<"Replaced.\n";
    }

    void undoLastChange(){
        if(!undoStack.empty()){
            redoStack.push(lines);
            lines = undoStack.top(); undoStack.pop();
            modified = true;
            std::cout<<"Undo done.\n";
        } else {
            std::cout<<"Nothing to undo.\n";
        }
    }

    void redoLastChange(){
        if(!redoStack.empty()){
            undoStack.push(lines);
            lines = redoStack.top(); redoStack.pop();
            modified = true;
            std::cout<<"Redo done.\n";
        } else std::cout<<"Nothing to redo.\n";
    }

    void showStatistics() const{
        int lc = lines.size(), wc=0, cc=0;
        for(const auto &l : lines){
            std::istringstream iss(l); std::string w;
            while(iss >> w) wc++;
            cc += l.length();
        }
        std::cout<<"Lines: "<<lc<<", Words: "<<wc<<", Characters: "<<cc<<"\n";
    }

    void run(){
        int c;
        do{
            std::cout<<"\n=== Menu ===\n";
            std::cout << "1. New\n2. Open\n3. Save\n4. Close\n5. Show\n";
            std::cout << "6. Insert\n7. Edit\n8. Delete\n9. Cut\n10. Copy\n";
            std::cout << "11. Paste\n12. Replace\n13. Undo\n14. Redo\n15. Stats\n16. Exit\n";
            std::cout << "Choice: ";
            std::cin >> c;

            switch(c){
                case 1: createFile(); break;
                case 2: openFile(); break;
                case 3: saveFile(); break;
                case 4: closeFile(); break;
                case 5: displayContent(); break;
                case 6: insertText(); break;
                case 7: editLine(); break;
                case 8: deleteLine(); break;
                case 9: cutLine(); break;
                case 10: copyLine(); break;
                case 11: pasteLine(); break;
                case 12: searchAndReplace(); break;
                case 13: undoLastChange(); break;
                case 14: redoLastChange(); break;
                case 15: showStatistics(); break;
                case 16:
                    if(modified){
                        std::cout<<"Unsaved changes. Save? (y/n): ";
                        char ch; std::cin>>ch;
                        if(ch=='y'||ch=='Y') saveFile();
                    }
                    std::cout<<"Bye.\n";
                    break;
                default:
                    std::cout<<"Invalid.\n";
            }

        } while(c!=16);
    }
};

int main(){
    TextEditor t;
    t.run();
    return 0;
}
