#include "Sound.h"
//#include "audio/include/AudioEngine.h"
//#include "editor-support/cocostudio/SimpleAudioEngine.h"

using namespace CocosDenshion;
Sound* Sound::_instance = nullptr;

Sound* Sound::getInstance()
{
	if (!_instance)
	{
		_instance = new Sound;
	}
	return _instance;
}

void Sound::init()
{
	_preloadBackgroundMusic("sound/sd_bg_play.mp3");
	_preloadBackgroundMusic("sound/sd_menu_bg.mp3");
	_preloadEffect("sound/sd_effect_attack.ogg");
	_preloadEffect("sound/sd_effect_btn.ogg");
	_preloadEffect("sound/sd_effect_btnleft.ogg");
	_preloadEffect("sound/sd_effect_btnright.ogg");
	_preloadEffect("sound/sd_effect_dead.ogg");
	_preloadEffect("sound/sd_effect_disapper.ogg");
	_preloadEffect("sound/sd_effect_doorclose.ogg");
	_preloadEffect("sound/sd_effect_dooropen.ogg");
	_preloadEffect("sound/sd_effect_fallground.ogg");
	_preloadEffect("sound/sd_effect_getbei.ogg");
	_preloadEffect("sound/sd_effect_getKey.ogg");
	_preloadEffect("sound/sd_effect_indoor.ogg");
	_preloadEffect("sound/sd_effect_minstep.ogg");
	_preloadEffect("sound/sd_effect_nogetbei.ogg");
}

void Sound::playMenuBackground()
{
	if (readBoolXML(MUSIC))
	{
		if (isBackground())
		{
			stopBackground();
		}
		_playBackgroundMusic("sound/sd_menu_bg.mp3",true);
	}
}

void Sound::playGameBackground()
{
	if (readBoolXML(MUSIC))
	{
		if (isBackground())
		{
			stopBackground();
		}
		_playBackgroundMusic("sound/sd_bg_play.mp3", true);
	}
}

void Sound::playAttack()
{
	if (readBoolXML(MUSIC))
	{
		_playEffect("sound/sd_effect_attack.ogg");
	}
}

void Sound::playBtn()
{
	if (readBoolXML(MUSIC))
	{
		_playEffect("sound/sd_effect_btn.ogg");
	}
}

void Sound::playBtnLeft()
{
	if (readBoolXML(MUSIC))
	{
		_playEffect("sound/sd_effect_btnleft.ogg");
	}
}

void Sound::playBtnRight()
{
	if (readBoolXML(MUSIC))
	{
		_playEffect("sound/sd_effect_btnright.ogg");
	}
}

void Sound::playDead()
{
	if (readBoolXML(MUSIC))
	{
		_playEffect("sound/sd_effect_dead.ogg");
	}
}

void Sound::playDisapper()
{
	if (readBoolXML(MUSIC))
	{
		_playEffect("sound/sd_effect_disapper.ogg");
	}
}

void Sound::playDoorOpen()
{
	if (readBoolXML(MUSIC))
	{
		_playEffect("sound/sd_effect_dooropen.ogg");
	}
}

void Sound::playDoorClose()
{
	if (readBoolXML(MUSIC))
	{
		_playEffect("sound/sd_effect_doorclose.ogg");
	}
}

void Sound::playFallGround()
{
	if (readBoolXML(MUSIC))
	{
		_playEffect("sound/sd_effect_fallground.ogg");
	}
}

void Sound::playGetKey()
{
	if (readBoolXML(MUSIC))
	{
		_playEffect("sound/sd_effect_getKey.ogg");
	}
}

void Sound::playInDoor()
{
	if (readBoolXML(MUSIC))
	{
		_playEffect("sound/sd_effect_indoor.ogg");
	}
}

void Sound::playMinStep()
{
	if (readBoolXML(MUSIC))
	{
		_playEffect("sound/sd_effect_minstep.ogg");
	}
}

void Sound::playGetBei()
{
	if (readBoolXML(MUSIC))
	{
		_playEffect("sound/sd_effect_getbei.ogg");
	}
}

void Sound::playNoGetBei()
{
	if (readBoolXML(MUSIC))
	{
		_playEffect("sound/sd_effect_nogetbei.ogg");
	}
}

bool Sound::isBackground()
{
	return SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying();
}

void Sound::stopBackground()
{
	if (isBackground())
	{
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	}	
}

void Sound::UnInit()
{
	_unloadEffect("sound/sd_bg_play.mp3");
	_unloadEffect("sound/sd_menu_bg.mp3");
	_unloadEffect("sound/sd_effect_attack.ogg");
	_unloadEffect("sound/sd_effect_btn.ogg");
	_unloadEffect("sound/sd_effect_btnleft.ogg");
	_unloadEffect("sound/sd_effect_btnright.ogg");
	_unloadEffect("sound/sd_effect_dead.ogg");
	_unloadEffect("sound/sd_effect_disapper.ogg");
	_unloadEffect("sound/sd_effect_doorclose.ogg");
	_unloadEffect("sound/sd_effect_dooropen.ogg");
	_unloadEffect("sound/sd_effect_fallground.ogg");
	_unloadEffect("sound/sd_effect_getbei.ogg");
	_unloadEffect("sound/sd_effect_getKey.ogg");
	_unloadEffect("sound/sd_effect_indoor.ogg");
	_unloadEffect("sound/sd_effect_minstep.ogg");
	_unloadEffect("sound/sd_effect_nogetbei.ogg");
}


