/*!
@file MoveAstar.h
@brief MoveAstarなど
担当者：丸山 裕喜
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/TaskList/TaskList.h"

namespace basecross {

	namespace Task {

		//--------------------------------------------------------------------------------------
		///	MoveAstarのパラメータ
		//--------------------------------------------------------------------------------------
		struct MoveAstar_Ex_Parametor
		{

		};

		//--------------------------------------------------------------------------------------
		///	ターゲットの近くまでAstarを利用して移動するタスク
		//--------------------------------------------------------------------------------------
		class MoveAstar_Ex : public TaskNodeBase<GameObject>
		{
		public:
			using Parametor = MoveAstar_Ex_Parametor;

		private:
			

		public:


		};

	}

}