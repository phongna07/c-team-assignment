A sleek, graphical GPA management system built with C and GTK4. This application allows you to add, edit, remove, and view university courses, automatically calculating your cumulative GPA. It features a custom VinUni-inspired theme (Endeavor Blue and Mahogany Red) and persistent data storage.

## Project Structure

Make sure you have the following files in your project directory before compiling:
* `gui.c` (The main GTK4 interface and application loop)
* `logic.c` & `logic.h` (Linked list and stack data structures)
* `file_manager.c` & `file_manager.h` (File I/O for saving/loading data)
* `data.txt` (Auto-generated to store your courses)

---

## Prerequisites & Installation

To compile and run this graphical app, you need a C compiler (`gcc`), `pkg-config`, and the **GTK4 development libraries**. Follow the instructions for your operating system:

### Linux (Ubuntu/Debian)
Open your terminal and run:
```bash
sudo apt update
sudo apt install build-essential libgtk-4-dev pkg-config
```

### macOS
The easiest way to install GTK4 on macOS is using [Homebrew](https://brew.sh/). Open your terminal and run:
```bash
brew install gtk4 pkg-config
```

### Windows (MSYS2)
Setting up GTK on Windows is best done via [MSYS2](https://www.msys2.org/). 
1. Download and install MSYS2.
2. Open the **MSYS2 MinGW 64-bit** terminal.
3. Run the following command to install the compiler and GTK4:
```bash
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-pkgconf mingw-w64-x86_64-gtk4
```

---

## How to Compile

Once GTK4 is installed, open your terminal (or MSYS2 MinGW64 terminal on Windows), navigate to the folder containing your code, and run this exact command:

```bash
gcc gui.c logic.c file_manager.c -o gpa_manager $(pkg-config --cflags --libs gtk4)
```

**What this command does:**
* `gcc`: Calls the C compiler.
* `gui.c logic.c file_manager.c`: The source files being compiled together.
* `-o gpa_manager`: Names the final executable file `gpa_manager`.
* `$(pkg-config --cflags --libs gtk4)`: Automatically finds and links all the necessary GTK4 background libraries and headers.

---

## How to Run

After a successful compilation, you will see a new executable file in your folder. Run the application from your terminal:

**On Linux and macOS:**
```bash
./gpa_manager
```

**On Windows:**
```bash
./gpa_manager.exe
```