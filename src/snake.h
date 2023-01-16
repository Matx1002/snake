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

    void newFood();
    void moveSnake();
    void growSnake();

public:
    CSnake(CRect r, char _c = ' ') : CFramedWindow(r, _c) {
        snake.push_back(CPoint(10, 10));
        snake.push_back(CPoint(10, 11));
        snake.push_back(CPoint(10, 12));
    }

    void paint() {
        CFramedWindow::paint();
        
        for (auto i = snake.begin(); i != snake.end(); i++) {
            gotoyx(i->y + geom.topleft.y, i->x + geom.topleft.x);
            if(i == snake.begin())
                printc('@');
            else
                printc('#');
            
        }
    }

    bool handleEvent(int key) {
        timeout(1000);
        switch (key) {
            case KEY_UP:
                dir = KEY_UP;
                break;
            case KEY_DOWN:
                dir = KEY_DOWN;
                break;
            case KEY_RIGHT:
                dir = KEY_RIGHT;
                break;
            case KEY_LEFT:
                dir = KEY_LEFT;
                break;
        };
        moveSake();
        return true;
    }

    void moveSake() {
        CPoint head = snake.front();
        CPoint tail = snake.back();
        snake.pop_back();
        switch (dir) {
            case KEY_UP:
                head.y--;
                break;
            case KEY_DOWN:
                head.y++;
                break;
            case KEY_RIGHT:
                head.x++;
                break;
            case KEY_LEFT:
                head.x--;
                break;
        };
        snake.push_front(head);

    }
};

#endif
