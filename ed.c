#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>


#define MAP_ROWS 50
#define MAP_COLS 100
#define MAX_ROOMS 6
#define ROOM_MIN_SIZE 4
#define ROOM_MAX_SIZE 10
#define MAX_USERS 100
#define USERS_FILE "users_data.txt"
struct Telesm
{
    int Health;
    int speed;
    int damage;
};
struct nre {
    char name[100];
    char r[100];
    char e[100];
    int g;
    int he;
    int hu;
    int rang;
    struct Telesm telesm;
};

struct nre homes[MAX_USERS];
int n = 0;

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
void ezafehgh(Room rooms[], int room_count, char map[MAP_ROWS][MAP_COLS]) {
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

    // قرار دادن '!' در اتاق اول
    int x1 = rand() % (room1.height - 2) + room1.row + 1; // از مرزهای دیوار فاصله بگیریم
    int y1 = rand() % (room1.width - 2) + room1.col + 1; // از مرزهای دیوار فاصله بگیریم
    map[x1][y1] = '!';

    // قرار دادن '!' در اتاق دوم
    int x2 = rand() % (room2.height - 2) + room2.row + 1; // از مرزهای دیوار فاصله بگیریم
    int y2 = rand() % (room2.width - 2) + room2.col + 1; // از مرزهای دیوار فاصله بگیریم
    map[x2][y2] = '!';
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
        // اضافه کردن در در شروع و پایان راهرو
    
    }

    
}

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




typedef struct {
    char username[50];
    char password[50];
} User;

// متغیرهای سراسری
User users[100];
int user_count = 0;
int logged_in = 0;
char current_user[50];

// پیمایش و انتخاب منو
int navigate_menu(char *choices[], int num_choices) {
    int highlight = 0; // گزینه هایلایت شده
    int choice = -1;   // گزینه انتخاب شده
    int ch;

    while (1) {
        clear();
        for (int i = 0; i < num_choices; i++) {
            if (i == highlight) {
                attron(A_REVERSE); // معکوس کردن رنگ
                mvprintw(i + 1, 1, "%s", choices[i]);
                attroff(A_REVERSE);
            } else {
                mvprintw(i + 1, 1, "%s", choices[i]);
            }
        }
        refresh();

        ch = getch();
        switch (ch) {
            case KEY_UP:
                highlight = (highlight == 0) ? num_choices - 1 : highlight - 1;
                break;
            case KEY_DOWN:
                highlight = (highlight == num_choices - 1) ? 0 : highlight + 1;
                break;
            case '\n': // Enter key
                choice = highlight;
                return choice;
        }
    }
}

// بارگذاری کاربران از فایل
// void load_users() {
//     FILE *file = fopen("users.txt", "r");
//     if (!file) return;

//     while (fscanf(file, "%s %s", users[user_count].username, users[user_count].password) != EOF) {
//         user_count++;
//     }
//     fclose(file);
// }

// // ذخیره‌سازی کاربر جدید
// void save_user(User user) {
//     FILE *file = fopen("users.txt", "a");
//     if (file) {
//         fprintf(file, "%s %s\n", user.username, user.password);
//         fclose(file);
//     }
// }

// بررسی وجود نام کاربری
// Check if the username already exists
int nameExists(const char *name) {
    for (int i = 0; i < n; i++) {
        if (strcmp(homes[i].name, name) == 0) {
            return 1; 
        }
    }
    return 0; 
}
#include <ncurses.h>

enum Colors {
    RED = 1,
    GREEN,
    BLUE,
    YELLOW
};

void initColors() {
    start_color();
    init_pair(RED, COLOR_RED, COLOR_BLACK);
    init_pair(GREEN, COLOR_GREEN, COLOR_BLACK);
    init_pair(BLUE, COLOR_BLUE, COLOR_BLACK);
    init_pair(YELLOW, COLOR_YELLOW, COLOR_BLACK);
}

// int main() {
//     initscr();
    

//     attron(COLOR_PAIR(RED));
//     printw("Hello in Red!");
//     attroff(COLOR_PAIR(RED));

//     refresh();
//     getch();
//     endwin();

//     return 0;
// }

// احراز هویت کاربر
int authenticate(char *username, char *password) {
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            return 1;
        }
    }
    return 0;
}

// تولید نقشه اولیه
void generate_map(char map[MAP_ROWS][MAP_COLS], Room rooms[], int *room_count) {
    for (int i = 0; i < MAP_ROWS; i++) {
        for (int j = 0; j < MAP_COLS; j++) {
            map[i][j] = ' ';
        }
    }

    *room_count = 0;
    srand(time(0));

    while (*room_count < MAX_ROOMS) {
        int width = ROOM_MIN_SIZE + rand() % (ROOM_MAX_SIZE - ROOM_MIN_SIZE + 1);
        int height = ROOM_MIN_SIZE + rand() % (ROOM_MAX_SIZE - ROOM_MIN_SIZE + 1);
        int row = rand() % (MAP_ROWS - height - 1);
        int col = rand() % (MAP_COLS - width - 1);

        Room new_room = {row, col, width, height};
        int valid = 1;

        for (int i = 0; i < *room_count; i++) {
            Room r = rooms[i];
            if (!(new_room.row + new_room.height < r.row || 
                  new_room.row > r.row + r.height || 
                  new_room.col + new_room.width < r.col || 
                  new_room.col > r.col + r.width)) {
                valid = 0;
                break;
            }
        }

        if (valid) {
            rooms[*room_count] = new_room;
            (*room_count)++;

            for (int i = row; i < row + height; i++) {
                for (int j = col; j < col + width; j++) {
                    if (i == row || i == row + height - 1 || j == col || j == col + width - 1) {
                        map[i][j] = '#'; // دیوار
                    } else {
                        map[i][j] = '.'; // فضای داخلی
                    }
                }
            }
        }
    }
}

// نمایش نقشه
void chapmap(char map[MAP_ROWS][MAP_COLS], int playerRow, int playerCol, int rr) {
    for (int i = 0; i < MAP_ROWS; i++) {
        for (int j = 0; j < MAP_COLS; j++) {
            if (i == playerRow && j == playerCol) {
                initColors();
                attron(COLOR_PAIR(rr));
              printw("*"); // نمایش بازیکن
            attroff(COLOR_PAIR(rr));
            } else {
                printw("%c", map[i][j]);
            }
        }
        printw("\n");
    }
}
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
    start_color();
    for (int i = 0; i<n; i++)
    {
        if (i==0)
        {
            int pink = 8;
    init_color(pink, 1000, 752, 796); // RGB values scaled to 1000
    init_pair(1, pink, COLOR_BLACK);
            attron(A_BOLD|COLOR_PAIR(1));
            printw("%s: %d(legend)\n", sorted[0].name, sorted[0].g);
            attroff(A_BOLD|COLOR_PAIR(1));
        } else if(i==1){
            init_pair(2,COLOR_BLUE,COLOR_BLACK);
            attron(A_BOLD|COLOR_PAIR(2));
            printw("%s: %d\n", sorted[1].name, sorted[1].g);
            attroff(A_BOLD|COLOR_PAIR(2));
        }else if(i==2){
            init_pair(3,COLOR_GREEN,COLOR_BLACK);
            attron(A_BOLD|COLOR_PAIR(3));
            printw("%s: %d\n", sorted[2].name, sorted[2].g);
            attroff(A_BOLD|COLOR_PAIR(3));
        } else{
         printw("%s: %d\n", sorted[i].name, sorted[i].g);
        }
        
    }
    
            
            
            
    
    // for (int i = 3; i < n; i++) {
    //     printw("%s: %d\n", sorted[i].name, sorted[i].g);
    // }
    // for (int i = 0; i < n; i++) {
    //     printw("%s: %d\n", sorted[i].name, sorted[i].g);
    // }
    getch();
}
// Login function
// int login(FILE *file , char *cplayer , int n) {
//     char username[100], password[100];
//     char buffer[100];

//     rewind(file); // Move to the beginning of the file
//     printw("Enter username: ");
//     echo();
//     getstr(username);

//     printw("Enter password: ");
//     getstr(password);
//     for (int i = 0; i < n; i++)
//     {
//         /* code */
//     }
    

//     while (fgets(buffer, sizeof(buffer), file)) {
//         buffer[strcspn(buffer, "\n")] = 0; // Remove newline character
//         if (strcmp(buffer, username) == 0) {
//             printw("Login successful for %s!\n", username);
//             strcpy(cplayer , username);
//             getch();
//             refresh();
//             return 1; // Login success
//         }
//     }
//     printw("Error: Invalid username or password.\n");
//     refresh();
//     return 0; // Login failed
// }
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
            *m=i;
            printw("Login successful for %s!\n", homes[*m].name);
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
            fprintf(file, "%s %s %s %d %d %d %d %d %d %d\n", homes[i].name, homes[i].r, homes[i].e, homes[i].g, homes[i].he, homes[i].hu ,homes[i].rang,homes[i].telesm.Health,homes[i].telesm.damage,homes[i].telesm.speed);
        }
        fclose(file);
    }
}

// Function to load users from file
void loadUsers() {
    FILE *file = fopen(USERS_FILE, "r");
    if (file) {
        while (fscanf(file, "%s %s %s %d %d %d %d %d %d %d", homes[n].name, homes[n].r, homes[n].e, &homes[n].g, &homes[n].he, &homes[n].hu, &homes[n].rang ,&homes[n].telesm.Health,&homes[n].telesm.damage,&homes[n].telesm.speed) == 10) {
            n++;
        }
        fclose(file);
    }
}

int main() {

    int m =0;
    char cplayer[1000];
    loadUsers();

    initscr();               

    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE);
    int logged_in = 0;
    FILE *file;
    char name[100];

    file = fopen("filename.txt", "a+");
    if (file == NULL) {
        printw("File error\n");
        endwin();
        return 1;
    }

    while (1) {
        if (!logged_in) {
            char *login_choices[] = {"Signup", "Login", "Exit"};
            int login_choice = navigate_menu(login_choices, 3);

            if (login_choice == 0) { // ثبت‌نام
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
                    homes[n].he = 20;
                    homes[n].hu = 20;
                    homes[n].rang = 1;
                    homes[n].telesm.Health = 0;
                    homes[n].telesm.damage = 0;
                    homes[n].telesm.speed = 0;
                    n++;
                    printw("Signup successful %s\n", name);
                    getch();
                } else {
                    printw("Username already exists\n");
                }
                
            } else if (login_choice == 1) { // ورود
             printw("%d\n", n);
             getch();
             
                logged_in = login(&m);
                             printw("%d\n", m);

                printw("Login successful for %s!\n", homes[m].name);
                getch();

                // if (authenticate(username, password)) {
                //     logged_in = 1;
                //     strcpy(current_user, username);
                // } 
                // else {
                //     printw("Invalid username or password.\n");
                //     getch();
                // }
            } else if (login_choice == 2) { // خروج
                break;
            }
        } else {
            char *main_choices[] = {"New Game", "Load Game", "Logout" , "Scoreboard","Settings"};
            int main_choice = navigate_menu(main_choices, 5);

            if (main_choice == 0) { // بازی جدید
                FILE *gold;
    int sum = 0;
    int har = 0;
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
    ezafehgh(rooms, room_count, map);


    // تنظیم موقعیت اولیه بازیکن به مرکز اولین اتاق
    playerRow = rooms[0].row + rooms[0].height / 2;
    playerCol = rooms[0].col + rooms[0].width / 2;

    int ch;
    mapk[playerRow][playerCol]= '.';
                 while ((ch = getch()) != 'q' ) {
        // پاک کردن جای قبلی بازیکن
        char prev = mapk[playerRow][playerCol];
              map[playerRow][playerCol] = prev;
        // حرکت بازیکن
        if (ch == '2' && map[playerRow + 1][playerCol] != ' ' && map[playerRow + 1][playerCol] != '|' && map[playerRow + 1][playerCol] != '-') playerRow++;
        if (ch == '8' && map[playerRow - 1][playerCol] != ' ' && map[playerRow - 1][playerCol] != '|' && map[playerRow - 1][playerCol] != '-') playerRow--;
        if (ch == '4' && map[playerRow][playerCol - 1] != ' ' && map[playerRow][playerCol - 1] != '|' && map[playerRow][playerCol - 1] != '-') playerCol--;
        if (ch == '6' && map[playerRow][playerCol + 1] != ' ' && map[playerRow][playerCol + 1] != '|' && map[playerRow][playerCol + 1] != '-') playerCol++;
        if (ch == '3' && map[playerRow + 1][playerCol+1] != ' ' && map[playerRow + 1][playerCol+1] != '|' && map[playerRow + 1][playerCol+1] != '-') {playerRow++;
         playerCol++;}
         if (ch == '1' && map[playerRow + 1][playerCol-1] != ' ' && map[playerRow + 1][playerCol-1] != '|' && map[playerRow + 1][playerCol-1] != '-') {playerRow++;
         playerCol--;}
         if (ch == '7' && map[playerRow - 1][playerCol-1] != ' ' && map[playerRow - 1][playerCol-1] != '|' && map[playerRow - 1][playerCol-1] != '-') {playerRow--;
         playerCol--;}
         if (ch == '9' && map[playerRow - 1][playerCol+1] != ' ' && map[playerRow - 1][playerCol+1] != '|' && map[playerRow - 1][playerCol+1] != '-') {playerRow--;
         playerCol++;}
         if(map[playerRow][playerCol] == '#'){
                    mapk[playerRow][playerCol] = '#';
                } else if(map[playerRow][playerCol] == '+'){
                    mapk[playerRow][playerCol] = '+';
                } else if (map[playerRow][playerCol]  == '@') {
            move(0,0);
    //         gold = fopen("g", "r");
    // if (gold != NULL) {
    //     fscanf(gold, "%d", &sum);
    //     fclose(gold);
    // }
    // int a;
    // a= rand() %20;
    // sum += a+20;

    // gold = fopen("g", "w");
    // fprintf(gold, "%d", sum);
    // fclose(gold);
    int g = homes[m].g;
    int a;
    a= rand() %20;
    g+=a+20;
        homes[m].g=g;

            printw("You have %d gold , %d\n",g , a);
            homes[m].telesm.Health = 1;
            har=0;
            mapk[playerRow][playerCol] = '.';
            // refresh();
            getch();
            
        }else if (map[playerRow][playerCol]  == '!') {
            if (homes[m].hu<=15)
                    {
                        homes[m].hu+=5;
                    } else{
                        int ghh=20-homes[m].hu;
                      homes[m].hu+=ghh;
                    }
                    mapk[playerRow][playerCol] = '.';
            
        }else{
                    mapk[playerRow][playerCol] = '.';
                }
        har++;
        // چاپ نقشه
        clear();
        move(0,25);
        if(har %10 == 0){
            if(homes[m].hu!=0){
               homes[m].hu--;
            }
            
        }
        if (homes[m].telesm.Health ==1)
        {
            if(har%20==0){
             homes[m].telesm.Health =0;
            }
            else if(homes[m].he<20 && har %3 == 0){
                homes[m].he++;
            }
            
        }
        
        if(homes[m].hu==20 && har %3 == 0){
            if(homes[m].he<20){
                homes[m].he++;
            }
            
        }
        if(homes[m].hu==0 && har %10 == 0){
            homes[m].he--;
        }
        if(homes[m].he==0){
            homes[m].hu =20;
            homes[m].he = 20;
            break;
        }
        printw("health = %d         " , homes[m].he);
        printw("Hunger = %d\n" , homes[m].hu);
        int rr;
        rr = homes[m].rang;
        // int rr=homes[m].rang;
        chapmap(map, playerRow, playerCol, rr);

    }
    homes[m].hu =20;
    homes[m].he = 20;
    homes[m].telesm.Health =0;
            } else if (main_choice == 1) {
                printw("Load game is not implemented yet.\n");
                getch();
            } else if (main_choice == 2) {
                                             printw("%d\n", m);
getch();
                printw("Login successful for %s!\n", homes[m].name);
                getch();
                logged_in = 0;
            } else if (main_choice == 3) {
                displayScoreboard();
                getch();
            } else if (main_choice == 4) {
                char *choices[] = {"Color *"};
            int choice = navigate_menu(choices, 1);
            if (choice == 0){
                char *choicesha[] = {"Color Blue", "Color Green" ,"Color Red" , "Color Yellow"};
            int choiceh = navigate_menu(choicesha, 4);
            if (choiceh==0)
            {
                homes[m].rang = 3;
                getch();
            } if (choiceh==1)
            {
                homes[m].rang = 2;
                getch();
            }
            if (choiceh==2)
            {
                homes[m].rang = 1;
                getch();
            }
            if (choiceh==3)
            {
                homes[m].rang = 4;
                getch();
            }
            
                getch();
            }
                getch();
            } 
        }
    }
    saveUsers();
    endwin();
    return 0;
}