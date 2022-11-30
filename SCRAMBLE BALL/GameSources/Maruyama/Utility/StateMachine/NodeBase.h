/*!
@file NodeBase.h
@brief NodeBase
�S���ҁF�ێR �T��
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	namespace maru {

		//--------------------------------------------------------------------------------------
		/// �m�[�h�̊��N���X(�Œ���̋@�\����������B)
		//--------------------------------------------------------------------------------------
		class NodeBase
		{
			bool m_isActive;	//�A�N�e�B�u���
			int m_index;		//�C���f�b�N�X

		public:
			NodeBase();

			NodeBase(const int index);

			virtual ~NodeBase() = default;

		public:
			//--------------------------------------------------------------------------------------
			/// �A�N�Z�b�T
			//--------------------------------------------------------------------------------------

			void SetIsActive(const bool isActive) noexcept { m_isActive = isActive; }

			bool IsActive() const noexcept { return m_isActive; }

			bool GetIsActive() const noexcept { return IsActive(); }

			void SetIndex(const int index) noexcept { m_index = index; }

			int GetIndex() const noexcept { return m_index; }

			/// <summary>
			/// �^�C�v�̐ݒ�
			/// </summary>
			template<class EnumType>
			void SetType(const EnumType type) { m_index = static_cast<int>(type); }

			/// <summary>
			/// ���^�C�v�����擾����
			/// </summary>
			template<class EnumType>
			EnumType GetType() const { return static_cast<EnumType>(m_index); }

		};

	}
}