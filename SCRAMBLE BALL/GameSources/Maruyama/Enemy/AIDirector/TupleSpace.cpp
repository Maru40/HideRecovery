
/*!
@file TupleSpace.cpp
@brief TupleSpace�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/Utility/MaruAction.h"
#include "TupleSpace.h"

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

		}
	}
}