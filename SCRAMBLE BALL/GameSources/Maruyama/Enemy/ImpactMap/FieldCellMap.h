
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
		class CellMap;

		//--------------------------------------------------------------------------------------
		///	�t�B�[���h�p�̃Z���}�b�v
		//--------------------------------------------------------------------------------------
		class FieldCellMap : public Component
		{
			std::shared_ptr<CellMap> m_cellMap;

			bool m_isDebugDraw;	//�f�o�b�ODraw�����邩�ǂ���

		public:
			FieldCellMap(const std::shared_ptr<GameObject>& objPtr);

			void OnCreate() override;
			void OnUpdate() override;

		public:
			//--------------------------------------------------------------------------------------
			///	�A�N�Z�b�T
			//--------------------------------------------------------------------------------------

			void SetCellMap(const std::shared_ptr<CellMap>& cellMap);

			const std::shared_ptr<CellMap>& GetCellMap() const;


			//--------------------------------------------------------------------------------------
			///	�f�o�b�O
			//--------------------------------------------------------------------------------------

			void DebugCellsDraw();	//Cell�̃f�o�b�O�\��
		};

	}
}