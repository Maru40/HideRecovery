
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
		class ImpactCellMap;

		//--------------------------------------------------------------------------------------
		///	�f�o�b�O�\��Cell
		//--------------------------------------------------------------------------------------
		class DebugCell : public Component
		{
			std::weak_ptr<ImpactCellMap> m_impactCellMap;

		public:
			DebugCell(const std::shared_ptr<GameObject>& objPtr);

			void OnCreate() override;
			void OnUpdate() override;
		};

	}
}