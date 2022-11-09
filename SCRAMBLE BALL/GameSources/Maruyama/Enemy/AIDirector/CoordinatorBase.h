
/*!
@file CoordinatorBase.h
@brief CoordinatorBase�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

#include "TupleSpace.h"

namespace basecross {

	namespace Enemy {

		//--------------------------------------------------------------------------------------
		/// �O���錾
		//--------------------------------------------------------------------------------------
		class I_FactionMember;

		namespace Tuple {
			class TupleSpace;
		}

		//--------------------------------------------------------------------------------------
		/// �����҂̊��N���X
		//--------------------------------------------------------------------------------------
		template<class T>
		class CoordinatorBase : public std::enable_shared_from_this<CoordinatorBase<T>>, public Tuple::I_Tupler
		{
		private:
			std::vector<std::weak_ptr<T>> m_members;			//�o�^���ꂽ�����o�[�z��

			std::shared_ptr<Tuple::TupleSpace> m_tupleSpace;	//�^�v���X�y�[�X

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
				m_members(members),
				m_tupleSpace(new Tuple::TupleSpace())
			{}
			
			virtual ~CoordinatorBase() = default;

			/// <summary>
			/// �������ɌĂяo������
			/// </summary>
			virtual void OnCreate() {}

			/// <summary>
			/// �J�n���ɌĂяo������(���݂͐������ɌĂяo���Ă���B)
			/// </summary>
			virtual void OnStart() {}

			virtual bool OnUpdate() = 0;

			/// <summary>
			/// �I�����ɌĂяo����������
			/// </summary>
			virtual void OnExit() {}

		public:
			//--------------------------------------------------------------------------------------
			/// �A�N�Z�b�T
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// �^�v���X�y�[�X�̎擾
			/// </summary>
			/// <returns>�^�v���X�y�[�X</returns>
			std::shared_ptr<Tuple::TupleSpace> GetTupleSpace() const noexcept { return m_tupleSpace; }

			/// <summary>
			/// �����o�[�̔z����擾
			/// </summary>
			/// <returns>�����o�[�z��</returns>
			std::vector<std::weak_ptr<T>> GetMembers() const noexcept { return m_members; }

			/// <summary>
			/// �����o�[�̒ǉ�
			/// </summary>
			/// <param name="member">�����o�[</param>
			virtual void AddMember(const std::shared_ptr<T>& member) { m_members.push_back(member); }

			/// <summary>
			/// �����o�[�̍폜
			/// </summary>
			/// <param name="member">�����o�[</param>
			/// <returns>�폜�ɐ���������true</returns>
			virtual bool RemoveMember(const std::shared_ptr<T>& member) {
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

			template<class T>
			std::shared_ptr<T> GetThis() {
				auto Ptr = std::dynamic_pointer_cast<T>(shared_from_this());
				if (Ptr) {
					return Ptr;
				}
				else {
					wstring str(L"this��");
					str += Util::GetWSTypeName<T>();
					str += L"�^�ɃL���X�g�ł��܂���";
					throw BaseException(
						str,
						L"if( ! dynamic_pointer_cast<T>(shared_from_this()) )",
						L"ObjectInterface::GetThis()"
					);
				}
				return nullptr;
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

			virtual ~HereOwnerCoordinatorBase() = default;

			virtual void OnCreate() override {  
				for (const std::weak_ptr<MemberType>& member : GetMembers()) {
					SettingMember(member.lock());
				}
			}

		protected:

			virtual void SettingMember(const std::shared_ptr<I_FactionMember>& member) {
				member->SetFactionCoordinator(GetOwner());
				member->SetAssignFaction(GetThis<CoordinatorBase<MemberType>>());
			}

			virtual void SettingRemoveMember(const std::shared_ptr<I_FactionMember>& member) {
				member->SetAssignFaction(nullptr);	//�A�T�C������O���B
			}

		public:
			//--------------------------------------------------------------------------------------
			/// �A�N�Z�b�T
			//--------------------------------------------------------------------------------------

			/// <summary>
			/// ���L�҂̎擾
			/// </summary>
			/// <returns>���L��</returns>
			std::shared_ptr<OwnerType> GetOwner() const noexcept { return m_owner.lock(); } 

			/// <summary>
			/// �����o�[�̒ǉ�
			/// </summary>
			/// <param name="member">�ǉ������������o�[</param>
			virtual void AddMember(const std::shared_ptr<MemberType>& member) override {
				CoordinatorBase::AddMember(member);
				SettingMember(member);
			}

			virtual bool RemoveMember(const std::shared_ptr<MemberType>& member) override {
				bool isRemove = CoordinatorBase::RemoveMember(member);
				if (isRemove) {
					SettingRemoveMember(member);
				}

				return isRemove;
			}
		};

	}
}