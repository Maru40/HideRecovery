/*!
@file Targeted.h
@brief Targetedなど
担当者：丸山 裕喜
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// ターゲットにされる対象のパラメータ
	//--------------------------------------------------------------------------------------
	struct Targeted_Parametor
	{
		Vec3 offset = Vec3(0.0f);
	};

	//--------------------------------------------------------------------------------------
	/// ターゲットにされる対象
	//--------------------------------------------------------------------------------------
	class Targeted : public Component
	{
	public:
		using Parametor = Targeted_Parametor;

	private:
		Parametor m_param; //パラメータ

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		Targeted(const std::shared_ptr<GameObject>& objPtr);
		
	public:
		//--------------------------------------------------------------------------------------
		/// アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// オフセットを考慮したターゲット位置を取得
		/// </summary>
		/// <returns>オフセットを考慮したターゲットの位置</returns>
		Vec3 GetPosition() const;

		/// <summary>
		/// ターゲット位置のオフセットを設定
		/// </summary>
		/// <param name="offset">ターゲット位置のオフセット</param>
		void SetOffset(const Vec3& offset);

		/// <summary>
		/// ターゲット位置のオフセットを取得
		/// </summary>
		/// <returns>ターゲット位置のオフセット</returns>
		Vec3 GetOffset() const;
	};

}