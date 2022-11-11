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
		float priority = 0;	//優先度
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

		//ターゲット指定できる状態どうかを判断するbool関数配列
		std::vector<std::function<bool()>> m_canTargetFunctions;	

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

		/// <summary>
		/// 優先度の設定
		/// </summary>
		/// <param name="priority">優先度</param>
		void SetPriority(const float priority) { m_param.priority = priority; }

		/// <summary>
		/// 優先度の取得
		/// </summary>
		/// <returns></returns>
		_NODISCARD float GetPriority() const noexcept { return m_param.priority; }

		/// <summary>
		/// ターゲットにすることができるか判断
		/// </summary>
		/// <returns>ターゲットにできるならtrue</returns>
		bool CanTarget() const;

		/// <summary>
		/// ターゲット指定できるかどうかの条件式を追加
		/// </summary>
		/// <param name="canTargetFunction">追加したい条件式</param>
		void AddCanTargetFunction(const std::function<bool()>& canTargetFunction);
	};

}