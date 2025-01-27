#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_USERS 100
#define USERS_FILE "users_data.txt"

// User structure
struct nre {
    char name[100];
    char r[100];
    char e[100];
    int g;
};

struct nre homes[MAX_USERS];
int n = 0;

// Function to validate the password
int ramz(const char *r) {
    int lr = strlen(r);
    if (lr >= 7) {
        int hasDigit = 0, hasLower = 0, hasUpper = 0;

        for (int i = 0; i < lr; i++) {
            if (isdigit(r[i])) hasDigit = 1;
            if (islower(r[i])) hasLower = 1;
            if (isupper(r[i])) hasUpper = 1;
        }

        return hasDigit && hasLower && hasUpper;
    }
    return 0;
}

// Function to validate the email
int email(const char *e) {
    int atCount = 0, dotCount = 0;
    const char *atPos = strchr(e, '@');

    if (atPos) {
        atCount = 1; 
        const char *dotPos = strchr(atPos, '.');
        if (dotPos) {
            dotCount = 1; 
        }
    }

    return atCount == 1 && dotCount == 1;
}

// Check if the username already exists
int nameExists(const char *name) {
    for (int i = 0; i < n; i++) {
        if (strcmp(homes[i].name, name) == 0) {
            return 1; 
        }
    }
    return 0; 
}

// Login function
int login(int *m) {
    char username[100], password[100];

    printw("Enter username: ");
    echo();
    getstr(username);

    printw("Enter password: ");
    getstr(password);

    for (int i = 0; i < n; i++) {
        if (strcmp(homes[i].name, username) == 0 && strcmp(homes[i].r, password) == 0) {
            *m = i;
            printw("Login successful for %s!\n", username);
            getch();
            refresh();
            return 1; // Login success
        }
    }
    printw("Error: Invalid username or password.\n");
    refresh();
    return 0; // Login failed
}

// Function to save users to file
void saveUsers() {
    FILE *file = fopen(USERS_FILE, "w");
    if (file) {
        for (int i = 0; i < n; i++) {
            fprintf(file, "%s %s %s %d\n", homes[i].name, homes[i].r, homes[i].e, homes[i].g);
        }
        fclose(file);
    }
}

// Function to load users from file
void loadUsers() {
    FILE *file = fopen(USERS_FILE, "r");
    if (file) {
        while (fscanf(file, "%s %s %s %d", homes[n].name, homes[n].r, homes[n].e, &homes[n].g) == 4) {
            n++;
        }
        fclose(file);
    }
}

// Function to display the scoreboard
void displayScoreboard() {
    // Temporary array to hold sorted users
    struct nre sorted[MAX_USERS];
    for (int i = 0; i < n; i++) {
        sorted[i] = homes[i];
    }

    // Sort users by their score in descending order
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (sorted[j].g > sorted[i].g) {
                struct nre temp = sorted[i];
                sorted[i] = sorted[j];
                sorted[j] = temp;
            }
        }
    }

    // Display the sorted scoreboard
    clear();
    printw("Scoreboard:\n\n");
    for (int i = 0; i < n; i++) {
        printw("%s: %d\n", sorted[i].name, sorted[i].g);
    }
    getch();
}

// Display menu and handle user selection
int showMenu(int logged_in) {
    const char *options_logged_out[] = {"Signup", "Login", "Exit", "Scoreboard"};
    const char *options_logged_in[] = {"Logout"};
    int choice = 0;
    int highlight = 0;

    while (1) {
        clear();
        printw("Use arrow keys to navigate and press Enter to select:\n\n");

        if (logged_in) {
            for (int i = 0; i < 1; i++) {
                if (i == highlight) {
                    attron(A_REVERSE); // Highlight the selected option
                    printw("%s\n", options_logged_in[i]);
                    attroff(A_REVERSE);
                } else {
                    printw("%s\n", options_logged_in[i]);
                }
            }
        } else {
            for (int i = 0; i < 4; i++) {
                if (i == highlight) {
                    attron(A_REVERSE); // Highlight the selected option
                    printw("%s\n", options_logged_out[i]);
                    attroff(A_REVERSE);
                } else {
                    printw("%s\n", options_logged_out[i]);
                }
            }
        }

        int c = getch();
        switch (c) {
            case KEY_UP:
                highlight = (highlight - 1 + (logged_in ? 1 : 4)) % (logged_in ? 1 : 4); // Move up
                break;
            case KEY_DOWN:
                highlight = (highlight + 1) % (logged_in ? 1 : 4); // Move down
                break;
            case '\n': // Enter
                choice = highlight;
                return choice;
        }
    }
}

// Main program
int main() {
    int m = 0;
    initscr();
    keypad(stdscr, TRUE); // Enable arrow keys
    noecho();

    loadUsers();

    int logged_in = 0;
    char name[100];

    while (1) {
        int choice = showMenu(logged_in);
        if (!logged_in) { // User is logged out
            if (choice == 0) { // Signup
                printw("Enter username: ");
                echo();
                getstr(name);

                if (!nameExists(name)) {
                    char r[100], e[100];

                    printw("Enter password: ");
                    getstr(r);
                    if (!ramz(r)) {
                        printw("Invalid password\n");
                        continue;
                    }

                    printw("Enter email: ");
                    getstr(e);
                    if (!email(e)) {
                        printw("Invalid email\n");
                        continue;
                    }

                    strcpy(homes[n].name, name);
                    strcpy(homes[n].r, r);
                    strcpy(homes[n].e, e);
                    n++;
                    printw("Signup successful\n");
                } else {
                    printw("Username already exists\n");
                }
            } else if (choice == 1) { // Login
                logged_in = login(&m);
                if (logged_in) {
                    printw("%d\n", m);
                    printw("Login successful for %s!\n", homes[m].name);
                    homes[m].g += 5;
                    getch();
                }
            } else if (choice == 2) { // Exit
                break;
            } else if (choice == 3) { // Scoreboard
                displayScoreboard();
            }
        } else { // User is logged in
            if (choice == 0) { // Logout
                logged_in = 0;
                printw("Logged out successfully.\n");
                getch();
            }
        }

        refresh();
    }

    saveUsers(); // Save the users before exiting

    endwin();
    return 0;
}
