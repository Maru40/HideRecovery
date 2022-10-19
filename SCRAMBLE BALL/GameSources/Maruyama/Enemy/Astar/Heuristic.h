/*!
@file Heuristic.h
@brief Heuristic
�S���ҁF�ێR �T��
*/

#pragma once

#include "NavGraphNode.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�ړI�̃m�[�h���L�����āA���̃m�[�h�Ƃ̌v�Z������N���X�B
	//--------------------------------------------------------------------------------------
	class Heuristic
	{
		ex_weak_ptr<NavGraphNode> m_targetNode;  //�ړI�n�ɐݒ肵���m�[�h

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		Heuristic();

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="node">�ړI�n�ɐݒ肵���m�[�h</param>
		Heuristic(const std::shared_ptr<NavGraphNode>& node);

		virtual ~Heuristic() = default;

		/// <summary>
		/// �n���ꂽ�m�[�h����^�[�Q�b�g�m�[�h�܂ł̋����𑪂��ĕԂ��B
		/// </summary>
		/// <param name="node">�J�n�n�_�̃m�[�h</param>
		/// <returns>targetNode.position - node.position</returns>
		float CalculateHeuristicRange(const std::shared_ptr<const NavGraphNode>& node);

		//--------------------------------------------------------------------------------------
		///	�A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// �ړI�n�ɐݒ肵�����m�[�h���Z�b�g
		/// </summary>
		/// <param name="node">�ړI�n�ɐݒ肵�����m�[�h</param>
		void SetTargetNode(const std::shared_ptr<NavGraphNode>& node) noexcept { m_targetNode = node; }
	};

}

//endbasecross