
/*!
@file Goal.cpp
@brief Goalクラス実体
担当：丸山裕喜
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
#include "Itabashi/OnlineManager.h"
#include "Itabashi/PlayerOnlineController.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// ゴール管理クラスのパラメータ
	//--------------------------------------------------------------------------------------

	struct OnlineGoalData
	{
		Team team;
		int playerNumber;
		int itemId;
		Vec3 hidePosition;

		OnlineGoalData(Team team,int playerNumber, int itemId,const Vec3& hidePosition) :
			team(team),
			playerNumber(playerNumber),
			itemId(itemId),
			hidePosition(hidePosition)
		{

		}
	};

	Goal_Parametor::Goal_Parametor(const Team& team) :
		team(team)
	{}

	//--------------------------------------------------------------------------------------
	/// ゴール管理クラス本体
	//--------------------------------------------------------------------------------------

	Goal::Goal(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor):
		OnlineComponent(objPtr), m_param(parametor)
	{}

	void Goal::OnCreate() {

	}

	void Goal::OnUpdate() {

	}

	void Goal::SuccessGoal(const CollisionPair& pair) {
		auto other = pair.m_Dest.lock()->GetGameObject();

		//ポイント加算


		//アイテム削除
		if (auto itemBag = other->GetComponent<ItemBag>(false)) {
			auto hideItem = itemBag->GetHideItem();
			auto item = hideItem->GetGameObject()->GetComponent<Item>();
			itemBag->RemoveItem(hideItem->GetGameObject()->GetComponent<Item>(false));
			auto onlineController = other->GetComponent<Online::PlayerOnlineController>();

			//アイテム再配置
			auto hidePlaces = maru::Utility::FindComponents<HidePlace>();
			auto hidePlace = maru::MyRandom::RandomArray(hidePlaces);
			
			auto hider = hideItem->GetObjectHider();
			hider->Appear(hidePlace->GetHidePosition());

			auto data = OnlineGoalData(m_param.team, onlineController->GetPlayerNumber(), item->GetItemId(), hidePlace->GetHidePosition());
			Online::OnlineManager::RaiseEvent(false, (std::uint8_t*)&data, sizeof(OnlineGoalData), EXECUTE_GOAL_EVENT_CODE);
		}

		Debug::GetInstance()->Log(L"SuccessGoal");
	}

	void Goal::SuccessGoal(Team team, int playerNumber, int itemId, const Vec3& hidePosition)
	{
		if (m_param.team != team)
		{
			return;
		}

		auto onlineController = Online::PlayerOnlineController::GetPlayerOnlineController(playerNumber);
		auto other = onlineController->GetGameObject();

		//ポイント加算


		//アイテム削除
		if (auto itemBag = other->GetComponent<ItemBag>(false)) {
			auto item = itemBag->GetItem(itemId);
			auto hideItem = item->GetGameObject()->GetComponent<HideItem>();
			itemBag->RemoveItem(item);

			auto hider = hideItem->GetObjectHider();
			hider->Appear(hidePosition);
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

		//チームタイプが同じなら、当たり判定をしない
		if (teamMember->GetTeam() == GetTeam()) {
			return false;
		}

		auto hideItem = itemBag->GetHideItem();
		if (!hideItem) {	//隠しアイテムを持っていないならfalse
			return false;
		}

		if (!Online::OnlineManager::GetLocalPlayer().getIsMasterClient())
		{
			return false;
		}

		return true;
	}

	void Goal::OnCollisionEnter(const CollisionPair& pair) {
		if (!IsCollision(pair)) {	//判定を取らないなら処理を飛ばす。
			return;
		}

		SuccessGoal(pair);
	}

	void Goal::OnCustomEventAction(int playerNumber, std::uint8_t eventCode, const std::uint8_t* bytes)
	{
		if (eventCode == EXECUTE_GOAL_EVENT_CODE)
		{
			auto data = *(OnlineGoalData*)bytes;
			SuccessGoal(data.team, data.playerNumber, data.itemId, data.hidePosition);
			return;
		}
	}
}

//endbasecross