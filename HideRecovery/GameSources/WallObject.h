
/*!
@file WallObject.h
@brief WallObjectクラス
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "StageObjectBase.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	壁オブジェクト
	//--------------------------------------------------------------------------------------
	class WallObject : public StageObjectBase
	{
		/// <summary>
		/// 描画設定
		/// </summary>
		void SettingDraw();

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="stage">このクラスが所属するステージ</param>
		WallObject(const std::shared_ptr<Stage>& stage)
			:WallObject(stage, L"WallObject", L"")
		{}

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="stage">このクラスが所属するステージ</param>
		/// <param name="name">オブジェクト名</param>
		/// <param name="texture">テクスチャ名</param>
		WallObject(const std::shared_ptr<Stage>& stage, const wstring& name, const wstring& texture)
			:StageObjectBase(stage, name, texture)
		{}

		void OnCreate() override;
	};

}

//endbasecross