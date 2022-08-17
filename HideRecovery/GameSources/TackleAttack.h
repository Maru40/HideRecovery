
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

	//--------------------------------------------------------------------------------------
	/// タックル攻撃
	//--------------------------------------------------------------------------------------
	class TackleAttack : public AttackBase
	{
		enum class TaskEnum {
			Preliminary,	//予備動作
			Tackle,			//タックル攻撃
			Wait,			//攻撃後の硬直
		};

		std::unique_ptr<TaskList<TaskEnum>> m_taskList;

	public:
		TackleAttack(const std::shared_ptr<GameObject>& objPtr);

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