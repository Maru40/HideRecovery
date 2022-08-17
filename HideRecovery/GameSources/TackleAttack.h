
/*!
@file TackleAttack.h
@brief TackleAttackなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "AttackBase.h"
#include "TaskList.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	template<class T>
	class TaskList;

	class PlayerAnimator;
	class VelocityManager;
	class GameTimer;

	//--------------------------------------------------------------------------------------
	/// タックル攻撃
	//--------------------------------------------------------------------------------------
	class TackleAttack : public AttackBase
	{
		enum class TaskEnum {
			Preliminary,	//予備動作
			Tackle,			//タックル攻撃
			EndWait,		//攻撃後の硬直
		};

		std::unique_ptr<TaskList<TaskEnum>> m_taskList;

	public:
		TackleAttack(const std::shared_ptr<GameObject>& objPtr);

		virtual ~TackleAttack() = default;

		void OnCreate() override;
		void OnUpdate() override;

	private:
		/// <summary>
		/// タスクの定義
		/// </summary>
		void DefineTask();

		/// <summary>
		/// タスクの選択
		/// </summary>
		void SelectTask();

	public:
		/// <summary>
		/// 攻撃開始
		/// </summary>
		void StartAttack();

		/// <summary>
		/// 強制攻撃開始
		/// </summary>
		void ForceStartAttack();

		/// <summary>
		/// タックル中かどうか
		/// </summary>
		bool IsTackle();
	};

	//--------------------------------------------------------------------------------------
	/// タックル攻撃のタスク
	//--------------------------------------------------------------------------------------

	namespace Task {

		//--------------------------------------------------------------------------------------
		/// タックル攻撃の予備動作
		//--------------------------------------------------------------------------------------
		class Preriminary_Tackle : public TaskNodeBase<GameObject>
		{
			std::weak_ptr<PlayerAnimator> m_animator;

		public:
			Preriminary_Tackle(const std::shared_ptr<GameObject>& objPtr);

			virtual ~Preriminary_Tackle() = default;

			void OnStart() override;
			bool OnUpdate() override;
			void OnExit() override;

		private:
			void PlayAnimation();
		};

		//--------------------------------------------------------------------------------------
		/// タックル攻撃の攻撃中動作
		//--------------------------------------------------------------------------------------
		class Attack_Tackle : public TaskNodeBase<GameObject>
		{
			std::weak_ptr<PlayerAnimator> m_animator;
			std::weak_ptr<VelocityManager> m_velocityManager;

			float m_tackleSpeed;

		public:
			Attack_Tackle(const std::shared_ptr<GameObject>& objPtr, const float tackleSpeed = 500.0f);

			virtual ~Attack_Tackle() = default;

			void OnStart() override;
			bool OnUpdate() override;
			void OnExit() override;

		private:
			void PlayAnimation();
		};

		//--------------------------------------------------------------------------------------
		/// タックル攻撃の攻撃後硬直
		//--------------------------------------------------------------------------------------
		class EndWait_Tackle : public TaskNodeBase<GameObject>
		{
			std::unique_ptr<GameTimer> m_timer;
			std::weak_ptr<VelocityManager> m_velocityManager;

			float m_waitTime = 0.0f;

		public:
			EndWait_Tackle(const std::shared_ptr<GameObject>& objPtr, const float waitTime = 0.5f);

			virtual ~EndWait_Tackle() = default;

			void OnStart() override;
			bool OnUpdate() override;
			void OnExit() override;
		};
	}

}