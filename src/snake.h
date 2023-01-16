#ifndef __SNAKE_H__
#define __SNAKE_H__

#include "screen.h"
#include "winsys.h"
#include <cstdlib>
#include <ctime>
#include <curses.h>
#include <list>
#include <unistd.h>


class CSnake : public CFramedWindow {
private:
    int dir = KEY_UP;
    CPoint food;
    list<CPoint> snake;
    int score = 0;

    bool pause = true;
    bool reset = false;
    bool help_menu = true;
    bool game_over = false;
    int level = 1;

    void newFood() {
        srand(time(NULL));
        food.x = rand() % (geom.size.x - 2) - 1;
        food.y = rand() % (geom.size.y - 2) + 1;
    }

    bool eatFood() {
        if (snake.front().x == food.x && snake.front().y == food.y) {
            // growSnake();
            newFood();
            score++;
            if (score % 5 == 0) {
                level++;
            }
            return true;
        }
        return false;
    }

    void growSnake() {
        CPoint tail = snake.back();
        snake.push_back(tail);
    }

    void checkCollisionOfSnakeItself() {
        auto it = snake.begin();
        it++;
        for (; it != snake.end(); it++) {
            if (snake.front().x == it->x && snake.front().y == it->y) {
                game_over = true;
            }
        }
    }

    void gameOverMenu() {
        gotoyx(geom.topleft.y + geom.size.y / 2 - 3, geom.topleft.x + geom.size.x / 2 - 10);
        printl("GAME OVER");
        gotoyx(geom.topleft.y + geom.size.y / 2 - 2, geom.topleft.x + geom.size.x / 2 - 10);
        printl("Your score is: %d", score);
        gotoyx(geom.topleft.y + geom.size.y / 2 - 1, geom.topleft.x + geom.size.x / 2 - 10);
        printl("Press 'r' to reset the game");
        gotoyx(geom.topleft.y + geom.size.y / 2, geom.topleft.x + geom.size.x / 2 - 10);
        printl("Press 'q' to quit the game");
    }

public:
    CSnake(CRect r, char _c = ' ') : CFramedWindow(r, _c) {
        snake.push_back(CPoint(7, 10));
        snake.push_back(CPoint(7, 11));
        snake.push_back(CPoint(7, 12));
        newFood();
    }


    void paint() {

        CFramedWindow::paint();

        if (pause) {
            // gotoyx(geom.topleft.y + geom.size.y / 2, geom.topleft.x + geom.size.x / 2);
            // printl("PAUSE");
        }


        if (reset) {
            snake.clear();
            snake.push_back(CPoint(7, 10));
            snake.push_back(CPoint(7, 11));
            snake.push_back(CPoint(7, 12));
            dir = KEY_UP;
            score = 0;
            reset = false;
            game_over = false;
            pause = true;
            help_menu = true;
        }

        // eatFood();


        gotoyx(geom.topleft.y + food.y, geom.topleft.x + food.x);
        printc('*');

        for (auto i = snake.begin(); i != snake.end(); i++) {
            gotoyx(i->y + geom.topleft.y, i->x + geom.topleft.x);
            if (i == snake.begin())
                printc('@');
            else
                printc('#');
        }


        if (help_menu) {
            gotoyx(geom.topleft.y + geom.size.y / 2 - 2, geom.topleft.x + geom.size.x / 2 - 12);
            printl("Press 'h' to show/hide this menu");
            gotoyx(geom.topleft.y + geom.size.y / 2 - 1, geom.topleft.x + geom.size.x / 2 - 12);
            printl("Press 'p' to toggle pause the game");
            gotoyx(geom.topleft.y + geom.size.y / 2 + 1, geom.topleft.x + geom.size.x / 2 - 12);
            printl("Press 'r' to reset the game");
            gotoyx(geom.topleft.y + geom.size.y / 2 + 2, geom.topleft.x + geom.size.x / 2 - 12);
            printl("Press 'q' to quit the game");
        }

        if (game_over) {
            pause = true;
            gameOverMenu();
        }

        gotoyx(geom.topleft.y - 1, geom.topleft.x);
        printl("|Level: %d|", level);
    }

    bool handleEvent(int key) {

        timeout(1000 - 50 * level);
        switch (key) {
            case KEY_UP:
                if (dir == KEY_DOWN)
                    break;
                dir = KEY_UP;
                break;
            case KEY_DOWN:
                if (dir == KEY_UP)
                    break;
                dir = KEY_DOWN;
                break;
            case KEY_RIGHT:
                if (dir == KEY_LEFT)
                    break;
                dir = KEY_RIGHT;
                break;
            case KEY_LEFT:
                if (dir == KEY_RIGHT)
                    break;
                dir = KEY_LEFT;
                break;
            case 'p':
                if (help_menu)
                    break;
                pause = !pause;
                return true;
            case 'r':
                reset = true;
                game_over = false;
                help_menu = true;
                return true;
            case 'h':
                help_menu = !help_menu;
                if (help_menu)
                    pause = true;
                else
                    pause = false;
                return true;
        };

        if (pause == false) moveSake();
        if (pause)
            return CFramedWindow::handleEvent(key);
        else
            checkCollisionOfSnakeItself();

        return true;
    }

    void moveSake() {
        CPoint head = snake.front();
        // CPoint tail = snake.back();
        if (eatFood() == false)
            snake.pop_back();

        switch (dir) {
            case KEY_UP:
                head.y--;
                if (head.y < 1)
                    head.y = geom.size.y - 2;
                break;
            case KEY_DOWN:
                head.y++;
                if (head.y > geom.size.y - 2)
                    head.y = 1;
                break;
            case KEY_RIGHT:
                head.x++;
                if (head.x > geom.size.x - 2)
                    head.x = 1;
                break;
            case KEY_LEFT:
                head.x--;
                if (head.x < 1)
                    head.x = geom.size.x - 2;
                break;
        };
        snake.push_front(head);
    }
};

#endif
