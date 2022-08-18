
/*!
@file Goar.cpp
@brief Goarクラス実体
担当：丸山裕喜
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
	/// ゴール管理クラスのパラメータ
	//--------------------------------------------------------------------------------------

	Goar_Parametor::Goar_Parametor(const Team& team) :
		team(team)
	{}

	//--------------------------------------------------------------------------------------
	/// ゴール管理クラス本体
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

		//ポイント加算


		//アイテム削除
		if (auto itemBag = other->GetComponent<ItemBag>(false)) {
			auto hideItem = itemBag->GetHideItem();
			itemBag->RemoveItem(hideItem->GetGameObject()->GetComponent<Item>(false));
		}

		//アイテム再配置


		Debug::GetInstance()->Log(L"SuccessGoar");
	}

	bool Goar::IsCollision(const CollisionPair& pair) const {
		auto other = pair.m_Dest.lock()->GetGameObject();
		auto teamMember = other->GetComponent<I_TeamMember>(false);
		auto itemBag = other->GetComponent<ItemBag>(false);

		if (!itemBag || !teamMember) {
			return false;
		}

		//チームタイプが同じなら、当たり判定をしない
		if (teamMember->GetTeam() == GetTeam()) {
			return false;
		}

		auto hideItem = itemBag->GetHideItem();
		if (!hideItem) {	//隠しアイテムを持っていないならfalse
			return false;
		}

		return true;
	}

	void Goar::OnCollisionEnter(const CollisionPair& pair) {
		if (!IsCollision(pair)) {	//判定を取らないなら処理を飛ばす。
			return;
		}

		SuccessGoar(pair);
	}
}

//endbasecross