/*!
@file MoveAstar.h
@brief MoveAstar�Ȃ�
�S���ҁF�ێR �T��
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/TaskList/TaskList.h"

namespace basecross {

	namespace Task {

		//--------------------------------------------------------------------------------------
		///	MoveAstar�̃p�����[�^
		//--------------------------------------------------------------------------------------
		struct MoveAstar_Ex_Parametor
		{

		};

		//--------------------------------------------------------------------------------------
		///	�^�[�Q�b�g�̋߂��܂�Astar�𗘗p���Ĉړ�����^�X�N
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