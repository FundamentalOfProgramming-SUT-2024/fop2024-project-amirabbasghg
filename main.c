#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <math.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

// تابعی برای تبدیل عدد به متن
void renderText(SDL_Renderer* renderer, TTF_Font* font, const char* text, int x, int y, SDL_Color color) {
    SDL_Surface* textSurface = TTF_RenderUTF8_Blended(font, text, color);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect textRect = {x, y, textSurface->w, textSurface->h};
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

int main(int argc, char* argv[]) {
    // ابتدا SDL و SDL_ttf را راه‌اندازی می‌کنیم
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL Initialization Error: %s\n", SDL_GetError());
        return 1;
    }
    if (TTF_Init() < 0) {
        printf("TTF Initialization Error: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    // ایجاد پنجره و رندرر
    SDL_Window* window = SDL_CreateWindow("نمودار arctan(x)", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Window Creation Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Renderer Creation Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // بارگذاری فونت
    TTF_Font* font = TTF_OpenFont("Arya-Bold.ttf", 16); // تغییر اندازه فونت در اینجا
    if (!font) {
        printf("Font Loading Error: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // رنگ پس‌زمینه سفید
SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_RenderClear(renderer);

    // رسم خطوط شبکه‌بندی
    SDL_SetRenderDrawColor(renderer, 200, 11, 200, 17); // خاکستری کم‌رنگ
    for (int i = 0; i <= WINDOW_WIDTH; i += 50) {
        SDL_RenderDrawLine(renderer, i, 0, i, WINDOW_HEIGHT); // خطوط عمودی
    }
    for (int i = 0; i <= WINDOW_HEIGHT; i += 50) {
        SDL_RenderDrawLine(renderer, 0, i, WINDOW_WIDTH, i); // خطوط افقی
    }

    // رسم محورهای مختصات
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // سیاه
    SDL_RenderDrawLine(renderer, WINDOW_WIDTH / 2, 0, WINDOW_WIDTH / 2, WINDOW_HEIGHT); // محور y
    SDL_RenderDrawLine(renderer, 0, WINDOW_HEIGHT / 2, WINDOW_WIDTH, WINDOW_HEIGHT / 2); // محور x

    // افزودن اعداد به محور x و y
    SDL_Color textColor = {0, 0, 0, 255}; // سیاه
    for (int i = -7; i <= 7; i++) {
        char label[50];
        sprintf(label, "%d", i); // تبدیل عدد به متن
        int screenX = WINDOW_WIDTH / 2 + i * 100; // مقیاس‌بندی محور x
        renderText(renderer, font, label, screenX - 10, WINDOW_HEIGHT / 2 + 10, textColor); // نمایش اعداد در زیر محور x
    }
    
    for (int i = -2; i <= 2; i++) {
        char label[50];
        sprintf(label, "%d", i); // تبدیل عدد به متن
        int screenY = WINDOW_HEIGHT / 2 - i * 100; // مقیاس‌بندی محور y
        renderText(renderer, font, label, WINDOW_WIDTH / 2 + 10, screenY - 10, textColor); // نمایش اعداد در کنار محور y
    }

    // رسم نمودار arctan(x)
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // آبی
    for (int i = -WINDOW_WIDTH / 2; i < WINDOW_WIDTH / 2; i++) {
        double x = i / 100.0;  // مقیاس x
        double y = atan(x);    // محاسبه مقدار arctan(x)
        
        // تبدیل مقادیر x و y به مختصات صفحه
        int screenX = i + WINDOW_WIDTH / 2;
        int screenY = WINDOW_HEIGHT / 2 - (int)(y * 100);  // مقیاس‌بندی بهتر برای y
        SDL_RenderDrawPoint(renderer, screenX, screenY);
    }

    // افزودن متن به نمودار
    renderText(renderer, font, "نمودار عبارت arctan(x)", WINDOW_WIDTH / 2 - 100, 10, textColor);

    // نمایش تغییرات
    SDL_RenderPresent(renderer);

    // منتظر ماندن برای بستن پنجره
    SDL_Event e;
    while (1) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                TTF_CloseFont(font);
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                TTF_Quit();
                SDL_Quit();
                return 0;
            }
        }
    }
    return 0;
}
