#ifndef __SNAKE_H__
#define __SNAKE_H__

#include "screen.h"
#include "winsys.h"
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

    void newFood();
    void moveSnake();
    void growSnake();

public:
    CSnake(CRect r, char _c = ' ') : CFramedWindow(r, _c) {
        snake.push_back(CPoint(10, 10));
        snake.push_back(CPoint(10, 11));
        snake.push_back(CPoint(10, 12));
    }

    void pauseMenu() {
    }

    void paint() {
        CFramedWindow::paint();


        for (auto i = snake.begin(); i != snake.end(); i++) {
            gotoyx(i->y + geom.topleft.y, i->x + geom.topleft.x);
            if (i == snake.begin())
                printc('@');
            else
                printc('#');
        }

        if (pause) {
            gotoyx(geom.topleft.y + geom.size.y / 2, geom.topleft.x + geom.size.x / 2);
            printl("PAUSE");
            return;
        }

        // // print score
        // gotoyx(geom.topleft.y + geom.size.y + 1, geom.topleft.x);
        // printl("Score: %d", score);
    }

    bool handleEvent(int key) {
        timeout(1000);
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
                pause = !pause;
                break;
        };
        if (pause == false) moveSake();
        return true;
    }

    void moveSake() {
        CPoint head = snake.front();
        CPoint tail = snake.back();
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
