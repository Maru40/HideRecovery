
/*!
@file FactionCoordinator.h
@brief FactionCoordinator�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "CoordinatorBase.h"

namespace basecross {

	namespace Enemy {

		//--------------------------------------------------------------------------------------
		/// �O���錾
		//--------------------------------------------------------------------------------------
		class AIDirector;
		class EnemyBase;
		class CombatCoordinator;
		class PatrolCoordinator;
		class FactionCoordinator;
		class I_FactionMember;

		namespace Tuple {
			class FindBall;
		}

		//--------------------------------------------------------------------------------------
		/// �O���[�v�^�C�v
		//--------------------------------------------------------------------------------------
		//enum class FactionType
		//{
		//	None,	//�������Ă��Ȃ�
		//	Patrol,	//�p�g���[��
		//	Combat,	//�퓬
		//};

		//--------------------------------------------------------------------------------------
		/// �O���[�v������
		//--------------------------------------------------------------------------------------
		class FactionCoordinator : public CoordinatorBase<EnemyBase>
		{
		public:
			using CoordinatorsMap = std::unordered_map<type_index, std::vector<std::shared_ptr<CoordinatorBase>>>;

		private:
			std::weak_ptr<AIDirector> m_director;	//AI�f�B���N�^�[

			CoordinatorsMap m_coordinatorsMap;		//���ꂼ��̃O���[�v���Ƃ̃}�b�v

			std::vector<std::shared_ptr<CoordinatorBase>> m_allCoordinators;	//�S�ẴO���[�v

		public:
			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="director">AI�f�B���N�^�[</param>
			FactionCoordinator(const std::shared_ptr<AIDirector>& director);

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="director">AI�f�B���N�^�[</param>
			/// <param name="members">�����o�[�z��</param>
			FactionCoordinator(const std::shared_ptr<AIDirector>& director, const std::vector<std::weak_ptr<EnemyBase>>& members);

			virtual ~FactionCoordinator() = default;

			void OnCreate() override;
			void OnStart() override;
			bool OnUpdate() override;
			void OnExit() override;

		private:
			/// <summary>
			/// �{�[�����������ʒm���󂯎�����Ƃ�
			/// </summary>
			/// <param name="tuple">�ʒm</param>
			void NotifyTuple_FindBall(const std::shared_ptr<Tuple::FindBall>& tuple);

			/// <summary>
			/// �O���[�v�̐���
			/// </summary>
			template<class T, class... Ts,
				std::enable_if_t<
					std::is_base_of_v<CoordinatorBase, T> &&
					std::is_constructible_v<T, const std::shared_ptr<FactionCoordinator>&, Ts...>,
				std::nullptr_t> = nullptr
			>
			std::shared_ptr<T> CreateFaction(Ts&&... params) {
				auto typeIndex = type_index(typeid(T));

				auto faction = std::make_shared<T>(GetThis<FactionCoordinator>(), params...);
				faction->OnCreate();
				faction->OnStart();
				m_coordinatorsMap[typeIndex].push_back(faction);	//�}�b�v�ɓo�^
				m_allCoordinators.push_back(faction);

				return faction;
			}

			/// <summary>
			/// �����o�[�̐U�蕪���ꏊ��T���āA�Ԃ��B
			/// </summary>
			template<class FactionType,
				std::enable_if_t<
					std::is_base_of_v<CoordinatorBase, FactionType>,	//���N���X�̐���
				std::nullptr_t> = nullptr
			>
			std::shared_ptr<FactionType> SearchAssignFaction() {
				auto typeIndex = type_index(typeid(FactionType));

				//�t�@�N�V���������݂��Ȃ��Ȃ�A����
				if (m_coordinatorsMap[typeIndex].size() == 0) {
					return CreateFaction<FactionType>();
				}

				auto coordinators = m_coordinatorsMap[typeIndex];

				//�{���Ȃ��Ԃ��������̃t�@�N�V������T���B
				auto result = std::dynamic_pointer_cast<FactionType>(coordinators[0]);
				return result;	//(���݂͕����̃t�@�N�V���������K�v���Ȃ����߁A�����I�Ȋg���|�C���g)
			}

			/// <summary>
			/// �����o�[���w�肵���t�@�N�V�����ɃA�T�C������B
			/// </summary>
			/// <returns>�A�T�C�����ꂽ�t�@�N�V����</returns>
			template<class FactionType,
				std::enable_if_t<
					std::is_base_of_v<CoordinatorBase, FactionType>,
				std::nullptr_t> = nullptr
			>
			std::shared_ptr<FactionType> AddFactionMember(const std::shared_ptr<I_FactionMember>& member) {
				auto typeIndex = type_index(typeid(FactionType));

				//�����o�[�̐U�蕪��������B
				auto assignFaction = SearchAssignFaction<FactionType>();
				assignFaction->AddMember(std::dynamic_pointer_cast<EnemyBase>(member));

				return assignFaction;
			}

		public:
			/// <summary>
			/// �����o�[�̃t�@�N�V�����J��
			/// </summary>
			template<class TransitionType,
				std::enable_if_t<
					std::is_base_of_v<CoordinatorBase, TransitionType>,
				std::nullptr_t> = nullptr
			>
			std::shared_ptr<TransitionType> TransitionFaction(const std::shared_ptr<I_FactionMember>& member) {
				//���X�A�T�C������Ă��������o�[����O���B(�ǂ��ɂ��A�T�C������Ă��Ȃ�������A�������΂��B)
				if (auto assignedFaction = member->GetAssignedFaction()) {
					assignedFaction->RemoveMember(std::dynamic_pointer_cast<EnemyBase>(member));
				}
				//assignedFaction->GetTupleSpace()->RemoveAllNotifys(member->GetSelfObject()->GetComponent<Tuple::I_Tupler>(false)); //�^�v���X�y�[�X�ɓo�^���ꂽ�҂�ύX

				return AddFactionMember<TransitionType>(member);
			}

			//--------------------------------------------------------------------------------------
			/// �A�N�Z�b�T
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// �f�B���N�^�[�̎擾
			/// </summary>
			/// <returns>�f�B���N�^�[</returns>
			std::shared_ptr<AIDirector> GetDirector() const noexcept { return m_director.lock(); }

			//--------------------------------------------------------------------------------------
			/// �f�o�b�O
			//--------------------------------------------------------------------------------------

			//�^�[�Q�b�g���������ƒm�点��f�o�b�O����
			void DebugWriteTarget(const std::shared_ptr<I_FactionMember>& member, const std::shared_ptr<GameObject>& target);
		};

	}
}