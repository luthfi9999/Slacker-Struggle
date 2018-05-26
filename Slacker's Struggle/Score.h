#pragma once
class Score {

public:
	Score();
	~Score();
	int getScore();
	void checkScore(float a);
	
private:
	void incScore();
	bool isScore();
	bool isLocked();
	int score, point=100;
	bool lock = false;
	float boxPos;
};