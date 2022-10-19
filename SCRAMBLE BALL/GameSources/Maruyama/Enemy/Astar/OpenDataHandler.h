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
	///	�I�[�v���f�[�^
	//--------------------------------------------------------------------------------------
	struct OpenData {
		std::weak_ptr<NavGraphNode> node;  //�m�[�h�̃|�C���^
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
		OpenData(const std::shared_ptr<NavGraphNode>& node, const float range, const float heuristic);

		/// <summary>
		/// �������ƃq���[�X���b�N�����̍��v��Ԃ�
		/// </summary>
		/// <returns>���v�l</returns>
		float GetSumRange() const;

		bool operator ==(const OpenData& data) const;
	};

	//--------------------------------------------------------------------------------------
	///	�I�[�v���f�[�^�������ҁB
	//--------------------------------------------------------------------------------------
	class OpenDataHandler
	{
		using DataPtrList = std::list<std::shared_ptr<OpenData>>;

		std::shared_ptr<Heuristic> m_heuristic;	//�q���[�X���b�N�̐��l���v�Z����N���X�B

		std::stack<std::shared_ptr<NavGraphNode>> m_route;	//�����������[�g

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
		/// �I�[�v���f�[�^�𐶐�����B
		/// </summary>
		/// <param name="baseNode">�x�[�X�ƂȂ�m�[�h</param>
		/// <param name="graph">�O���t�̎Q��</param>
		/// <returns>�I�[�v���f�[�^</returns>
		bool CreateOpenDatas(
			DataPtrList& openDataList,
			DataPtrList& closeDataList,
			const std::shared_ptr<OpenData>& openData, 
			const std::shared_ptr<AstarGraph>& graph
		);

		/// <summary>
		/// �ŒZ���[�g�̐���
		/// </summary>
		/// <param name="startNode">�J�n�m�[�h</param>
		/// <param name="targetNode">�ڕW�m�[�h</param>
		/// <param name="graph">�O���t</param>
		void OpenDataHandler::CreateRoute(
			const std::shared_ptr<NavGraphNode>& startNode,
			const std::shared_ptr<NavGraphNode>& targetNode,
			const std::shared_ptr<AstarGraph>& graph,
			const DataPtrList& openDataList
		);

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
			const std::shared_ptr<AstarGraph>& graph
		);

		//--------------------------------------------------------------------------------------
		///	�A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		std::stack<std::shared_ptr<NavGraphNode>> GetRoute();

	};

}