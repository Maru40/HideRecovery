#pragma once
#include "stdafx.h"
#include "FadeUI.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	リザルトステージクラス
	//--------------------------------------------------------------------------------------
	class StageSelect : public  Stage {
		//ビューの作成
		void CreateViewLight();
	public:
		shared_ptr<FadeUI> m_FU;
		bool m_sceneflag;
		//構築と破棄
		StageSelect() :Stage(), m_sceneflag(false) {}
		virtual ~StageSelect() {}
		//初期化
		void OnCreate()override;
		void OnUpdate()override;
	};
}
//end basecross

