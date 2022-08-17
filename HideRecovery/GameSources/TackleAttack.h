
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

	//--------------------------------------------------------------------------------------
	/// ^bNU
	//--------------------------------------------------------------------------------------
	class TackleAttack : public AttackBase
	{
		enum class TaskEnum {
			Preliminary,	//\õ®ì
			Tackle,			//^bNU
			Wait,			//UãÌd¼
		};

		std::unique_ptr<TaskList<TaskEnum>> m_taskList;

	public:
		TackleAttack(const std::shared_ptr<GameObject>& objPtr);

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
	};


	namespace Task {

		class Preriminary_Tackle : public TaskNodeBase<GameObject>
		{
		public:
			Preriminary_Tackle(const std::shared_ptr<GameObject>& objPtr);

			void OnStart() override;
			bool OnUpdate() override;
			void OnExit() override;
		};

	}

}