
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
				const std::shared_ptr<GameObject>& requester, 
				const float value
			) :
				requester(requester),
				value(value)
			{}

			bool TupleRequestBase::operator== (const TupleRequestBase& other) {
				if (this->requester.lock() == other.requester.lock() &&
					this->value == other.value)
				{
					return true;
				}

				return false;
			}

			//--------------------------------------------------------------------------------------
			/// �^�[�Q�b�g�����������Ƃ�m�点��^�v��
			//--------------------------------------------------------------------------------------

			FindTarget::FindTarget(const std::shared_ptr<GameObject>& requester, const std::shared_ptr<GameObject>& target, const float value):
				TupleRequestBase(requester, value), target(target)
			{}

			bool FindTarget::operator== (const FindTarget& other) {
				if (this->requester.lock() == other.requester.lock() &&
					this->target.lock() == other.target.lock() &&
					this->value == other.value)
				{
					return true;
				}

				return false;
			}

			//--------------------------------------------------------------------------------------
			/// �^�[�Q�b�g�ƃo�g�����邱�Ƃ����N�G�X�g����^�v��
			//--------------------------------------------------------------------------------------

			ButtleTarget::ButtleTarget(
				const std::shared_ptr<GameObject>& requester,
				const std::shared_ptr<GameObject>& target,
				const float value
			):
				TupleRequestBase(requester, value),
				target(target)
			{}

			//--------------------------------------------------------------------------------------
			/// �ʒm�p�f�[�^�Ǘ�
			//--------------------------------------------------------------------------------------

			//NotifyController::NotifyController(
			//	const std::function<void(const std::shared_ptr<I_Tuple>&)>& func,
			//	const std::function<bool(const std::shared_ptr<I_Tuple>&)>& isCall
			//):
			//	func(func),
			//	isCall(isCall)
			//{}

			//void NotifyController::Invoke(const std::shared_ptr<I_Tuple>& tuple) {
			//	if (isCall(tuple)) {
			//		func(tuple);
			//	}
			//}

			//--------------------------------------------------------------------------------------
			/// �^�v���X�y�[�X�{��
			//--------------------------------------------------------------------------------------

			//void TupleSpace::CallNotifys(const type_index typeIndex, const std::shared_ptr<I_Tuple>& tuple) {
			//	auto notifys = m_notifysMap[typeIndex];

			//	//�S�Ă̒ʒm���Ăяo���B
			//	for (auto& notify : notifys) {
			//		notify->Invoke(tuple);
			//	}
			//}

		}
	}
}