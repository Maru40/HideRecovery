#include"GameSaveManager.h"
#include"SoundHelper.h"

namespace basecross
{
	const std::wstring GameSaveManager::saveDir = L"..\\Saves\\";
	const std::wstring GameSaveManager::m_soundFilePath = L"Sound.sav";

	std::shared_ptr<SoundSetting> GameSaveManager::m_soundSetting = std::make_unique<SoundSetting>(GameSaveManager::saveDir + GameSaveManager::m_soundFilePath);

	std::shared_ptr<SoundSetting>& GameSaveManager::GetSoundSetting()
	{
		return m_soundSetting;
	}
}