/*!
@file Ticket_AstarRoute.h
@brief Ticket_AstarRoute
�S���ҁF�ێR �T��
*/

#pragma once

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�O���錾
	//--------------------------------------------------------------------------------------
	class AstarThreadController;
	namespace maru {
		class NodeBase;
		class EdgeBase;
		class AstarNode;
		class AstarEdge;

		template<class NodeType, class EdgeType,
			std::enable_if_t<
				std::is_base_of_v<NodeBase, NodeType>&&		//NodeType��NodeBase���p�����Ă��邱�Ƃ�ۏ؂���
				std::is_base_of_v<EdgeBase, EdgeType>,		//EdgeType��EdgeBase���p�����Ă��邱�Ƃ�ۏ؂���
			std::nullptr_t
		>>
		class SparseGraph;
	}

	namespace Ticket {

		//--------------------------------------------------------------------------------------
		///	�O���錾
		//--------------------------------------------------------------------------------------
		

		//--------------------------------------------------------------------------------------
		///	���[�g���s�p�`�P�b�g
		//--------------------------------------------------------------------------------------
		class AstarRoute
		{
		public:
			friend class AstarThreadController;

			using RouteStack = std::stack<std::weak_ptr<maru::AstarNode>>;						//�������[�g�̃X�^�b�N
			using GraphType = maru::SparseGraph < maru::AstarNode, maru::AstarEdge, nullptr> ;	//�O���t�^�C�v�ݒ�

		private:
			bool m_isValid;		//�L����Ԃł��邩�ǂ���

			/// <summary>
			/// �L����Ԃ̐ݒ�
			/// </summary>
			/// <param name="isValid"></param>
			void SetIsValid(const bool isValid) { m_isValid = isValid; }

			RouteStack m_route;	//�����������[�g

		public:
			AstarRoute();

		private:

			/// <summary>
			/// ���[�g�����J�n
			/// </summary>
			/// <param name="startNode">�J�n�m�[�h</param>
			/// <param name="targetNode">�^�[�Q�b�g�m�[�h</param>
			/// <param name="graph">�O���t</param>
			void Start_RouteSearch(
				const std::shared_ptr<maru::AstarNode>& startNode, 
				const std::shared_ptr<maru::AstarNode>& targetNode,
				const std::shared_ptr<GraphType>& graph
			);

		public:

			//--------------------------------------------------------------------------------------
			///	�A�N�Z�b�T
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// �L����Ԃł��邩�ǂ���
			/// </summary>
			/// <returns>�L����ԂȂ�true</returns>
			bool IsValid() const { return m_isValid; }

			/// <summary>
			/// �����������[�g�̎擾
			/// </summary>
			/// <returns>���[�g</returns>
			RouteStack GetRoute() const;

		};

	}
}