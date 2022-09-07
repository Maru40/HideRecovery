#pragma once
#include "stdafx.h"
#include "Maruyama/StageObject/StageObjectBase.h"

namespace basecross {
	class Container :public StageObjectBase {
		// コンテナの色（Red,Green,Blue,Yeloowの文字列）
		//（wstringのほうが便宜上楽）
		wstring m_color;
		shared_ptr<GameObject> m_hitTestObject;
	public:
		Container(const shared_ptr<Stage>& stage);
		Container(const shared_ptr<Stage>& stage, const wstring& line);

		void OnCreate()override;
	};
}
