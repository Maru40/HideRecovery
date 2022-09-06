/*!
@file SoundManager.cpp
@brief SoundManagerのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "SoundManager.h"

#include "Maruyama/Debug/Object/DebugObject.h"

namespace basecross {

	using namespace maru;

	std::weak_ptr<SoundManager> SingletonComponent<SoundManager>::sm_instance; //スタティックメンバ変数の初期化

	SoundManager::SoundManager(const std::shared_ptr<GameObject>& objPtr)
		:maru::SingletonComponent<SoundManager>(objPtr), m_baseVolume(1.0f)
	{}

	void SoundManager::OnCreate() {
		SettingSoundMap();
	}

	void SoundManager::OnDestroy() {
		AllStop();
		SingletonComponent::OnDestroy();
	}

	void SoundManager::SettingSoundMap() {
		m_soundMap[ID::SlimePressing] = L"SlimePressing_SE";
		m_soundMap[ID::SlimePressed] = L"SlimePressed_SE";
		m_soundMap[ID::ClearSE] = L"Clear_SE";
		m_soundMap[ID::BackSE] = L"Back_SE";
		m_soundMap[ID::NextSE] = L"Next_SE";
		m_soundMap[ID::CursorSE] = L"CursorSE";
		m_soundMap[ID::EnterSE] = L"EnterSE";

		m_soundMap[ID::TitleBGM] = L"Title_BGM_01";
		m_soundMap[ID::SelectBGM] = L"Select_BGM_01";
		m_soundMap[ID::GameBGM] = L"Game_BGM_01";

		m_soundMap[ID::PlayerWalk] = L"PlayerWalk";
		m_soundMap[ID::PlayerTackle] = L"PlayerTackle";
		m_soundMap[ID::JettPomp] = L"JettPomp";

		m_soundMap[ID::ShotSE] = L"Shot_SE";
	}

	void SoundManager::AddSoundItem(const ID& id, const std::shared_ptr<SoundItem>& item) {
		m_items[id] = item;
	}

	//--------------------------------------------------------------------------------------
	/// アクセッサ
	//--------------------------------------------------------------------------------------

	std::shared_ptr<SoundItem> SoundManager::PlayOneShot(const ID& id, const float& volume, const float& LoopCount) {
		if (!m_soundMap.count(id)) {  //particleが存在しないなら
			DebugObject::AddString(L"idが存在しません");
			return nullptr;
		}
		
		auto loopCount = static_cast<size_t>(LoopCount);
		auto ptrXA = App::GetApp()->GetXAudio2Manager();
		auto item = ptrXA->Start(m_soundMap[id], loopCount, volume * m_baseVolume);
		AddSoundItem(id, item);
		return item;
	}

	std::shared_ptr<SoundItem> SoundManager::PlayBGM(const ID& id, const float& volume, const float& LoopCount) {
		if (!m_soundMap.count(id)) {  //particleが存在しないなら
			DebugObject::AddString(L"idが存在しません");
			return nullptr;
		}

		auto loopCount = static_cast<size_t>(LoopCount);
		auto ptrXA = App::GetApp()->GetXAudio2Manager();
		auto item = ptrXA->Start(m_soundMap[id], loopCount, volume * m_baseVolume);
		AddSoundItem(id, item);
		return item;
	}

	void SoundManager::Stop(const ID& id) {
		auto ptrXA = App::GetApp()->GetXAudio2Manager();
		//存在するなら、かつ、nullptrでないなら
		if (m_items.count(id) && m_items[id].GetShard() != nullptr) {
			ptrXA->Stop(m_items[id].GetShard());
			m_items[id] = nullptr;
		}
	}

	void SoundManager::AllStop() {
		for (auto& pair : m_items) {
			Stop(pair.first);
		}
	}
}

//endbasecross