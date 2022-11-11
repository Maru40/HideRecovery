
/*!
@file PlayerDeader.h
@brief PlayerDeaderなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

namespace Online
{
	class OnlineTransformSynchronization;
}

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	class PlayerAnimator;

	//--------------------------------------------------------------------------------------
	/// プレイヤーの死亡管理するクラスのパラメータ
	//--------------------------------------------------------------------------------------
	struct PlayerDeader_Parametor {
		
	};

	//--------------------------------------------------------------------------------------
	/// プレイヤーの死亡管理するクラス
	//--------------------------------------------------------------------------------------
	class PlayerDeader : public Component
	{
	public:
		using Parametor = PlayerDeader_Parametor;

	private:
		std::weak_ptr<PlayerAnimator> m_animator;	//アニメーター
		std::function<void()> m_updateFunction;		//更新処理

	public:
		PlayerDeader(const std::shared_ptr<GameObject>& objPtr);

		void OnLateStart() override;
		void OnUpdate() override;

	private:
		/// <summary>
		/// アニメーション監視中
		/// </summary>
		void ObserveAnimation();

	public:
		/// <summary>
		/// 死亡開始
		/// </summary>
		void StartDead();

		/// <summary>
		/// 死亡できる状態かどうか
		/// </summary>
		/// <returns>死亡できるならtrue</returns>
		bool IsDead();
	};

}