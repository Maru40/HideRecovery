/*!
@file StageObjectBase.h
@brief StageObjectBaseクラス
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"
#include "Watanabe/Utility/TransformData.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	///	ステージオブジェクトの基底クラス
	//--------------------------------------------------------------------------------------
	class StageObjectBase : public GameObject
	{
	protected:
		wstring m_name;    //ステージオブジェクトネーム
		wstring m_texture; //テクスチャネーム
		TransformData m_transformData; //トランスフォームデータ

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="stage">このクラスが所属するステージ</param>
		/// <param name="name">オブジェクト名</param>
		/// <param name="texture">テクスチャ名</param>
		StageObjectBase(const std::shared_ptr<Stage>& stage, const wstring& name, const wstring& texture = L"");

		virtual void OnPreCreate()override;
		virtual void OnCreate() override;

		//--------------------------------------------------------------------------------------
		///	アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// オブジェクト名の取得
		/// </summary>
		/// <returns>オブジェクト名</returns>
		wstring GetName() const noexcept { return m_name; }

		/// <summary>
		/// テクスチャ名の取得
		/// </summary>
		/// <returns>テクスチャ名</returns>
		wstring GetTexture() const noexcept { return m_texture; }
	};
}

//endbasecross