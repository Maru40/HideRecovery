
/*!
@file EnemyAIDirector.h
@brief EnemyAIDirector�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "Maruyama/Enemy/AIDirector/TupleSpace.h"

namespace basecross {

	namespace team {
		enum class TeamType;
	}

	namespace Enemy {

		//--------------------------------------------------------------------------------------
		/// �O���錾
		//--------------------------------------------------------------------------------------
		class EnemyBase;
		class FactionCoordinator;

		namespace Tuple {
			class FindBall;
		}

		//--------------------------------------------------------------------------------------
		/// �t�B�[���h�S�̂̐���
		//--------------------------------------------------------------------------------------
		class AIDirector : public maru::SingletonComponent<AIDirector>, public Tuple::I_Tupler
		{
			//�S�ẴG�l�~�[
			vector<std::weak_ptr<EnemyBase>> m_enemys;

			//�O���[�v�Ǘ��̔z��
			std::vector<std::shared_ptr<FactionCoordinator>> m_factionCoordinators;

			//�^�v���X�y�[�X
			std::shared_ptr<Tuple::TupleSpace> m_tupleSapce;

		public :
			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
			AIDirector(const std::shared_ptr<GameObject>& objPtr);

			void OnCreate() override;
			void OnLateStart() override;
			void OnUpdate() override;

		private:
			/// <summary>
			/// �{�[�����������ʒm���󂯎�����Ƃ��̏����B
			/// </summary>
			void NotifyTuple_FindBall(const std::shared_ptr<Tuple::FindBall>& tuple);

			/// <summary>
			/// �����z�u���ꂽ�G�l�~�[��S�Ď擾
			/// </summary>
			void SettingStartAllEnemys();

			/// <summary>
			/// �O���[�v�Ǘ��̐���
			/// </summary>
			/// <returns>���������h��</returns>
			std::shared_ptr<FactionCoordinator> CreateFaction(const std::vector<std::weak_ptr<EnemyBase>>& assignMembers);

			/// <summary>
			/// �O���[�v�Ǘ��̍폜
			/// </summary>
			/// <param name="removeCoordinator">�폜�������h��</param>
			/// <returns>�폜�ł�����true</returns>
			bool RemoveFaction(const std::shared_ptr<FactionCoordinator>& removeCoordinator);

			/// <summary>
			/// �`�[�����ƂɃ}�b�v�ɓG�𕪂��ĕԂ��B
			/// </summary>
			/// <param name="enemys">�`�[�����Ƃɕ������������o�[</param>
			/// <returns>�}�b�v�Ń`�[�����Ƃɕ������G�l�~�[�z��</returns>
			std::unordered_map<team::TeamType, std::vector<weak_ptr<EnemyBase>>> DivideTeamType(const std::vector<std::weak_ptr<EnemyBase>>& members);

		public:
			/// <summary>
			/// �Q�[���J�n����AIDirector�ɑS�Ă̓G���A�T�C�����鏈��
			/// </summary>
			void StartAssign();

			//--------------------------------------------------------------------------------------
			/// �A�N�Z�b�T
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// �G�l�~�[�̒ǉ�
			/// </summary>
			/// <param name="enemy">�G�l�~�[</param>
			void AddEnemy(const std::shared_ptr<EnemyBase>& enemy);

			/// <summary>
			/// �O���[�v�Ǘ��̎擾
			/// </summary>
			/// <returns></returns>
			std::shared_ptr<FactionCoordinator> GetFactionCoordinator(const int index) const;

			/// <summary>
			/// �^�v���X�y�[�X�̎擾
			/// </summary>
			/// <returns></returns>
			_NODISCARD std::shared_ptr<Tuple::TupleSpace> GetTupleSpace() const noexcept override;

		};

	}
}