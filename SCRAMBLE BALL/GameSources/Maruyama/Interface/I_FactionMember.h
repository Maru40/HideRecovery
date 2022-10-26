
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
		//enum class FactionType;
		class EnemyBase;
		template<class T>
		class CoordinatorBase;
		class FactionCoordinator;

		//--------------------------------------------------------------------------------------
		/// �O���[�v�����o�[�̃C���^�[�t�F�[�X�̃p�����[�^
		//--------------------------------------------------------------------------------------
		struct I_FactionMember_Parametor
		{
			//FactionType factionType = FactionType(0);					//�O���[�v�^�C�v
			std::weak_ptr<FactionCoordinator> factionCoordinator;		//�O���[�v������
			std::weak_ptr<CoordinatorBase<EnemyBase>> assignedFaction;	//�A�T�C�������O���[�v
		};

		//--------------------------------------------------------------------------------------
		/// �O���[�v�����o�[�̃C���^�[�t�F�[�X
		//--------------------------------------------------------------------------------------
		class I_FactionMember {
		public:
			using Parametor = I_FactionMember_Parametor;

		private:
			Parametor m_param = Parametor();	//�p�����[�^

		public:
			virtual ~I_FactionMember() = default;

			///// <summary>
			///// �O���[�v�^�C�v�̐ݒ�
			///// </summary>
			///// <param name="type">�O���[�v�^�C�v</param>
			//void SetFactionType(const FactionType& type) noexcept { m_param.factionType = type; }

			///// <summary>
			///// �O���[�v�^�C�v�̎擾
			///// </summary>
			///// <returns>�O���[�v�^�C�v</returns>
			//FactionType GetFactionType() const noexcept { m_param.factionType; }

			/// <summary>
			/// �O���[�v�����҂̐ݒ�
			/// </summary>
			/// <param name="faction">�O���[�v������</param>
			void SetFactionCoordinator(const std::shared_ptr<FactionCoordinator>& faction) { m_param.factionCoordinator = faction; }

			/// <summary>
			/// �O���[�v�����҂̎擾
			/// </summary>
			/// <returns>�O���[�v������</returns>
			std::shared_ptr<FactionCoordinator> GetFactionCoordinator() const { return m_param.factionCoordinator.lock(); }

			/// <summary>
			/// �A�T�C�����ꂽ�O���[�v�̐ݒ�
			/// </summary>
			/// <param name="faction">�A�T�C�����ꂽ�O���[�v</param>
			void SetAssignFaction(const std::shared_ptr<CoordinatorBase<EnemyBase>>& faction) { m_param.assignedFaction = faction; }

			/// <summary>
			/// �A�T�C�����ꂽ�O���[�v���擾
			/// </summary>
			/// <returns>�A�T�C�����ꂽ�O���[�v</returns>
			std::shared_ptr<CoordinatorBase<EnemyBase>> GetAssignedFaction() const { return m_param.assignedFaction.lock(); }

			/// <summary>
			/// �A�T�C�����ꂽ�O���[�v���L���X�g���Ď擾
			/// </summary>
			/// <returns>�L���X�g���ꂽ�O���[�v</returns>
			template<class T>
			std::shared_ptr<T> GetAssignedFaction() const { return dynamic_pointer_cast<T>(m_param.assignedFaction.lock()); }

			/// <summary>
			/// �^�[�Q�b�g�̐ݒ�
			/// </summary>
			/// <param name="target">�^�[�Q�b�g</param>
			virtual void SetTarget(const std::shared_ptr<GameObject>& target) = 0;

			/// <summary>
			/// �^�[�Q�b�g�̎擾
			/// </summary>
			/// <returns>�^�[�Q�b�g</returns>
			virtual std::shared_ptr<GameObject> GetTarget() const = 0;

			/// <summary>
			/// �������g�̃I�u�W�F�N�g���擾
			/// </summary>
			/// <returns>�������g�̃I�u�W�F�N�g</returns>
			virtual std::shared_ptr<GameObject> GetSelfObject() const = 0;

			/// <summary>
			/// �p�����[�^�ݒ�
			/// </summary>
			/// <param name="param">�p�����[�^</param>
			void SetParametor(const Parametor& parametor) noexcept { m_param = parametor; }

			/// <summary>
			/// �p�����[�^�̎擾
			/// </summary>
			/// <returns>�p�����[�^</returns>
			Parametor GetParametor() const noexcept { return m_param; };

		};

	}
}