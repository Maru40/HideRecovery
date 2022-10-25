
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
	class NavGraphNode;
	class ImpactDataDraw;
	class ImpactDataDrawObject;

	//--------------------------------------------------------------------------------------
	///	�f�o�b�O�\���p�̃f�[�^
	//--------------------------------------------------------------------------------------
	struct DrawNodeData {
		std::weak_ptr<NavGraphNode> node;					//�m�[�h
		std::shared_ptr<NumbersObject> nodeNumber;			//�����\�L
		std::shared_ptr<ImpactDataDrawObject> impactData;	//�e���f�[�^�̕\���Ǘ��N���X

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="node">�m�[�h�̃|�C���^</param>
		DrawNodeData(const std::shared_ptr<NavGraphNode>& node);

		/// <summary>
		/// �m�[�h�����݂��邩�ǂ���
		/// </summary>
		/// <returns>�m�[�h�����݂���Ȃ�true</returns>
		bool HasNode() const noexcept { return !node.expired(); }

		/// <summary>
		/// �f�[�^�̍X�V�Ăяo��
		/// </summary>
		void OnUpdate();
		
		/// <summary>
		/// �f�[�^�̕\���X�V�Ăяo��
		/// </summary>
		void OnDraw();

		/// <summary>
		/// �f�[�^�̃A�N�e�B�u�̐ݒ�
		/// </summary>
		/// <param name="isActive">�f�[�^�̃A�N�e�B�u���</param>
		void SetDrawActive(const bool isActive);
	};

	//--------------------------------------------------------------------------------------
	///	Astar�m�[�h�\��
	//--------------------------------------------------------------------------------------
	class AstarNodeDraw : public Component
	{
		std::weak_ptr<const GraphAstar::GraphType> m_astarGraph;		//Astar�O���t

		std::vector<DrawNodeData> m_drawDatas;	//���������\���p�f�[�^

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
		/// <param name="astar">Astar�O���t</param>
		AstarNodeDraw(const std::shared_ptr<GameObject>& objPtr, const std::shared_ptr<const GraphAstar::GraphType>& astar);

		void OnCreate() override;
		void OnUpdate() override;
		void OnDraw() override;
		void OnDestroy() override;

	private:
		/// <summary>
		/// �\���f�[�^��Astar�̃m�[�h������
		/// </summary>
		void CreateDrawDatas();

		/// <summary>
		/// �m�[�h�ԍ��\���I�u�W�F�N�g�̐���
		/// </summary>
		/// <param name="node">�����������m�[�h</param>
		/// <returns>���������I�u�W�F�N�g</returns>
		std::shared_ptr<NumbersObject> CreateNodeNumber(const std::shared_ptr<NavGraphNode>& node);

		/// <summary>
		/// �e���f�[�^�\���I�u�W�F�N�g�̐���
		/// </summary>
		/// <param name="node">�����������m�[�h</param>
		/// <returns>���������I�u�W�F�N�g</returns>
		std::shared_ptr<ImpactDataDrawObject> CreateImpactDataDraw(const std::shared_ptr<NavGraphNode>& node);

		/// <summary>
		/// �m�[�h�̍X�V
		/// </summary>
		void UpdateNodes();

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