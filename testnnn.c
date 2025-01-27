#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void rahro(int i1, int j1, int i2, int j2, char map[30][40]) {
    while (i1 != i2 || j1 != j2) {
        if (map[i1][j1] == ' ') {
            map[i1][j1] = '#'; // مسیر راهرو
        }

        clear(); // پاک کردن صفحه
        for (int i = 0; i < 30; i++) {
            for (int j = 0; j < 40; j++) {
                if (map[i][j] == '#') {
                    attron(COLOR_PAIR(2)); // راهرو به رنگ سبز
                    printw("%c", map[i][j]);
                    attroff(COLOR_PAIR(2));
                } else if (map[i][j] == '.') {
                    attron(COLOR_PAIR(3)); // فضای خالی به رنگ سفید
                    printw("%c", map[i][j]);
                    attroff(COLOR_PAIR(3));
                } else if (map[i][j] == '-') {
                    attron(COLOR_PAIR(1)); // دیوار افقی به رنگ قرمز
                    printw("%c", map[i][j]);
                    attroff(COLOR_PAIR(1));
                } else if (map[i][j] == '|') {
                    attron(COLOR_PAIR(1)); // دیوار عمودی به رنگ قرمز
                    printw("%c", map[i][j]);
                    attroff(COLOR_PAIR(1));
                } else if (map[i][j] == '+') {
                    attron(COLOR_PAIR(4)); // درب به رنگ آبی
                    printw("%c", map[i][j]);
                    attroff(COLOR_PAIR(4));
                } else if (map[i][j] == '*') {
                    attron(COLOR_PAIR(5)); // شخصیت به رنگ زرد
                    printw("%c", map[i][j]);
                    attroff(COLOR_PAIR(5));
                }
            }
            printw("\n");
        }
        refresh();
        napms(50);

        // محاسبه جهت حرکت به سمت مقصد
        if (i1 != i2 && (map[i1 + (i1 < i2 ? 1 : -1)][j1] == ' ' || map[i1 + (i1 < i2 ? 1 : -1)][j1] == '+' || map[i1 + (i1 < i2 ? 1 : -1)][j1] == '-')) {
            i1 += (i1 < i2) ? 1 : -1;
        } else if (j1 != j2 && (map[i1][j1 + (j1 < j2 ? 1 : -1)] == ' ' || map[i1][j1 + (j1 < j2 ? 1 : -1)] == '+' || map[i1][j1 + (j1 < j2 ? 1 : -1)] == '-'  )) {
            j1 += (j1 < j2) ? 1 : -1;
        } else if (i1 != i2) {
            i1 += (i1 < i2) ? 1 : -1;
        } else if (j1 != j2) {
            j1 += (j1 < j2) ? 1 : -1;
        }

        // اگر به در رسید
        if (map[i1][j1] == '+') {
            return;
        }
    }
}

void tarahimap(char map[30][40]) {
    for (int i = 0; i < 30; i++) {
        for (int j = 0; j < 40; j++) {
            map[i][j] = ' '; // مقداردهی اولیه با فضای خالی
        }
    }

    // طراحی اتاق اول (بالا سمت چپ)
    for (int i = 1; i < 9; i++) {
        for (int j = 1; j < 19; j++) {
            if (i == 1 || i == 8) {
                map[i][j] = '-'; // دیوارهای بالا و پایین
            } else if (j == 1 || j == 18) {
                map[i][j] = '|'; // دیوارهای کناری
            } else {
                map[i][j] = '.'; // فضای داخلی اتاق
            }
        }
    }
    map[8][10] = '+'; // درب اتاق اول (بالا مرکز)
    int rand_i = rand() % 8 ;  // برای اتاق اول
    int rand_j = rand() % 18 ;
    map[rand_i][rand_j] = '@';


    // طراحی اتاق دوم (پایین سمت راست)
    for (int i = 11; i < 19; i++) {
        for (int j = 21; j < 39; j++) {
            if (i == 11 || i == 18) {
                map[i][j] = '-'; // دیوارهای بالا و پایین
            } else if (j == 21 || j == 38) {
                map[i][j] = '|'; // دیوارهای کناری
            } else {
                map[i][j] = '.'; // فضای داخلی اتاق
            }
        }
    }
    map[11][25] = '+'; // درب اتاق دوم (پایین مرکز)

    // ایجاد راهرو بین اتاق‌ها
    rahro(9, 10, 11, 25, map); // ایجاد راهرو بین دو اتاق
    
}

void chapmap(char map[30][40]) {
    for (int i = 0; i < 30; i++) {
        for (int j = 0; j < 40; j++) {
            if (map[i][j] == '#') {
                attron(COLOR_PAIR(2)); // راهرو به رنگ سبز
                printw("%c", map[i][j]);
                attroff(COLOR_PAIR(2));
            } else if (map[i][j] == '.') {
                attron(COLOR_PAIR(3)); // فضای خالی به رنگ سفید
                printw("%c", map[i][j]);
                attroff(COLOR_PAIR(3));
            } else if (map[i][j] == '-') {
                attron(COLOR_PAIR(1)); // دیوار افقی به رنگ قرمز
                printw("%c", map[i][j]);
                attroff(COLOR_PAIR(1));
            } else if (map[i][j] == '|') {
                attron(COLOR_PAIR(1)); // دیوار عمودی به رنگ قرمز
                printw("%c", map[i][j]);
                attroff(COLOR_PAIR(1));
            } else if (map[i][j] == '+') {
                attron(COLOR_PAIR(4)); // درب به رنگ آبی
                printw("%c", map[i][j]);
                attroff(COLOR_PAIR(4));
            } else if (map[i][j] == '*') {
                attron(COLOR_PAIR(5)); // شخصیت به رنگ زرد
                printw("%c", map[i][j]);
                attroff(COLOR_PAIR(5));
            } else if (map[i][j] == '@') {
                attron(COLOR_PAIR(5)); // شخصیت به رنگ زرد
                printw("%c", map[i][j]);
                attroff(COLOR_PAIR(5));
            }
        }
        printw("\n");
    }
}

int main() {
    int score= 0;
    char mapk[30][40];
    char map[30][40];
    int i1 = 2, j1 = 10; // موقعیت اولیه در اتاق اول
    int i2 = 15, j2 = 30; // موقعیت انتقال به اتاق دوم
    int i3 = 5 , j3 = 10;
    int currentMap = 1;   // شروع از اتاق اول

    srand(time(0));

    initscr();
    noecho();
    curs_set(FALSE);
    start_color(); // فعال‌سازی رنگ‌ها

    // تعریف رنگ‌ها
    init_pair(1, COLOR_RED, COLOR_BLACK);  // رنگ قرمز برای دیوار
    init_pair(2, COLOR_GREEN, COLOR_BLACK); // رنگ سبز برای راهرو
    init_pair(3, COLOR_WHITE, COLOR_BLACK); // رنگ سفید برای فضای خالی
    init_pair(4, COLOR_BLUE, COLOR_BLACK);  // رنگ آبی برای درب
    init_pair(5, COLOR_BLUE, COLOR_RED); // رنگ زرد برای شخصیت

    tarahimap(map); // طراحی نقشه‌ها با راهروها
    map[i1][j1] = '*'; // قرار دادن شخصیت در اتاق اول
    

    int ch;
    mapk[i1][j1] = '.';
    while ((ch = getch()) != 'q') {
        
        // پاک کردن موقعیت قبلی
        if (currentMap == 1) {
            char prev = mapk[i1][j1];
            map[i1][j1] = prev;
        }

        /// منطق حرکت
if (ch == '2' && (map[i1 + 1][j1] == '.' || map[i1 + 1][j1] == '#' || map[i1 + 1][j1] == '+' || map[i1 + 1][j1] == '@')) {
    i1++;
}
if (ch == '8' && (map[i1 - 1][j1] == '.' || map[i1 - 1][j1] == '#' || map[i1 - 1][j1] == '+' || map[i1 - 1][j1] == '@')) {
    i1--;
}
if (ch == '4' && (map[i1][j1 - 1] == '.' || map[i1][j1 - 1] == '#' || map[i1][j1 - 1] == '+' || map[i1][j1 - 1] == '@')) {
    j1--;
}
if (ch == '6' && (map[i1][j1 + 1] == '.' || map[i1][j1 + 1] == '#' || map[i1][j1 + 1] == '+' || map[i1][j1 + 1] == '@')) {
    j1++;
}
if (ch == '9' && (map[i1 - 1][j1 + 1] == '.' || map[i1 - 1][j1 + 1] == '#' || map[i1 - 1][j1 + 1] == '+' || map[i1 - 1][j1 + 1] == '@')) {
    i1--; j1++;
}
if (ch == '7' && (map[i1 - 1][j1 - 1] == '.' || map[i1 - 1][j1 - 1] == '#' || map[i1 - 1][j1 - 1] == '+' || map[i1 - 1][j1 - 1] == '@')) {
    i1--; j1--;
}
if (ch == '1' && (map[i1 + 1][j1 - 1] == '.' || map[i1 + 1][j1 - 1] == '#' || map[i1 + 1][j1 - 1] == '+' || map[i1 + 1][j1 - 1] == '@')) {
    i1++; j1--;
}
if (ch == '3' && (map[i1 + 1][j1 + 1] == '.' || map[i1 + 1][j1 + 1] == '#' || map[i1 + 1][j1 + 1] == '+' || map[i1 + 1][j1 + 1] == '@')) {
    i1++; j1++;
}

        if(map[i1][j1] == '#'){
            mapk[i1][j1] = '#';
        } else if(map[i1][j1] == '+'){
            mapk[i1][j1] = '+';
        } else if (map[i1][j1] == '@') {
            move(0,0);
            score+=40;
            printw("You have %d gold\n", score);
            mapk[i1][j1] = '.';
            refresh();
            getch();
            
        }else{
            mapk[i1][j1] = '.';
        }

        // موقعیت جدید را تنظیم کن
        map[i1][j1] = '*';

        clear();
        chapmap(map);
    }

    endwin();
    return 0;
}
