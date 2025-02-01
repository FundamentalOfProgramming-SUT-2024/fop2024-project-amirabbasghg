#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <unistd.h> 
#include <pthread.h>




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
struct harekat
{
    char pr;
    int i;
    int j;
};
// struct tirs
// {
//     int t;
// };

struct guns
{
    int mace;
    int dagger;
    int magicwand;
    int normallarrow;
    int sword;
    int normallarrowcount[MAP_ROWS][MAP_COLS];
    int daggercount[MAP_ROWS][MAP_COLS];
    int magicwandcount[MAP_ROWS][MAP_COLS];
    // struct tirs tir;
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
    struct guns gun;
    struct harekat hdeamon;
    struct harekat hfire;
    struct harekat hgiant;
    struct harekat hsnake;
    struct harekat hundead; 
    int music;
};
int music_playing = 0;
pthread_t music_thread;



struct harekat harek[1000];

void* play_music(void* arg) {
    char* filepath = (char*) arg;
    char command[300];
    while (music_playing) {
        snprintf(command, sizeof(command), "mpg123 '%s' > /dev/null 2>&1", filepath);
        system(command);
    }
    pthread_exit(NULL);
}

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

void ezafehez(Room rooms[], int room_count, char map[MAP_ROWS][MAP_COLS] , char ez) {
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
    map[x1][y1] = ez;

    // قرار دادن '!' در اتاق دوم
    int x2 = rand() % (room2.height - 2) + room2.row + 1; // از مرزهای دیوار فاصله بگیریم
    int y2 = rand() % (room2.width - 2) + room2.col + 1; // از مرزهای دیوار فاصله بگیریم
    map[x2][y2] = ez;
}
void ezafeh1enemi(Room rooms[], int room_count, char map[MAP_ROWS][MAP_COLS] , char ez , int m) {
    if (room_count < 2) return; // اگر تعداد اتاق‌ها کمتر از ۲ باشد، نمی‌توانیم این کار را انجام دهیم

    // انتخاب دو اتاق به طور تصادفی
    int room1_index = rand() % room_count;

    // انتخاب نقطه‌ای تصادفی در هر اتاق
    Room room1 = rooms[room1_index];

    // قرار دادن '!' در اتاق اول
    int x1 = rand() % (room1.height - 2) + room1.row + 1; // از مرزهای دیوار فاصله بگیریم
    int y1 = rand() % (room1.width - 2) + room1.col + 1; // از مرزهای دیوار فاصله بگیریم
    map[x1][y1] = ez;
    if (ez=='D')
    {
        homes[m].hdeamon.i = x1;
    homes[m].hdeamon.j = y1;
    homes[m].hdeamon.pr = ez;
    }
    if (ez=='F')
    {
        homes[m].hfire.i = x1;
    homes[m].hfire.j = y1;
    homes[m].hfire.pr = ez;
    }
    if (ez=='G')
    {
        homes[m].hgiant.i = x1;
    homes[m].hgiant.j = y1;
    homes[m].hgiant.pr = ez;
    }
    if (ez=='S')
    {
        homes[m].hsnake.i = x1;
    homes[m].hsnake.j = y1;
    homes[m].hsnake.pr = ez;
    }
    if (ez=='U')
    {
        homes[m].hundead.i = x1;
    homes[m].hundead.j = y1;
    homes[m].hundead.pr = ez;
    }
}
void ezafeh1gun(Room rooms[], int room_count, char map[MAP_ROWS][MAP_COLS] , char ez) {
    if (room_count < 2) return; // اگر تعداد اتاق‌ها کمتر از ۲ باشد، نمی‌توانیم این کار را انجام دهیم

    // انتخاب دو اتاق به طور تصادفی
    int room1_index = rand() % room_count;

    // انتخاب نقطه‌ای تصادفی در هر اتاق
    Room room1 = rooms[room1_index];

    // قرار دادن '!' در اتاق اول
    int x1 = rand() % (room1.height - 2) + room1.row + 1; // از مرزهای دیوار فاصله بگیریم
    int y1 = rand() % (room1.width - 2) + room1.col + 1; // از مرزهای دیوار فاصله بگیریم
    map[x1][y1] = ez;
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
                    homes[n].gun.mace = 0;
                    homes[n].gun.dagger = 0;
                    homes[n].gun.magicwand = 0;
                    homes[n].gun.sword = 0;
                    homes[n].gun.normallarrow = 0;
                    homes[n].music = 0;
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
            char* filepath = "/home/amirabbas/Hans-Zimmer-S.T.A.Y.mp3";
            char* url = "http://www.coca.ir/wp-content/uploads/2018/04/Hans-Zimmer-S.T.A.Y.mp3";
            char command[300];

            // if (homes[m].music == 1 && !music_playing) {
            //     if (access(filepath, F_OK) != 0) {
            //         snprintf(command, sizeof(command), "wget -O '%s' '%s'", filepath, url);
            //         system(command);
            //     }

            //     music_playing = 1;
            //     pthread_create(&music_thread, NULL, play_music, (void*) filepath);
            // }  else if (homes[m].music == 0 && music_playing) {
            //     music_playing = 0;
            //     pthread_join(music_thread, NULL); // انتظار برای توقف نخ پخش موسیقی
            // }

            
            
            char *main_choices[] = {"New Game", "Load Game", "Logout" , "Scoreboard","Settings"};
            int main_choice = navigate_menu(main_choices, 5);

            if (main_choice == 0) { // بازی جدید
                FILE *gold;
    int sum = 0;
    int har = 0;
    int tirna =20;
    int tirda =10;
    int tirmw =8;
    int d = 0;
    int f = 0;
    int g = 0;
    int s = 0;
    int u = 0;
    int htel = 0;
    int stel = 0;
    int dtel= 0;
    int sw=0;
    int nw =0;
    int dg = 0;
    int mw =0;
    int hed=5;
    int hef= 10;
    int heg= 15;
    int hes= 20;
    int heu= 30;
    char map[MAP_ROWS][MAP_COLS];
    char mapk[MAP_ROWS][MAP_COLS];
    char mapk2[MAP_ROWS][MAP_COLS];
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
    char at = '@';
    ezafehez(rooms, room_count, map , at);
    char gh = '!';
    ezafehez(rooms, room_count, map , gh);
    char he = 'H';
    ezafehez(rooms, room_count, map , he);
    char sp = 'P';
    ezafehez(rooms, room_count, map , he);
    char dt = 'T';
    ezafehez(rooms, room_count, map ,dt );
    char da = 'D';
    ezafeh1enemi(rooms, room_count, map ,da , m);
    char w = 'W';
    ezafeh1gun(rooms, room_count, map ,w);
    char n = 'N';
    ezafeh1gun(rooms, room_count, map ,n);
    char Dg = 'd';
    ezafeh1gun(rooms, room_count, map ,Dg);
    char fir = 'F';
    ezafeh1enemi(rooms, room_count, map ,fir,m); 
    char gia = 'G';
    ezafeh1enemi(rooms, room_count, map ,gia,m);
    char sna = 'S';
    ezafeh1enemi(rooms, room_count, map ,sna,m); 
     char und = 'U';
    ezafeh1enemi(rooms, room_count, map ,und,m); 


    // تنظیم موقعیت اولیه بازیکن به مرکز اولین اتاق
    playerRow = rooms[0].row + rooms[0].height / 2;
    playerCol = rooms[0].col + rooms[0].width / 2;

    int ch;
    mapk[playerRow][playerCol]= '.';
    int i3 , j3;
                 while ((ch = getch()) != 'q' ) {
                    if (hed<=0)
            {
                i3 = harek[har-1].i ;
        j3 = harek[har-1].j ;
        map[i3][j3] = harek[har-1].pr;
        map[homes[m].hdeamon.i][homes[m].hdeamon.j] = '.';
        d=0;
            }
               if (hef<=0)
            {
                i3 = harek[har-1].i ;
        j3 = harek[har-1].j ;
        map[i3][j3] = harek[har-1].pr;
        map[homes[m].hfire.i][homes[m].hfire.j] = '.';
        f=0;
            }
               if (heg<=0)
            {
                i3 = harek[har-1].i ;
        j3 = harek[har-1].j ;
        map[i3][j3] = harek[har-1].pr;
        map[homes[m].hgiant.i][homes[m].hgiant.j] = '.';
        g=0;
            }
               if (hes<=0)
            {
                i3 = harek[har-1].i ;
        j3 = harek[har-1].j ;
        map[i3][j3] = harek[har-1].pr;
        map[homes[m].hsnake.i][homes[m].hsnake.j] = '.';
        s=0;
            }
            if (heu<=0)
            {
                i3 = harek[har-1].i ;
        j3 = harek[har-1].j ;
        map[i3][j3] = harek[har-1].pr;
        map[homes[m].hundead.i][homes[m].hundead.j] = '.';
        u=0;
            }
           
             if(d==1 && har%10==0){
            d= 0;
        }
        if(f==1 && har%10==0){
            f= 0;
        }
        if(g==1 && har%50==0){
            g= 0;
        }
        if(u==1 && har%50==0){
            u= 0;
        }
        // پاک کردن جای قبلی بازیکن
        if((d==1||g==1||s==1||u==1||f==1) && har > 0){
        i3 = harek[har-1].i ;
        j3 = harek[har-1].j ;
        map[i3][j3] = harek[har-1].pr;
        }
        char prev = mapk[playerRow][playerCol];
              map[playerRow][playerCol] = prev;
              char pre;
              if (d==1 || f==1 || g ==1 || s==1 || u==1)
              {
                pre = mapk2[playerRow][playerCol];
              map[playerRow][playerCol] = prev;
              }
              
              
              
        // حرکت بازیکن
        if (ch == '2' && map[playerRow + 1][playerCol] != ' ' && map[playerRow + 1][playerCol] != '|' && map[playerRow + 1][playerCol] != '-') if (homes[m].telesm.speed >=1)
        {
            playerRow++;
            if (map[playerRow][playerCol]  == '@') {
            move(0,0);
    int g = homes[m].g;
    int a;
    a= rand() %20;
    g+=a+20;
        homes[m].g=g;

            printw("You have %d gold , %d\n",g , a);
            mapk[playerRow][playerCol] = '.';
            map[playerRow][playerCol] = '.';
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
                    map[playerRow][playerCol] = '.';
            
        }else if (map[playerRow][playerCol]  == 'H') {
            homes[m].telesm.Health += 1;
            har=0;
                    mapk[playerRow][playerCol] = '.';
                    map[playerRow][playerCol] = '.';
            
        }else if (map[playerRow][playerCol]  == 'P') {
            homes[m].telesm.speed += 1;
            har=0;
                    mapk[playerRow][playerCol] = '.';
                    map[playerRow][playerCol] = '.';
            
        }
            if(map[playerRow + 1][playerCol] != ' ' && map[playerRow + 1][playerCol] != '|' && map[playerRow + 1][playerCol] != '-') playerRow++;
        }
        else{
          playerRow++;
        }
        if (ch == '8' && map[playerRow - 1][playerCol] != ' ' && map[playerRow - 1][playerCol] != '|' && map[playerRow - 1][playerCol] != '-') if (homes[m].telesm.speed >=1)
        {
            playerRow--;
            if (map[playerRow][playerCol]  == '@') {
            move(0,0);
    int g = homes[m].g;
    int a;
    a= rand() %20;
    g+=a+20;
        homes[m].g=g;

            printw("You have %d gold , %d\n",g , a);
            mapk[playerRow][playerCol] = '.';
            map[playerRow][playerCol] = '.';
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
                    map[playerRow][playerCol] = '.';
            
        }else if (map[playerRow][playerCol]  == 'H') {
            homes[m].telesm.Health += 1;
            har=0;
                    mapk[playerRow][playerCol] = '.';
                    map[playerRow][playerCol] = '.';
            
        }else if (map[playerRow][playerCol]  == 'P') {
            homes[m].telesm.speed += 1;
            har=0;
                    mapk[playerRow][playerCol] = '.';
                    map[playerRow][playerCol] = '.';
            
        }
            if(map[playerRow - 1][playerCol] != ' ' && map[playerRow - 1][playerCol] != '|' && map[playerRow - 1][playerCol] != '-') playerRow--;
        }
        else{
          playerRow--;
        }
        if (ch == '4' && map[playerRow][playerCol - 1] != ' ' && map[playerRow][playerCol - 1] != '|' && map[playerRow][playerCol - 1] != '-') if (homes[m].telesm.speed >=1)
        {
            playerCol--;
            if (map[playerRow][playerCol]  == '@') {
            move(0,0);
    int g = homes[m].g;
    int a;
    a= rand() %20;
    g+=a+20;
        homes[m].g=g;

            printw("You have %d gold , %d\n",g , a);
            mapk[playerRow][playerCol] = '.';
            map[playerRow][playerCol] = '.';
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
                    map[playerRow][playerCol] = '.';
            
        }else if (map[playerRow][playerCol]  == 'H') {
            homes[m].telesm.Health += 1;
            har=0;
                    mapk[playerRow][playerCol] = '.';
                    map[playerRow][playerCol] = '.';
            
        }else if (map[playerRow][playerCol]  == 'P') {
            homes[m].telesm.speed += 1;
            har=0;
                    mapk[playerRow][playerCol] = '.';
                    map[playerRow][playerCol] = '.';
            
        }
            if(map[playerRow][playerCol - 1] != ' ' && map[playerRow][playerCol - 1] != '|' && map[playerRow][playerCol - 1] != '-') playerCol--;
        }
        else{
          playerCol--;
        }
        if (ch == '6' && map[playerRow][playerCol + 1] != ' ' && map[playerRow][playerCol + 1] != '|' && map[playerRow][playerCol + 1] != '-') if (homes[m].telesm.speed >=1)
        {
            playerCol++;
            if (map[playerRow][playerCol]  == '@') {
            move(0,0);
    int g = homes[m].g;
    int a;
    a= rand() %20;
    g+=a+20;
        homes[m].g=g;

            printw("You have %d gold , %d\n",g , a);
            mapk[playerRow][playerCol] = '.';
            map[playerRow][playerCol] = '.';
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
                    map[playerRow][playerCol] = '.';
            
        }else if (map[playerRow][playerCol]  == 'H') {
            homes[m].telesm.Health += 1;
            har=0;
                    mapk[playerRow][playerCol] = '.';
                    map[playerRow][playerCol] = '.';
            
        }else if (map[playerRow][playerCol]  == 'P') {
            homes[m].telesm.speed += 1;
            har=0;
                    mapk[playerRow][playerCol] = '.';
                    map[playerRow][playerCol] = '.';
            
        }
            if(map[playerRow][playerCol + 1] != ' ' && map[playerRow][playerCol + 1] != '|' && map[playerRow][playerCol + 1] != '-') playerCol++;
        }
        else{
          playerCol++;
        }
        if (ch == '3' && map[playerRow + 1][playerCol+1] != ' ' && map[playerRow + 1][playerCol+1] != '|' && map[playerRow + 1][playerCol+1] != '-') if (homes[m].telesm.speed >=1)
         {
            playerRow++;
         playerCol++;
         if (map[playerRow][playerCol]  == '@') {
            move(0,0);
    int g = homes[m].g;
    int a;
    a= rand() %20;
    g+=a+20;
        homes[m].g=g;

            printw("You have %d gold , %d\n",g , a);
            mapk[playerRow][playerCol] = '.';
            map[playerRow][playerCol] = '.';
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
                    map[playerRow][playerCol] = '.';
            
        }else if (map[playerRow][playerCol]  == 'H') {
            homes[m].telesm.Health += 1;
            har=0;
                    mapk[playerRow][playerCol] = '.';
                    map[playerRow][playerCol] = '.';
            
        }else if (map[playerRow][playerCol]  == 'P') {
            homes[m].telesm.speed += 1;
            har=0;
                    mapk[playerRow][playerCol] = '.';
                    map[playerRow][playerCol] = '.';
            
        }
            if(map[playerRow + 1][playerCol+1] != ' ' && map[playerRow + 1][playerCol+1] != '|' && map[playerRow + 1][playerCol+1] != '-') {playerRow++;
         playerCol++;}
        }
        else{
          playerRow++;
         playerCol++;
        }
         if (ch == '1' && map[playerRow + 1][playerCol-1] != ' ' && map[playerRow + 1][playerCol-1] != '|' && map[playerRow + 1][playerCol-1] != '-') if (homes[m].telesm.speed >=1)
         {
            playerRow++;
         playerCol--;
         if (map[playerRow][playerCol]  == '@') {
            move(0,0);
    int g = homes[m].g;
    int a;
    a= rand() %20;
    g+=a+20;
        homes[m].g=g;

            printw("You have %d gold , %d\n",g , a);
            mapk[playerRow][playerCol] = '.';
            map[playerRow][playerCol] = '.';
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
                    map[playerRow][playerCol] = '.';
            
        }else if (map[playerRow][playerCol]  == 'H') {
            homes[m].telesm.Health += 1;
            har=0;
                    mapk[playerRow][playerCol] = '.';
                    map[playerRow][playerCol] = '.';
            
        }else if (map[playerRow][playerCol]  == 'P') {
            homes[m].telesm.speed += 1;
            har=0;
                    mapk[playerRow][playerCol] = '.';
                    map[playerRow][playerCol] = '.';
            
        }
            if(map[playerRow + 1][playerCol-1] != ' ' && map[playerRow + 1][playerCol-1] != '|' && map[playerRow + 1][playerCol-1] != '-') {playerRow++;
         playerCol--;}
        }
        else{
          playerRow++;
         playerCol--;
        }
         if (ch == '7' && map[playerRow - 1][playerCol-1] != ' ' && map[playerRow - 1][playerCol-1] != '|' && map[playerRow - 1][playerCol-1] != '-') if (homes[m].telesm.speed >=1)
         {
        playerRow--;
         playerCol--;
         if (map[playerRow][playerCol]  == '@') {
            move(0,0);
    int g = homes[m].g;
    int a;
    a= rand() %20;
    g+=a+20;
        homes[m].g=g;

            printw("You have %d gold , %d\n",g , a);
            mapk[playerRow][playerCol] = '.';
            map[playerRow][playerCol] = '.';
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
                    map[playerRow][playerCol] = '.';
            
        }else if (map[playerRow][playerCol]  == 'H') {
            homes[m].telesm.Health += 1;
            har=0;
                    mapk[playerRow][playerCol] = '.';
                    map[playerRow][playerCol] = '.';
            
        }else if (map[playerRow][playerCol]  == 'P') {
            homes[m].telesm.speed += 1;
            har=0;
                    mapk[playerRow][playerCol] = '.';
                    map[playerRow][playerCol] = '.';
            
        }
            if(map[playerRow - 1][playerCol-1] != ' ' && map[playerRow - 1][playerCol-1] != '|' && map[playerRow - 1][playerCol-1] != '-') {playerRow--;
         playerCol--;}
        }
        else{
          playerRow--;
         playerCol--;
        }
         if (ch == '9' && map[playerRow - 1][playerCol+1] != ' ' && map[playerRow - 1][playerCol+1] != '|' && map[playerRow - 1][playerCol+1] != '-') if (homes[m].telesm.speed >=1)
         {
        playerRow--;
         playerCol++;
         if (map[playerRow][playerCol]  == '@') {
            move(0,0);
    int g = homes[m].g;
    int a;
    a= rand() %20;
    g+=a+20;
        homes[m].g=g;

            printw("You have %d gold , %d\n",g , a);
            mapk[playerRow][playerCol] = '.';
            map[playerRow][playerCol] = '.';
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
                    map[playerRow][playerCol] = '.';
            
        }else if (map[playerRow][playerCol]  == 'H') {
            homes[m].telesm.Health += 1;
            har=0;
                    mapk[playerRow][playerCol] = '.';
                    map[playerRow][playerCol] = '.';
            
        }else if (map[playerRow][playerCol]  == 'P') {
            homes[m].telesm.speed += 1;
            har=0;
                    mapk[playerRow][playerCol] = '.';
                    map[playerRow][playerCol] = '.';
            
        }
            if(map[playerRow - 1][playerCol+1] != ' ' && map[playerRow - 1][playerCol+1] != '|' && map[playerRow - 1][playerCol+1] != '-') {playerRow--;
         playerCol++;}
        }
        else{
          playerRow--;
         playerCol++;
        }
         if (ch == 'b')
         {
            // move(0,0);
            // printw("You have %d Health spell\nYou have %d damage spell\nYou have %d Speed spell\n", homes[m].telesm.Health, homes[m].telesm.damage, homes[m].telesm.speed);
            // getch();
            char *choicesg[] = {"Health spell" , "damage spell" , "Speed spell"};
            int choiceg = navigate_menu(choicesg, 3);
            if (choiceg == 0){
            //     if (homes[m].gun.mace ==1||homes[m].gun.sword ==1 || homes[m].gun.normallarrow ==1 || homes[m].gun.dagger ==1 || homes[m].gun.magicwand ==1)
            // {
                
            //  move(0,0);
            // printw("First, put the previous weapon in your backpack");
            // getch();
            // }
            if(htel>0){
                  homes[m].telesm.Health +=1;
                  move(0,0);
            printw("Health spell chosen as default spell");
            getch();
            }else {
                move(0,0);
            printw("You don't have the Health spell");
            getch();
            }
            //  homes[m].gun.sword =0;
            } 
            if (choiceg == 1){
            if(dtel>0){
                  homes[m].telesm.damage +=1;
                  move(0,0);
            printw("damage spell chosen as default spell");
            getch();
            }else {
                move(0,0);
            printw("You don't have the damage spell");
            getch();
            }
            //  homes[m].gun.sword =0;
            } 
            if (choiceg == 2){
            if(stel>0){
                  homes[m].telesm.speed +=1;
                  move(0,0);
            printw("Speed spell chosen as default spell");
            getch();
            }else {
                move(0,0);
            printw("You don't have the Speed spell");
            getch();
            }
            //  homes[m].gun.sword =0;
            }
            // move(0,30);
            // printw("You have %d damage telesm\n", homes[m].telesm.damage);
            // getch();
            // move(0,30);
            // printw("You have %d Speed telesm\n", homes[m].telesm.speed);
            // getch();
         }
         if (ch == 'w')
         {
            if (homes[m].gun.mace ==1||homes[m].gun.sword ==1 || homes[m].gun.normallarrow ==1 || homes[m].gun.dagger ==1 || homes[m].gun.magicwand ==1)
            {
                homes[m].gun.mace =0;
             homes[m].gun.sword =0;
             homes[m].gun.normallarrow =0;
             homes[m].gun.dagger =0;
             homes[m].gun.magicwand =0;
             move(0,0);
            printw("The weapon was placed in a backpack");
            getch();
            }else{
             move(0,0);
            printw("No default weapon");
            getch();
            }
            
            
            
         }
         if (ch == 'i')
         {
             char *choicesg[] = {"mace gun" , "sword gun" , "normall arrow" , "dagger" , "magic wand"};
            int choiceg = navigate_menu(choicesg, 5);
            if (choiceg == 0){
                if (homes[m].gun.mace ==1||homes[m].gun.sword ==1 || homes[m].gun.normallarrow ==1 || homes[m].gun.dagger ==1 || homes[m].gun.magicwand ==1)
            {
                
             move(0,0);
            printw("First, put the previous weapon in your backpack");
            getch();
            }else{
                  homes[m].gun.mace =1;
                  move(0,0);
            printw("Default weapon changed to mace");
            getch();
            }
            //  homes[m].gun.sword =0;
            }
            if (choiceg == 1 && sw==1){
                 if (homes[m].gun.mace ==1||homes[m].gun.sword ==1 || homes[m].gun.normallarrow ==1 || homes[m].gun.dagger ==1 || homes[m].gun.magicwand ==1)
            {
                
             move(0,0);
            printw("First, put the previous weapon in your backpack");
            getch();
            }else{
                   homes[m].gun.sword =1;
                  move(0,0);
            printw("Default weapon changed to sword");
            getch();
            }
            
            //  homes[m].gun.mace =0;
            } else if(choiceg == 1){
                move(0,0);
            printw("You don't have the sword");
            getch();
                
            } if (choiceg==2 && nw==1)
            {
                if (homes[m].gun.mace ==1||homes[m].gun.sword ==1 || homes[m].gun.normallarrow ==1 || homes[m].gun.dagger ==1 || homes[m].gun.magicwand ==1)
            {
                
             move(0,0);
            printw("First, put the previous weapon in your backpack");
            getch();
            }else{
                   homes[m].gun.normallarrow =1;
                  move(0,0);
            printw("Default weapon changed to normall arrow");
            getch();
            }
            }else if(choiceg == 2){
                move(0,0);
            printw("You don't have the normall arrow");
            getch();
                
            } if (choiceg==4 && mw==1)
            {
                if (homes[m].gun.mace ==1||homes[m].gun.sword ==1 || homes[m].gun.normallarrow ==1 || homes[m].gun.dagger ==1 || homes[m].gun.magicwand ==1)
            {
                
             move(0,0);
            printw("First, put the previous weapon in your backpack");
            getch();
            }else{
                   homes[m].gun.magicwand =1;
                  move(0,0);
            printw("Default weapon changed to magicwand");
            getch();
            }
            }else if(choiceg == 4){
                move(0,0);
            printw("You don't have the magicwand");
            getch();
                
            } if (choiceg==3 && dg==1)
            {
                if (homes[m].gun.mace ==1||homes[m].gun.sword ==1 || homes[m].gun.normallarrow ==1 || homes[m].gun.dagger ==1 || homes[m].gun.magicwand ==1)
            {
                
             move(0,0);
            printw("First, put the previous weapon in your backpack");
            getch();
            }else{
                   homes[m].gun.dagger =1;
                  move(0,0);
            printw("Default weapon changed to dagger");
            getch();
            }
            }else if(choiceg == 3){
                move(0,0);
            printw("You don't have the dagger");
            getch();
                
            } 
            
            move(0,0);
            printw("Short Range\nYou have %d sword gun\nYou have %d mace gun\nLong range\nYou have %d dagger gun\nYou have %d magic wand gun\nYou have %d normal arrow gun", homes[m].gun.sword, homes[m].gun.mace, homes[m].gun.dagger, homes[m].gun.magicwand, homes[m].gun.normallarrow);
            getch();
            // move(0,30);
            // printw("You have %d damage telesm\n", homes[m].telesm.damage);
            // getch();
            // move(0,30);
            // printw("You have %d Speed telesm\n", homes[m].telesm.speed);
            // getch();
         }
         if(map[playerRow][playerCol] == '#'){
                    mapk[playerRow][playerCol] = '#';
                } else if(map[playerRow][playerCol] == '+'){
                    mapk[playerRow][playerCol] = '+';
                }else if(map[playerRow][playerCol] == '/'){
                    mapk[playerRow][playerCol] = '.';
                    while (homes[m].gun.normallarrowcount[playerRow][playerCol]!=0)
                    {
                        tirna++;
                        homes[m].gun.normallarrowcount[playerRow][playerCol]--;
                    }
                    while (homes[m].gun.magicwandcount[playerRow][playerCol]!=0)
                    {
                        tirmw++;
                        homes[m].gun.magicwandcount[playerRow][playerCol]--;
                    }
                    while (homes[m].gun.daggercount[playerRow][playerCol]!=0)
                    {
                        tirda++;
                        homes[m].gun.daggercount[playerRow][playerCol]--;
                    }
                    move(0, 0);
        printw("%d %d %d", tirna , tirda , tirmw);
        refresh();
        getch();
                } else if (map[playerRow][playerCol]  == '@') {
            move(0,0);
    int g = homes[m].g;
    int a;
    a= rand() %20;
    g+=a+20;
        homes[m].g=g;

            printw("You have %d gold , %d\n",g , a);
            mapk[playerRow][playerCol] = '.';
            // refresh();
            getch();
            
        }else if(map[playerRow][playerCol] == 'W'){
                    mapk[playerRow][playerCol] = '.';
                    sw=1;
                }else if(map[playerRow][playerCol] == 'N'){
                    mapk[playerRow][playerCol] = '.';
                    nw=1;
                }else if(map[playerRow][playerCol] == 'M'){
                    mapk[playerRow][playerCol] = '.';
                    mw=1;
                }else if(map[playerRow][playerCol] == 'd'){
                    mapk[playerRow][playerCol] = '.';
                    dg=1;
                }else if (map[playerRow][playerCol]  == '!') {
            if (homes[m].hu<=15)
                    {
                        homes[m].hu+=5;
                    } else{
                        int ghh=20-homes[m].hu;
                      homes[m].hu+=ghh;
                    }
                    mapk[playerRow][playerCol] = '.';
            
        }else if (map[playerRow][playerCol]  == 'H') {
            htel += 1;
            har=0;
                    mapk[playerRow][playerCol] = '.';
            
        }else if (map[playerRow][playerCol]  == 'P') {
            stel += 1;
            har=0;
                    mapk[playerRow][playerCol] = '.';
            
        }else if (map[playerRow][playerCol]  == 'T') {
            dtel += 1;
            har=0;
                    mapk[playerRow][playerCol] = '.';
            
        }else if (map[playerRow][playerCol]  == 'D') {
            homes[m].he -= 1;
            har=0;
            d=1;

                    mapk[playerRow][playerCol] = '.';
            
        }else if (map[playerRow][playerCol]  == 'F') {
            homes[m].he -= 1;
            har=0;
            f=1;

                    mapk[playerRow][playerCol] = '.';
            
        }else if (map[playerRow][playerCol]  == 'G') {
            homes[m].he -= 1;
            har=0;
            g=1;

                    mapk[playerRow][playerCol] = '.';
            
        }else if (map[playerRow][playerCol]  == 'S') {
            homes[m].he -= 1;
            har=0;
            s=1;

                    mapk[playerRow][playerCol] = '.';
            
        }else if (map[playerRow][playerCol]  == 'U') {
            homes[m].he -= 1;
            har=0;
            u=1;

                    mapk[playerRow][playerCol] = '.';
            
        }else{
                    mapk[playerRow][playerCol] = '.';
                }
                if (homes[m].gun.mace ==1 && ch == ' ')
                {
                    int l;
                    if (homes[m].telesm.damage>0)
                    {
                        l=10;
                    }else
                    {
                        l=5;
                    }
                    for(int i=-1 ; i<2; i++){
                        for (int j = -1; j < 2; j++)
                        {
                            if (map[playerRow+i][playerCol+j]  == 'D')
                    {
                        hed -=l;
                    }
                    if (map[playerRow+i][playerCol+j]  == 'F')
                    {
                        hef -=l;
                    }
                    if (map[playerRow+i][playerCol+j]  == 'G')
                    {
                        heg -=l;
                    }
                    if (map[playerRow+i][playerCol+j]  == 'S')
                    {
                        hes -=l;
                    }
                    if (map[playerRow+i][playerCol+j]  == 'U')
                    {
                        heu -=l;
                    }
                    
                            
                        }
                        
                    }
                    
                    
                }
                if (homes[m].gun.sword ==1 && ch == ' ')
                {
                    int l;
                    if (homes[m].telesm.damage>0)
                    {
                        l=20;
                    }else
                    {
                        l=10;
                    }
                    
                    
                    for(int i=-1 ; i<2; i++){
                        for (int j = -1; j < 2; j++)
                        {
                            if (map[playerRow+i][playerCol+j]  == 'D')
                    {
                        hed -=l;
                    }
                    if (map[playerRow+i][playerCol+j]  == 'F')
                    {
                        hef -=l;
                    }
                    if (map[playerRow+i][playerCol+j]  == 'G')
                    {
                        heg -=l;
                    }
                    if (map[playerRow+i][playerCol+j]  == 'S')
                    {
                        hes -=l;
                    }
                    if (map[playerRow+i][playerCol+j]  == 'U')
                    {
                        heu -=l;
                    }
                            
                        }
                        
                    }
                    
                    
                }
                if (homes[m].gun.normallarrow ==1 && ch == ' ')
                {
                    int l;
                    if (homes[m].telesm.damage>0)
                    {
                        l=10;
                    }else
                    {
                        l=5;
                    }
                           int chc;
    if (tirna > 0) {
        move(0, 0);
        printw("Choose the direction");
        refresh();
        do {
            chc = getch();
            if (chc == '1' || chc == '2' || chc == '3' || chc == '4' || chc == '6' || chc == '7' || chc == '8' || chc == '9') {
                // Handle direction input while in space mode
                int i4 = playerRow, j4 = playerCol;
                for (int k = 0; k < 5; k++) {
                    if (chc == '2' && (map[playerRow + 1][playerCol] != ' ' && map[playerRow + 1][playerCol] != '|' && map[playerRow + 1][playerCol] != '-')) {
                        playerRow++;
                    } else if (chc == '8' && (map[playerRow - 1][playerCol] != ' ' && map[playerRow - 1][playerCol] != '|' && map[playerRow - 1][playerCol] != '-')) {
                        playerRow--;
                    } else if (chc == '4' && (map[playerRow][playerCol - 1] != ' ' && map[playerRow][playerCol - 1] != '|' && map[playerRow][playerCol - 1] != '-')) {
                        playerCol--;
                    } else if (chc == '6' && (map[playerRow][playerCol + 1] != ' ' && map[playerRow][playerCol + 1] != '|' && map[playerRow][playerCol + 1] != '-')) {
                        playerCol++;
                    } else if (chc == '9' && (map[playerRow - 1][playerCol + 1] != ' ' && map[playerRow - 1][playerCol + 1] != '|' && map[playerRow - 1][playerCol + 1] != '-')) {
                        playerRow--;
                        playerCol++;
                    } else if (chc == '7' && (map[playerRow - 1][playerCol - 1] != ' ' && map[playerRow - 1][playerCol - 1] != '|' && map[playerRow - 1][playerCol - 1] != '-')) {
                        playerRow--;
                        playerCol--;
                    } else if (chc == '1' && (map[playerRow + 1][playerCol - 1] != ' ' && map[playerRow + 1][playerCol - 1] != '|' && map[playerRow + 1][playerCol - 1] != '-')) {
                        playerRow++;
                        playerCol--;
                    } else if (chc == '3' && (map[playerRow + 1][playerCol + 1] != ' ' && map[playerRow + 1][playerCol + 1] != '|' && map[playerRow + 1][playerCol + 1] != '-')) {
                        playerRow++;
                        playerCol++;
                    } else {
                        break;
                    }

                    if (map[playerRow][playerCol] == 'D' || map[harek[har].i][harek[har].j] == 'D') {
                        hed-=l;
                        break;
                    }
                    if (map[playerRow][playerCol] == 'F' || map[harek[har].i][harek[har].j] == 'F') {
                        hef-=l;
                        break;
                    }
                    if (map[playerRow][playerCol] == 'G' || map[harek[har].i][harek[har].j] == 'G') {
                        heg-=l;
                        break;
                    }
                    if (map[playerRow][playerCol] == 'S' || map[harek[har].i][harek[har].j] == 'S') {
                        hes-=l;
                        break;
                    }
                    if (map[playerRow][playerCol] == 'U' || map[harek[har].i][harek[har].j] == 'U') {
                        heu-=l;
                        break;
                    }
                }

                if (map[playerRow][playerCol] != 'D' && map[harek[har].i][harek[har].j] != 'D' && map[playerRow][playerCol] != 'F' && map[harek[har].i][harek[har].j] != 'F' && map[playerRow][playerCol] != 'G' && map[harek[har].i][harek[har].j] != 'G' && map[playerRow][playerCol] != 'S' && map[harek[har].i][harek[har].j] != 'S' && map[playerRow][playerCol] != 'U' && map[harek[har].i][harek[har].j] != 'U') {
                    map[playerRow][playerCol] = '/';
                    homes[m].gun.normallarrowcount[playerRow][playerCol]++;
                }

                playerRow = i4;
                playerCol = j4;
                tirna--;
            }
        } while (chc != '1' && chc != '2' && chc != '3' && chc != '4' && chc != '6' && chc != '7' && chc != '8' && chc != '9');
    } else {
        move(0, 0);
        printw("you don't have any tir");
        refresh();
        getch();
    }
 
                        }
                        if (homes[m].gun.dagger ==1 && ch == ' ')
                {
                    int l;
                    if (homes[m].telesm.damage>0)
                    {
                        l=24;
                    }else
                    {
                        l=12;
                    }
                           int chc;
    if (tirda > 0) {
        move(0, 0);
        printw("Choose the direction");
        refresh();
        do {
            chc = getch();
            if (chc == '1' || chc == '2' || chc == '3' || chc == '4' || chc == '6' || chc == '7' || chc == '8' || chc == '9') {
                // Handle direction input while in space mode
                int i4 = playerRow, j4 = playerCol;
                for (int k = 0; k < 5; k++) {
                    if (chc == '2' && (map[playerRow + 1][playerCol] != ' ' && map[playerRow + 1][playerCol] != '|' && map[playerRow + 1][playerCol] != '-')) {
                        playerRow++;
                    } else if (chc == '8' && (map[playerRow - 1][playerCol] != ' ' && map[playerRow - 1][playerCol] != '|' && map[playerRow - 1][playerCol] != '-')) {
                        playerRow--;
                    } else if (chc == '4' && (map[playerRow][playerCol - 1] != ' ' && map[playerRow][playerCol - 1] != '|' && map[playerRow][playerCol - 1] != '-')) {
                        playerCol--;
                    } else if (chc == '6' && (map[playerRow][playerCol + 1] != ' ' && map[playerRow][playerCol + 1] != '|' && map[playerRow][playerCol + 1] != '-')) {
                        playerCol++;
                    } else if (chc == '9' && (map[playerRow - 1][playerCol + 1] != ' ' && map[playerRow - 1][playerCol + 1] != '|' && map[playerRow - 1][playerCol + 1] != '-')) {
                        playerRow--;
                        playerCol++;
                    } else if (chc == '7' && (map[playerRow - 1][playerCol - 1] != ' ' && map[playerRow - 1][playerCol - 1] != '|' && map[playerRow - 1][playerCol - 1] != '-')) {
                        playerRow--;
                        playerCol--;
                    } else if (chc == '1' && (map[playerRow + 1][playerCol - 1] != ' ' && map[playerRow + 1][playerCol - 1] != '|' && map[playerRow + 1][playerCol - 1] != '-')) {
                        playerRow++;
                        playerCol--;
                    } else if (chc == '3' && (map[playerRow + 1][playerCol + 1] != ' ' && map[playerRow + 1][playerCol + 1] != '|' && map[playerRow + 1][playerCol + 1] != '-')) {
                        playerRow++;
                        playerCol++;
                    } else {
                        break;
                    }

                    if (map[playerRow][playerCol] == 'D' || map[harek[har].i][harek[har].j] == 'D') {
                        hed-=l;
                        break;
                    }
                    if (map[playerRow][playerCol] == 'F' || map[harek[har].i][harek[har].j] == 'F') {
                        hef-=l;
                        break;
                    }
                    if (map[playerRow][playerCol] == 'G' || map[harek[har].i][harek[har].j] == 'G') {
                        heg-=l;
                        break;
                    }
                    if (map[playerRow][playerCol] == 'S' || map[harek[har].i][harek[har].j] == 'S') {
                        hes-=l;
                        break;
                    }
                    if (map[playerRow][playerCol] == 'U' || map[harek[har].i][harek[har].j] == 'U') {
                        heu-=l;
                        break;
                    }
                }

                if (map[playerRow][playerCol] != 'D' && map[harek[har].i][harek[har].j] != 'D' && map[playerRow][playerCol] != 'F' && map[harek[har].i][harek[har].j] != 'F' && map[playerRow][playerCol] != 'G' && map[harek[har].i][harek[har].j] != 'G' && map[playerRow][playerCol] != 'S' && map[harek[har].i][harek[har].j] != 'S' && map[playerRow][playerCol] != 'U' && map[harek[har].i][harek[har].j] != 'U') {
                    map[playerRow][playerCol] = '/';
                    homes[m].gun.daggercount[playerRow][playerCol]++;
                }

                playerRow = i4;
                playerCol = j4;
                tirda--;
            }
        } while (chc != '1' && chc != '2' && chc != '3' && chc != '4' && chc != '6' && chc != '7' && chc != '8' && chc != '9');
    } else {
        move(0, 0);
        printw("you don't have any tir");
        refresh();
        getch();
    }
 
                        }if (homes[m].gun.magicwand ==1 && ch == ' ')
                {
                     int l;
                    if (homes[m].telesm.damage>0)
                    {
                        l=20;
                    }else
                    {
                        l=10;
                    }
                           int chc;
    if (tirmw > 0) {
        move(0, 0);
        printw("Choose the direction");
        refresh();
        do {
            chc = getch();
            if (chc == '1' || chc == '2' || chc == '3' || chc == '4' || chc == '6' || chc == '7' || chc == '8' || chc == '9') {
                // Handle direction input while in space mode
                int i4 = playerRow, j4 = playerCol;
                for (int k = 0; k < 10; k++) {
                    if (chc == '2' && (map[playerRow + 1][playerCol] != ' ' && map[playerRow + 1][playerCol] != '|' && map[playerRow + 1][playerCol] != '-')) {
                        playerRow++;
                    } else if (chc == '8' && (map[playerRow - 1][playerCol] != ' ' && map[playerRow - 1][playerCol] != '|' && map[playerRow - 1][playerCol] != '-')) {
                        playerRow--;
                    } else if (chc == '4' && (map[playerRow][playerCol - 1] != ' ' && map[playerRow][playerCol - 1] != '|' && map[playerRow][playerCol - 1] != '-')) {
                        playerCol--;
                    } else if (chc == '6' && (map[playerRow][playerCol + 1] != ' ' && map[playerRow][playerCol + 1] != '|' && map[playerRow][playerCol + 1] != '-')) {
                        playerCol++;
                    } else if (chc == '9' && (map[playerRow - 1][playerCol + 1] != ' ' && map[playerRow - 1][playerCol + 1] != '|' && map[playerRow - 1][playerCol + 1] != '-')) {
                        playerRow--;
                        playerCol++;
                    } else if (chc == '7' && (map[playerRow - 1][playerCol - 1] != ' ' && map[playerRow - 1][playerCol - 1] != '|' && map[playerRow - 1][playerCol - 1] != '-')) {
                        playerRow--;
                        playerCol--;
                    } else if (chc == '1' && (map[playerRow + 1][playerCol - 1] != ' ' && map[playerRow + 1][playerCol - 1] != '|' && map[playerRow + 1][playerCol - 1] != '-')) {
                        playerRow++;
                        playerCol--;
                    } else if (chc == '3' && (map[playerRow + 1][playerCol + 1] != ' ' && map[playerRow + 1][playerCol + 1] != '|' && map[playerRow + 1][playerCol + 1] != '-')) {
                        playerRow++;
                        playerCol++;
                    } else {
                        break;
                    }

                    if (map[playerRow][playerCol] == 'D' || map[harek[har].i][harek[har].j] == 'D') {
                        hed-=l;
                        break;
                    }
                    if (map[playerRow][playerCol] == 'F' || map[harek[har].i][harek[har].j] == 'F') {
                        hef-=l;
                        break;
                    }
                    if (map[playerRow][playerCol] == 'G' || map[harek[har].i][harek[har].j] == 'G') {
                        heg-=l;
                        break;
                    }
                    if (map[playerRow][playerCol] == 'S' || map[harek[har].i][harek[har].j] == 'S') {
                        hes-=l;
                        break;
                    }
                    if (map[playerRow][playerCol] == 'U' || map[harek[har].i][harek[har].j] == 'U') {
                        heu-=l;
                        break;
                    }
                }

                if (map[playerRow][playerCol] != 'D' && map[harek[har].i][harek[har].j] != 'D' && map[playerRow][playerCol] != 'F' && map[harek[har].i][harek[har].j] != 'F' && map[playerRow][playerCol] != 'G' && map[harek[har].i][harek[har].j] != 'G' && map[playerRow][playerCol] != 'S' && map[harek[har].i][harek[har].j] != 'S' && map[playerRow][playerCol] != 'U' && map[harek[har].i][harek[har].j] != 'U') {
                    map[playerRow][playerCol] = '/';
                    homes[m].gun.magicwandcount[playerRow][playerCol]++;
                }

                playerRow = i4;
                playerCol = j4;
                tirda--;
            }
        } while (chc != '1' && chc != '2' && chc != '3' && chc != '4' && chc != '6' && chc != '7' && chc != '8' && chc != '9');
    } else {
        move(0, 0);
        printw("you don't have any tir");
        refresh();
        getch();
    }
 
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
        if (homes[m].telesm.Health>=1)
        {
            if(har%10==0){
             homes[m].telesm.Health --;
            }
            else if(homes[m].he<20 && har %10 == 0){
                homes[m].he++;
            }
            
        }
        if (homes[m].telesm.speed>=1)
        {
            if(har%10==0){
             homes[m].telesm.speed --;
            }
            // else if(homes[m].he<20 && har %10 == 0){
            //     homes[m].he++;
            // }
            
        }
        if (homes[m].telesm.damage >=1)
        {
            if(har%10==0){
             homes[m].telesm.damage --;
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
        
        if(d==1 && har%10!=0){
            if (har%3==0)
            {
               homes[m].he --;
            }
            move(0,0);
            printw( "the enemy D have %d" , hed);
             harek[har].i = playerRow;
            harek[har].j = playerCol;
            harek[har].pr = mapk[playerRow][playerCol];
             mapk[playerRow][playerCol] = 'D';
             homes[m].hdeamon.i = playerRow;
             homes[m].hdeamon.j = playerCol;
           
        } 
        if(f==1 && har%10!=0){
            if (har%3==0)
            {
               homes[m].he --;
            }
             harek[har].i = playerRow;
            harek[har].j = playerCol;
            harek[har].pr = mapk[playerRow][playerCol];
             mapk[playerRow][playerCol] = 'F';
             homes[m].hfire.i = playerRow;
             homes[m].hfire.j = playerCol;
           
        } 
         if(s==1 ){
            if (har%3==0)
            {
               homes[m].he --;
            }
             harek[har].i = playerRow;
            harek[har].j = playerCol;
            harek[har].pr = mapk[playerRow][playerCol];
             mapk[playerRow][playerCol] = 'S';
             homes[m].hsnake.i = playerRow;
             homes[m].hsnake.j = playerCol;
           
        } 
         if(u==1 && har%50!=0){
            if (har%3==0)
            {
               homes[m].he --;
            }
             harek[har].i = playerRow;
            harek[har].j = playerCol;
            harek[har].pr = mapk[playerRow][playerCol];
             mapk[playerRow][playerCol] = 'U';
             homes[m].hundead.i = playerRow;
             homes[m].hundead.j = playerCol;
           
        }
          if(g==1 && har%50!=0){
            if (har%3==0)
            {
               homes[m].he --;
            }
             harek[har].i = playerRow;
            harek[har].j = playerCol;
            harek[har].pr = mapk[playerRow][playerCol];
             mapk[playerRow][playerCol] = 'G';
             homes[m].hgiant.i = playerRow;
             homes[m].hgiant.j = playerCol;
           
        }  
        // if (d==0)
        // {
        //     for(int i=-1 ; i<2; i++){
        //                 for (int j = -1; j < 2; j++)
        //                 {
        //                     if (map[playerRow+i][playerCol+j]  == 'D' )
        //             {
        //                map[homes[m].hdeamon.i][homes[m].hdeamon.j]  = '.';
        //                 d=1;
        //             }
                            
        //                 }
                        
        //             }
        // }
        
        
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
    homes[m].telesm.speed =0;
    homes[m].telesm.damage =0;
    homes[m].gun.sword =0;
    homes[m].gun.dagger =0;
    homes[m].gun.magicwand =0;
    homes[m].gun.normallarrow =0;
    sw =0;
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
                char *choices[] = {"Color *" , "music"};
            int choice = navigate_menu(choices, 2);
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
            } else if(choice == 1){
              char *choicesmu[] = {"OFF", "ON" };
            int choicemu = navigate_menu(choicesmu, 2);
            if (choicemu==0)
            {
                homes[m].music = 0;
            }else if (choicemu==1)
            {
                homes[m].music = 1;
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