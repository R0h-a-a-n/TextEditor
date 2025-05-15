# 📝 C++ Command-Line Text Editor

A lightweight, console-based text editor written in modern C++. This project demonstrates file I/O, regex-based text processing, undo-redo logic using stacks, and basic text buffer operations—simulating the behavior of a simple text editor.

---

## ✨ Features

- 📂 Create, open, save, and close `.txt` files
- ✏️ Edit, insert, delete individual lines
- 📋 Cut, copy, and paste operations with clipboard buffer
- 🔄 Undo and redo changes with full stack-based state history
- 🔍 Regex-powered search and replace
- 📊 Word, line, and character count statistics
- 🔒 Automatic file extension handling and validation
- 🧠 In-memory buffer manipulation without file dependency until save

---

## 📸 Sample Usage

```bash
$ ./TextEditor
=== Menu ===
1. New
2. Open
3. Save
...
12. Replace
13. Undo
14. Redo
15. Stats
16. Exit
Choice: 1
Enter the filename: example
New file created.
