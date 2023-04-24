/*!
@file AstarThreadController.h
@brief AstarThreadController
�S���ҁF�ێR �T��
*/

#pragma once

#include "stdafx.h"

#include <future>

//namespace std {
//	template<class T>
//	class future;
//}

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �O���錾
	//--------------------------------------------------------------------------------------
	class AstarRouteRequester;
	class I_SelfAstarNodeRequester;
	class ThreadPool;

	namespace maru {
		class AstarNode;
		class AstarEdge_Ex;

		template<class NodeType, class EdgeType,
			std::enable_if_t<
				std::is_base_of_v<NodeBase, NodeType>&&		//NodeType��NodeBase���p�����Ă��邱�Ƃ�ۏ؂���
				std::is_base_of_v<EdgeBase, EdgeType>,		//EdgeType��EdgeBase���p�����Ă��邱�Ƃ�ۏ؂���
			std::nullptr_t>
		>
		class SparseGraph;
	}

	namespace Ticket {
		class AstarRoute;
	}

	//--------------------------------------------------------------------------------------
	///	Astar�̃X���b�h�R���g���[���[
	//--------------------------------------------------------------------------------------
	class AstarThreadController
	{
	public:
		using GraphType = maru::SparseGraph<maru::AstarNode, maru::AstarEdge, nullptr>;

	private:
		std::unique_ptr<ThreadPool> m_threadPool;	//�X���b�h�v�[��

		std::unordered_map<AstarRouteRequester*, std::shared_ptr<Ticket::AstarRoute>> m_ticketMap;	//�`�P�b�g�}�b�v

	public:
		AstarThreadController(const std::uint_fast32_t& threadCount = 1);

		virtual ~AstarThreadController() = default;

	private:
		/// <summary>
		/// �`�P�b�g�̐���
		/// </summary>
		/// <returns></returns>
		std::shared_ptr<Ticket::AstarRoute> CreateTicket();

		/// <summary>
		/// �`�P�b�g�̔��s�˗�
		/// </summary>
		/// <param name="requester">�`�P�b�g���s�˗���</param>
		/// <returns>���s�����`�P�b�g</returns>
		_NODISCARD std::shared_ptr<Ticket::AstarRoute> PublishTicket(const std::shared_ptr<AstarRouteRequester>& requester);

		/// <summary>
		/// �`�P�b�g�̓o�^
		/// </summary>
		/// <param name="requester">�\����</param>
		/// <param name="ticket">�o�^�������`�P�b�g</param>
		void RegisterTicket(
			const std::shared_ptr<AstarRouteRequester>& requester, 
			const std::shared_ptr<Ticket::AstarRoute>& ticket
		);

		/// <summary>
		/// �`�P�b�g�o�^����
		/// </summary>
		/// <param name="requester">�o�^��</param>
		void UnRegisterTicket(const std::shared_ptr<AstarRouteRequester>& requester);

		/// <summary>
		/// ���łɃ`�P�b�g���o�^����Ă��邩�ǂ���
		/// </summary>
		/// <param name="requester">�o�^��</param>
		bool HasRegisterTicket(const std::shared_ptr<AstarRouteRequester>& requester) const;

		/// <summary>
		/// �`�P�b�g�̍폜�\��
		/// </summary>
		/// <param name="requester">�폜�\����</param>
		/// <returns>�폜�������������ǂ���</returns>
		void DeleteTicket(const std::shared_ptr<AstarRouteRequester>& requester);

		/// <summary>
		/// �`�P�b�g���d�����Ă��邩�ǂ���
		/// </summary>
		/// <returns>�d�����Ă���Ȃ�true</returns>
		bool IsDuplicationTicket(const std::shared_ptr<AstarRouteRequester>& requester);

	public:
		/// <summary>
		/// ���[�g�̌����J�n
		/// </summary>
		/// <param name="requester">���N�G�X�g��</param>
		/// <param name="startNode">�J�n�m�[�h</param>
		/// <param name="targetNode">�ڕW�m�[�h</param>
		/// <param name="graph">�O���t</param>
		/// <returns>�����`�P�b�g</returns>
		std::shared_ptr<Ticket::AstarRoute> Start_RouteSearch(
			const std::shared_ptr<AstarRouteRequester>& requester,
			const std::shared_ptr<maru::AstarNode>& startNode,
			const std::shared_ptr<maru::AstarNode>& targetNode,
			const std::shared_ptr<GraphType>& graph
		);

		//�����̃m�[�h�������J�n
		std::future<std::shared_ptr<maru::AstarNode>> Start_SelfAstarNodeSearch(
			const std::shared_ptr<I_SelfAstarNodeRequester>& requester,
			const std::shared_ptr<GraphType>& graph
		);
		
	};

}