#pragma once

namespace basecross
{
	class SoundSetting;

	class GameSaveManager
	{
		static const std::wstring saveDir;
		static const std::wstring m_soundFilePath;
		
		static std::shared_ptr<SoundSetting> m_soundSetting;

	public:

		static std::shared_ptr<SoundSetting>& GetSoundSetting();
	};
}