#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
#define HEIGHT 20
#define WIDTH 70
void gotoxy(int x, int y) {
    HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD dwPos;
    dwPos.X = x;
    dwPos.Y = y;
    SetConsoleCursorPosition(hcon, dwPos);
}
struct coord {
    int x;
    int y;
};
void limites() {
    for (int i = 0; i < WIDTH; i++) {
        gotoxy(i, 0); printf("%c", 205);
        gotoxy(i, HEIGHT); printf("%c", 205);
    }
    for (int j = 0; j < HEIGHT; j++) {
        gotoxy(0, j); printf("%c", 186);
        gotoxy(WIDTH, j); printf("%c", 186);
    }
    gotoxy(0, 0); printf("%c", 201);
    gotoxy(WIDTH, 0); printf("%c", 187);
    gotoxy(0, HEIGHT); printf("%c", 200);
    gotoxy(WIDTH, HEIGHT); printf("%c", 188);
}
void player(coord pos, int c) {
    switch (c) {
        case 0:
            gotoxy(pos.x, pos.y - 1); printf("%c", 219);
            gotoxy(pos.x, pos.y);     printf("%c", 219);
            gotoxy(pos.x, pos.y + 1); printf("%c", 219);
            break;
        case 1:
            gotoxy(pos.x, pos.y - 1); printf(" ");
            gotoxy(pos.x, pos.y);     printf(" ");
            gotoxy(pos.x, pos.y + 1); printf(" ");
            break;
    }
}
void pelota(coord pel, int c) {
    switch (c) {
        case 0:
            gotoxy(pel.x, pel.y); printf("%c", 184);
            break;
        case 1:
            gotoxy(pel.x, pel.y); printf(" ");
            break;
    }
}
void move(coord &pos) {
    if (kbhit()) {
        coord copy = pos;
        player(pos, 1);
        char tecla = getch();
        if (tecla == 'w') copy.y--;
        if (tecla == 's') copy.y++;
        if (copy.y > 1 && copy.y < HEIGHT - 1) pos = copy;
        player(pos, 0);
    }
}
void move_ia(coord &poe, coord pel) {
    if (pel.x > WIDTH / 2 + 15) {
        player(poe, 1);
        if (pel.y > poe.y && poe.y < HEIGHT - 2) poe.y++;
        if (pel.y < poe.y && poe.y > 2) poe.y--;
        player(poe, 0);
    }
}
void marcador(coord mar) {
    gotoxy(WIDTH + 7, 1); printf("MARCADOR");
    gotoxy(WIDTH + 2, 3); printf("Jugador");
    gotoxy(WIDTH + 5, 5); printf("%d", mar.x);
    gotoxy(WIDTH + 12, 3); printf("Oponente");
    gotoxy(WIDTH + 16, 5); printf("%d", mar.y);
}
void reset(coord &mar, coord &pel, bool &mov) {
    pelota(pel, 1);
    mov = false;
    mar = {0, 0};
    pel = {WIDTH / 2, HEIGHT / 2};
    marcador(mar);
}
void inicio(coord &aum, coord &pel, bool &mov, coord &mar) {
    mov = false;
    if (pel.x > WIDTH / 2 + 10) mar.x++;
    else if (pel.x < WIDTH / 2 - 10) mar.y++;
    pelota(pel, 1);
    pel = {WIDTH / 2, HEIGHT / 2};
    aum.y = -aum.y;
    marcador(mar);
}
void move_pel(coord &pel, coord &aum, bool mov) {
    coord copy = pel;
    pelota(pel, 1);

    if (mov) {
        copy.x += aum.x;
        copy.y += aum.y;
    }


    if (copy.y >= HEIGHT) {
        copy.y = HEIGHT - 1;
        aum.y = -aum.y;
    } else if (copy.y <= 1) {
        copy.y = 2;
        aum.y = -aum.y;
    }

    pel = copy;
    pelota(pel, 0);
}
void colision(coord pos, coord pel, coord &aum, coord poe) {
    if (pos.x == pel.x - 1 && pel.y >= pos.y - 1 && pel.y <= pos.y + 1) {
        aum.x = 1;
        aum.y = (pel.y == pos.y ? 0 : pel.y > pos.y ? 1 : -1); 
    }
    else if (poe.x == pel.x + 1 && pel.y >= poe.y - 1 && pel.y <= poe.y + 1) {
        aum.x = -1;
        aum.y = (pel.y == poe.y ? 0 : pel.y > poe.y ? 1 : -1); 
    }
}
void update(coord &pel, coord &aum, bool &mov, coord &mar) {
    if (kbhit()) {
        char tecla = getch();
        if (tecla == 'e') mov = true;
        if (tecla == 'q') reset(mar, pel, mov);
    }

    if (pel.x < 3 || pel.x > WIDTH - 3) {
        inicio(aum, pel, mov, mar);
    }
}
void red() {
    for (int i = 1; i < HEIGHT; i++) {
        gotoxy(WIDTH / 2, i); printf("%c", 176);
    }
}
int main() {
    coord pos = {10, HEIGHT / 2};
    coord pel = {WIDTH / 2, HEIGHT / 2};
    coord aum = {-1, -1};
    coord poe = {WIDTH - 10, HEIGHT / 2};
    coord mar = {0, 0};
    bool game = true;
    int u = 0;
    bool mov = false;
    limites();
    red();
    player(pos, 0);
    player(poe, 0);
    pelota(pel, 0);
    marcador(mar);
    float fps = 24.0f;
    float dt = 1.0f / fps;
    float acumulador = 0.0f;
    clock_t inicio = clock();
    while (game) {
        clock_t final = clock();
        acumulador += float(final - inicio) / CLOCKS_PER_SEC;
        inicio = final;

        if (acumulador <= 0.2f) acumulador = 2.0f;

        while (acumulador >= dt) {
            move(pos);
            update(pel, aum, mov, mar);
            acumulador -= dt;
        }

        u++;
        if (u == 35) {
            move_pel(pel, aum, mov);
            u = 0;
        }

        move_ia(poe, pel);
        colision(pos, pel, aum, poe);

        if (pel.x == WIDTH / 2 - 1 || pel.x == WIDTH / 2 + 1) red();
    }
    return 0;
}


