#pragma once
#include "stdafx.h"
#include "FadeUI.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	リザルトステージクラス
	//--------------------------------------------------------------------------------------
	class ResultStage : public  Stage{
		//ビューの作成
		void CreateViewLight();
	public:
		shared_ptr<FadeUI> m_FU;
		bool m_sceneflag;
		int m_sceneMode;
		wstring m_StageName[7] = {
			L"StageS1.csv",
			L"StageS2.csv", // 最初のステージ
			L"StageM1.csv",
			L"StageM2.csv",
			L"StageL1.csv",
			L"StageL2.csv",
			L"StageS1.csv",
			//L"StageA.csv", // 最初のステージに戻るようにする
		};
		//構築と破棄
		ResultStage() :Stage(), m_sceneflag(false), m_sceneMode(0) {}
		virtual ~ResultStage() {}
		//初期化
		void OnCreate()override;
		void OnUpdate()override;
	};


}
//end basecross

