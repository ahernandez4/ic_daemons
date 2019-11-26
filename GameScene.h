class GameScene{
    protected:
	int playerx;
	int playery;
    public:
	GameScene(int*,int*);
	virtual ~GameScene(){};
	//virtual Update();
	virtual void Draw() = 0;
};
