
/*!
@file AstarEdge.h
@brief AstarEdge�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�O���錾
	//--------------------------------------------------------------------------------------
	class GraphAstar;

	//--------------------------------------------------------------------------------------
	///	Astar�p�̃G�b�W
	//--------------------------------------------------------------------------------------
	class AstarEdge
	{
		//�G�b�W�͓�̃m�[�h��A������B
		int m_from;      //��O�̃m�[�h�̃C���f�b�N�X
		int m_to;        //��̃m�[�h�̃C���f�b�N�X

		float m_cost;    //�ړ��̃R�X�g

		bool m_isActive; //�A�N�e�B�u���

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="from">��O�̃C���f�b�N�X</param>
		/// <param name="to">��̃C���f�b�N�X</param>
		AstarEdge(const int& from, const int& to);

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="from">��O�̃C���f�b�N�X</param>
		/// <param name="to">��̃C���f�b�N�X</param>
		/// <param name="cost">�ړ��R�X�g</param>
		AstarEdge(const int& from, const int& to, const float& cost);

		virtual ~AstarEdge() = default;

		//-----------------------------------------------------------------------------------------------
		// �A�N�Z�b�T
		//-----------------------------------------------------------------------------------------------

		/// <summary>
		/// ��O�̃C���f�b�N�X���Z�b�g
		/// </summary>
		/// <param name="from">��O�̃C���f�b�N�X</param>
		/// <returns>��O�̃C���f�b�N�X</returns>
		void SetFrom(const int& from) noexcept { m_from = from; }

		/// <summary>
		/// ��O�̃C���f�b�N�X���擾
		/// </summary>
		/// <returns>��O�̃C���f�b�N�X</returns>
		int GetFrom() const noexcept { return m_from; }

		/// <summary>
		/// ��̃C���f�b�N�X�̃Z�b�g
		/// </summary>
		/// <param name="to">��̃C���f�b�N�X</param>
		void SetTo(const int& to) noexcept { m_to = to; }

		/// <summary>
		/// ��̃C���f�b�N�X�̎擾
		/// </summary>
		/// <returns>��̃C���f�b�N�X</returns>
		int GetTo() const noexcept { return m_to; }

		/// <summary>
		/// �ړ��R�X�g�̃Z�b�g
		/// </summary>
		/// <param name="cost">�ړ��R�X�g</param>
		void SetCost(const float& cost) noexcept { m_cost = cost; }

		/// <summary>
		/// �ړ��R�X�g�̎擾
		/// </summary>
		/// <returns>�ړ��R�X�g</returns>
		float GetCost() const noexcept { return m_cost; }

		/// <summary>
		/// �A�N�e�B�u��Ԃ̃Z�b�g
		/// </summary>
		/// <param name="isActive">�A�N�e�B�u���</param>
		void SetIsActive(const bool isActive) noexcept { m_isActive = isActive; }

		/// <summary>
		/// �A�N�e�B�u��Ԃ̎擾
		/// </summary>
		/// <returns>�A�N�e�B�u�Ȃ�true</returns>
		bool IsActive() const noexcept { return m_isActive; }

		/// <summary>
		/// from,to,�����̃m�[�h��Active��Ԃ��ǂ����𔻒f
		/// </summary>
		/// <param name="astar">��������������GraphAstar</param>
		/// <returns>����Active�Ȃ�true</returns>
		bool IsNodeActive(const std::shared_ptr<GraphAstar>& astar) const;

	};

}

//endbasecross