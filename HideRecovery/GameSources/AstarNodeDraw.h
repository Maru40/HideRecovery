
/*!
@file AstarNodeDraw.h
@brief AstarNodeDraw�N���X
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�O���錾
	//--------------------------------------------------------------------------------------
	class GraphAstar;
	class NumbersObject;

	//--------------------------------------------------------------------------------------
	///	Astar�m�[�h�\��
	//--------------------------------------------------------------------------------------
	class AstarNodeDraw : public Component
	{
		ex_weak_ptr<GraphAstar> m_astar;				//Astar�O���t

		vector<std::shared_ptr<NumbersObject>> m_nodes;	//���������m�[�h�I�u�W�F�N�g

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		/// <param name="astar">Astar�O���t</param>
		AstarNodeDraw(const std::shared_ptr<GameObject>& objPtr, const std::shared_ptr<GraphAstar>& astar);

		void OnCreate() override;
		void OnUpdate() override;
		void OnDraw() override;

	private:
		/// <summary>
		/// �m�[�h�̐���
		/// </summary>
		void CreateNodeDraw();

		/// <summary>
		/// �m�[�h�̈ʒu�X�V
		/// </summary>
		void UpdateNodesPosition();

	public:
		//--------------------------------------------------------------------------------------
		///	�A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// �f�o�b�O�\���ݒ�
		/// </summary>
		/// <param name="isDraw">�f�o�b�O�\�����邩�ǂ���</param>
		void SetDebugDraw(const bool isDraw);

		/// <summary>
		/// �f�o�b�O�\�����邩�ǂ���
		/// </summary>
		/// <returns>�f�o�b�O�\��������Ȃ�true</returns>
		bool GetDebugDraw() const;

		/// <summary>
		/// �F�̐ݒ�
		/// </summary>
		/// <param name="nodeIndex">�ݒ肵�����m�[�h�̃C���f�b�N�X</param>
		/// <param name="color">�F</param>
		void SetColor(const int& nodeIndex, const Col4& color);
	};

}

//endbasecorss