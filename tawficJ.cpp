//Tawfic Jobah
//My project file

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "fonts.h"
#include <iostream>
#include <fstream>

struct enemy{
    int x;
    int y;
    int moveSpeed;
    int health;

} enemy;

void tjcredits(int x, int y) //, GLuint texid)
{
    Rect r;
    r.bot = y;
    r.left = x;
    r.center = 0;
    ggprint8b(&r, 16, 0x00ffff44, "Tawfic Jobah");
}
void createEnemy()
{
    enemy.x = 220;
    enemy.y = 80;
    enemy.moveSpeed = 15;
    enemy.health = 3;
}

void updatingTime(int time)
{
    //deletes file, to rewrite so it can be updated
    if( remove( "highscores.txt" ) != 0 )
        std::cout << "error deleting file\n";
    else {
        std::ofstream myfile ("highscores.txt");
        if (myfile.is_open()) {
            myfile << '*';
            myfile << time;
            myfile.close();
        }
        else
            std::cout << "unable to open file\n";
    }
}
