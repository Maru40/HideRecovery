/*!
@file Slime_Pressed.h
@brief Slime_Pressedなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "EnemyStateNodeBase.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	class TargetManager;
	class GameTimer;

	namespace Enemy {

		class EnemyBase;

		namespace StateNode {

			//--------------------------------------------------------------------------------------
			/// スライムのつぶされる時のパラメータ
			//--------------------------------------------------------------------------------------
			struct Slime_Pressed_Parametor {
				std::function<void()> m_startAnimation = nullptr;
				std::function<void()> m_endAnimation = nullptr;
			};

			//--------------------------------------------------------------------------------------
			/// スライムのつぶされる時のステート
			//--------------------------------------------------------------------------------------
			class Slime_Pressed : public EnemyStateNodeBase<EnemyBase>
			{
			public:
				using Parametor = Slime_Pressed_Parametor;

			private:
				ex_weak_ptr<Parametor> m_paramPtr = nullptr;  //パラメータ
				std::unique_ptr<GameTimer> m_timer = nullptr; //タイム管理

			public:
				/// <summary>
				/// コンストラクタ
				/// </summary>
				/// <param name="owner">このクラスを所有するクラス</param>
				/// <param name="paramPtr">パラメータ</param>
				Slime_Pressed(const std::shared_ptr<EnemyBase>& owner, const std::shared_ptr<Parametor>& paramPtr);

				void OnStart() override;
				bool OnUpdate() override;
				void OnExit() override;
			};

		}
	}
}