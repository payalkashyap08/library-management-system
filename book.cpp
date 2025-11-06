#include <iostream>
#include <unordered_map>
#include <queue>
#include <vector>
#include <map>
#include <string>
#include <iomanip>
#include <limits>
#include <cstdlib>
#include <algorithm>
using namespace std;

struct Book {
    string id, title, author;
    int borrowCount;
    bool isBorrowed;
    int totalCopies;
    int borrowedCopies;
    vector<pair<string, string>> borrowers; // borrowerID, borrowerName
    Book(string i, string t, string a, int copies = 1)
        : id(i), title(t), author(a), borrowCount(0), isBorrowed(false), 
          totalCopies(copies), borrowedCopies(0) {}
};

struct Borrower {
    string id, name, phone;
    vector<string> borrowedBooks; // book IDs
    Borrower(string bid, string n, string p) : id(bid), name(n), phone(p) {}
};

unordered_map<string, Book> books;
map<string, Book*> bstByTitle;
priority_queue<pair<int, string>> maxHeap; // borrowCount, bookID
queue<string> borrowQueue;
unordered_map<string, Borrower> borrowers;

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void printHeader() {
    cout << "\n";
    cout << "╔══════════════════════════════════════════════════════════════╗\n";
    cout << "║                    📚 LIBRARY MANAGEMENT SYSTEM 📚           ║\n";
    cout << "╚══════════════════════════════════════════════════════════════╝\n";
    cout << "\n";
}

void printDivider() {
    cout << "──────────────────────────────────────────────────────────────────\n";
}

void waitForEnter() {
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void showStats() {
    int totalBooks = books.size();
    int totalBorrowed = 0;
    int totalBorrowers = borrowers.size();
    
    for (auto &pair : books) {
        totalBorrowed += pair.second.borrowedCopies;
    }
    
    cout << "\n📊 LIBRARY STATISTICS:\n";
    printDivider();
    cout << "   📚 Total Books: " << totalBooks << "\n";
    cout << "   📖 Books Borrowed: " << totalBorrowed << "\n";
    cout << "   👥 Active Borrowers: " << totalBorrowers << "\n";
    printDivider();
}

void addBook(string id, string title, string author, int copies = 1) {
    if (books.count(id)) {
        cout << "❌ Book with ID '" << id << "' already exists!\n";
        return;
    }
    
    books[id] = Book(id, title, author, copies);
    bstByTitle[title] = &books[id];
    
    cout << "✅ Book added successfully!\n";
    cout << "   📖 Title: " << title << "\n";
    cout << "   👤 Author: " << author << "\n";
    cout << "   🆔 ID: " << id << "\n";
    cout << "   📚 Copies: " << copies << "\n";
}

void searchBook(string id) {
    if (books.count(id)) {
        Book& book = books[id];
        cout << "\n📖 Book Found:\n";
        printDivider();
        cout << "   🆔 ID: " << book.id << "\n";
        cout << "   📚 Title: " << book.title << "\n";
        cout << "   👤 Author: " << book.author << "\n";
        cout << "   📊 Borrow Count: " << book.borrowCount << " times\n";
        cout << "   📚 Available: " << (book.totalCopies - book.borrowedCopies) << "/" << book.totalCopies << "\n";
        cout << "   📍 Status: " << (book.borrowedCopies >= book.totalCopies ? "🔴 All Copies Borrowed" : "🟢 Available") << "\n";
        
        if (!book.borrowers.empty()) {
            cout << "   👥 Current Borrowers:\n";
            for (auto &borrower : book.borrowers) {
                cout << "      - " << borrower.second << " (ID: " << borrower.first << ")\n";
            }
        }
        printDivider();
    } else {
        cout << "❌ Book with ID '" << id << "' not found!\n";
    }
}

void borrowBook(string bookId, string borrowerId, string borrowerName, string phone) {
    if (!books.count(bookId)) {
        cout << "❌ Book with ID '" << bookId << "' not found!\n";
        return;
    }
    
    Book& book = books[bookId];
    if (book.borrowedCopies >= book.totalCopies) {
        cout << "❌ No copies available for borrowing!\n";
        return;
    }
    
    // Add or update borrower
    borrowers[borrowerId] = Borrower(borrowerId, borrowerName, phone);
    borrowers[borrowerId].borrowedBooks.push_back(bookId);
    
    // Update book
    book.borrowedCopies++;
    book.borrowCount++;
    book.borrowers.push_back({borrowerId, borrowerName});
    borrowQueue.push(borrowerId);
    maxHeap.push({book.borrowCount, bookId});
    
    cout << "✅ Book borrowed successfully!\n";
    cout << "   📖 '" << book.title << "' by " << book.author << "\n";
    cout << "   👤 Borrower: " << borrowerName << " (ID: " << borrowerId << ")\n";
    cout << "   📞 Phone: " << phone << "\n";
    cout << "   📅 Due date: 14 days from now\n";
}

void returnBookByBorrowerId(string borrowerId) {
    bool found = false;
    
    for (auto &pair : books) {
        Book& book = pair.second;
        auto it = find_if(book.borrowers.begin(), book.borrowers.end(),
                         [&borrowerId](const pair<string, string>& p) {
                             return p.first == borrowerId;
                         });
        
        if (it != book.borrowers.end()) {
            string borrowerName = it->second;
            book.borrowers.erase(it);
            book.borrowedCopies--;
            
            // Remove from borrower's list
            if (borrowers.count(borrowerId)) {
                auto& borrower = borrowers[borrowerId];
                borrower.borrowedBooks.erase(
                    remove(borrower.borrowedBooks.begin(), borrower.borrowedBooks.end(), book.id),
                    borrower.borrowedBooks.end()
                );
                
                // Remove borrower if no more books
                if (borrower.borrowedBooks.empty()) {
                    borrowers.erase(borrowerId);
                }
            }
            
            cout << "🔄 Book returned successfully!\n";
            cout << "   📖 '" << book.title << "' by " << book.author << "\n";
            cout << "   👤 Returned by: " << borrowerName << " (ID: " << borrowerId << ")\n";
            cout << "   ✅ Thank you for returning on time!\n";
            found = true;
            break;
        }
    }
    
    if (!found) {
        cout << "❌ Borrower ID '" << borrowerId << "' not found!\n";
    }
}

void returnBook() {
    if (!borrowQueue.empty()) {
        string borrowerId = borrowQueue.front();
        borrowQueue.pop();
        returnBookByBorrowerId(borrowerId);
    } else {
        cout << "📚 No books are currently borrowed.\n";
    }
}

void mostPopular() {
    if (maxHeap.empty()) {
        cout << "📊 No borrowing data available.\n";
        return;
    }
    
    string id = maxHeap.top().second;
    Book& book = books[id];
    
    cout << "\n🔥 Most Popular Book:\n";
    printDivider();
    cout << "   📖 Title: " << book.title << "\n";
    cout << "   👤 Author: " << book.author << "\n";
    cout << "   📊 Borrowed: " << book.borrowCount << " times\n";
    cout << "   📚 Available: " << (book.totalCopies - book.borrowedCopies) << "/" << book.totalCopies << "\n";
    cout << "   📍 Status: " << (book.borrowedCopies >= book.totalCopies ? "🔴 All Copies Borrowed" : "🟢 Available") << "\n";
    printDivider();
}

void showAllBooks() {
    if (books.empty()) {
        cout << "📚 No books available in the library.\n";
        return;
    }
    
    cout << "\n📚 All Books in Library:\n";
    printDivider();
    cout << setw(8) << left << "ID" << " | ";
    cout << setw(25) << left << "Title" << " | ";
    cout << setw(20) << left << "Author" << " | ";
    cout << setw(8) << left << "Borrows" << " | ";
    cout << setw(12) << left << "Available" << " | ";
    cout << "Status\n";
    printDivider();
    
    for (auto &pair : books) {
        Book& b = pair.second;
        cout << setw(8) << left << b.id << " | ";
        cout << setw(25) << left << (b.title.length() > 23 ? b.title.substr(0, 20) + "..." : b.title) << " | ";
        cout << setw(20) << left << (b.author.length() > 17 ? b.author.substr(0, 14) + "..." : b.author) << " | ";
        cout << setw(8) << left << b.borrowCount << " | ";
        cout << setw(12) << left << (b.totalCopies - b.borrowedCopies) + "/" + to_string(b.totalCopies) << " | ";
        cout << (b.borrowedCopies >= b.totalCopies ? "🔴 All Borrowed" : "🟢 Available") << "\n";
    }
    printDivider();
}

void showBorrowersList() {
    if (borrowers.empty()) {
        cout << "📚 No active borrowers found.\n";
        return;
    }
    
    cout << "\n👥 Active Borrowers List:\n";
    printDivider();
    
    int totalBorrowedBooks = 0;
    for (auto &borrower : borrowers) {
        totalBorrowedBooks += borrower.second.borrowedBooks.size();
    }
    
    cout << "📊 Summary:\n";
    cout << "   👥 Total Active Borrowers: " << borrowers.size() << "\n";
    cout << "   📚 Total Borrowed Books: " << totalBorrowedBooks << "\n\n";
    
    for (auto &pair : borrowers) {
        Borrower& borrower = pair.second;
        cout << "👤 " << borrower.name << "\n";
        cout << "   📞 Phone: " << borrower.phone << "\n";
        cout << "   🆔 ID: " << borrower.id << "\n";
        cout << "   📚 Books Borrowed: " << borrower.borrowedBooks.size() << "\n";
        
        if (!borrower.borrowedBooks.empty()) {
            cout << "   📖 Borrowed Books:\n";
            for (int i = 0; i < borrower.borrowedBooks.size(); i++) {
                string bookId = borrower.borrowedBooks[i];
                if (books.count(bookId)) {
                    Book& book = books[bookId];
                    cout << "      " << (i+1) << ". " << book.title << " by " << book.author << " (ID: " << bookId << ")\n";
                }
            }
        }
        cout << "\n";
    }
    printDivider();
}

void showMenu() {
    printHeader();
    cout << "🔧 Available Operations:\n";
    cout << "   1. 📊 Show Library Statistics\n";
    cout << "   2. 📖 Add a new book\n";
    cout << "   3. 🔍 Search for a book\n";
    cout << "   4. 📚 Borrow a book\n";
    cout << "   5. 🔄 Return a book (by borrower ID)\n";
    cout << "   6. 🔄 Return a book (FIFO)\n";
    cout << "   7. 🔥 Show most popular book\n";
    cout << "   8. 📋 Display all books\n";
    cout << "   9. 👥 Show borrowers list\n";
    cout << "   0. 🚪 Exit\n";
    printDivider();
    cout << "Enter your choice (0-9): ";
}

void run() {
    // Preload some sample books
    addBook("B101", "The Great Gatsby", "F. Scott Fitzgerald", 5);
    addBook("B102", "To Kill a Mockingbird", "Harper Lee", 3);
    addBook("B103", "Pride and Prejudice", "Jane Austen", 4);
    addBook("B104", "1984", "George Orwell", 6);
    addBook("B105", "The Catcher in the Rye", "J.D. Salinger", 2);
    addBook("B201", "The Alchemist", "Paulo Coelho", 4);
    addBook("B202", "The Kite Runner", "Khaled Hosseini", 3);
    addBook("B301", "Dune", "Frank Herbert", 3);
    addBook("B401", "The Da Vinci Code", "Dan Brown", 7);
    addBook("B501", "Sapiens", "Yuval Noah Harari", 4);
    
    int choice;
    string id, title, author, borrowerId, borrowerName, phone;
    int copies;
    
    while (true) {
        clearScreen();
        showMenu();
        cin >> choice;
        
        switch (choice) {
            case 0:
                clearScreen();
                printHeader();
                cout << "👋 Thank you for using the Library Management System!\n";
                cout << "   Have a great day! 📚\n";
                printDivider();
                return;
                
            case 1:
                clearScreen();
                printHeader();
                showStats();
                waitForEnter();
                break;
                
            case 2:
                clearScreen();
                printHeader();
                cout << "📖 Add New Book\n";
                printDivider();
                cout << "Enter book ID: ";
                cin >> id;
                cin.ignore();
                cout << "Enter book title: ";
                getline(cin, title);
                cout << "Enter author name: ";
                getline(cin, author);
                cout << "Enter number of copies: ";
                cin >> copies;
                addBook(id, title, author, copies);
                waitForEnter();
                break;
                
            case 3:
                clearScreen();
                printHeader();
                cout << "🔍 Search Book\n";
                printDivider();
                cout << "Enter book ID: ";
                cin >> id;
                searchBook(id);
                waitForEnter();
                break;
                
            case 4:
                clearScreen();
                printHeader();
                cout << "📚 Borrow Book\n";
                printDivider();
                cout << "Enter book ID: ";
                cin >> id;
                cin.ignore();
                cout << "Enter borrower name: ";
                getline(cin, borrowerName);
                cout << "Enter borrower ID: ";
                cin >> borrowerId;
                cin.ignore();
                cout << "Enter phone number: ";
                getline(cin, phone);
                borrowBook(id, borrowerId, borrowerName, phone);
                waitForEnter();
                break;
                
            case 5:
                clearScreen();
                printHeader();
                cout << "🔄 Return Book by Borrower ID\n";
                printDivider();
                cout << "Enter borrower ID: ";
                cin >> borrowerId;
                returnBookByBorrowerId(borrowerId);
                waitForEnter();
                break;
                
            case 6:
                clearScreen();
                printHeader();
                cout << "🔄 Return Book (FIFO)\n";
                printDivider();
                returnBook();
                waitForEnter();
                break;
                
            case 7:
                clearScreen();
                printHeader();
                cout << "🔥 Most Popular Book\n";
                printDivider();
                mostPopular();
                waitForEnter();
                break;
                
            case 8:
                clearScreen();
                printHeader();
                cout << "📋 All Books\n";
                printDivider();
                showAllBooks();
                waitForEnter();
                break;
                
            case 9:
                clearScreen();
                printHeader();
                cout << "👥 Borrowers List\n";
                printDivider();
                showBorrowersList();
                waitForEnter();
                break;
                
            default:
                cout << "❌ Invalid choice! Please enter a number between 0-9.\n";
                waitForEnter();
                break;
        }
    }
}

int main() {
    run();
    return 0;
}
