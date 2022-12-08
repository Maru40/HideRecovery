
/*!
@file DebugCell.h
@brief DebugCell�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	namespace maru {

		//--------------------------------------------------------------------------------------
		///	�O���錾
		//--------------------------------------------------------------------------------------
		class CellMap;

		//--------------------------------------------------------------------------------------
		///	�f�o�b�O�\��Cell
		//--------------------------------------------------------------------------------------
		class DebugCellObject : public GameObject
		{

		public:
			DebugCellObject(const std::shared_ptr<Stage>& stage);

			void OnCreate() override;
			void OnUpdate() override;
		};

	}
}