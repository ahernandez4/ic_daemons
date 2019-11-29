#ifndef ENEMY_H
#define ENEMY_H
//maybe some of these should be private
class Enemy{
    private:
    GLuint enemyTexture;
    public:
    //Enemy(GLuint atexture);
    int health;
    int xpos;
    int ypos;
    void Draw();
    void Spawn(int x, int y);
    bool Died();
};

#endif
