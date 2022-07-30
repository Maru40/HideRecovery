/*!
@file TactileObject.h
@brief TactileObject
担当者：丸山 裕喜
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	触覚オブジェクト
	//--------------------------------------------------------------------------------------
	class TactileObject : public GameObject
	{
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="stage">このクラスが所属するステージ</param>
		TactileObject(const std::shared_ptr<Stage>& stage) 
			:GameObject(stage)
		{}

		void OnCreate() override;

	};

}

//endbasecross