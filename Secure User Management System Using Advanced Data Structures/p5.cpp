#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <conio.h>
#include <vector>
#include <stack>
#include <queue>
#include <map>
#include <windows.h>

using namespace std;

// Abstract base class for User
class User {
protected:
    string email;
    string pw;

public:
    // Constructor for User class
    User(string email, string pw) : email(email), pw(pw) {}
    virtual ~User() {}  // Destructor
    virtual void display() = 0;  // Pure virtual method to display user information
    string getId() const { return email; }  // Return email
    string getPW() const { return pw; }  // Return password
};

// Derived classes for User
// LoginUser class for users who have logged in
class LoginUser : public User {
public:
    LoginUser(string email, string pw) : User(email, pw) {}
    void display() override { cout << "Login User: " << email << endl; }  // Display login user information
};

// SignUpUser class for users who are signing up
class SignUpUser : public User {
public:
    SignUpUser(string email, string pw) : User(email, pw) {}
    void display() override { cout << "Sign Up User: " << email << endl; }  // Display signup user information
};

// Encrypt/Decrypt functions
// Encrypt a single character using Caesar Cipher
char encryptCh(char ch, int shift) {
    if (isalpha(ch)) {
        char base = isupper(ch) ? 'A' : 'a';
        return (ch - base + shift + 26) % 26 + base;  // Perform character shifting for letters
    }
    if (isdigit(ch)) return (ch - '0' + shift + 10) % 10 + '0';  // Perform character shifting for digits
    return ch;  // If it's neither, return the character as is (e.g., special characters)
}

// Encrypt the entire password
string encrypt(const string &password, int shift) {
    string encrypted = "";
    for (char ch : password) encrypted += encryptCh(ch, shift);  // Encrypt each character
    return encrypted;
}

// Decrypt a single character using Caesar Cipher
char decryptCh(char ch, int shift) {
    if (isalpha(ch)) {
        char base = isupper(ch) ? 'A' : 'a';
        return (ch - base - shift + 26) % 26 + base;  // Reverse the encryption for letters
    }
    if (isdigit(ch)) return (ch - '0' - shift + 10) % 10 + '0';  // Reverse the encryption for digits
    return ch;  // Return the character as is if it is a special character
}

// Decrypt the entire password
string decrypt(const string &encrypted, int shift) {
    string decrypted = "";
    for (char ch : encrypted) decrypted += decryptCh(ch, shift);  // Decrypt each character
    return decrypted;
}

// CheckError Class for handling validation and error checking
class CheckError {
public:
    // Check if entered password matches the decrypted password
    void checkingPassword(const string &decryptedPassword, const string &password) {
        if (decryptedPassword == password) {
            cout << "You have successfully logged in." << endl;  // Password match
        } else {
            cout << "Your password does not match." << endl;  // Password mismatch
        }
    }

    // Match the confirm password with the entered password
    string matchPassword(const string &confirmPassword, const string &password) {
        if (confirmPassword != password) {
            cout << "------------------------( Error )-----------------------------" << endl;
            cout << "Your password does not match." << endl;
            cout << "--------------------------------------------------------------" << endl;
            return "";  // Return empty if passwords don't match
        }
        return password;  // Return password if match
    }

    // Check if the email is valid
    bool isValidEmail(const string &email) {
        const regex pattern(R"((\w+)(\.|\+)?(\w*)@(\w+)\.(\w+)(\.(\w+))?)");
        return regex_match(email, pattern);  // Regex pattern to validate email
    }
};

// stericPassword class for secure password input
class stericPassword {
public:
    // Method to get password securely (masking input)
    string getPassword() {
        string password;
        char ch;

        while (true) {
            ch = _getch();  // Get input without showing characters on screen

            if (ch == '\r') {  // Enter key is pressed, end password input
                cout << endl;
                break;
            } else if (ch == '\b') {  // Backspace is pressed, remove last character
                if (!password.empty()) {
                    cout << "\b \b";  // Move back and erase the character
                    password.pop_back();  // Remove character from the password
                }
            } else {
                password.push_back(ch);  // Add character to password
                cout << '*';  // Display asterisk instead of actual character
            }
        }

        return password;  // Return the entered password
    }
};

// LinkedList for user management (storing user data)
struct Node {
    User *user;  // Pointer to a User
    Node *next;  // Pointer to the next Node
    Node(User *u) : user(u), next(nullptr) {}  // Constructor
};

class LinkedList {
private:
    Node *head;  // Pointer to the first node

public:
    LinkedList() : head(nullptr) {}  // Constructor to initialize head to nullptr

    // Add a new user to the list
    void addUser(User *user) {
        Node *newNode = new Node(user);
        newNode->next = head;
        head = newNode;  // Add to the front of the list
    }

    // Find a user by email
    User *findUser(const string &email) {
        Node *current = head;
        while (current) {
            if (current->user->getId() == email) return current->user;  // Return user if email matches
            current = current->next;
        }
        return nullptr;  // Return nullptr if user not found
    }

    // Display all users in the list
    void displayUsers() {
        Node *current = head;
        while (current) {
            current->user->display();  // Call display on each user
            current = current->next;
        }
    }

    ~LinkedList() {
        Node *current = head;
        while (current) {
            Node *temp = current;
            current = current->next;
            delete temp->user;  // Delete user to free memory
            delete temp;  // Delete node
        }
    }
};

// Stack for login attempts (stores recent login attempts)
class LoginStack {
private:
    stack<string> loginAttempts;
    int maxSize;  // Maximum size of the stack

public:
    LoginStack(int size) : maxSize(size) {}  // Constructor with max size

    // Add a login attempt to the stack
    void addAttempt(const string &attempt) {
        if (loginAttempts.size() == maxSize) loginAttempts.pop();  // Remove the oldest attempt if max size reached
        loginAttempts.push(attempt);  // Add the new attempt
    }

    // Display all login attempts
    void displayAttempts() {
        stack<string> temp = loginAttempts;
        while (!temp.empty()) {
            cout << temp.top() << endl;  // Display each login attempt
            temp.pop();
        }
    }
};

// Queue for recovery requests (manages password recovery requests)
class RecoveryQueue {
private:
    queue<string> recoveryRequests;

public:
    // Add a recovery request to the queue
    void addRequest(const string &email) { recoveryRequests.push(email); }

    // Process all recovery requests in the queue
    void processRequests() {
        while (!recoveryRequests.empty()) {
            cout << "Processing recovery for: " << recoveryRequests.front() << endl;  // Process and print request
            recoveryRequests.pop();  // Remove the request after processing
        }
    }
};

// Binary Search Tree (BST) for user connections (stores email addresses)
class BST {
private:
    struct TreeNode {
        string email;  // User's email
        TreeNode *left, *right;  // Pointers to left and right children
        TreeNode(const string &e) : email(e), left(nullptr), right(nullptr) {}  // Constructor
    };

    TreeNode *root;  // Root of the tree

    // Insert email into the BST
    void insert(TreeNode *&node, const string &email) {
        if (node == nullptr) {
            node = new TreeNode(email);  // Insert new node
        } else if (email < node->email) {
            insert(node->left, email);  // Insert into the left subtree
        } else if (email > node->email) {
            insert(node->right, email);  // Insert into the right subtree
        }
    }

    // Display all emails in the BST in-order
    void displayInOrder(TreeNode *node) {
        if (node) {
            displayInOrder(node->left);  // Traverse left
            cout << node->email << " ";  // Print email
            displayInOrder(node->right);  // Traverse right
        }
    }

public:
    BST() : root(nullptr) {}  // Constructor to initialize root to nullptr

    // Insert an email into the BST
    void insert(const string &email) {
        insert(root, email);  // Call the recursive insert function
    }

    // Display the emails in the BST
    void display() {
        displayInOrder(root);  // In-order traversal to display emails
        cout << endl;
    }
};

// File Handling Function (saves credentials to file)
void saveCredentialsToFile(const string &email, const string &password) {
    ofstream file("credentials.txt", ios::app);  // Open the file in append mode
    if (!file.is_open()) {
        cerr << "Error: Could not open the file for writing." << endl;
        return;  // Return if file could not be opened
    }
    file << email << ":" << password << endl;  // Write email and encrypted password to file
    if (file.good()) {
        cout << "Credentials for " << email << " have been saved successfully." << endl;
    } else {
        cerr << "Error: Failed to write to the file." << endl;
    }
    file.close();  // Close the file
}

// Main Function (User Interaction)
int main() {
    CheckError checker;  // Create instance for error checking
    stericPassword passwordMasker;  // Create instance for password masking

    LinkedList userList;  // LinkedList to manage users
    LoginStack loginStack(5);  // Stack to store login attempts (max 5)
    RecoveryQueue recoveryQueue;  // Queue for password recovery requests
    BST userConnections;  // BST to manage user connections (emails)

    bool exit = false;  // Flag to exit the loop

    // Main program loop
    while (!exit) {
        cout << "1. SignUp\n2. LogIn\n3. View Login Attempts\n4. Add Recovery Request\n5. Process Recovery Requests\n6. View Users in BST\n0. Exit\nChoice: ";
        int choice;
        cin >> choice;

        if (choice == 1) {  // Sign Up
            string email, pw, confirmPw;
            cout << "Enter Email: ";
            cin >> email;

            if (!checker.isValidEmail(email)) {
                cout << "Invalid email format. Please try again." << endl;
                continue;  // Skip to next iteration if email is invalid
            }

            cout << "Enter Password: ";
            pw = passwordMasker.getPassword();  // Get password securely
            cout << "Confirm Password: ";
            confirmPw = passwordMasker.getPassword();  // Confirm password

            // Check if passwords match
            string finalPassword = checker.matchPassword(confirmPw, pw);
            if (finalPassword == "") {
                continue;  // If passwords don't match, stop sign-up process
            }

            // Proceed with user registration if passwords match
            userList.addUser(new SignUpUser(email, encrypt(pw, 4)));  // Add user with encrypted password
            userConnections.insert(email);  // Insert email into BST
            cout << "----------SignUp Successful----------\n";
            saveCredentialsToFile(email, encrypt(pw, 4));  // Save encrypted password to file
        } else if (choice == 2) {  // Log In
            string email, pw;
            cout << "Enter Email: ";
            cin >> email;

            cout << "Enter Password: ";
            pw = passwordMasker.getPassword();  // Get password securely

            // Find user by email
            User *foundUser = userList.findUser(email);
            if (foundUser) {
                // Decrypt password and check if it matches entered password
                string decryptedPassword = decrypt(foundUser->getPW(), 4);
                checker.checkingPassword(decryptedPassword, pw);
                loginStack.addAttempt(email);  // Add login attempt to stack
            } else {
                cout << "Email not found. Please try again." << endl;
            }
        } else if (choice == 3) {  // View login attempts
            cout << "Login Attempts: ";
            loginStack.displayAttempts();
        } else if (choice == 4) {  // Add recovery request
            string email;
            cout << "Enter Email for Recovery: ";
            cin >> email;
            recoveryQueue.addRequest(email);  // Add recovery request to queue
        } else if (choice == 5) {  // Process recovery requests
            recoveryQueue.processRequests();  // Process all recovery requests
        } else if (choice == 6) {  // View users in BST
            cout << "Users in BST: ";
            userConnections.display();  // Display all users in BST
        } else if (choice == 0) {  // Exit
            exit = true;
            cout << "Exiting..." << endl;
        } else {
            cout << "Invalid choice. Please try again." << endl;
        }
    }

    return 0;
}
