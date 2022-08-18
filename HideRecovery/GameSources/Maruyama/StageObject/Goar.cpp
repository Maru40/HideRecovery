
/*!
@file Goar.cpp
@brief Goar�N���X����
�S���F�ێR�T��
*/


#include "stdafx.h"
#include "Project.h"

#include "Goar.h"

#include "ItemBag.h"
#include "Maruyama/Interface/I_TeamMember.h"

#include "HideItem.h"
#include "Itabashi/Item.h"

#include "Watanabe/DebugClass/Debug.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// �S�[���Ǘ��N���X�̃p�����[�^
	//--------------------------------------------------------------------------------------

	Goar_Parametor::Goar_Parametor(const Team& team) :
		team(team)
	{}

	//--------------------------------------------------------------------------------------
	/// �S�[���Ǘ��N���X�{��
	//--------------------------------------------------------------------------------------

	Goar::Goar(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor):
		Component(objPtr), m_param(parametor)
	{}

	void Goar::OnCreate() {

	}

	void Goar::OnUpdate() {

	}

	void Goar::SuccessGoar(const CollisionPair& pair) {
		auto other = pair.m_Dest.lock()->GetGameObject();

		//�|�C���g���Z


		//�A�C�e���폜
		if (auto itemBag = other->GetComponent<ItemBag>(false)) {
			auto hideItem = itemBag->GetHideItem();
			itemBag->RemoveItem(hideItem->GetGameObject()->GetComponent<Item>(false));
		}

		//�A�C�e���Ĕz�u


		Debug::GetInstance()->Log(L"SuccessGoar");
	}

	bool Goar::IsCollision(const CollisionPair& pair) const {
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

	void Goar::OnCollisionEnter(const CollisionPair& pair) {
		if (!IsCollision(pair)) {	//��������Ȃ��Ȃ珈�����΂��B
			return;
		}

		SuccessGoar(pair);
	}
}

//endbasecross