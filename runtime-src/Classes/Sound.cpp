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
	preloadBackgroundMusic("sound/sd_bg_play.mp3");
	preloadBackgroundMusic("sound/sd_menu_bg.mp3");
	preloadEffect("sound/sd_effect_attack.ogg");
	preloadEffect("sound/sd_effect_btn.ogg");
	preloadEffect("sound/sd_effect_btnleft.ogg");
	preloadEffect("sound/sd_effect_btnright.ogg");
	preloadEffect("sound/sd_effect_dead.ogg");
	preloadEffect("sound/sd_effect_disapper.ogg");
	preloadEffect("sound/sd_effect_doorclose.ogg");
	preloadEffect("sound/sd_effect_dooropen.ogg");
	preloadEffect("sound/sd_effect_fallground.ogg");
	preloadEffect("sound/sd_effect_getbei.ogg");
	preloadEffect("sound/sd_effect_getKey.ogg");
	preloadEffect("sound/sd_effect_indoor.ogg");
	preloadEffect("sound/sd_effect_minstep.ogg");
	preloadEffect("sound/sd_effect_nogetbei.ogg");
}

void Sound::playMenuBackground()
{
	if (readBoolXML(MUSIC))
	{
		if (isBackground())
		{
			stopBackground();
		}
		playBackgroundMusic("sound/sd_menu_bg.mp3",true);
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
		playBackgroundMusic("sound/sd_bg_play.mp3", true);
	}
}

void Sound::playAttack()
{
	if (readBoolXML(MUSIC))
	{
		playEffect("sound/sd_effect_attack.ogg");
	}
}

void Sound::playBtn()
{
	if (readBoolXML(MUSIC))
	{
		playEffect("sound/sd_effect_btn.ogg");
	}
}

void Sound::playBtnLeft()
{
	if (readBoolXML(MUSIC))
	{
		playEffect("sound/sd_effect_btnleft.ogg");
	}
}

void Sound::playBtnRight()
{
	if (readBoolXML(MUSIC))
	{
		playEffect("sound/sd_effect_btnright.ogg");
	}
}

void Sound::playDead()
{
	if (readBoolXML(MUSIC))
	{
		playEffect("sound/sd_effect_dead.ogg");
	}
}

void Sound::playDisapper()
{
	if (readBoolXML(MUSIC))
	{
		playEffect("sound/sd_effect_disapper.ogg");
	}
}

void Sound::playDoorOpen()
{
	if (readBoolXML(MUSIC))
	{
		playEffect("sound/sd_effect_dooropen.ogg");
	}
}

void Sound::playDoorClose()
{
	if (readBoolXML(MUSIC))
	{
		playEffect("sound/sd_effect_doorclose.ogg");
	}
}

void Sound::playFallGround()
{
	if (readBoolXML(MUSIC))
	{
		playEffect("sound/sd_effect_fallground.ogg");
	}
}

void Sound::playGetKey()
{
	if (readBoolXML(MUSIC))
	{
		playEffect("sound/sd_effect_getKey.ogg");
	}
}

void Sound::playInDoor()
{
	if (readBoolXML(MUSIC))
	{
		playEffect("sound/sd_effect_indoor.ogg");
	}
}

void Sound::playMinStep()
{
	if (readBoolXML(MUSIC))
	{
		playEffect("sound/sd_effect_minstep.ogg");
	}
}

void Sound::playGetBei()
{
	if (readBoolXML(MUSIC))
	{
		playEffect("sound/sd_effect_getbei.ogg");
	}
}

void Sound::playNoGetBei()
{
	if (readBoolXML(MUSIC))
	{
		playEffect("sound/sd_effect_nogetbei.ogg");
	}
}

bool Sound::isBackground()
{
	return SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying();
	return true;
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
	unloadEffect("sound/sd_bg_play.mp3");
	unloadEffect("sound/sd_menu_bg.mp3");
	unloadEffect("sound/sd_effect_attack.ogg");
	unloadEffect("sound/sd_effect_btn.ogg");
	unloadEffect("sound/sd_effect_btnleft.ogg");
	unloadEffect("sound/sd_effect_btnright.ogg");
	unloadEffect("sound/sd_effect_dead.ogg");
	unloadEffect("sound/sd_effect_disapper.ogg");
	unloadEffect("sound/sd_effect_doorclose.ogg");
	unloadEffect("sound/sd_effect_dooropen.ogg");
	unloadEffect("sound/sd_effect_fallground.ogg");
	unloadEffect("sound/sd_effect_getbei.ogg");
	unloadEffect("sound/sd_effect_getKey.ogg");
	unloadEffect("sound/sd_effect_indoor.ogg");
	unloadEffect("sound/sd_effect_minstep.ogg");
	unloadEffect("sound/sd_effect_nogetbei.ogg");
}


