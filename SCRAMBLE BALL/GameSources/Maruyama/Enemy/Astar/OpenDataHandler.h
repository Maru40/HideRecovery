/*!
@file OpenDataHandler.h
@brief OpenDataHandler�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�O���錾
	//--------------------------------------------------------------------------------------
	class NavGraphNode;
	class AstarGraph;
	class Heuristic;

	//--------------------------------------------------------------------------------------
	///	�I�[�v���f�[�^(�ߋ���)
	//--------------------------------------------------------------------------------------
	struct OpenData {
		std::weak_ptr<NavGraphNode> parent;	//�����̎�O�̃m�[�h
		std::weak_ptr<NavGraphNode> node;	//�m�[�h�̃|�C���^
		float range;            //������
		float heuristic;        //�q���[�X���b�N����
		bool isActive = true;

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		OpenData();

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="node">�m�[�h</param>
		/// <param name="range">������</param>
		/// <param name="heuristic">�q���[�X���b�N����</param>
		OpenData(const std::shared_ptr<NavGraphNode>& parent, const std::shared_ptr<NavGraphNode>& node, const float range, const float heuristic);

		/// <summary>
		/// �������ƃq���[�X���b�N�����̍��v��Ԃ�
		/// </summary>
		/// <returns>���v�l</returns>
		float GetSumRange() const;

		bool operator ==(const OpenData& data) const;
	};

	//--------------------------------------------------------------------------------------
	///	�I�[�v���f�[�^��������(�ߋ���)
	//--------------------------------------------------------------------------------------
	class OpenDataHandler
	{
		using DataPtrList = std::list<std::shared_ptr<OpenData>>;

		std::shared_ptr<Heuristic> m_heuristic;				//�q���[�X���b�N�̐��l���v�Z����N���X�B

		std::stack<std::weak_ptr<NavGraphNode>> m_route;	//�����������[�g

		std::weak_ptr<NavGraphNode> m_otherAreaNode;		//�ʂ̃G���A�m�[�h

	public:
		OpenDataHandler();

		virtual ~OpenDataHandler() = default;
	
	private:

		/// <summary>
		/// �I�[�v���f�[�^�𐶐����邽�߂̊�ƂȂ�I�[�v���f�[�^������(��Ԋ��Ғl�̍����m�[�h���擾����B)
		/// </summary>
		/// <returns>�I�[�v���f�[�^�𐶐����邽�߂̊J�n�m�[�h</returns>
		std::shared_ptr<OpenData> FindSearchBaseOpenData(DataPtrList& openDataList);

		/// <summary>
		/// ���̃G���A���ڕW�ɂȂ��Ă��邩�ǂ���
		/// </summary>
		/// <param name="startNode">�J�n�m�[�h</param>
		/// <param name="targetAreaIndex">�ڕW�G���A</param>
		/// <returns>���̃G���A���ڕW�Ȃ�true</returns>
		bool IsOhterAreaTarget(const std::shared_ptr<NavGraphNode>& startNode, const int targetAreaIndex);

		/// <summary>
		/// �I�[�v���f�[�^�𐶐�����B
		/// </summary>
		/// <param name="baseNode">�x�[�X�ƂȂ�m�[�h</param>
		/// <param name="graph">�O���t�̎Q��</param>
		/// <returns>�I�[�v���f�[�^</returns>
		bool CreateOpenDatas(
			DataPtrList& openDataList,
			DataPtrList& closeDataList,
			const std::shared_ptr<OpenData>& openData, 
			const std::shared_ptr<AstarGraph>& graph,
			const std::shared_ptr<NavGraphNode>& startNode,
			const std::shared_ptr<NavGraphNode>& targetNode,
			const int targetAreaIndex = -1
		);

		/// <summary>
		/// ���[�g�̐���
		/// </summary>
		/// <param name="openDataList">�I�[�v���f�[�^���X�g</param>
		/// <param name="targetNode">�ڕW�m�[�h</param>
		/// <returns>����������������true</returns>
		bool CreateRoute(const DataPtrList& openDataList, const std::shared_ptr<NavGraphNode>& targetNode);

		/// <summary>
		/// �I�[�v���f�[�^�̎擾
		/// </summary>
		/// <param name="openDataList">�I�[�v���f�[�^���X�g</param>
		/// <param name="node">�m�F�������m�[�h</param>
		/// <returns>�����f�[�^������Ȃ炻�̃f�[�^��Ԃ�</returns>
		std::shared_ptr<OpenData> FindSomeOpenData(const DataPtrList& dataList, const std::shared_ptr<NavGraphNode>& node);

		/// <summary>
		/// �I�[�v���f�[�^�̎擾
		/// </summary>
		/// <param name="openDataList">�I�[�v���f�[�^���X�g</param>
		/// <param name="openData">�����f�[�^�����݂��邩���f</param>
		/// <returns>�����f�[�^������Ȃ炻�̃f�[�^��Ԃ�</returns>
		std::shared_ptr<OpenData> FindSomeOpenData(const DataPtrList& dataList, const std::shared_ptr<OpenData>& openData);

		/// <summary>
		/// �o�^���ꂽ�I�[�v���f�[�^�����f
		/// </summary>
		/// <param name="openDataList">�f�[�^���X�g</param>
		/// <param name="openData">�m�F�������f�[�^</param>
		/// <returns>�o�^����Ă�����true</returns>
		bool IsRegisterData(const DataPtrList& dataList, const std::shared_ptr<OpenData>& openData);

		/// <summary>
		/// �I�[�v���f�[�^�̒ǉ�
		/// </summary>
		/// <param name="openDatas">�ǉ��������I�[�v���f�[�^�̔z��</param>
		/// <param name="openData">�ǉ��������I�[�v���f�[�^</param>
		/// <returns></returns>
		bool AddOpenData(DataPtrList& openDataList, DataPtrList& closeDataList, const std::shared_ptr<OpenData>& openData);

	public:

		/// <summary>
		/// Astar�𗘗p�����o�H�T���J�n
		/// </summary>
		/// <param name="startNode">�J�n�m�[�h</param>
		/// <param name="targetNode">�ڕW�m�[�h</param>
		/// <param name="graph">�T���ɗ��p����O���t</param>
		/// <returns>�o�H�T��������������true</returns>
		bool StartSearchAstar(
			const std::shared_ptr<NavGraphNode>& startNode, 
			const std::shared_ptr<NavGraphNode>& targetNode,
			const std::shared_ptr<AstarGraph>& graph,
			const int targetAreaIndex = -1
		);

		//--------------------------------------------------------------------------------------
		///	�A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// ���[�g�̎擾
		/// </summary>
		std::stack<std::weak_ptr<NavGraphNode>> GetRoute();

	};

}