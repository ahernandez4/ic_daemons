#ifndef GAMESCENE_H
#define GAMESCENE_H

class GameScene{
    protected:
	int playerx;
	int playery;
    public:
    GameScene(){}
	//GameScene(int*,int*);
	virtual ~GameScene(){};
	//virtual Update();
	virtual void Draw() = 0;
};
#endif
