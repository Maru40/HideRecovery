
/*!
@file NavGraphNode.h
@brief NavGraphNode�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "GraphNodeBase.h"

#include "I_Impact.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�i�r�O���t�p�̃m�[�h
	//--------------------------------------------------------------------------------------
	class NavGraphNode : public GraphNodeBase
	{
	public:
		using ImpactData = maru::ImpactData;

	private:
		Vec3 m_position;                  //�m�[�h�̏ꏊ
		ImpactData m_impactData;		  //�e���}�b�v�f�[�^

		ex_weak_ptr<GameObject> m_parent; //�e�I�u�W�F�N�g

	public:

		NavGraphNode();

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="index">���̃N���X�̃C���f�b�N�X</param>
		NavGraphNode(const int& index);

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="index">���̃N���X�̃C���f�b�N�X</param>
		/// <param name="position">���W</param>
		NavGraphNode(const int& index, const Vec3& position);

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="index">���̃N���X�̃C���f�b�N�X</param>
		/// <param name="position">���W</param>
		/// <param name="impactData">�e���}�b�v�f�[�^</param>
		NavGraphNode(const int& index, const Vec3& position, const maru::ImpactData& impactData);

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="index">���̃N���X�̃C���f�b�N�X</param>
		/// <param name="position">���W</param>
		/// <param name="impactData">�e���}�b�v�f�[�^</param>
		/// <param name="parent">�e�I�u�W�F�N�g</param>
		NavGraphNode(const int& index, const Vec3& position, const maru::ImpactData& impactData, const std::shared_ptr<GameObject>& parent);

		//--------------------------------------------------------------------------------------
		///	�A�N�Z�b�T
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// ���W�̃Z�b�g
		/// </summary>
		/// <param name="position">���W</param>
		void SetPosition(const Vec3& position) noexcept { m_position = position; }

		/// <summary>
		/// ���W�̎擾
		/// </summary>
		/// <returns>���W</returns>
		Vec3 GetPosition() const;

		/// <summary>
		/// ���[�J�����W�̎擾
		/// </summary>
		/// <returns>���[�J�����W</returns>
		Vec3 GetLocalPosition() const noexcept { return m_position; }

		/// <summary>
		/// �e���f�[�^�̃Z�b�g
		/// </summary>
		/// <param name="data">�e���f�[�^</param>
		void SetImpactData(const ImpactData& data) noexcept { m_impactData = data; }

		/// <summary>
		/// �e���f�[�^�̎擾
		/// </summary>
		/// <returns>�e���f�[�^</returns>
		ImpactData GetImpactData() const noexcept { return m_impactData; }

		/// <summary>
		/// �e�I�u�W�F�N�g�����݂��邩�ǂ������f
		/// </summary>
		/// <returns>�e�����݂���Ȃ�true</returns>
		bool IsParent() const noexcept { return m_parent; }

		/// <summary>
		/// �e�I�u�W�F�N�g�̃Z�b�g
		/// </summary>
		/// <param name="parent">�e�I�u�W�F�N�g</param>
		void SetParent(const std::shared_ptr<GameObject>& parent) noexcept { m_parent = parent; }

		/// <summary>
		/// �e�I�u�W�F�N�g�̎擾
		/// </summary>
		/// <returns>�e�I�u�W�F�N�g</returns>
		std::shared_ptr<GameObject> GetParent() const { return m_parent.GetShard(); }

	};

}

//endbasecross