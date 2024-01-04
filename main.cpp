#include <ncurses.h>
#undef timeout
#include <boost/asio.hpp>
#include <string>
#include <vector>
#include "Menu.h"
#include "Download.h"

void initialize_ncurses() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    if (has_colors() == FALSE) {
        endwin();
        printf("Your terminal does not support color\n");
        exit(1);
    }
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
}

int main() {
    initialize_ncurses();

    // Create main menu and submenus
    Menu mainMenu("Choose protocol\n", {"FTP", "FTPS", "HTTP", "HTTPS", "Exit"});
    Menu ftpMenu("FTP Options\n", {"Option 1", "Option 2", "Back"});

    // Set up actions for main menu
    mainMenu.setAction(0, [&]() { ftpMenu.display(); });
    mainMenu.setAction(4, [&]() { endwin(); exit(0); });

    ftpMenu.setAction(2, [&]() { mainMenu.display(); });
    ftpMenu.setAction(0, [&]() {
        Download download("FTP", "klokanek.endora.cz", "/Users/olivermrovcak/test.txt", "/web/style.css", "olivergg", "Heslo5.", 1);
    });

    // Display the main menu
    mainMenu.display();

    endwin();
    return 0;
}


