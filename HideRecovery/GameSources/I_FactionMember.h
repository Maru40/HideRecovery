
/*!
@file I_FactionMember.h
@brief I_FactionMember�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	namespace Enemy {

		//--------------------------------------------------------------------------------------
		/// �O���錾
		//--------------------------------------------------------------------------------------
		enum class FactionType;
		class EnemyBase;
		template<class T>
		class CoordinatorBase;
		class FactionCoordinator;

		//--------------------------------------------------------------------------------------
		/// �O���[�v�����o�[�̃C���^�[�t�F�[�X
		//--------------------------------------------------------------------------------------
		class I_FactionMember {
			FactionType m_factionType;										//�O���[�v�^�C�v
			std::weak_ptr<FactionCoordinator> m_factionCoordinator;			//�O���[�v������
			std::weak_ptr<CoordinatorBase<EnemyBase>> m_assignedFaction;	//�A�T�C�������O���[�v

		public:
			virtual ~I_FactionMember() = default;

			/// <summary>
			/// �O���[�v�^�C�v�̐ݒ�
			/// </summary>
			/// <param name="type">�O���[�v�^�C�v</param>
			void SetFactionType(const FactionType& type) noexcept { m_factionType = type; }

			/// <summary>
			/// �O���[�v�^�C�v�̎擾
			/// </summary>
			/// <returns>�O���[�v�^�C�v</returns>
			FactionType GetFactionType() const noexcept { m_factionType; }

			/// <summary>
			/// �O���[�v�����҂̐ݒ�
			/// </summary>
			/// <param name="faction">�O���[�v������</param>
			void SetFactionCoordinator(const std::shared_ptr<FactionCoordinator>& faction) { m_factionCoordinator = faction; }

			/// <summary>
			/// �O���[�v�����҂̎擾
			/// </summary>
			/// <returns>�O���[�v������</returns>
			std::shared_ptr<FactionCoordinator> GetFactionCoordinator() const { return m_factionCoordinator.lock(); }

			/// <summary>
			/// �A�T�C�����ꂽ�O���[�v�̐ݒ�
			/// </summary>
			/// <param name="faction">�A�T�C�����ꂽ�O���[�v</param>
			void SetAssignFaction(const std::shared_ptr<CoordinatorBase<EnemyBase>>& faction) { m_assignedFaction = faction; }

			/// <summary>
			/// �A�T�C�����ꂽ�O���[�v���擾
			/// </summary>
			/// <returns>�A�T�C�����ꂽ�O���[�v</returns>
			std::shared_ptr<CoordinatorBase<EnemyBase>> GetAssignedFaction() const { return m_assignedFaction.lock(); }

			/// <summary>
			/// �A�T�C�����ꂽ�O���[�v���L���X�g���Ď擾
			/// </summary>
			template<class T>
			std::shared_ptr<T> GetAssignedFaction() const { return dynamic_pointer_cast<T>(m_assignedFaction.lock()); }

		};

	}
}