
/*!
@file TupleSpace.cpp
@brief TupleSpace�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/Utility/MaruAction.h"
#include "TupleSpace.h"

#include "Watanabe/DebugClass/Debug.h"

namespace basecross {

	namespace Enemy {

		namespace Tuple {
			//--------------------------------------------------------------------------------------
			/// �^�v���̃C���^�[�t�F�[�X
			//--------------------------------------------------------------------------------------

			bool I_Tuple::operator== (const I_Tuple& other) {
				return this == &other;
			}

			//--------------------------------------------------------------------------------------
			/// ��]�s���̊��N���X
			//--------------------------------------------------------------------------------------

			TupleActionBase::TupleActionBase(
				const std::shared_ptr<GameObject>& acter, 
				const std::shared_ptr<GameObject>& requester, 
				const float value
			) :
				acter(acter),
				requester(requester),
				value(value)
			{}

			//--------------------------------------------------------------------------------------
			/// ���N�G�X�g�̊��N���X
			//--------------------------------------------------------------------------------------

			TupleRequestBase::TupleRequestBase(
				const std::shared_ptr<I_Tupler>& requester, 
				const float value
			) :
				m_requester(requester),
				m_value(value)
			{}

			bool TupleRequestBase::operator== (const TupleRequestBase& other) {
				if (m_requester.lock() == other.GetRequester() &&
					m_value == other.GetValue())
				{
					return true;
				}

				return false;
			}

			//--------------------------------------------------------------------------------------
			/// �^�[�Q�b�g�����������Ƃ�m�点��^�v��
			//--------------------------------------------------------------------------------------

			FindTarget::FindTarget(const std::shared_ptr<I_Tupler>& requester, const std::shared_ptr<GameObject>& target, const float value):
				TupleRequestBase(requester, value), m_target(target)
			{}

			bool FindTarget::operator== (const FindTarget& other) {
				if (GetRequester() == other.GetRequester() &&
					GetTarget() == other.GetTarget() &&
					GetValue() == other.GetValue())
				{
					return true;
				}

				return false;
			}

			//--------------------------------------------------------------------------------------
			/// �^�[�Q�b�g�ƃo�g�����邱�Ƃ����N�G�X�g����^�v��
			//--------------------------------------------------------------------------------------

			ButtleTarget::ButtleTarget(
				const std::shared_ptr<I_Tupler>& requester,
				const std::shared_ptr<GameObject>& target,
				const float value
			):
				TupleRequestBase(requester, value),
				target(target)
			{}

			bool ButtleTarget::operator==(const ButtleTarget& other) {
				if (GetRequester() == other.GetRequester() &&
					GetTarget() == other.GetTarget() &&
					GetValue() == other.GetValue())
				{
					return true;
				}

				return false;
			}

			//--------------------------------------------------------------------------------------
			/// �o�g���ɑJ�ڂ��邱�Ƃ����N�G�X�g����^�v��
			//--------------------------------------------------------------------------------------

			ButtleTransition::ButtleTransition(
				const std::shared_ptr<I_Tupler>& requester,
				const std::shared_ptr<GameObject>& target,
				const float value
			):
				TupleRequestBase(requester, value),
				m_target(target)
			{}

			bool ButtleTransition::operator==(const ButtleTransition& other) {
				if (GetRequester() == other.GetRequester() &&
					GetTarget() == other.GetTarget() &&
					GetValue() == other.GetValue())
				{
					return true;
				}

				return false;
			}

			//--------------------------------------------------------------------------------------
			/// �p�g���[���ɑJ�ڂ��邱�Ƃ����N�G�X�g����^�v��
			//--------------------------------------------------------------------------------------

			PatrolTransition::PatrolTransition(
				const std::shared_ptr<I_Tupler>& requester,
				const float value
			):
				TupleRequestBase(requester, value)
			{}

			bool PatrolTransition::operator==(const PatrolTransition& other) {
				if (GetRequester() == other.GetRequester() &&
					GetValue() == other.GetValue())
				{
					return true;
				}

				return false;
			}

			//--------------------------------------------------------------------------------------
			/// �_���[�W���󂯂����Ƃ�`����^�v��
			//--------------------------------------------------------------------------------------

			Damaged::Damaged(
				const std::shared_ptr<I_Tupler>& requester,
				const DamageData& data,
				const float value
			):
				TupleRequestBase(requester, value),
				m_damageData(data)
			{}

			bool Damaged::operator ==(const Damaged& other) {
				if (GetRequester() == other.GetRequester() &&
					GetDamageData().attacker == other.GetDamageData().attacker &&
					GetValue() == other.GetValue())
				{
					return true;
				}

				return false;
			}

			//--------------------------------------------------------------------------------------
			///	Help�^�v��
			//--------------------------------------------------------------------------------------

			HelpAction::HelpAction(
				const std::shared_ptr<I_Tupler>& requester,
				const std::shared_ptr<GameObject>& target,
				const float value
			):
				TupleRequestBase(requester, value),
				m_target(target)
			{}

			bool HelpAction::operator ==(const HelpAction& other) {
				if (GetRequester() == other.GetRequester() &&
					GetTarget() == other.GetTarget() &&
					GetValue() == other.GetValue())
				{
					return true;
				}

				return false;
			}

			//--------------------------------------------------------------------------------------
			///	FindBall�^�v��
			//--------------------------------------------------------------------------------------

			FindBall::FindBall(
				const std::shared_ptr<I_Tupler>& requester,
				const std::shared_ptr<I_TeamMember>& teamMember,
				const float value
			):
				TupleRequestBase(requester, value),
				m_teamMember(teamMember)
			{}

			bool FindBall::operator== (const FindBall& other) {
				if (GetRequester() == other.GetRequester() &&
					GetValue() == other.GetValue())
				{
					return true;
				}

				return false;
			}

			std::shared_ptr<I_TeamMember> FindBall::GetTeamMember() const noexcept {
				return m_teamMember.lock();
			}

			//--------------------------------------------------------------------------------------
			///	Kill�^�v��
			//--------------------------------------------------------------------------------------

			Kill::Kill(
				const std::shared_ptr<I_Tupler>& requester,
				const std::shared_ptr<GameObject>& killer,
				const std::shared_ptr<GameObject>& killed,
				const float value
			) :
				TupleRequestBase(requester, value),
				m_killer(killer),
				m_killed(killed)
			{}

			bool Kill::operator==(const Kill& other) {
				if (GetRequester() == other.GetRequester()	&&
					GetValue() == other.GetValue()			&&
					GetKiller() == other.GetKiller()		&&
					GetKilled() == other.GetKilled() 
				) {
					return true;
				}

				return false;
			}

			std::shared_ptr<GameObject> Kill::GetKiller() const noexcept {
				return m_killer.lock();
			}

			std::shared_ptr<GameObject> Kill::GetKilled() const noexcept {
				return m_killed.lock();
			}

			//--------------------------------------------------------------------------------------
			///	�^�[�Q�b�g�������������Ƃ�ʒm
			//--------------------------------------------------------------------------------------

			LostTarget::LostTarget(
				const std::shared_ptr<I_Tupler>& requester,
				const std::shared_ptr<TargetManager>& targetManager,
				const float value
			):
				TupleRequestBase(requester, value),
				m_targetManager(targetManager)
			{}

			bool LostTarget::operator == (const LostTarget& other) {
				if (GetRequester() == other.GetRequester() &&
					GetValue() == other.GetValue())
				{
					return true;
				}

				return false;
			}

			//--------------------------------------------------------------------------------------
			///	�^�[�Q�b�g�̌��������肢����^�v��
			//--------------------------------------------------------------------------------------

			SearchTarget::SearchTarget(
				const std::shared_ptr<I_Tupler>& requester,
				const std::shared_ptr<TargetManager>& targetManager,
				const float value
			):
				TupleRequestBase(requester, value),
				m_targetManager(targetManager)
			{}

			bool SearchTarget::operator ==(const SearchTarget& other) {
				if (GetRequester() == other.GetRequester() &&
					GetValue() == other.GetValue())
				{
					return true;
				}

				return false;
			}

			//--------------------------------------------------------------------------------------
			/// �^�v���X�y�[�X�{��
			//--------------------------------------------------------------------------------------

			bool TupleSpace::RemoveAllNotifys(const std::shared_ptr<I_Tupler>& tupler) {
				std::vector<std::function<bool()>> removeFuncs;

				//�폜��������
				for (auto& pair : m_notifysMap) {
					for (auto& notify : pair.second) {
						if (notify->GetRequester() == tupler) {
							removeFuncs.push_back([&, notify]() { return RemoveNotify(notify); });
						}
					}
				}

				//�폜����
				for (auto& removeFunc : removeFuncs) {
					bool isRemove = removeFunc();
				}

				return !removeFuncs.empty();	//empty�Ȃ�폜������ĂȂ����Ƃ������B
			}

			bool TupleSpace::RemoveAllTuples(const std::shared_ptr<I_Tupler>& tupler) {
				std::vector<std::function<bool()>> removeFuncs;

				//�폜��������
				for (auto& pair : m_tuplesMap) {
					for (auto& tuple : pair.second) {
						if (tuple->GetRequester() == tupler) {
							removeFuncs.push_back([&, tuple]() { return RemoveTuple(tuple); });
						}
					}
				}

				//�폜����
				for (auto& removeFunc : removeFuncs) {
					bool isRemove = removeFunc();
				}

				return !removeFuncs.empty();	//empty�Ȃ�폜������ĂȂ����Ƃ������B
			}
		}
	}
}