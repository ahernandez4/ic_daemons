#ifndef GAMESCENE_H
#define GAMESCENE_H

class GameScene{
    protected:
	int playerx;
	int playery;
    public:
    int prev_playerx;
    int prev_playery;
    bool deleteSoon;
    GameScene(){}
	//GameScene(int*,int*);
	virtual ~GameScene(){};
	//virtual Update();
	virtual void Draw() = 0;
};
#endif
