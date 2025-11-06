# Library Management System
A console-based library management system built in C++ with a simple HTML front-end interface.
This project helps manage books, borrowers, borrowing and returning books, search functionality, and basic statistics.

## Table of Contents
1. [Features](#features)
2. [Technologies Used](#technologies-used)
3. [Getting Started](#getting-started)
   - Prerequisites
   - Setup / Compilation
   - Running the Program
4. [Usage](#usage)
   - Book Management
   - Borrower / Member Management
   - Borrowing & Returning Books
   - Searching & Statistics
5. [Project Structure](#project-structure)
6. [Future Enhancements](#future-enhancements)
7. [Contributing](#contributing)
8. [License](#license)
9. [Acknowledgements](#acknowledgements)

## Features
- Add, edit, delete books
- Maintain borrower/member records
- Borrow a book and return a book operations
- Search for books by title, author, ID
- Display statistics such as number of books issued, number available, etc.
- Simple HTML front-end (index.html) for viewing or launching operations (optional)
- File persistence (so data remains between runs)

## Technologies Used
- C++ (for the core console application)
- HTML (index.html — lightweight front interface or landing page)
- Standard file I/O for persistence
- Basic data structures (arrays / vectors, classes)
- Console I/O for user interaction

## Getting Started

### Prerequisites
- C++ compiler (g++, clang, or Visual Studio)
- Basic familiarity with the console / command line

### Setup / Compilation
1. Clone the repository:
   ```bash
   git clone https://github.com/payalkashyap08/library-management-system.git
   ```
2. Navigate into the folder:
   ```bash
   cd library-management-system
   ```
3. Compile the C++ source file (example using g++):
   ```bash
   g++ book.cpp -o library_system
   ```
   (Adjust if you add more .cpp or header files.)

### Running the Program
Execute the compiled binary:
```bash
./library_system
```
Follow the menu prompts in the console: add books, register members, issue books, etc.

If you like, you can open `index.html` in a browser for the front page (though full interaction is via console).

## Usage
Here’s an overview of how to interact with the system:

### Book Management
- Add new books: input book id, title, author, number of copies.
- Edit book details: update any field for an existing book.
- Delete book: remove an entry (if no copies issued) or mark as inactive.

### Borrower / Member Management
- Register a new borrower/member: assign member id, name, contact information.
- Edit borrower details.
- Delete / deactivate borrower (if no outstanding borrowed book).

### Borrowing & Returning Books
- Issue a book: specify borrower id, book id, check if copies available.
- Return a book: update the system, increment available copies, record returned date.
- Handle overdue logic (if implemented) or track status (issued/returned).

### Searching & Statistics
- Search books by title, author or id.
- View total number of books, available copies, issued copies.
- View list of borrowers with issued books.

## Project Structure
```
library-management-system/
├── book.cpp           # Main C++ source file for the console app
├── index.html         # Basic front-page HTML
├── README.md          # This documentation
└── (other files e.g., data files)
```
Feel free to refactor into multiple `.cpp/.h` files for better organization.

## Future Enhancements
Here are some ideas to extend the system:
- GUI version (e.g., using Qt or SFML)
- Multi-file architecture with headers, classes, modules
- Database backend (SQLite) instead of file I/O
- Fine management: track overdue returns & fines
- Reports: most borrowed books, active borrowers, history logs
- Web interface: convert `index.html` into a full-web app front-end
- User authentication (admin, librarian, member roles)
- Book reservations, waitlists

## Contributing
Contributions are welcome!
1. Fork the repository
2. Create your feature branch (`git checkout -b feature-xyz`)
3. Commit your changes (`git commit -m "Add feature xyz"`)
4. Push to your branch (`git push origin feature-xyz`)
5. Create a Pull Request describing your changes

Please ensure your code is well-documented, follows good C++ practices, and includes any required tests or sample data.

## License
This project is open-source and available under the [MIT License](LICENSE) (or specify whichever license you prefer).

## Acknowledgements
- Thanks to all open-source C++ resources and tutorials that helped build this system.
- Inspired by standard library management use-cases in academic settings.

---

*Enjoy coding and thanks for checking out the project!*
