#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAP_ROWS 50
#define MAP_COLS 100
#define MAX_ROOMS 6
#define ROOM_MIN_SIZE 4
#define ROOM_MAX_SIZE 10

typedef struct {
    int row, col, width, height;
} Room;

// بررسی تداخل اتاق‌ها
int has_collision(Room rooms[], int room_count, Room new_room) {
    for (int i = 0; i < room_count; i++) {
        Room r = rooms[i];
        if (!(new_room.row + new_room.height < r.row || 
              new_room.row > r.row + r.height || 
              new_room.col + new_room.width < r.col || 
              new_room.col > r.col + r.width)) {
            return 1; // تداخل وجود دارد
        }
    }
    return 0; // بدون تداخل
}

// قرار دادن علامت '@' در ۲ اتاق به طور تصادفی
void ezafeh_at(Room rooms[], int room_count, char map[MAP_ROWS][MAP_COLS]) {
    if (room_count < 2) return; // اگر تعداد اتاق‌ها کمتر از ۲ باشد، نمی‌توانیم این کار را انجام دهیم

    // انتخاب دو اتاق به طور تصادفی
    int room1_index = rand() % room_count;
    int room2_index = rand() % room_count;

    // اطمینان از اینکه اتاق اول و دوم یکی نباشند
    while (room2_index == room1_index) {
        room2_index = rand() % room_count;
    }

    // انتخاب نقطه‌ای تصادفی در هر اتاق
    Room room1 = rooms[room1_index];
    Room room2 = rooms[room2_index];

    // قرار دادن '@' در اتاق اول
    int x1 = rand() % (room1.height - 2) + room1.row + 1; // از مرزهای دیوار فاصله بگیریم
    int y1 = rand() % (room1.width - 2) + room1.col + 1; // از مرزهای دیوار فاصله بگیریم
    map[x1][y1] = '@';

    // قرار دادن '@' در اتاق دوم
    int x2 = rand() % (room2.height - 2) + room2.row + 1; // از مرزهای دیوار فاصله بگیریم
    int y2 = rand() % (room2.width - 2) + room2.col + 1; // از مرزهای دیوار فاصله بگیریم
    map[x2][y2] = '@';
}

// قرار دادن علامت '&' در اتاقی که بازیکن در آن قرار ندارد
void ezafeh_and(Room rooms[], int room_count, char map[MAP_ROWS][MAP_COLS], int playerRow, int playerCol) {
    if (room_count < 2) return; // اگر تعداد اتاق‌ها کمتر از ۲ باشد، نمی‌توانیم این کار را انجام دهیم

    int room1_index;

    // جستجوی اتاقی که بازیکن در آن قرار ندارد
    do {
        room1_index = rand() % room_count;
    } while (playerRow >= rooms[room1_index].row && playerRow < rooms[room1_index].row + rooms[room1_index].height &&
             playerCol >= rooms[room1_index].col && playerCol < rooms[room1_index].col + rooms[room1_index].width);

    // انتخاب نقطه‌ای تصادفی در هر اتاق
    Room room1 = rooms[room1_index];

    // قرار دادن '&' در اتاق انتخاب شده
    int x1 = rand() % (room1.height - 2) + room1.row + 1; // از مرزهای دیوار فاصله بگیریم
    int y1 = rand() % (room1.width - 2) + room1.col + 1; // از مرزهای دیوار فاصله بگیریم
    map[x1][y1] = '&';
}

// ایجاد نقشه جدید
void jadid_map(char map[MAP_ROWS][MAP_COLS], Room rooms[], int *room_count);
void check_and_move(int *playerRow, int *playerCol, char map[MAP_ROWS][MAP_COLS], Room rooms[], int *room_count);
void rahro(int i1, int j1, int i2, int j2, char map[MAP_ROWS][MAP_COLS]);
void ezafeh_dar(char map[MAP_ROWS][MAP_COLS]);
void tarahimap(char map[MAP_ROWS][MAP_COLS], Room rooms[], int *room_count);
void chapmap(char map[MAP_ROWS][MAP_COLS], int playerRow, int playerCol);
// ایجاد نقشه جدید
void jadid_map(char map[MAP_ROWS][MAP_COLS], Room rooms[], int *room_count) {
    tarahimap(map, rooms, room_count);
    ezafeh_dar(map);
    ezafeh_at(rooms, *room_count, map);
    ezafeh_and(rooms, *room_count, map, -1, -1); // '-1' to ignore player position
}

// بررسی و انتقال بازیکن
void check_and_move(int *playerRow, int *playerCol, char map[MAP_ROWS][MAP_COLS], Room rooms[], int *room_count) {
    if (map[*playerRow][*playerCol] == '&') {
        jadid_map(map, rooms, room_count);
        *playerRow = rooms[0].row + rooms[0].height / 2;
        *playerCol = rooms[0].col + rooms[0].width / 2;
    }
}

// ایجاد راهرو با اضافه کردن در
void rahro(int i1, int j1, int i2, int j2, char map[MAP_ROWS][MAP_COLS]) {
    int start_i = i1, start_j = j1;


    // حرکت تا زمانی که به نقطه پایانی برسیم
    while (i1 != i2 || j1 != j2) {
        if (map[i1][j1] == ' ') {
            map[i1][j1] = '#'; // ایجاد راهرو
        }

        // حرکت به سمت هدف
        if (rand() % 2) {
            i1 += (i1 < i2) ? 1 : (i1 > i2) ? -1 : 0;
        } else {
            j1 += (j1 < j2) ? 1 : (j1 > j2) ? -1 : 0;
        }
    }
}

// اضافه کردن در
void ezafeh_dar(char map[MAP_ROWS][MAP_COLS]) {
    for (int i = 1; i < MAP_ROWS - 1; i++) {
        for (int j = 1; j < MAP_COLS - 1; j++) {
            // بررسی اینکه آیا نقطه مورد نظر دیوار است
            if (map[i][j] == '-' || map[i][j] == '|') {
                // بررسی اتصال به راهرو
                if ((map[i - 1][j] == '#') || // بالا
                    (map[i + 1][j] == '#') || // پایین
                    (map[i][j - 1] == '#') || // چپ
                    (map[i][j + 1] == '#')) { // راست
                    map[i][j] = '+'; // تبدیل دیوار به در
                }
            }
        }
    }
}

// طراحی نقشه
void tarahimap(char map[MAP_ROWS][MAP_COLS], Room rooms[], int *room_count) {
    // مقداردهی اولیه نقشه
    for (int i = 0; i < MAP_ROWS; i++) {
        for (int j = 0; j < MAP_COLS; j++) {
            map[i][j] = ' ';
        }
    }

    *room_count = 0;

    // تولید اتاق‌ها
    while (*room_count < MAX_ROOMS) {
        int width = ROOM_MIN_SIZE + rand() % (ROOM_MAX_SIZE - ROOM_MIN_SIZE + 1);
        int height = ROOM_MIN_SIZE + rand() % (ROOM_MAX_SIZE - ROOM_MIN_SIZE + 1);
        int row = rand() % (MAP_ROWS - height - 1);
        int col = rand() % (MAP_COLS - width - 1);

        Room new_room = {row, col, width, height};

        // بررسی تداخل اتاق‌ها
        if (!has_collision(rooms, *room_count, new_room)) {
            rooms[(*room_count)++] = new_room;

            // طراحی اتاق
            for (int i = row; i < row + height; i++) {
                for (int j = col; j < col + width; j++) {
                    if (i == row || i == row + height - 1) {
                        map[i][j] = '-'; // دیوارهای بالا و پایین
                    } else if (j == col || j == col + width - 1) {
                        map[i][j] = '|'; // دیوارهای کناری
                    } else {
                        map[i][j] = '.'; // فضای داخلی
                    }
                }
            }
        }
    }

    // ایجاد راهروها
    for (int i = 0; i < *room_count - 1; i++) {
        int x1 = rooms[i].row + rooms[i].height / 2;
        int y1 = rooms[i].col + rooms[i].width / 2;
        int x2 = rooms[i + 1].row + rooms[i + 1].height / 2;
        int y2 = rooms[i + 1].col + rooms[i + 1].width / 2;
        rahro(x1, y1, x2, y2, map);
    }
}

// چاپ نقشه
void chapmap(char map[MAP_ROWS][MAP_COLS], int playerRow, int playerCol) {
    for (int i = 0; i < MAP_ROWS; i++) {
        for (int j = 0; j < MAP_COLS; j++) {
            if (i == playerRow && j == playerCol) {
                printw("*"); // نمایش بازیکن
            } else {
                printw("%c", map[i][j]);
            }
        }
        printw("\n");
    }
}

int main() {
    FILE *gold;
    int sum = 0;
    
    char map[MAP_ROWS][MAP_COLS];
    char mapk[MAP_ROWS][MAP_COLS];
    Room rooms[MAX_ROOMS];
    int room_count;

    srand(time(0));

    // موقعیت اولیه بازیکن
    int playerRow = 1, playerCol = 1;

    initscr();
    noecho();
    curs_set(FALSE);

    tarahimap(map, rooms, &room_count);
    ezafeh_dar(map);
    ezafeh_at(rooms, room_count, map);
    ezafeh_and(rooms, room_count, map, playerRow, playerCol);

    // تنظیم موقعیت اولیه بازیکن به مرکز اولین اتاق
    playerRow = rooms[0].row + rooms[0].height / 2;
    playerCol = rooms[0].col + rooms[0].width / 2;


    int ch;
    mapk[playerRow][playerCol] = '.';
    while ((ch = getch()) != 'q') {
        // پاک کردن جای قبلی بازیکن
        char prev = mapk[playerRow][playerCol];
        map[playerRow][playerCol] = prev;
        
        // حرکت بازیکن
        if (ch == '2' && map[playerRow + 1][playerCol] != ' ' && map[playerRow + 1][playerCol] != '|' && map[playerRow + 1][playerCol] != '-') playerRow++;
        if (ch == '8' && map[playerRow - 1][playerCol] != ' ' && map[playerRow - 1][playerCol] != '|' && map[playerRow - 1][playerCol] != '-') playerRow--;
        if (ch == '4' && map[playerRow][playerCol - 1] != ' ' && map[playerRow][playerCol - 1] != '|' && map[playerRow][playerCol - 1] != '-') playerCol--;
        if (ch == '6' && map[playerRow][playerCol + 1] != ' ' && map[playerRow][playerCol + 1] != '|' && map[playerRow][playerCol + 1] != '-') playerCol++;
        if (ch == '3' && map[playerRow + 1][playerCol + 1] != ' ' && map[playerRow + 1][playerCol + 1] != '|' && map[playerRow + 1][playerCol + 1] != '-') { playerRow++; playerCol++; }
        if (ch == '1' && map[playerRow + 1][playerCol - 1] != ' ' && map[playerRow + 1][playerCol - 1] != '|' && map[playerRow + 1][playerCol - 1] != '-') { playerRow++; playerCol--; }
        if (ch == '7' && map[playerRow - 1][playerCol - 1] != ' ' && map[playerRow - 1][playerCol - 1] != '|' && map[playerRow - 1][playerCol - 1] != '-') { playerRow--; playerCol--; }
        if (ch == '9' && map[playerRow - 1][playerCol + 1] != ' ' && map[playerRow - 1][playerCol + 1] != '|' && map[playerRow - 1][playerCol + 1] != '-') { playerRow--; playerCol++; }
        
        // بررسی و انتقال بازیکن
        check_and_move(&playerRow, &playerCol, map, rooms, &room_count);

        if (map[playerRow][playerCol] == '#') {
            mapk[playerRow][playerCol] = '#';
        } else if (map[playerRow][playerCol] == '+') {
            mapk[playerRow][playerCol] = '+';
        } else if (map[playerRow][playerCol] == '@') {
            move(0, 0);
            gold = fopen("g", "r");
            if (gold != NULL) {
                fscanf(gold, "%d", &sum);
                fclose(gold);
            }
            int a = rand() % 20;
            sum += a + 20;
            gold = fopen("g", "w");
            fprintf(gold, "%d", sum);
            fclose(gold);
            printw("You have %d gold\n", sum);
            mapk[playerRow][playerCol] = '.';
            getch();
        } else {
            mapk[playerRow][playerCol] = '.';
        }

        // چاپ نقشه
        clear();
        chapmap(map, playerRow, playerCol);
    }

    endwin();
    return 0;
}
