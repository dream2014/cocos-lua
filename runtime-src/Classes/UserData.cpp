#include "UserData.h"
#include "GameConst.h"
#include <ctime>
#include <fstream>
#include "scripting/deprecated/CCDeprecated.h"

static UserData* s_userData;

int GetTime();

UserData::~UserData()
{
	saveData();
	s_userData = nullptr;
}

UserData* UserData::getInstance()
{
	if (s_userData == nullptr)
	{
		s_userData = new UserData();
		s_userData->init();
	}
	return s_userData;
}

bool UserData::init()
{
	
	if (UserDefault::getInstance()->getBoolForKey("Beginner", true))
	{
		reset();
		_isBeginner = false;
		_beginner = true;
		return true;
	}
	else
	{
		_score = UserDefault::getInstance()->getIntegerForKey("Score", 0);
		_level = UserDefault::getInstance()->getIntegerForKey("Level", 0);
		_isMusicOn = true;
		auto day = UserDefault::getInstance()->getIntegerForKey("Time", 0);
		if (day == GetTime())
		{
			_haveRaffle = UserDefault::getInstance()->getBoolForKey("HaveRaffle");
		}
		else
		{
			_haveRaffle = false;
			UserDefault::getInstance()->setIntegerForKey("Time", GetTime());
		}
		_userProp = true;
		_beginner = false;
		_bomb = UserDefault::getInstance()->getIntegerForKey("Bomb", 0);
		_colorBubble = UserDefault::getInstance()->getIntegerForKey("ColorBubble", 0);
		_hourglass = UserDefault::getInstance()->getIntegerForKey("Hourglass", 0);
		_gold = UserDefault::getInstance()->getIntegerForKey("Gold", 0);
		_bestScore = UserDefault::getInstance()->getIntegerForKey("BestScore", 0);
		_rebirth = UserDefault::getInstance()->getIntegerForKey("Rebirth", 0);
		_remainingTime = UserDefault::getInstance()->getIntegerForKey("RETime", 0);
		_isGameOver = UserDefault::getInstance()->getBoolForKey("IsGameOver", true);
		_newGift = UserDefault::getInstance()->getBoolForKey("NewGift", false);
		std::string documentPath = CCFileUtils::getInstance()->getWritablePath();
		string fileName = "CurrentBorad.dat";
		string filePath = documentPath + fileName;
		CCLOG("path %s", filePath.c_str());
		string path = filePath;
		ifstream fp(path.c_str());
		for (int i = 0; i < MAX_ROWS; i++)
		{
			for (int j = 0; j < MAX_COLS; j++)
			{
				char temp;
				fp >> temp;
				_boradArray[i][j] = temp - '0';
			}
		}
		fp.close();
	}
	return true;
}

void UserData::reset()
{
	_userProp = true;
	_bomb = 0;
	_colorBubble = 0;
	_gold = 0;
	_hourglass = 0;
	_isBeginner = true;
	_beginner = true;
	_score = 0;
	_bestScore = 0;
	_raffle = true;
	_haveRaffle = false;
	_remainingTime = 60;
	_level = 0;
	_isGameOver = true;
	_isMusicOn = true;
	_newGift = true;
	for (int i = 0; i < MAX_ROWS; i++)
	{
		for (int j = 0; j < MAX_COLS; j++)
		{
			_boradArray[i][j] = customs[0][i][j];
		}
	}
}

void UserData::newReset()
{
	_remainingTime = 61;
	_level = 1;
	_score = 0;
}


void UserData::saveData()
{
	UserDefault::getInstance()->setBoolForKey("Beginner", _isBeginner);
	UserDefault::getInstance()->setIntegerForKey("Bomb", _bomb);
	UserDefault::getInstance()->setIntegerForKey("ColorBubble", _colorBubble);
	UserDefault::getInstance()->setIntegerForKey("Hourglass", _hourglass);
	UserDefault::getInstance()->setIntegerForKey("Gold", _gold);
	UserDefault::getInstance()->setIntegerForKey("BestScore", _bestScore);
	UserDefault::getInstance()->setBoolForKey("HaveRaffle", _haveRaffle);
	UserDefault::getInstance()->setIntegerForKey("Rebirth", _rebirth);
	UserDefault::getInstance()->setIntegerForKey("RETime", _remainingTime);
	UserDefault::getInstance()->setIntegerForKey("Score", _score);
	UserDefault::getInstance()->setIntegerForKey("Level", _level);
	UserDefault::getInstance()->setBoolForKey("IsGameOver", _isGameOver);
	UserDefault::getInstance()->setBoolForKey("NewGift", _newGift);
	std::string documentPath = CCFileUtils::getInstance()->getWritablePath();
	string fileName = "CurrentBorad.dat";
	string filePath = documentPath + fileName;
	CCLOG("path %s", filePath.c_str());
	string path = filePath;
	ofstream fp(path.c_str());
	for (int i = 0; i < MAX_ROWS; i++)
	{
		for (int j = 0; j < MAX_COLS; j++)
		{
			auto temp = _boradArray[i][j];
			fp << temp;
		}
	}
	fp.close();

	UserDefault::getInstance()->flush();
}
void UserData::addScore(int var)
{
	setScore(getScore() + var);
	if (getScore() > getBestScore())
	{
		setBestScore(getScore());
	}
	saveData();
}
int GetTime()
{
	time_t timep;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	time(&timep);
#else
	struct timeval now;
	gettimeofday(&now, NULL);
	timep = now.tv_sec;
#endif

	struct tm tm1;
	tm1.tm_year = 2015 - 1900;
	tm1.tm_mon = 3 - 1;
	tm1.tm_mday = 1;
	tm1.tm_hour = 0;
	tm1.tm_min = 0;
	tm1.tm_sec = 0;
	auto time = mktime(&tm1);

	double diff = difftime(timep, time);
	return (int)(diff / (3600 * 24));
}