/*!
@file GraphAstar.h
@brief GraphAstar�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "SparseGraph.h"

namespace basecross {

	//-----------------------------------------------------------------------------------------------
	// �O���錾
	//-----------------------------------------------------------------------------------------------
	class NavGraphNode;
	class AstarEdge;

	//-----------------------------------------------------------------------------------------------
	// Astar�p�̃O���t
	//-----------------------------------------------------------------------------------------------
	class AstarGraph : public SparseGraph<NavGraphNode, AstarEdge>
	{
		Vec3 m_centerPosition;	//�O���t�̒��S�ʒu

	public:
		AstarGraph(bool digraph);

		virtual ~AstarGraph() = default;

		/// <summary>
		/// ���S�ʒu�̌v�Z
		/// </summary>
		/// <returns>���S�ʒu</returns>
		Vec3 CalculateCenterPosition();

		//-----------------------------------------------------------------------------------------------
		// �A�N�Z�b�T
		//-----------------------------------------------------------------------------------------------

		/// <summary>
		/// ���S�ʒu�̎擾
		/// </summary>
		/// <returns>���S�ʒu</returns>
		Vec3 GetCenterPosition() const noexcept { return m_centerPosition; }
	};

}