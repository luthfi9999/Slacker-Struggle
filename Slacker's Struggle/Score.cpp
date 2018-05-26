#include "Score.h"
#include <cstdio>
Score::Score() {
	score = 0;
}

Score::~Score() {

}

void Score::incScore() {
	printf("waaaaa");
	score += point;
}

int Score::getScore() {
	return score;
}

void Score::checkScore(float a) {
	boxPos = a;
	if (boxPos > 600) {
		lock = true;
	}
	if (isLocked()) {
		if (isScore()) {
			incScore();
			lock = false;
		}
	}
}

bool Score::isLocked() {
	return lock;
}

bool Score::isScore() {
	return(boxPos<0);
}