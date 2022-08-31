
/*!
@file GraphStateNode.h
@brief GraphStateNode
�S���ҁF�ێR �T��
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�O���錾
	//--------------------------------------------------------------------------------------
	class I_StateNode;

	//--------------------------------------------------------------------------------------
	///	�X�e�[�g�p�̃O���t�m�[�h�N���X
	//--------------------------------------------------------------------------------------
	class GraphStateNode : public GraphNodeBase
	{
	private:
		std::shared_ptr<I_StateNode> m_stateNode = nullptr;  //�X�e�[�g�m�[�h

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="stateNode">�m�[�h�̃C���^�[�t�F�[�X</param>
		GraphStateNode(const std::shared_ptr<I_StateNode>& stateNode);

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="index">�m�[�h�̃C���f�b�N�X</param>
		/// <param name="stateNode">�m�[�h�̃C���^�[�t�F�[�X</param>
		GraphStateNode(const int& index, const std::shared_ptr<I_StateNode>& stateNode);

		virtual void OnStart();
		virtual void OnUpdate();
		virtual void OnExit();

	};

}