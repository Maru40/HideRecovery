/*!
@file ScaleParent.h
@brief ScaleParent
担当者：丸山 裕喜
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	大きさを設定した親オブジェクトに合わせるコンポ―ネント
	//--------------------------------------------------------------------------------------
	class ScaleParent : public Component
	{
		ex_weak_ptr<GameObject> m_parent;	//ペアレント
		Vec3 m_beforeScale = Vec3(0.0f);	//1フレーム前の大きさ
		Vec3 m_firstScale = Vec3(0.0f);		//初期の大きさ

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		/// <param name="parent">親オブジェクト</param>
		ScaleParent(const std::shared_ptr<GameObject>& objPtr, const std::shared_ptr<GameObject>& parent);

		void OnCreate() override;
		void OnUpdate() override;
	};

}