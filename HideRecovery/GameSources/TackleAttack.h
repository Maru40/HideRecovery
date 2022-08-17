
/*!
@file TackleAttack.h
@brief TackleAttackÈÇ
SFÛRTì
*/

#pragma once
#include "stdafx.h"

#include "AttackBase.h"
#include "TaskList.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	/// Oûé¾
	//--------------------------------------------------------------------------------------
	template<class T>
	class TaskList;

	class PlayerAnimator;
	class VelocityManager;

	//--------------------------------------------------------------------------------------
	/// ^bNU
	//--------------------------------------------------------------------------------------
	class TackleAttack : public AttackBase
	{
		enum class TaskEnum {
			Preliminary,	//\õ®ì
			Tackle,			//^bNU
			EndWait,		//UãÌd¼
		};

		std::unique_ptr<TaskList<TaskEnum>> m_taskList;

	public:
		TackleAttack(const std::shared_ptr<GameObject>& objPtr);

		virtual ~TackleAttack() = default;

		void OnCreate() override;
		void OnUpdate() override;

	private:
		/// <summary>
		/// ^XNÌè`
		/// </summary>
		void DefineTask();

		/// <summary>
		/// ^XNÌIð
		/// </summary>
		void SelectTask();

	public:
		/// <summary>
		/// UJn
		/// </summary>
		void StartAttack();
	};

	//--------------------------------------------------------------------------------------
	/// ^bNUÌ^XN
	//--------------------------------------------------------------------------------------

	namespace Task {

		//--------------------------------------------------------------------------------------
		/// ^bNUÌ\õ®ì
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
		/// ^bNUÌU®ì
		//--------------------------------------------------------------------------------------
		class Attack_Tackle : public TaskNodeBase<GameObject>
		{
			std::weak_ptr<PlayerAnimator> m_animator;
			std::weak_ptr<VelocityManager> m_velocityManager;

		public:
			Attack_Tackle(const std::shared_ptr<GameObject>& objPtr);

			virtual ~Attack_Tackle() = default;

			void OnStart() override;
			bool OnUpdate() override;
			void OnExit() override;

		private:
			void PlayAnimation();
		};

		//--------------------------------------------------------------------------------------
		/// ^bNUÌUãd¼
		//--------------------------------------------------------------------------------------
		class EndWait_Tackle : public TaskNodeBase<GameObject>
		{
		public:
			EndWait_Tackle(const std::shared_ptr<GameObject>& objPtr);

			virtual ~EndWait_Tackle() = default;

			void OnStart() override;
			bool OnUpdate() override;
			void OnExit() override;
		};
	}

}