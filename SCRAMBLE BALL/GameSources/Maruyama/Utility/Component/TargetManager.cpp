/*!
@file TargetManager.cpp
@brief TargetManager����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "TargetManager.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	���������^�[�Q�b�g�f�[�^
	//--------------------------------------------------------------------------------------

	struct TargetManager::LostData
	{
		bool isLost = false;
		Vec3 position;
	};

	//--------------------------------------------------------------------------------------
	///	�p�����[�^
	//--------------------------------------------------------------------------------------

	struct TargetManager::Parametor
	{
		ex_weak_ptr<GameObject> target = nullptr;
		LostData lostData;

		//�^�[�Q�b�g�����X�g���Ă��邩�ǂ���
		bool IsLost() const { return lostData.isLost; }
		bool SetIsLost(const bool isLost) {
			lostData.isLost = isLost;
			if (isLost && target) { //�^�[�Q�b�g�����݂��ALost�����Ȃ�
				//���X�g�����ꏊ���Z�b�g�B
				lostData.position = target->GetComponent<Transform>()->GetPosition();
			}
		}
	};

	//--------------------------------------------------------------------------------------
	///	�^�[�Q�b�g�Ǘ��{��
	//--------------------------------------------------------------------------------------

	TargetManager::TargetManager(const std::shared_ptr<GameObject>& objPtr)
		:Component(objPtr), m_paramPtr(new Parametor())
	{}

	void TargetManager::OnCreate() {

	}

	//--------------------------------------------------------------------------------------
	///	�A�N�Z�b�T
	//--------------------------------------------------------------------------------------

	bool TargetManager::HasTarget() const {
		if (!m_paramPtr->target) {
			return false;
		}

		//return m_paramPtr->target->IsActive();

		return m_paramPtr->target ? true : false;
	}

	Vec3 TargetManager::GetTargetPosition() const {
		return GetTarget()->GetComponent<Transform>()->GetPosition();
	}

	Vec3 TargetManager::GetLostPosition() const {
		return m_paramPtr->lostData.position;
	}

	void TargetManager::SetTarget(const std::shared_ptr<GameObject>& target) {
		if (target == nullptr && GetTarget()) {
			m_paramPtr->lostData.position = GetTargetPosition();
		}

		m_paramPtr->target = target;
	}

	std::shared_ptr<GameObject> TargetManager::GetTarget() const {
		return m_paramPtr->target.GetShard();
	}

	Vec3 TargetManager::CalcuToTargetVec() {
		return GetTarget()->GetComponent<Transform>()->GetPosition() - transform->GetPosition();
	}
}

//endbasecross