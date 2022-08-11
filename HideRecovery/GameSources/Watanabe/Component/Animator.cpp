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

		auto fileData = CSVLoad::GetInstance()->GetData(csvKey);
		for (int i = 0; i < fileData.size(); i++) {
			// 1行目は見出しなので無視
			if (i == 0)
				continue;
			// ","で区切る
			auto delimiterData = DataExtracter::DelimitData(fileData[i]);
			AnimationClip clip(
				delimiterData[0],
				(float)_wtof(delimiterData[1].c_str()),
				(float)_wtof(delimiterData[2].c_str()),
				Utility::WStrToBool(delimiterData[3]),
				(float)_wtof(delimiterData[4].c_str())
			);
			outputData.push_back(clip);
		}

		return outputData;
	}
}