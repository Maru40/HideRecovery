#include"SoundHelper.h"
#include"MathHelper.h"
#include"GameSaveManager.h"

using itbs::Math::MyMath;

namespace basecross
{
	// SoundSetting ---------------------------
	
	SoundSetting::SoundSetting(const std::wstring& filePath) :
		SavableBase(m_soundSettingData, filePath)
	{
		m_soundSettingData.bgmVolume = 1.0f;
		m_soundSettingData.seVolume = 1.0f;
	}
	
	void SoundSetting::SetBGMVolume(const float bgmVolume)
	{
		m_soundSettingData.bgmVolume = MyMath::Clamp(bgmVolume, 0.0f, 1.0f);
		changeEvent();
	}

	void SoundSetting::SetSEVolume(const float seVolume)
	{
		m_soundSettingData.seVolume = MyMath::Clamp(seVolume, 0.0f, 1.0f);
		changeEvent();
	}

	void SoundSetting::AddBGMVolume(const float addVolume)
	{
		SetBGMVolume(m_soundSettingData.bgmVolume + addVolume);
		changeEvent();
	}

	void SoundSetting::AddSEVolume(const float addVolume)
	{
		SetSEVolume(m_soundSettingData.seVolume + addVolume);
		changeEvent();
	}

	float SoundSetting::GetBGMVolume() const
	{
		return m_soundSettingData.bgmVolume;
	}

	float SoundSetting::GetSEVolume() const
	{
		return m_soundSettingData.seVolume;
	}

	// SoundClip ------------------------------
	
	SoundClip::SoundClip() : SoundClip(L"", false, 1.0f) {}

	SoundClip::SoundClip(const std::wstring& soundName, const bool isLoop, const float volume) :
		soundName(soundName),
		isLoop(isLoop),
		volume(volume)
	{

	}

	// SoundItemVolume ------------------------------

	SoundData::SoundData() :
		SoundData(nullptr, 0.0f, L"")
	{

	}

	SoundData::SoundData(const std::shared_ptr<SoundItem>& soundItem, const float volume, const std::wstring& soundKey) :
		soundItem(soundItem),
		volume(volume),
		soundKey(soundKey)
	{

	}

	// SimpleSoundManager ---------------------------


	bool SimpleSoundManager::m_canSetting = false;

	SoundData SimpleSoundManager::m_bgmSoundData;

	std::vector<SoundData> SimpleSoundManager::m_seSoundDatas;

	std::shared_ptr<SoundItem> SimpleSoundManager::CreateSoundItem(const SoundClip& soundClip)
	{
		if (!m_canSetting)
		{
			GameSaveManager::GetSoundSetting()->changeEvent.AddFunc(UpdateSoundItemVolume);

			m_canSetting = true;
		}

		auto& xAudio2Manager = App::GetApp()->GetXAudio2Manager();

		UINT32 loopCount = soundClip.isLoop ? XAUDIO2_LOOP_INFINITE : 0;

		float volume = MyMath::Clamp(soundClip.volume, 0.0f, 1.0f);

		auto soundItem = xAudio2Manager->Start(soundClip.soundName, loopCount, volume);

		return soundItem;
	}

	void SimpleSoundManager::UpdateSoundItemVolume()
	{
		if (m_bgmSoundData.soundItem->m_SourceVoice)
		{
			SettingSoundVolume(m_bgmSoundData, GameSaveManager::GetSoundSetting()->GetBGMVolume());
		}

		auto seUpdateFunc =
			[](SoundData& soundItemAndVolume) {SettingSoundVolume(soundItemAndVolume, GameSaveManager::GetSoundSetting()->GetSEVolume()); };

		UpdateSESoundItemAndVolumes(seUpdateFunc);
	}

	void SimpleSoundManager::SettingSoundVolume(const SoundData& soundItemAndVolume, const float baseVolume)
	{
		soundItemAndVolume.soundItem->m_SourceVoice->SetVolume(soundItemAndVolume.volume * baseVolume);
	}

	void SimpleSoundManager::UpdateSESoundItemAndVolumes(void(*func)(SoundData&))
	{
		auto it = m_seSoundDatas.begin();

		while (it != m_seSoundDatas.end())
		{
			auto& soundItemAndVolume = (*it);

			if (!soundItemAndVolume.soundItem->m_SourceVoice)
			{
				it = m_seSoundDatas.erase(it);
				continue;
			}

			func(soundItemAndVolume);

			it++;
		}
	}

	void SimpleSoundManager::ChangeBGM(const std::wstring& bgmName, const float volume)
	{
		auto& soundItem = m_bgmSoundData.soundItem;

		if (soundItem && soundItem->m_SourceVoice)
		{
			soundItem->m_SourceVoice->Stop();
		}

		m_bgmSoundData.volume = volume;

		m_bgmSoundData.soundKey = bgmName;

		SoundClip soundClip = SoundClip(bgmName, true, volume * GameSaveManager::GetSoundSetting()->GetBGMVolume());

		soundItem = CreateSoundItem(soundClip);
	}

	void SimpleSoundManager::PauseBGM()
	{
		auto& soundItem = m_bgmSoundData.soundItem;

		if (!soundItem || !soundItem->m_SourceVoice)
		{
			return;
		}

		soundItem->m_SourceVoice->Stop();
	}

	void SimpleSoundManager::UnPauseBGM()
	{
		auto& soundItem = m_bgmSoundData.soundItem;

		if (!soundItem || !soundItem->m_SourceVoice)
		{
			return;
		}

		soundItem->m_SourceVoice->Start();
	}

	void SimpleSoundManager::StopBGM()
	{
		auto& soundItem = m_bgmSoundData.soundItem;

		if (!soundItem || !soundItem->m_SourceVoice)
		{
			return;
		}

		soundItem->m_SourceVoice->Stop();
		soundItem->m_SourceVoice->FlushSourceBuffers();
	}

	bool SimpleSoundManager::IsPlayingBGM(const std::wstring& bgmName)
	{
		return m_bgmSoundData.soundKey == bgmName;
	}

	void SimpleSoundManager::OnePlaySE(const wstring& soundName, const float volume)
	{
		auto& xAudio2Manager = App::GetApp()->GetXAudio2Manager();

		auto setVolume = MyMath::Clamp(volume, 0.0f, 1.0f);

		auto soundItem = xAudio2Manager->Start(soundName, 0, setVolume * GameSaveManager::GetSoundSetting()->GetSEVolume());

		auto soundItemAndVolume = SoundData(soundItem, setVolume, soundName);

		m_seSoundDatas.push_back(soundItemAndVolume);
	}

	void SimpleSoundManager::PauseSE()
	{
		auto seUpdateFunc =
			[](SoundData& soundItemAndVolume) {soundItemAndVolume.soundItem->m_SourceVoice->Stop(); };

		UpdateSESoundItemAndVolumes(seUpdateFunc);
	}

	void SimpleSoundManager::UnPauseSE()
	{
		auto seUpdateFunc =
			[](SoundData& soundItemAndVolume) {soundItemAndVolume.soundItem->m_SourceVoice->Start(); };

		UpdateSESoundItemAndVolumes(seUpdateFunc);
	}

	void SimpleSoundManager::StopSE()
	{
		auto seUpdateFunc =
			[](SoundData& soundItemAndVolume)
		{
			soundItemAndVolume.soundItem->m_SourceVoice->Stop();
			soundItemAndVolume.soundItem->m_SourceVoice->FlushSourceBuffers();
		};

		UpdateSESoundItemAndVolumes(seUpdateFunc);
	}

	// SoundListener --------------------------

	bool SoundListener::m_isInited = false;
	X3DAUDIO_HANDLE SoundListener::m_X3DAudioHandle;
	X3DAUDIO_DSP_SETTINGS SoundListener::m_dspSettring;

	void SoundListener::CheckInit()
	{
		// 初期化されていたら何もしない
		if (m_isInited)
		{
			return;
		}

		auto masteringVoice = App::GetApp()->GetXAudio2Manager()->GetMasteringVoice();

		// 出力するスピーカーのパターン(横に2個、囲むように6個など)
		DWORD channelMask;
		masteringVoice->GetChannelMask(&channelMask);

		// X3Audioの初期化を行う
		auto hr = X3DAudioInitialize(channelMask, X3DAUDIO_SPEED_OF_SOUND, m_X3DAudioHandle);

		m_isInited = (hr == S_OK);

		// 初期化が成功しなかった場合
		if (!m_isInited)
		{
			return;
		}

		XAUDIO2_VOICE_DETAILS voiceDetails;
		masteringVoice->GetVoiceDetails(&voiceDetails);

		m_dspSettring.DstChannelCount = voiceDetails.InputChannels; // 出力チャンネルの数の設定

		m_dspSettring.pMatrixCoefficients = new FLOAT32[16]{};
	}

	X3DAUDIO_LISTENER SoundListener::CreateListener() const
	{
		X3DAUDIO_LISTENER listener{};
		listener.OrientFront = transform->GetForward(); // Z軸の向き
		listener.OrientTop = transform->GetUp(); // Y軸の向き
		listener.Position = transform->GetWorldPosition(); // ワールド座標
		//listener.Velocity = transform->GetVelocity();

		return listener;
	}

	SoundListener::SoundListener(std::shared_ptr<GameObject>& owner) :
		Component(owner)
	{

	}

	const X3DAUDIO_DSP_SETTINGS& SoundListener::SoundCalculate(const X3DAUDIO_EMITTER& emitter) const
	{
		CheckInit();

		/// <summary>
		/// 初期化されていなかったら何もしない
		/// </summary>
		if (!m_isInited)
		{
			return m_dspSettring;
		}

		auto listener = CreateListener();
		X3DAUDIO_EMITTER calculateEmitter = emitter;
		m_dspSettring.SrcChannelCount = emitter.ChannelCount;

		XAUDIO2_VOICE_DETAILS details;
		App::GetApp()->GetXAudio2Manager()->GetMasteringVoice()->GetVoiceDetails(&details);

		m_dspSettring.DstChannelCount = details.InputChannels;
		UINT32 flag = X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER | X3DAUDIO_CALCULATE_LPF_DIRECT | X3DAUDIO_CALCULATE_REVERB;
		X3DAudioCalculate(m_X3DAudioHandle, &listener, &calculateEmitter, flag, &m_dspSettring);

		return m_dspSettring;
	}

	// SoundPlayer ----------------------------

	SoundEmitter::SoundEmitter(std::shared_ptr<GameObject>& owner) :
		Component(owner)
	{

	}

	X3DAUDIO_EMITTER SoundEmitter::CreateEmitter(const ex_weak_ptr<SoundItem>& soundItem) const
	{
		X3DAUDIO_EMITTER emitter{};
		emitter.OrientFront = transform->GetForward(); // Z軸方向
		emitter.OrientTop = transform->GetUp(); // Y軸方向
		emitter.Position = transform->GetWorldPosition(); // ワールド座標
		emitter.ChannelCount = 1; // おそらく音が鳴る個数 基本1
		emitter.CurveDistanceScaler = 1.0f; // 音の位置による減衰曲線のスケール

		return emitter;
	}

	void SoundEmitter::CheckListener()
	{
		if (m_listener)
		{
			return;
		}

		for (auto& gameObject : GetStage()->GetGameObjectVec())
		{
			auto listener = gameObject->GetComponent<SoundListener>(false);

			if (listener)
			{
				m_listener = listener;

				break;
			}
		}
	}


	void SoundEmitter::UpdateSoundItemAndVolume(const SoundData& soundItemAndVolume, const X3DAUDIO_DSP_SETTINGS& dspSetting) const
	{
		auto& sourceVoice = soundItemAndVolume.soundItem->m_SourceVoice;

		const auto& masterVoice = App::GetApp()->GetXAudio2Manager()->GetMasteringVoice();

		XAUDIO2_VOICE_DETAILS voiceDetails;
		sourceVoice->GetVoiceDetails(&voiceDetails);

		XAUDIO2_VOICE_DETAILS masterVoiceDetails;
		App::GetApp()->GetXAudio2Manager()->GetMasteringVoice()->GetVoiceDetails(&masterVoiceDetails);

		auto left = dspSetting.pMatrixCoefficients[0];
		auto right = dspSetting.pMatrixCoefficients[1];

		sourceVoice->SetOutputMatrix(masterVoice, voiceDetails.InputChannels, masterVoiceDetails.InputChannels, dspSetting.pMatrixCoefficients);
		sourceVoice->SetFrequencyRatio(dspSetting.DopplerFactor);

		XAUDIO2_FILTER_PARAMETERS filterParam{};

		filterParam.Type = XAUDIO2_FILTER_TYPE::LowPassFilter;
		filterParam.Frequency = std::sinf(X3DAUDIO_PI / 6.0f * dspSetting.LPFDirectCoefficient) * 2.0f;
		filterParam.OneOverQ = 1.0f;
		sourceVoice->SetFilterParameters(&filterParam);

		sourceVoice->SetVolume(soundItemAndVolume.volume * GameSaveManager::GetSoundSetting()->GetSEVolume());
	}

	void SoundEmitter::PlaySoundClip(const SoundClip& soundClip)
	{
		CheckListener();

		if (!m_listener)
		{
			return;
		}

		UINT32 loopCount = soundClip.isLoop ? XAUDIO2_LOOP_INFINITE : 0;
		float volume = MyMath::Clamp(soundClip.volume, 0.0f, 1.0f);
		auto soundItem = App::GetApp()->GetXAudio2Manager()->Start(soundClip.soundName, loopCount, volume * GameSaveManager::GetSoundSetting()->GetSEVolume());

		auto soundItemAndVolume = SoundData(soundItem, volume, soundClip.soundName);

		auto& dspSetting = m_listener->SoundCalculate(CreateEmitter(soundItem));

		UpdateSoundItemAndVolume(soundItemAndVolume, dspSetting);

		m_soundItemAndVolumes.push_back(soundItemAndVolume);
	}


	void SoundEmitter::OnStart()
	{
		CheckListener();
	}

	void SoundEmitter::OnUpdate()
	{
		CheckListener();

		if (!m_listener)
		{
			return;
		}


		auto it = m_soundItemAndVolumes.begin();

		while (it != m_soundItemAndVolumes.end())
		{
			auto& soundItem = (*it).soundItem;

			if (!soundItem->m_SourceVoice)
			{
				it = m_soundItemAndVolumes.erase(it);
				continue;
			}

			auto& dspSetting = m_listener->SoundCalculate(CreateEmitter(soundItem));

			UpdateSoundItemAndVolume((*it), dspSetting);

			it++;
		}
	}

	void SoundEmitter::OnEnable()
	{
		auto it = m_soundItemAndVolumes.begin();

		while (it != m_soundItemAndVolumes.end())
		{
			auto& soundItem = (*it).soundItem;

			if (!soundItem->m_SourceVoice)
			{
				it = m_soundItemAndVolumes.erase(it);
				continue;
			}

			soundItem->m_SourceVoice->Start();

			it++;
		}
	}

	void SoundEmitter::OnDisable()
	{
		auto it = m_soundItemAndVolumes.begin();

		while (it != m_soundItemAndVolumes.end())
		{
			auto& soundItem = (*it).soundItem;

			if (!soundItem->m_SourceVoice)
			{
				it = m_soundItemAndVolumes.erase(it);
				continue;
			}

			soundItem->m_SourceVoice->Stop();

			it++;
		}
	}
}