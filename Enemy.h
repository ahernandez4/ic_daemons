#ifndef ENEMY_H
#define ENEMY_H
//maybe some of these should be private
class Enemy{
    private:
    GLuint enemyTexture;
    bool moving;
    bool alive;
    public:
    //Enemy(GLuint atexture);
    int health;
    int xpos;
    int ypos;
    bool isMoving();
    bool isAlive();
    void Draw();
    void Spawn(int x, int y);
    void updatePosition();
    void hit(int h);
    bool checkAreaCollision(int x1,int x2,int y1,int y2);
    //bool Died();
};

#endif
