
/*!
@file Goal.cpp
@brief Goal�N���X����
�S���F�ێR�T��
*/


#include "stdafx.h"
#include "Project.h"

#include "Goal.h"

#include "ItemBag.h"
#include "Maruyama/Interface/I_TeamMember.h"

#include "HideItem.h"
#include "Itabashi/Item.h"

#include "Watanabe/DebugClass/Debug.h"

#include "MaruUtility.h"
#include "HidePlace.h"
#include "MyRandom.h"

#include "Itabashi/ObjectHider.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �S�[���Ǘ��N���X�̃p�����[�^
	//--------------------------------------------------------------------------------------

	Goal_Parametor::Goal_Parametor(const Team& team) :
		team(team)
	{}

	//--------------------------------------------------------------------------------------
	/// �S�[���Ǘ��N���X�{��
	//--------------------------------------------------------------------------------------

	Goal::Goal(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor):
		Component(objPtr), m_param(parametor)
	{}

	void Goal::OnCreate() {

	}

	void Goal::OnUpdate() {

	}

	void Goal::SuccessGoal(const CollisionPair& pair) {
		auto other = pair.m_Dest.lock()->GetGameObject();

		//�|�C���g���Z


		//�A�C�e���폜
		if (auto itemBag = other->GetComponent<ItemBag>(false)) {
			auto hideItem = itemBag->GetHideItem();
			itemBag->RemoveItem(hideItem->GetGameObject()->GetComponent<Item>(false));

			//�A�C�e���Ĕz�u
			auto hidePlaces = maru::Utility::FindComponents<HidePlace>();
			auto hidePlace = maru::MyRandom::RandomArray(hidePlaces);
			
			auto hider = hideItem->GetObjectHider();
			hider->Appear(hidePlace->GetHidePosition());
		}

		Debug::GetInstance()->Log(L"SuccessGoal");
	}

	bool Goal::IsCollision(const CollisionPair& pair) const {
		auto other = pair.m_Dest.lock()->GetGameObject();
		auto teamMember = other->GetComponent<I_TeamMember>(false);
		auto itemBag = other->GetComponent<ItemBag>(false);

		if (!itemBag || !teamMember) {
			return false;
		}

		//�`�[���^�C�v�������Ȃ�A�����蔻������Ȃ�
		if (teamMember->GetTeam() == GetTeam()) {
			return false;
		}

		auto hideItem = itemBag->GetHideItem();
		if (!hideItem) {	//�B���A�C�e���������Ă��Ȃ��Ȃ�false
			return false;
		}

		return true;
	}

	void Goal::OnCollisionEnter(const CollisionPair& pair) {
		if (!IsCollision(pair)) {	//��������Ȃ��Ȃ珈�����΂��B
			return;
		}

		SuccessGoar(pair);
	}
}

//endbasecross