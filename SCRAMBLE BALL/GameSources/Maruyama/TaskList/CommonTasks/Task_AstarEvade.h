
/*!
@file Task_AstarEvade.h
@brief Task_AstarEvadeなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	template<class T>
	class TaskList;

	namespace Task {

		//--------------------------------------------------------------------------------------
		/// Astarを利用した逃走パラメータ
		//--------------------------------------------------------------------------------------
		struct AstarEvade_Parametor {

		};

		//--------------------------------------------------------------------------------------
		/// Astarを利用した逃走タスク
		//--------------------------------------------------------------------------------------
		class AstarEvade : public TaskNodeBase<GameObject>
		{
		public:
			using Parametor = AstarEvade_Parametor;

			/// <summary>
			/// タスクタイプ
			/// </summary>
			enum class TaskEnum {

			};

		private:
			ex_weak_ptr<Parametor> m_paramPtr = nullptr;     //パラメータ

			std::unique_ptr<TaskList<TaskEnum>> m_taskList;  //タスク管理クラス

		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="objPtr">このクラスを所有するクラス</param>
			/// <param name="paramPtr">パラメータ</param>
			AstarEvade(const std::shared_ptr<GameObject>& owner, const std::shared_ptr<Parametor>& paramPtr);

			void OnStart() override;
			bool OnUpdate() override;
			void OnExit() override;

		};

	}

}