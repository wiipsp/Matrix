#include "MatrixLogic.h"


MatrixLogic::MatrixLogic(float width, float height, int spritSize)
{
	this->width = (int)width;
	this->height = (int)height;
	this->spritSize = spritSize;
	this->map = new int*[this->height];
	for (int i = 0; i < height; i++)
		map[i] = new int[this->width];

	this->countSize = this->width * this->height;
	tilesInt = new int[countSize];
	int account = 0;
	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){
			map[i][j] = 0;
			tilesInt[account++] = 1;
		}
	}
}

MatrixLogic::MatrixLogic(int width, int height, int spritSize)
{
	this->width = width;
	this->height = height;
	this->spritSize = spritSize;
	this->map = new int*[this->height];
	for (int i = 0; i < height; i++)
		map[i] = new int[this->width];

	this->countSize = height * width;
	tilesInt = new int[countSize];
	int account = 0;
	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){
			map[i][j] = 0;
			tilesInt[account++] = 1;
		}
	}
}
void MatrixLogic::reset(){
	this->countSize = height * width;
	int account = 0;
	for(int i = 0; i < height;i++){
		for(int j = 0; j < width ; j++){
			map[i][j] = 0;
			tilesInt[account++] = 1;
		}
	}
}

TileBean MatrixLogic::getSelectedTile(Vec2 positoion){
	int x = positoion.x;
	int y = positoion.y;
	if (x < 0 || y < 0 || x >= width || y >= height){
		return TileBean(Vec2(0, 0), -1);
	}
	if(map[y][x] == 0){
		return TileBean(Vec2(x, y), 0);
	}
	return TileBean(Vec2(x, y), map[y][x]);
}

MatrixLogic::~MatrixLogic(){
	for (int i = 0; i < height; i++)
		delete []map[i];
	delete []map;
}

std::vector<TileBean> MatrixLogic::addSpritInMap(int count){
	srand((unsigned)time(0));
	std::vector<TileBean> result;
	for(int i=0; i<count; i++){
		if (countSize == 0){
			return result;
		}
		int count = rand()%(countSize);
		int tem =0;
		for (int i = 0; i < height*width; i++){
			if(tilesInt[i] == 0)
				continue;
			if(tem == count){
				count = i;
				break;
			}
			tem++;
		}
		int x = count%width;
		int y = count/width;
		int spriteCount = rand()%spritSize+1;	
		map[y][x]  = spriteCount;
		tilesInt[count] = 0;
		result.push_back(TileBean(Vec2(x, y), spriteCount));
		countSize--;
	}
	return result;
}

bool MatrixLogic::isOutMap(Vec2 point){
	if(point.x < 0 || point.y < 0 || point.x >= width || point.y >= height){
		return true;
	}
	return false;
}

bool MatrixLogic::isSameValue(Vec2 point, int val){
	if(map[(int)point.y][(int)point.x] == val){
		return true;
	}
	return false;
}

std::vector<Vec2> MatrixLogic::caculateSame(TileBean targetTile, Vec2 direction){
	std::vector<Vec2> result;

	//the direction way find
	Vec2 point = targetTile.getLocation();
	do {
		result.push_back(point);
		point = point + direction;
	} while (!isOutMap(point) && isSameValue(point, targetTile.getVal()));

	//the neg direction way find
	Vec2 negDirection = -direction;
	point = targetTile.getLocation() + negDirection;
	while (!isOutMap(point) && isSameValue(point, targetTile.getVal())){
		result.push_back(point);
		point = point + negDirection;
	}
	return result;
}

std::vector<Vec2> MatrixLogic::judgeFives(TileBean targetTile){
	std::vector<Vec2> results;
	std::vector<Vec2> array0 = caculateSame(targetTile, Vec2(0, 1));
	std::vector<Vec2> array45 = caculateSame(targetTile, Vec2(1, 1));
	std::vector<Vec2> array90 = caculateSame(targetTile, Vec2(1, 0));
	std::vector<Vec2> array135 = caculateSame(targetTile, Vec2(1, -1));
	if(array0.size() > 4){
		results.insert(results.end(), array0.begin(), array0.end());
	}
	if(array45.size() > 4){
		results.insert(results.end(), array45.begin(), array45.end());
	}
	if(array90.size() > 4){
		results.insert(results.end(), array90.begin(), array90.end());
	}
	if (array135.size() > 4){
		results.insert(results.end(), array135.begin(), array135.end());
	}
	return results;
}

