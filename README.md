# Study Arc

A console-based **University Management and Quiz System** built as a first-semester **Programming Fundamentals (PF)** project in C++.

## Project Overview
Study Arc demonstrates core PF concepts through a role-based system where **Admin**, **Teacher**, and **Student** users interact with user records and quizzes.

## Core Functionality

### 1) Authentication and User Management
- Sign up and sign in for users
- Role-based access using user types:
  - `a` = Admin
  - `t` = Teacher
  - `s` = Student
- Admin can add, view, update, and delete user records
- Input validation for safer and cleaner data entry

### 2) Quiz Management
- **Admin** can:
  - Create, edit, view, and delete quizzes
- **Teacher** can:
  - Create quizzes
  - Edit their own quizzes
  - View quizzes they created
- Quizzes are identified by a unique **quiz name**
- Each quiz supports multiple-choice questions with 4 options and one correct answer

### 3) Student Quiz Attempts
- Students can browse available quizzes
- Students can attempt a quiz by entering its exact name
- Automatic scoring is shown after submission
- Attempt history is saved per student

### 4) Persistent File Storage
The project stores data permanently using text files:
- `users.txt` → user accounts and profile data
- `quizzes.txt` → quiz definitions, questions, and correct answers
- `attempts.txt` → student attempt history and scores

## Technologies
- **Language:** C++ (C++14 compatible)
- **IDE:** Visual Studio 2022
- **Interface:** Console-based
- **Storage:** Plain text files (`.txt`)

## How to Run
1. Open the solution/project in Visual Studio 2022.
2. Build the project.
3. Run the program.
4. Use the menu to sign in/sign up and test role-based features.

## Educational Value
This project was designed to practice:
- Arrays and string handling
- Functions and modular design
- File handling (`ifstream`/`ofstream`)
- Conditional logic and loops
- Input validation and basic role-based design

---

**Study Arc** is a practical PF-level system that combines user management with quiz workflows in a clean, menu-driven console application.
