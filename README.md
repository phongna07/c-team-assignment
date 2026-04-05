A graphical GPA management system built with C and GTK4. This app lets you add, edit, remove, and view courses while automatically calculating cumulative GPA. It includes a custom VinUni-inspired theme and persistent course storage in `data.txt`.

## Project Structure

Make sure you have the following files in your project directory before compiling:
* `main.c` (Application entry point)
* `gui.c` & `gui.h` (GTK4 interface logic, callbacks, and GUI utilities)
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

Once GTK4 is installed, open your terminal (or MSYS2 MinGW64 terminal on Windows), navigate to the folder containing your code, and build with Make:

```bash
make
```

This builds the `gpa_manager` executable using:
* `main.c` as the single entry point.
* `gui.c` for GTK behavior and callbacks.
* `logic.c` and `file_manager.c` for course logic and persistence.

---

## How to Run

After a successful build, run the app:

**On Linux and macOS:**
```bash
./gpa_manager
```

**On Windows:**
```bash
./gpa_manager.exe
```

You can also use Make targets:

```bash
make run
```

`make gui` and `make run-gui` are kept as compatibility aliases and run the same GUI app.