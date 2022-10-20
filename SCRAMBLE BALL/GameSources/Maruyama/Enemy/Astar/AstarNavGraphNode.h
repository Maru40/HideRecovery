
/*!
@file AstarNavGraphNode.h
@brief AstarNavGraphNode�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "NavGraphNode.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�O���錾
	//--------------------------------------------------------------------------------------
	class GraphAstar;

	//--------------------------------------------------------------------------------------
	///	Astar�p�̃i�r�O���t�m�[�h
	//--------------------------------------------------------------------------------------
	class AstarNavGraphNode : public NavGraphNode
	{
		std::shared_ptr<GraphAstar> m_astar;	//Astar�O���t

	public:

		AstarNavGraphNode();

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="index">���̃N���X�̃C���f�b�N�X</param>
		AstarNavGraphNode(const int& index);

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="index">���̃N���X�̃C���f�b�N�X</param>
		/// <param name="position">���W</param>
		AstarNavGraphNode(const int& index, const Vec3& position);

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="index">���̃N���X�̃C���f�b�N�X</param>
		/// <param name="position">���W</param>
		/// <param name="impactData">�e���}�b�v�f�[�^</param>
		AstarNavGraphNode(const int& index, const Vec3& position, const maru::ImpactData& impactData);

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="index">���̃N���X�̃C���f�b�N�X</param>
		/// <param name="position">���W</param>
		/// <param name="impactData">�e���}�b�v�f�[�^</param>
		/// <param name="parent">�e�I�u�W�F�N�g</param>
		AstarNavGraphNode(const int& index, const Vec3& position, const maru::ImpactData& impactData, const std::shared_ptr<GameObject>& parent);

		/// <summary>
		/// �R�s�[�R���X�g���N�^
		/// </summary>
		/// <param name="node">�R�s�[����m�[�h</param>
		AstarNavGraphNode(const AstarNavGraphNode& node);

		virtual ~AstarNavGraphNode() = default;

	public:

		//--------------------------------------------------------------------------------------
		///	�A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// Astar�̐ݒ�
		/// </summary>
		/// <param name="astar">Astar</param>
		void SetGraphAstar(const std::shared_ptr<GraphAstar>& astar) noexcept;

		/// <summary>
		/// Astar�̎擾
		/// </summary>
		/// <returns>Astar�̎擾</returns>
		std::shared_ptr<GraphAstar> GetGraphAstar() const noexcept;
	};

}