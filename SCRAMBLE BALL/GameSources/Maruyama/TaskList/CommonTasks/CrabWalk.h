
/*!
@file CrabWalk.h
@brief CrabWalkなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/TaskList/TaskList.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	前方宣言
	//--------------------------------------------------------------------------------------
	template<class EnumType>
	class TaskList;

	class GameTimer;
	class VelocityManager;
	class PlayerControlManager;
	class OnlinePlayerSynchronizer;

	class UseWeapon;

	namespace Operator {
		class ObjectMover;
	}

	namespace TaskListNode {

		//--------------------------------------------------------------------------------------
		///	かに歩きタスクのパラメータ
		//--------------------------------------------------------------------------------------
		struct CrabWalk_Parametor {
			float speed;		//移動スピード
			float minMoveTime;	//移動時間の最小時間
			float maxMoveTime;	//移動時間の最大時間

			CrabWalk_Parametor();
			CrabWalk_Parametor(const float speed);
			CrabWalk_Parametor(const float speed, const float minMoveTime, const float maxMoveTime);
		};

		//--------------------------------------------------------------------------------------
		///	かに歩きタスク
		//--------------------------------------------------------------------------------------
		class CrabWalk : public TaskNodeBase<GameObject>
		{
		public:
			using Parametor = CrabWalk_Parametor;

			enum class MoveDirectionType {
				None,
				Right,
				Left,
			};

		private:
			const Parametor* m_paramPtr;			//パラメータ

			MoveDirectionType m_directionType;		//移動方向
			std::unique_ptr<GameTimer> m_timer;		//タイマー

			std::weak_ptr<Transform> m_transform;					//トランスフォーム
			std::weak_ptr<UseWeapon> m_useWepon;					//武器の使用
			std::weak_ptr<Operator::ObjectMover> m_mover;			//移動系
			std::weak_ptr<PlayerControlManager> m_playerController;	//playerコントローラー
			std::weak_ptr<VelocityManager> m_velocityManager;		//速度管理
			std::weak_ptr<OnlinePlayerSynchronizer> m_onlineSychoronizer;	//オンラインシンクロ

		public:
			CrabWalk(const std::shared_ptr<GameObject>& owner, const Parametor* paramPtr);

			virtual ~CrabWalk() = default;

			void OnStart() override;

			bool OnUpdate() override;

			void OnExit() override;

		private:

			void MoveUpdate();

			/// <summary>
			/// ランダムに移動方向を決める
			/// </summary>
			/// <returns>ランダムに決められた移動方向</returns>
			MoveDirectionType GetRandomMoveDirectionType();

			/// <summary>
			/// 移動方向を返す
			/// </summary>
			/// <returns>移動方向</returns>
			Vec3 CalculateMoveDirection();

			bool IsEnd() const;
		};

	}
}