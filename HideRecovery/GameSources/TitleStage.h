#pragma once
#include "stdafx.h"
#include "FadeUI.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	リザルトステージクラス
	//--------------------------------------------------------------------------------------
	class TitleStage : public  Stage {
		//ビューの作成
		void CreateViewLight();
	public:
		shared_ptr<FadeUI> m_FU;

		std::shared_ptr<Cursor> m_CursorUi;

		bool m_sceneflag;
		//構築と破棄
		TitleStage() :Stage(),m_sceneflag(false) {}
		virtual ~TitleStage() {}
		//初期化
		void OnCreate()override;
		void OnUpdate()override;
	};


}
//end basecross

