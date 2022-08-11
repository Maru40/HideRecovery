
/*!
@file CoordinatorBase.h
@brief CoordinatorBase�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	namespace Enemy {

		//--------------------------------------------------------------------------------------
		/// �����҂̊��N���X
		//--------------------------------------------------------------------------------------
		template<class T>
		class CoordinatorBase
		{
		private:
			std::vector<std::weak_ptr<T>> m_members;	//�o�^���ꂽ�����o�[�z��

		public:
			CoordinatorBase():
				CoordinatorBase(std::vector<std::weak_ptr<T>>())
			{}

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="member">�������郁���o�[</param>
			CoordinatorBase(const std::shared_ptr<T>& member):
				CoordinatorBase(std::vector<std::weak_ptr<T>>(1, member))
			{}

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="members">�������郁���o�[�ꗗ</param>
			CoordinatorBase(const vector<std::weak_ptr<T>>& members) :
				m_members(members)
			{}
			
			virtual ~CoordinatorBase() = default;

			virtual void OnStart() {}
			virtual void OnUpdate() {}
			virtual void OnExit() {}

		public:
			//--------------------------------------------------------------------------------------
			/// �A�N�Z�b�T
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// �����o�[�̔z����擾
			/// </summary>
			/// <returns>�����o�[�z��</returns>
			std::vector<std::weak_ptr<T>> GetMembers() const noexcept { return m_members; }

			/// <summary>
			/// �����o�[�̒ǉ�
			/// </summary>
			/// <param name="member">�����o�[</param>
			void AddMember(const std::shared_ptr<T>& member) { m_members.push_back(member); }

			/// <summary>
			/// �����o�[�̍폜
			/// </summary>
			/// <param name="member">�����o�[</param>
			/// <returns>�폜�ɐ���������true</returns>
			bool RemoveMember(const std::shared_ptr<T>& member) {
				auto iter = m_members.begin();
				while (iter != m_members.end()) {
					if (iter->lock() == member) {
						iter = m_members.erase(iter);
						return true;
					}
					iter++;
				}

				return false;
			}

		};


		//--------------------------------------------------------------------------------------
		/// ���L�҂����݂��钲���҂̊��N���X
		//--------------------------------------------------------------------------------------
		template<class OwnerType, class MemberType>
		class HereOwnerCoordinatorBase : public CoordinatorBase<MemberType>
		{
			std::weak_ptr<OwnerType> m_owner;	//���L��

		public:
			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="owner">���̃N���X�����L����N���X</param>
			HereOwnerCoordinatorBase(const std::shared_ptr<OwnerType>& owner):
				HereOwnerCoordinatorBase(owner, std::vector<std::weak_ptr<MemberType>>())
			{}

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="owner">���̃N���X�����L����N���X</param>
			/// <param name="member">�A�T�C�����郁���o�[</param>
			HereOwnerCoordinatorBase(const std::shared_ptr<OwnerType>& owner, const std::shared_ptr<MemberType>& member) :
				HereOwnerCoordinatorBase(owner, std::vector<std::weak_ptr<OwnerType>>(1, member))
			{}

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="owner">���̃N���X�����L����N���X</param>
			/// <param name="members">�A�T�C�����郁���o�[�ꗗ</param>
			HereOwnerCoordinatorBase(const std::shared_ptr<OwnerType>& owner, const vector<std::weak_ptr<MemberType>>& members) :
				CoordinatorBase(members), m_owner(owner)
			{}

			~HereOwnerCoordinatorBase() = default;

		public:
			//--------------------------------------------------------------------------------------
			/// �A�N�Z�b�T
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// ���L�҂̎擾
			/// </summary>
			/// <returns>���L��</returns>
			std::shared_ptr<OwnerType> GetOwner() const noexcept { return m_owner.lock(); }
		};

	}
}