
/*!
@file FieldCellMap.h
@brief FieldCellMap�Ȃ�
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
		///	�t�B�[���h�p�̃Z���}�b�v
		//--------------------------------------------------------------------------------------
		class FieldCellMap : public Component
		{
		public:
			FieldCellMap(const std::shared_ptr<GameObject>& objPtr);

			void OnCreate() override;
			void OnUpdate() override;

			
		};

	}
}