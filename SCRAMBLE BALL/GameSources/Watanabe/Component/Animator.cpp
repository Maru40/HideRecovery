/*!
@file   Animator.cpp
@brief  アニメーション管理クラス実体
*/

#include "stdafx.h"
#include "Animator.h"
#include "../Utility/CSVLoad.h"
#include "../Utility/DataExtracter.h"

namespace basecross {
	Animator::Animator(const shared_ptr<GameObject>& owner)
		:Component(owner)
	{}

	vector<AnimationClip> Animator::LoadAnimationData(const wstring& csvKey) {
		if (!CSVLoad::GetInstance()->CheckData(csvKey)) {
			throw BaseException(
				L"指定したキーがありません。",
				L"csvKey : " + csvKey,
				L"Animator::LoadMotionData()"
			);
		}

		vector<AnimationClip> outputData;
		m_animationClipMap.clear();

		auto fileData = CSVLoad::GetInstance()->GetData(csvKey);
		for (int i = 0; i < fileData.size(); i++) {
			// 1行目は見出しなので無視
			if (i == 0) {
				continue;
			}
			// ","で区切る
			auto delimiterData = DataExtracter::DelimitData(fileData[i]);
			AnimationClip clip(
				delimiterData[0],
				(int)_wtof(delimiterData[1].c_str()),
				(int)_wtof(delimiterData[2].c_str()),
				Utility::WStrToBool(delimiterData[3]),
				(float)_wtof(delimiterData[4].c_str())
			);
			outputData.push_back(clip);
			m_animationClipMap[delimiterData[0]] = clip;
		}

		return outputData;
	}

	void Animator::RegisterAnimationClip(AnimationClip clip) {
		auto drawer = GetGameObject()->GetComponent<PNTBoneModelDraw>();
		drawer->AddAnimation(clip.name,
			clip.start, clip.end - clip.start,
			clip.isLoop, 30);
	}

	void Animator::ChangeAnimation(wstring key) {
		auto drawer = GetGameObject()->GetComponent<PNTBoneModelDraw>();
		drawer->ChangeCurrentAnimation(key);
	}

	void Animator::OnUpdate() {
		auto drawer = GetGameObject()->GetComponent<PNTBoneModelDraw>();
		drawer->UpdateAnimation(App::GetApp()->GetElapsedTime() * GetPlaySpeed());
	}

	bool Animator::IsTargetAnimationEnd() {
		auto drawer = GetGameObject()->GetComponent<PNTBoneModelDraw>();
		return drawer->IsTargetAnimeEnd();
	}

	float Animator::GetPlaySpeed() {
		auto drawer = GetGameObject()->GetComponent<PNTBoneModelDraw>();
		auto keyName = drawer->GetCurrentAnimation();

		return m_animationClipMap[keyName].playSpeed;
	}
}