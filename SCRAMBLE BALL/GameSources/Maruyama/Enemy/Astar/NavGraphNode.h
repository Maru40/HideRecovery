
/*!
@file NavGraphNode.h
@brief NavGraphNode�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "GraphNodeBase.h"

#include "Maruyama/Interface/I_Impact.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�O���錾
	//--------------------------------------------------------------------------------------
	template<class T> 
	class ReactiveProperty;

	namespace maru {
		template<class T> 
		class Action;
	}

	//--------------------------------------------------------------------------------------
	///	�i�r�O���t�p�̃m�[�h
	//--------------------------------------------------------------------------------------
	class NavGraphNode : public GraphNodeBase
	{
	public:
		using ImpactData = maru::ImpactData;

	private:
		Vec3 m_position;											//�m�[�h�̏ꏊ
		std::unique_ptr<ReactiveProperty<ImpactData>> m_impactData;	//�e���}�b�v�f�[�^

		std::weak_ptr<GameObject> m_parent;							//�e�I�u�W�F�N�g

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

		/// <summary>
		/// �R�s�[�R���X�g���N�^
		/// </summary>
		/// <param name="node">�R�s�[����m�[�h</param>
		NavGraphNode(const NavGraphNode& node);

		virtual ~NavGraphNode();

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
		void SetImpactData(const ImpactData& data) noexcept;

		/// <summary>
		/// �e���f�[�^�̎擾
		/// </summary>
		/// <returns>�e���f�[�^</returns>
		ImpactData GetImpactData() const noexcept;

		/// <summary>
		/// �e�I�u�W�F�N�g�����݂��邩�ǂ������f
		/// </summary>
		/// <returns>�e�����݂���Ȃ�true</returns>
		bool IsParent() const noexcept { return !m_parent.expired(); }

		/// <summary>
		/// �e�I�u�W�F�N�g�̃Z�b�g
		/// </summary>
		/// <param name="parent">�e�I�u�W�F�N�g</param>
		void SetParent(const std::shared_ptr<GameObject>& parent) noexcept { m_parent = parent; }

		/// <summary>
		/// �e�I�u�W�F�N�g�̎擾
		/// </summary>
		/// <returns>�e�I�u�W�F�N�g</returns>
		std::shared_ptr<GameObject> GetParent() const { return m_parent.lock(); }

		/// <summary>
		/// �C���p�N�g�f�[�^�̕ύX���ɌĂяo�������C�x���g�̒ǉ�
		/// </summary>
		/// <param name="whereAction">������</param>
		/// <param name="action">�Ăяo�������C�x���g</param>
		void AddSubscribeImpactData(const std::function<bool()>& whereAction, const std::function<void()>& action);

		/// <summary>
		/// �G���A�C���f�b�N�X�̐ݒ�
		/// </summary>
		/// <param name="index">�G���A�C���f�b�N�X</param>
		void SetAreaIndex(const int index) noexcept;

		/// <summary>
		/// �G���A�C���f�b�N�X�̎擾
		/// </summary>
		/// <returns>�G���A�C���f�b�N�X</returns>
		int GetAreaIndex() const noexcept;

		//--------------------------------------------------------------------------------------
		///	�I�y���[�^
		//--------------------------------------------------------------------------------------

		NavGraphNode& operator= (const NavGraphNode& other);
	};

}

//endbasecross