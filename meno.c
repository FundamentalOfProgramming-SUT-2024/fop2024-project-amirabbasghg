#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
//kjshdjksd

int ramz(const char *r) {
    int lr = strlen(r);
    if (lr >= 7) {
        int hasDigit = 0, hasLower = 0, hasUpper = 0;

        for (int i = 0; i < lr; i++) {
            if (isdigit(r[i])) hasDigit = 1;
            if (islower(r[i])) hasLower = 1;
            if (isupper(r[i])) hasUpper = 1;
        }

        if (hasDigit && hasLower && hasUpper) {
            return 1; 
        } else {
            printw("invalid command\n");
            return 0;
        }
    } else {
        printw("invalid command\n");
        return 0;
    }
}

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

    if (atCount == 1 && dotCount == 1) {
        return 1;
    } else {
        printw("invalid command\n");
        return 0;
    }
}


int nameExists(FILE *file, const char *name) {
    char buffer[100];
    rewind(file);
    while (fgets(buffer, sizeof(buffer), file)) {
        buffer[strcspn(buffer, "\n")] = 0; 
        if (strcmp(buffer, name) == 0) {
            return 1; 
        }
    }
    return 0; 
}


struct nre {
    char name[100];
    char r[100];
    char e[100];
};

int main() {
    initscr(); 
    int n = 0;
    struct nre homes[100]; 
    FILE *file;
    char name[100];

    file = fopen("filename.txt", "a+");
    if (file == NULL) {
        printw("invalid command\n");
        endwin();
        return 1;
    }

    printw("enter name: ");
    getstr(name);

    if (!nameExists(file, name)) {
        char r[100], e[100];

        printw("enter ramz: ");
        getstr(r);
        if (!ramz(r)) {
            printw("invalid command\n");
            fclose(file);
            endwin();
            return 0;
        }

        printw("enter email: ");
        getstr(e);
        if (!email(e)) {
            printw("invalid command\n");
            fclose(file);
            endwin();
            return 0;
        }

        fprintf(file, "%s\n", name);
        strcpy(homes[n].name, name);
        strcpy(homes[n].r, r);
        strcpy(homes[n].e, e);
        n++;
        printw("sucsess\n");

    } else {
        printw("again\n");
    }

    fclose(file);
    endwin(); 
    return 0;
}