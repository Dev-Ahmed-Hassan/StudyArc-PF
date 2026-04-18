#include <iostream>
#include <string>
#include <conio.h>
#include <iomanip>
#include <fstream>
#include <limits>
using namespace std;

// ---------------- Constants ----------------
const int arrayLength = 15;
const int maxQuizzes = 40;
const int maxQuestions = 10;
const int maxAttempts = 300;

// ---------------- User Data ----------------
int usersCount = 0;
int activeUserIndex = -1;

string userName[arrayLength];
string password[arrayLength];
char userType[arrayLength];
int age[arrayLength];

// ---------------- Quiz Data ----------------
int quizzesCount = 0;
string quizName[maxQuizzes];
string quizCreator[maxQuizzes];
int quizQuestionCount[maxQuizzes];

string quizQuestion[maxQuizzes][maxQuestions];
string quizOption1[maxQuizzes][maxQuestions];
string quizOption2[maxQuizzes][maxQuestions];
string quizOption3[maxQuizzes][maxQuestions];
string quizOption4[maxQuizzes][maxQuestions];
int quizCorrectOption[maxQuizzes][maxQuestions];

// ---------------- Attempt History Data ----------------
int attemptsCount = 0;
string attemptStudent[maxAttempts];
string attemptQuiz[maxAttempts];
int attemptScore[maxAttempts];
int attemptTotal[maxAttempts];

// ---------------- Function Declarations ----------------
void printTitle();

int mainMenu();
char signIn(string name, string pass);
bool signUp(string name, string pass, char type);

int printAdminMenu();
int printStudentMenu();
int printTeacherMenu();

int addRecord();
int viewRecord();
int deleteRecord();
int updateRecord();
int editInformation();

void loadData();
void saveData();

void loadQuizzes();
void saveQuizzes();

void loadAttempts();
void saveAttempts();

void shiftArrayLeft(int index, string[]);
void shiftArrayLeftCH(int index, char[]);

// Input/validation helpers
void clearInputBuffer();
int getIntInRange(const string& prompt, int minVal, int maxVal);
string getTokenInput(const string& prompt);
string getLineInput(const string& prompt);
bool isValidUserType(char type, bool allowAdmin);
bool usernameExists(const string& name, int ignoreIndex = -1);
bool containsPipeChar(const string& value);

// Quiz helpers
int findQuizByName(const string& name);
void listAllQuizzes();
void listTeacherQuizzes(const string& teacherName);
void createQuiz(const string& creatorName);
void editQuiz(const string& editorName, bool adminAccess);
void deleteQuizByAdmin();
void attemptQuizByStudent();
void removeQuizzesByCreator(const string& creatorName);

// Attempts helpers
void addAttemptRecord(const string& student, const string& quiz, int score, int total);
void viewAttemptHistoryForStudent(const string& student);

// ---------------- Main ----------------
int main() {
    loadData();
    loadQuizzes();
    loadAttempts();
    printTitle();

    while (true) {
        int mainOption = mainMenu();

        if (mainOption == 1) {
            string inputUserName = getTokenInput("Please Enter your username: ");
            string inputPassword = getTokenInput("Please Enter your password: ");

            char type = signIn(inputUserName, inputPassword);

            if (type == 'a' || type == 'A') {
                int adminOption = 0;
                while (adminOption != 6) {
                    system("cls");
                    adminOption = printAdminMenu();

                    if (adminOption == 1) {
                        int addOption = addRecord();
                        if (addOption == 1) {
                            string name = getTokenInput("Enter Username: ");
                            string pass = getTokenInput("Enter Password: ");
                            char uType = static_cast<char>(tolower(getTokenInput("Enter UserType (s/t/a): ")[0]));

                            if (!isValidUserType(uType, true)) {
                                cout << "Invalid user type.\n";
                            }
                            else if (usernameExists(name)) {
                                cout << "Username already exists.\n";
                            }
                            else if (usersCount >= arrayLength) {
                                cout << "User capacity reached.\n";
                            }
                            else {
                                userName[usersCount] = name;
                                password[usersCount] = pass;
                                userType[usersCount] = uType;
                                age[usersCount] = 18;
                                usersCount++;
                                saveData();
                                cout << "User added successfully!\n";
                            }
                            _getch();
                        }
                    }
                    else if (adminOption == 2) {
                        int viewOption = viewRecord();
                        int count = 1;
                        for (int i = 0; i < usersCount; i++) {
                            if ((viewOption == 1 && (userType[i] == 't' || userType[i] == 'T')) ||
                                (viewOption == 2 && (userType[i] == 's' || userType[i] == 'S')) ||
                                (viewOption == 3 && (userType[i] == 'a' || userType[i] == 'A')) ||
                                viewOption == 4) {
                                cout << count++ << ". " << left << setw(15) << userName[i]
                                    << " Password: " << setw(15) << password[i]
                                    << " Type: " << userType[i]
                                    << " Age: " << age[i] << endl;
                            }
                        }
                        _getch();
                    }
                    else if (adminOption == 3) {
                        int delOption = deleteRecord();
                        if (delOption == 1) {
                            string name = getTokenInput("Enter Username to Delete: ");
                            int index = -1;

                            for (int i = 0; i < usersCount; i++) {
                                if (userName[i] == name) {
                                    index = i;
                                    break;
                                }
                            }

                            if (index == -1) {
                                cout << "User Not Found!\n";
                            }
                            else {
                                if (index == activeUserIndex) {
                                    cout << "You cannot delete your own active account.\n";
                                }
                                else {
                                    removeQuizzesByCreator(userName[index]);

                                    for (int i = index; i < usersCount - 1; i++) {
                                        userName[i] = userName[i + 1];
                                        password[i] = password[i + 1];
                                        userType[i] = userType[i + 1];
                                        age[i] = age[i + 1];
                                    }

                                    userName[usersCount - 1] = "";
                                    password[usersCount - 1] = "";
                                    userType[usersCount - 1] = ' ';
                                    age[usersCount - 1] = 0;
                                    usersCount--;

                                    saveData();
                                    saveQuizzes();
                                    cout << "User deleted successfully!\n";
                                }
                            }
                            _getch();
                        }
                    }
                    else if (adminOption == 4) {
                        int updOption = updateRecord();
                        string oldName = getTokenInput("Enter Username to Update: ");

                        int index = -1;
                        for (int i = 0; i < usersCount; i++) {
                            if (userName[i] == oldName) {
                                index = i;
                                break;
                            }
                        }

                        if (index == -1) {
                            cout << "User Not Found!\n";
                        }
                        else {
                            if (updOption == 1) {
                                string newName = getTokenInput("New Username: ");
                                if (usernameExists(newName, index)) {
                                    cout << "Username already exists.\n";
                                }
                                else {
                                    // rename creator in quiz ownership
                                    for (int q = 0; q < quizzesCount; q++) {
                                        if (quizCreator[q] == userName[index]) {
                                            quizCreator[q] = newName;
                                        }
                                    }
                                    userName[index] = newName;
                                    saveQuizzes();
                                    cout << "Username updated!\n";
                                }
                            }
                            else if (updOption == 2) {
                                string newPass = getTokenInput("New Password: ");
                                password[index] = newPass;
                                cout << "Password updated!\n";
                            }
                            else if (updOption == 3) {
                                char newType = static_cast<char>(tolower(getTokenInput("New Type (s/t/a): ")[0]));
                                if (!isValidUserType(newType, true)) {
                                    cout << "Invalid user type.\n";
                                }
                                else {
                                    userType[index] = newType;
                                    cout << "Type updated!\n";
                                }
                            }
                            saveData();
                        }
                        _getch();
                    }
                    else if (adminOption == 5) {
                        int quizOption = 0;
                        while (quizOption != 5) {
                            system("cls");
                            cout << "===== Admin Quiz Management =====\n";
                            cout << "1. Create Quiz\n";
                            cout << "2. Edit Quiz\n";
                            cout << "3. Delete Quiz\n";
                            cout << "4. View All Quizzes\n";
                            cout << "5. Back\n";
                            quizOption = getIntInRange("Your Option: ", 1, 5);

                            if (quizOption == 1) {
                                createQuiz(userName[activeUserIndex]);
                                _getch();
                            }
                            else if (quizOption == 2) {
                                editQuiz(userName[activeUserIndex], true);
                                _getch();
                            }
                            else if (quizOption == 3) {
                                deleteQuizByAdmin();
                                _getch();
                            }
                            else if (quizOption == 4) {
                                listAllQuizzes();
                                _getch();
                            }
                        }
                    }
                }
            }
            else if (type == 't' || type == 'T') {
                int teacherOption = 0;
                while (teacherOption != 6) {
                    system("cls");
                    teacherOption = printTeacherMenu();

                    if (teacherOption == 1) {
                        cout << "Name: " << left << setw(15) << userName[activeUserIndex]
                            << "Password: " << left << setw(15) << password[activeUserIndex]
                            << "Age: " << left << setw(15) << age[activeUserIndex]
                            << "User Type: " << left << setw(15) << userType[activeUserIndex] << endl;
                        _getch();
                    }
                    else if (teacherOption == 2) {
                        int editOption = editInformation();
                        if (editOption == 1) {
                            string newName = getTokenInput("New Username: ");
                            if (usernameExists(newName, activeUserIndex)) {
                                cout << "Username already exists.\n";
                            }
                            else {
                                string oldName = userName[activeUserIndex];
                                userName[activeUserIndex] = newName;
                                for (int q = 0; q < quizzesCount; q++) {
                                    if (quizCreator[q] == oldName) {
                                        quizCreator[q] = newName;
                                    }
                                }
                                saveQuizzes();
                                cout << "Username updated.\n";
                            }
                        }
                        else if (editOption == 2) {
                            password[activeUserIndex] = getTokenInput("New Password: ");
                            cout << "Password updated.\n";
                        }
                        else if (editOption == 3) {
                            age[activeUserIndex] = getIntInRange("New Age (5-100): ", 5, 100);
                            cout << "Age updated.\n";
                        }
                        saveData();
                        _getch();
                    }
                    else if (teacherOption == 3) {
                        createQuiz(userName[activeUserIndex]);
                        _getch();
                    }
                    else if (teacherOption == 4) {
                        editQuiz(userName[activeUserIndex], false);
                        _getch();
                    }
                    else if (teacherOption == 5) {
                        listTeacherQuizzes(userName[activeUserIndex]);
                        _getch();
                    }
                }
            }
            else if (type == 's' || type == 'S') {
                int studentOption = 0;
                while (studentOption != 6) {
                    system("cls");
                    studentOption = printStudentMenu();

                    if (studentOption == 1) {
                        cout << "Name: " << left << setw(15) << userName[activeUserIndex]
                            << "Password: " << left << setw(15) << password[activeUserIndex]
                            << "Age: " << left << setw(15) << age[activeUserIndex]
                            << "User Type: " << left << setw(15) << userType[activeUserIndex] << endl;
                        _getch();
                    }
                    else if (studentOption == 2) {
                        int editOption = editInformation();
                        if (editOption == 1) {
                            string newName = getTokenInput("New Username: ");
                            if (usernameExists(newName, activeUserIndex)) {
                                cout << "Username already exists.\n";
                            }
                            else {
                                userName[activeUserIndex] = newName;
                                cout << "Username updated.\n";
                            }
                        }
                        else if (editOption == 2) {
                            password[activeUserIndex] = getTokenInput("New Password: ");
                            cout << "Password updated.\n";
                        }
                        else if (editOption == 3) {
                            age[activeUserIndex] = getIntInRange("New Age (5-100): ", 5, 100);
                            cout << "Age updated.\n";
                        }
                        saveData();
                        _getch();
                    }
                    else if (studentOption == 3) {
                        attemptQuizByStudent();
                        _getch();
                    }
                    else if (studentOption == 4) {
                        listAllQuizzes();
                        _getch();
                    }
                    else if (studentOption == 5) {
                        viewAttemptHistoryForStudent(userName[activeUserIndex]);
                        _getch();
                    }
                }
            }
            else {
                cout << "Your username or password is incorrect!\n";
                _getch();
            }
        }
        else if (mainOption == 2) {
            string newUserName = getTokenInput("Enter your Username: ");
            string newPassword = getTokenInput("Enter password: ");
            char newUserType = static_cast<char>(tolower(getTokenInput("Enter UserType (s/t): ")[0]));

            if (!isValidUserType(newUserType, false)) {
                cout << "Only student/teacher signup allowed.\n";
            }
            else if (signUp(newUserName, newPassword, newUserType)) {
                cout << "Saved successfully.\n";
                saveData();
            }
            else {
                cout << "Username already exists or limit reached.\n";
            }
            _getch();
        }
        else if (mainOption == 3) {
            saveData();
            saveQuizzes();
            saveAttempts();
            cout << "Exiting the program. Goodbye!\n";
            break;
        }
    }

    return 0;
}

// ---------------- File Handling: Users ----------------
void loadData() {
    ifstream inFile("users.txt");
    if (!inFile) {
        userName[0] = "Student";
        password[0] = "student";
        userType[0] = 's';
        age[0] = 19;

        userName[1] = "Teacher";
        password[1] = "teacher";
        userType[1] = 't';
        age[1] = 35;

        userName[2] = "Admin";
        password[2] = "admin";
        userType[2] = 'a';
        age[2] = 49;

        usersCount = 3;
        return;
    }

    usersCount = 0;
    while (usersCount < arrayLength &&
        inFile >> userName[usersCount] >> password[usersCount] >> userType[usersCount] >> age[usersCount]) {
        usersCount++;
    }
    inFile.close();
}

void saveData() {
    ofstream outFile("users.txt");
    for (int i = 0; i < usersCount; i++) {
        if (!userName[i].empty()) {
            outFile << userName[i] << " " << password[i] << " " << userType[i] << " " << age[i] << endl;
        }
    }
    outFile.close();
}

// ---------------- File Handling: Quizzes ----------------
void loadQuizzes() {
    ifstream inFile("quizzes.txt");
    if (!inFile) {
        quizzesCount = 0;
        return;
    }

    quizzesCount = 0;
    int countFromFile = 0;
    if (!(inFile >> countFromFile)) {
        inFile.close();
        return;
    }

    inFile.ignore(numeric_limits<streamsize>::max(), '\n');

    if (countFromFile < 0) {
        countFromFile = 0;
    }
    if (countFromFile > maxQuizzes) {
        countFromFile = maxQuizzes;
    }

    for (int i = 0; i < countFromFile; i++) {
        getline(inFile, quizName[i]);
        getline(inFile, quizCreator[i]);

        if (!(inFile >> quizQuestionCount[i])) {
            break;
        }
        inFile.ignore(numeric_limits<streamsize>::max(), '\n');

        if (quizQuestionCount[i] < 0) {
            quizQuestionCount[i] = 0;
        }
        if (quizQuestionCount[i] > maxQuestions) {
            quizQuestionCount[i] = maxQuestions;
        }

        for (int q = 0; q < quizQuestionCount[i]; q++) {
            getline(inFile, quizQuestion[i][q]);
            getline(inFile, quizOption1[i][q]);
            getline(inFile, quizOption2[i][q]);
            getline(inFile, quizOption3[i][q]);
            getline(inFile, quizOption4[i][q]);

            if (!(inFile >> quizCorrectOption[i][q])) {
                quizCorrectOption[i][q] = 1;
            }
            inFile.ignore(numeric_limits<streamsize>::max(), '\n');

            if (quizCorrectOption[i][q] < 1 || quizCorrectOption[i][q] > 4) {
                quizCorrectOption[i][q] = 1;
            }
        }

        quizzesCount++;
    }

    inFile.close();
}

void saveQuizzes() {
    ofstream outFile("quizzes.txt");
    outFile << quizzesCount << endl;

    for (int i = 0; i < quizzesCount; i++) {
        outFile << quizName[i] << endl;
        outFile << quizCreator[i] << endl;
        outFile << quizQuestionCount[i] << endl;

        for (int q = 0; q < quizQuestionCount[i]; q++) {
            outFile << quizQuestion[i][q] << endl;
            outFile << quizOption1[i][q] << endl;
            outFile << quizOption2[i][q] << endl;
            outFile << quizOption3[i][q] << endl;
            outFile << quizOption4[i][q] << endl;
            outFile << quizCorrectOption[i][q] << endl;
        }
    }

    outFile.close();
}

// ---------------- File Handling: Attempts ----------------
void loadAttempts() {
    ifstream inFile("attempts.txt");
    if (!inFile) {
        attemptsCount = 0;
        return;
    }

    attemptsCount = 0;
    int countFromFile = 0;
    if (!(inFile >> countFromFile)) {
        inFile.close();
        return;
    }
    inFile.ignore(numeric_limits<streamsize>::max(), '\n');

    if (countFromFile < 0) {
        countFromFile = 0;
    }
    if (countFromFile > maxAttempts) {
        countFromFile = maxAttempts;
    }

    for (int i = 0; i < countFromFile; i++) {
        string line;
        if (!getline(inFile, line)) {
            break;
        }

        size_t p1 = line.find('|');
        size_t p2 = line.find('|', p1 == string::npos ? 0 : p1 + 1);
        size_t p3 = line.find('|', p2 == string::npos ? 0 : p2 + 1);

        if (p1 == string::npos || p2 == string::npos || p3 == string::npos) {
            continue;
        }

        string st = line.substr(0, p1);
        string qz = line.substr(p1 + 1, p2 - p1 - 1);
        string sc = line.substr(p2 + 1, p3 - p2 - 1);
        string tt = line.substr(p3 + 1);

        int scoreVal = 0;
        int totalVal = 0;
        try {
            scoreVal = stoi(sc);
            totalVal = stoi(tt);
        }
        catch (...) {
            continue;
        }

        attemptStudent[attemptsCount] = st;
        attemptQuiz[attemptsCount] = qz;
        attemptScore[attemptsCount] = scoreVal;
        attemptTotal[attemptsCount] = totalVal;
        attemptsCount++;
    }

    inFile.close();
}

void saveAttempts() {
    ofstream outFile("attempts.txt");
    outFile << attemptsCount << endl;
    for (int i = 0; i < attemptsCount; i++) {
        outFile << attemptStudent[i] << "|" << attemptQuiz[i] << "|"
            << attemptScore[i] << "|" << attemptTotal[i] << endl;
    }
    outFile.close();
}

// ---------------- Input/Validation Helpers ----------------
void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int getIntInRange(const string& prompt, int minVal, int maxVal) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            if (value >= minVal && value <= maxVal) {
                clearInputBuffer();
                return value;
            }
        }
        cout << "Invalid input. Enter value in range [" << minVal << "-" << maxVal << "].\n";
        clearInputBuffer();
    }
}

string getTokenInput(const string& prompt) {
    string value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            if (!value.empty()) {
                clearInputBuffer();
                return value;
            }
        }
        cout << "Invalid input. Try again.\n";
        clearInputBuffer();
    }
}

string getLineInput(const string& prompt) {
    string value;
    while (true) {
        cout << prompt;
        getline(cin, value);
        if (!value.empty()) {
            return value;
        }
        cout << "Input cannot be empty.\n";
    }
}

bool isValidUserType(char type, bool allowAdmin) {
    type = static_cast<char>(tolower(type));
    if (allowAdmin) {
        return type == 's' || type == 't' || type == 'a';
    }
    return type == 's' || type == 't';
}

bool usernameExists(const string& name, int ignoreIndex) {
    for (int i = 0; i < usersCount; i++) {
        if (i != ignoreIndex && userName[i] == name) {
            return true;
        }
    }
    return false;
}

bool containsPipeChar(const string& value) {
    return value.find('|') != string::npos;
}

// ---------------- Core Logic ----------------
void printTitle() {
    system("cls");
    cout << "=====================================\n";
    cout << "      University Management System   \n";
    cout << "=====================================\n";
    cout << "Press Any Key to continue...";
    _getch();
    system("cls");
}

int mainMenu() {
    system("cls");
    cout << "1. Signin\n";
    cout << "2. Signup\n";
    cout << "3. Exit\n";
    return getIntInRange("Your Option: ", 1, 3);
}

char signIn(string name, string pass) {
    for (int i = 0; i < usersCount; i++) {
        if (userName[i] == name && password[i] == pass) {
            activeUserIndex = i;
            return userType[i];
        }
    }
    return 'x';
}

bool signUp(string name, string pass, char type) {
    if (usernameExists(name)) {
        return false;
    }
    if (usersCount >= arrayLength) {
        return false;
    }

    userName[usersCount] = name;
    password[usersCount] = pass;
    userType[usersCount] = static_cast<char>(tolower(type));
    age[usersCount] = 18;
    usersCount++;
    return true;
}

// ---------------- Menus ----------------
int printAdminMenu() {
    cout << "1. Add Record\n";
    cout << "2. View Record\n";
    cout << "3. Delete Record\n";
    cout << "4. Update Record\n";
    cout << "5. Manage Quizzes\n";
    cout << "6. Logout\n";
    return getIntInRange("Your Option: ", 1, 6);
}

int printStudentMenu() {
    cout << "1. View Profile\n";
    cout << "2. Edit Info\n";
    cout << "3. Attempt Quiz\n";
    cout << "4. View Available Quizzes\n";
    cout << "5. View Attempt History\n";
    cout << "6. Logout\n";
    return getIntInRange("Your Option: ", 1, 6);
}

int printTeacherMenu() {
    cout << "1. View Profile\n";
    cout << "2. Edit Info\n";
    cout << "3. Create Quiz\n";
    cout << "4. Edit My Quiz\n";
    cout << "5. View My Quizzes\n";
    cout << "6. Logout\n";
    return getIntInRange("Your Option: ", 1, 6);
}

int addRecord() {
    cout << "1. Add New User\n";
    return getIntInRange("Option: ", 1, 1);
}

int viewRecord() {
    cout << "1. Teachers\n";
    cout << "2. Students\n";
    cout << "3. Admins\n";
    cout << "4. All\n";
    return getIntInRange("Option: ", 1, 4);
}

int deleteRecord() {
    cout << "1. Delete User\n";
    return getIntInRange("Option: ", 1, 1);
}

int updateRecord() {
    cout << "1. Username\n";
    cout << "2. Password\n";
    cout << "3. User Type\n";
    return getIntInRange("Option: ", 1, 3);
}

int editInformation() {
    cout << "1. Username\n";
    cout << "2. Password\n";
    cout << "3. Age\n";
    return getIntInRange("Option: ", 1, 3);
}

// ---------------- Quiz Helpers ----------------
int findQuizByName(const string& name) {
    for (int i = 0; i < quizzesCount; i++) {
        if (quizName[i] == name) {
            return i;
        }
    }
    return -1;
}

void listAllQuizzes() {
    system("cls");
    cout << "===== Available Quizzes =====\n";
    if (quizzesCount == 0) {
        cout << "No quizzes available.\n";
        return;
    }

    for (int i = 0; i < quizzesCount; i++) {
        cout << i + 1 << ". " << quizName[i]
            << " | Creator: " << quizCreator[i]
            << " | Questions: " << quizQuestionCount[i] << endl;
    }
}

void listTeacherQuizzes(const string& teacherName) {
    system("cls");
    cout << "===== My Quizzes =====\n";
    int count = 0;
    for (int i = 0; i < quizzesCount; i++) {
        if (quizCreator[i] == teacherName) {
            count++;
            cout << count << ". " << quizName[i]
                << " | Questions: " << quizQuestionCount[i] << endl;
        }
    }
    if (count == 0) {
        cout << "No quizzes created by you.\n";
    }
}

void createQuiz(const string& creatorName) {
    system("cls");

    if (quizzesCount >= maxQuizzes) {
        cout << "Quiz limit reached.\n";
        return;
    }

    string qName;
    while (true) {
        qName = getLineInput("Enter Quiz Name: ");
        if (containsPipeChar(qName)) {
            cout << "Quiz name cannot contain '|'.\n";
            continue;
        }
        if (findQuizByName(qName) != -1) {
            cout << "Quiz name already exists.\n";
            continue;
        }
        break;
    }

    int qCount = getIntInRange("How many questions (1-10): ", 1, maxQuestions);

    int index = quizzesCount;
    quizName[index] = qName;
    quizCreator[index] = creatorName;
    quizQuestionCount[index] = qCount;

    for (int q = 0; q < qCount; q++) {
        cout << "\nQuestion " << q + 1 << ":\n";
        quizQuestion[index][q] = getLineInput("Enter question text: ");
        quizOption1[index][q] = getLineInput("Option 1: ");
        quizOption2[index][q] = getLineInput("Option 2: ");
        quizOption3[index][q] = getLineInput("Option 3: ");
        quizOption4[index][q] = getLineInput("Option 4: ");
        quizCorrectOption[index][q] = getIntInRange("Correct option (1-4): ", 1, 4);
    }

    quizzesCount++;
    saveQuizzes();
    cout << "\nQuiz created successfully!\n";
}

void editQuiz(const string& editorName, bool adminAccess) {
    system("cls");
    if (quizzesCount == 0) {
        cout << "No quizzes available.\n";
        return;
    }

    listAllQuizzes();

    string nameToEdit = getLineInput("\nEnter exact quiz name to edit: ");
    int index = findQuizByName(nameToEdit);

    if (index == -1) {
        cout << "Quiz not found.\n";
        return;
    }

    if (!adminAccess && quizCreator[index] != editorName) {
        cout << "You can edit only your own quizzes.\n";
        return;
    }

    cout << "\n1. Rename Quiz\n";
    cout << "2. Edit a Question\n";
    cout << "3. Add New Question\n";
    cout << "4. Remove Last Question\n";
    int opt = getIntInRange("Option: ", 1, 4);

    if (opt == 1) {
        string newName;
        while (true) {
            newName = getLineInput("New quiz name: ");
            if (containsPipeChar(newName)) {
                cout << "Quiz name cannot contain '|'.\n";
                continue;
            }
            if (newName != quizName[index] && findQuizByName(newName) != -1) {
                cout << "Quiz name already exists.\n";
                continue;
            }
            break;
        }
        quizName[index] = newName;
        cout << "Quiz renamed successfully.\n";
    }
    else if (opt == 2) {
        int qNo = getIntInRange("Question number to edit: ", 1, quizQuestionCount[index]) - 1;
        quizQuestion[index][qNo] = getLineInput("New question text: ");
        quizOption1[index][qNo] = getLineInput("Option 1: ");
        quizOption2[index][qNo] = getLineInput("Option 2: ");
        quizOption3[index][qNo] = getLineInput("Option 3: ");
        quizOption4[index][qNo] = getLineInput("Option 4: ");
        quizCorrectOption[index][qNo] = getIntInRange("Correct option (1-4): ", 1, 4);
        cout << "Question updated successfully.\n";
    }
    else if (opt == 3) {
        if (quizQuestionCount[index] >= maxQuestions) {
            cout << "Cannot add more questions. Maximum reached.\n";
            return;
        }

        int qNo = quizQuestionCount[index];
        quizQuestion[index][qNo] = getLineInput("Question text: ");
        quizOption1[index][qNo] = getLineInput("Option 1: ");
        quizOption2[index][qNo] = getLineInput("Option 2: ");
        quizOption3[index][qNo] = getLineInput("Option 3: ");
        quizOption4[index][qNo] = getLineInput("Option 4: ");
        quizCorrectOption[index][qNo] = getIntInRange("Correct option (1-4): ", 1, 4);

        quizQuestionCount[index]++;
        cout << "Question added successfully.\n";
    }
    else if (opt == 4) {
        if (quizQuestionCount[index] <= 1) {
            cout << "Quiz must contain at least 1 question.\n";
            return;
        }
        quizQuestionCount[index]--;
        cout << "Last question removed.\n";
    }

    saveQuizzes();
}

void deleteQuizByAdmin() {
    system("cls");
    if (quizzesCount == 0) {
        cout << "No quizzes available.\n";
        return;
    }

    listAllQuizzes();
    string nameToDelete = getLineInput("\nEnter exact quiz name to delete: ");
    int index = findQuizByName(nameToDelete);

    if (index == -1) {
        cout << "Quiz not found.\n";
        return;
    }

    for (int i = index; i < quizzesCount - 1; i++) {
        quizName[i] = quizName[i + 1];
        quizCreator[i] = quizCreator[i + 1];
        quizQuestionCount[i] = quizQuestionCount[i + 1];

        for (int q = 0; q < maxQuestions; q++) {
            quizQuestion[i][q] = quizQuestion[i + 1][q];
            quizOption1[i][q] = quizOption1[i + 1][q];
            quizOption2[i][q] = quizOption2[i + 1][q];
            quizOption3[i][q] = quizOption3[i + 1][q];
            quizOption4[i][q] = quizOption4[i + 1][q];
            quizCorrectOption[i][q] = quizCorrectOption[i + 1][q];
        }
    }

    quizzesCount--;
    saveQuizzes();
    cout << "Quiz deleted successfully.\n";
}

void attemptQuizByStudent() {
    system("cls");
    if (quizzesCount == 0) {
        cout << "No quizzes available.\n";
        return;
    }

    listAllQuizzes();
    string targetQuizName = getLineInput("\nEnter exact quiz name to attempt: ");
    int index = findQuizByName(targetQuizName);

    if (index == -1) {
        cout << "Quiz not found.\n";
        return;
    }

    int total = quizQuestionCount[index];
    int score = 0;

    for (int q = 0; q < total; q++) {
        system("cls");
        cout << "Quiz: " << quizName[index] << "\n";
        cout << "Question " << q + 1 << ": " << quizQuestion[index][q] << "\n";
        cout << "1. " << quizOption1[index][q] << "\n";
        cout << "2. " << quizOption2[index][q] << "\n";
        cout << "3. " << quizOption3[index][q] << "\n";
        cout << "4. " << quizOption4[index][q] << "\n";

        int answer = getIntInRange("Your answer (1-4): ", 1, 4);
        if (answer == quizCorrectOption[index][q]) {
            score++;
        }
    }

    cout << "\nResult: " << score << " / " << total << endl;

    addAttemptRecord(userName[activeUserIndex], quizName[index], score, total);
    saveAttempts();
}

void removeQuizzesByCreator(const string& creatorName) {
    int i = 0;
    while (i < quizzesCount) {
        if (quizCreator[i] == creatorName) {
            for (int j = i; j < quizzesCount - 1; j++) {
                quizName[j] = quizName[j + 1];
                quizCreator[j] = quizCreator[j + 1];
                quizQuestionCount[j] = quizQuestionCount[j + 1];

                for (int q = 0; q < maxQuestions; q++) {
                    quizQuestion[j][q] = quizQuestion[j + 1][q];
                    quizOption1[j][q] = quizOption1[j + 1][q];
                    quizOption2[j][q] = quizOption2[j + 1][q];
                    quizOption3[j][q] = quizOption3[j + 1][q];
                    quizOption4[j][q] = quizOption4[j + 1][q];
                    quizCorrectOption[j][q] = quizCorrectOption[j + 1][q];
                }
            }
            quizzesCount--;
        }
        else {
            i++;
        }
    }
}

// ---------------- Attempts Helpers ----------------
void addAttemptRecord(const string& student, const string& quiz, int score, int total) {
    if (attemptsCount >= maxAttempts) {
        // drop oldest
        for (int i = 1; i < attemptsCount; i++) {
            attemptStudent[i - 1] = attemptStudent[i];
            attemptQuiz[i - 1] = attemptQuiz[i];
            attemptScore[i - 1] = attemptScore[i];
            attemptTotal[i - 1] = attemptTotal[i];
        }
        attemptsCount--;
    }

    attemptStudent[attemptsCount] = student;
    attemptQuiz[attemptsCount] = quiz;
    attemptScore[attemptsCount] = score;
    attemptTotal[attemptsCount] = total;
    attemptsCount++;
}

void viewAttemptHistoryForStudent(const string& student) {
    system("cls");
    cout << "===== Attempt History: " << student << " =====\n";

    int count = 0;
    for (int i = 0; i < attemptsCount; i++) {
        if (attemptStudent[i] == student) {
            count++;
            cout << count << ". Quiz: " << left << setw(20) << attemptQuiz[i]
                << " Score: " << attemptScore[i] << "/" << attemptTotal[i] << endl;
        }
    }

    if (count == 0) {
        cout << "No attempts found.\n";
    }
}

// ---------------- Legacy Helpers (kept) ----------------
void shiftArrayLeft(int index, string list[]) {
    for (int i = index; i < arrayLength - 1; i++) {
        list[i] = list[i + 1];
    }
    list[arrayLength - 1] = "-1";
}

void shiftArrayLeftCH(int index, char list[]) {
    for (int i = index; i < arrayLength - 1; i++) {
        list[i] = list[i + 1];
    }
    list[arrayLength - 1] = ' ';
}