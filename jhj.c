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
                printw("%c", map[i][j]);
            }
            printw("\n");
        }
        refresh();
        napms(50);

        // محاسبه جهت حرکت به سمت مقصد
        if (i1 != i2 && (map[i1 + (i1 < i2 ? 1 : -1)][j1] == ' ' || map[i1 + (i1 < i2 ? 1 : -1)][j1] == '+')) {
            i1 += (i1 < i2) ? 1 : -1;
        } else if (j1 != j2 && (map[i1][j1 + (j1 < j2 ? 1 : -1)] == ' ' || map[i1][j1 + (j1 < j2 ? 1 : -1)] == '+')) {
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
// تابع طراحی نقشه‌ها با راهروها
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
    map[15][21] = '+'; // درب اتاق دوم (پایین مرکز)

    // ایجاد راهرو بین اتاق‌ها
    rahro(9, 10, 15, 21, map); // ایجاد راهرو بین دو اتاق
}

// تابع چاپ نقشه کامل
// void chapmap1(char map[30][40]) {
//     for (int i = 0; i < 19; i++) {
//         for (int j = 0; j < 21; j++) {
//             printw("%c", map[i][j]);
//         }
//         printw("\n");
//     }
// }
void chapmap1(char map[30][40]) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 21; j++) {
            printw("%c", map[i][j]);
        }
        printw("\n");
    }
}
void chapmap2(char map[30][40]) {
    for (int i = 0; i < 30; i++) {
        for (int j = 0; j < 40; j++) {
            printw("%c", map[i][j]);
        }
        printw("\n");
    }
}
// تابع چاپ نقشه با دید محدود در راهرو
void chapmap_rahro(char map[30][40], int i1, int j1) {
    for (int i = 0; i < 30; i++) {
        for (int j = 0; j < 40; j++) {
            // فقط راهروها و موقعیت فعلی نمایش داده می‌شود
            if ((map[i][j] == '#' || map[i][j] == '+' || (i == i1 && j == j1))) {
                printw("%c", map[i][j]);
            } else {
                printw(" "); // سایر نقاط مخفی
            }
        }
        printw("\n");
    }
}
void chapmap_rahro_limited(char map[30][40], int i1, int j1) {
    for (int i = 0; i < 30; i++) {
        for (int j = 0; j < 40; j++) {
            // نمایش فقط دو خانه در هر جهت اطراف شخصیت
            if ((i >= i1 - 2 && i <= i1 + 2 && j >= j1 - 2 && j <= j1 + 2) && 
                (map[i][j] == '#' || map[i][j] == '+' || (i == i1 && j == j1))) {
                printw("%c", map[i][j]);
            } 
        //     else {
        //     //     printw(" "); // سایر نقاط مخفی
        //     // }
        }
        printw("\n");
    }
}

int main() {
    char mapk[30][40];
    char map[30][40];
    int i1 = 2, j1 = 10; // موقعیت اولیه در اتاق اول
    int i2 = 15, j2 = 30; // موقعیت انتقال به اتاق دوم
    int currentMap = 1;   // شروع از اتاق اول
    int darrahro = 0;     // نشانگر اینکه شخصیت وارد راهرو شده است

    srand(time(0));

    initscr();
    noecho();
    curs_set(FALSE);

    tarahimap(map); // طراحی نقشه‌ها با راهروها
    map[i1][j1] = '*'; // قرار دادن شخصیت در اتاق اول

    int ch;
    mapk[i1][j1] = '.';
    while ((ch = getch()) != 'q') {
        // پاک کردن موقعیت قبلی
        char prev = mapk[i1][j1];
        map[i1][j1] = prev;

        // منطق حرکت
        if (ch == '2' && (map[i1 + 1][j1] == '.' || map[i1 + 1][j1] == '#' || map[i1 + 1][j1] == '+')) i1++;
        if (ch == '8' && (map[i1 - 1][j1] == '.' || map[i1 - 1][j1] == '#' || map[i1 - 1][j1] == '+')) i1--;
        if (ch == '4' && (map[i1][j1 - 1] == '.' || map[i1][j1 - 1] == '#' || map[i1][j1 - 1] == '+')) j1--;
        if (ch == '6' && (map[i1][j1 + 1] == '.' || map[i1][j1 + 1] == '#' || map[i1][j1 + 1] == '+')) j1++;
        if (ch == '9' && (map[i1 - 1][j1 + 1] == '.' || map[i1 - 1][j1 + 1] == '#' || map[i1 - 1][j1 + 1] == '+')) { i1--; j1++; }
        if (ch == '7' && (map[i1 - 1][j1 - 1] == '.' || map[i1 - 1][j1 - 1] == '#' || map[i1 - 1][j1 - 1] == '+')) { i1--; j1--; }
        if (ch == '1' && (map[i1 + 1][j1 - 1] == '.' || map[i1 + 1][j1 - 1] == '#' || map[i1 + 1][j1 - 1] == '+')) { i1++; j1--; }
        if (ch == '3' && (map[i1 + 1][j1 + 1] == '.' || map[i1 + 1][j1 + 1] == '#' || map[i1 + 1][j1 + 1] == '+')) { i1++; j1++; }

        // بروزرسانی نوع مکانی که شخصیت در آن است
        if (map[i1][j1] == '#') {
            mapk[i1][j1] = '#';
            darrahro = 1; // وارد راهرو شده است
        } else if (map[i1][j1] == '+') {
            mapk[i1][j1] = '+';
            darrahro = 0; // رسیدن به در
        } else {
            mapk[i1][j1] = '.';
        }

        if (i1 == 15 && j1 == 21) {
            currentMap = 2;
        }

        // موقعیت جدید را تنظیم کن
        map[i1][j1] = '*';

        clear();
        if (currentMap == 1) {
            if (darrahro) {
                chapmap1(map);
                chapmap_rahro_limited(map, i1, j1); // چاپ با دید محدود در راهرو
            } else {
                chapmap1(map); // چاپ نقشه کامل اتاق
            }
        }
        if (currentMap == 2) {
            chapmap2(map); // چاپ کل نقشه
        }
    }

    endwin();
    return 0;
}

// if (ch == '8' && (map[i1 - 1][j1] == '.' || map[i1 - 1][j1] == '#' || map[i1 - 1][j1] == '+')) i1--; 
// if (ch == '4' && (map[i1][j1 - 1] == '.' || map[i1][j1 - 1] == '#' || map[i1][j1 - 1] == '+')) j1--; 
// if (ch == '6' && (map[i1][j1 + 1] == '.' || map[i1][j1 + 1] == '#' || map[i1][j1 + 1] == '+')) j1++; 
// if (ch == '9' && (map[i1 - 1][j1 + 1] == '.' || map[i1 - 1][j1 + 1] == '#' || map[i1 - 1][j1 + 1] == '+')) { i1--; j1++; } 
// if (ch == '7' && (map[i1 - 1][j1 - 1] == '.' || map[i1 - 1][j1 - 1] == '#' || map[i1 - 1][j1 - 1] == '+')) { i1--; j1--; } 
// if (ch == '1' && (map[i1 + 1][j1 - 1] == '.' || map[i1 + 1][j1 - 1] == '#' || map[i1 + 1][j1 - 1] == '+')) { i1++; j1--; } 
// if (ch == '3' && (map[i1 + 1][j1 + 1] == '.' || map[i1 + 1][j1 + 1] == '#' || map[i1 + 1][j1 + 1] == '+')) { i1++; j1++; }
