
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

#include "Watanabe/Manager/PointManager.h"
#include "Watanabe/Component/PlayerAnimator.h"

#include "GameTimer.h"

#include "TackleAttack.h"
#include "Maruyama/Player/Component/GoalAnimationController.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// オンライン用データ
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
		{}
	};

	//--------------------------------------------------------------------------------------
	/// ゴール管理クラスのパラメータ
	//--------------------------------------------------------------------------------------

	Goal_Parametor::Goal_Parametor(const Team& team) :
		team(team),
		itemHiderTime(3.0f),
		timeDrawPosition(Vec3(0.0f, 0.0f, 0.0f)),
		dunkPositionOffset(Vec3(0.0f, 3.0f, 0.0f))
	{}

	//--------------------------------------------------------------------------------------
	/// ゴール管理クラス本体
	//--------------------------------------------------------------------------------------

	Goal::Goal(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor):
		OnlineComponent(objPtr),
		m_param(parametor),
		m_timer(new GameTimer(0))
	{}

	void Goal::OnCreate() {

	}

	void Goal::OnUpdate() {
		if (!m_timer->IsTimeUp()) {
			TimerUpdate();
		}
	}

	void Goal::TimerUpdate() {
		m_timer->UpdateTimer();

		//残り時間を渡す。
		float leftTime = m_timer->GetLeftTime();
	}

	void Goal::SuccessGoal(const CollisionPair& pair) {
		auto other = pair.m_Dest.lock()->GetGameObject();

		//タックル状態なら状態をリセット
		if (auto tackle = other->GetComponent<TackleAttack>(false)) {
			tackle->ForceTaskReset();
		}

		//ゴールアニメーションの設定
		if (auto goalAnimationController = other->GetComponent<GoalAnimationController>(false)) {
			goalAnimationController->SetDunkPosition(transform->GetPosition() + m_param.dunkPositionOffset);
		}

		//ポイント加算
		AddPoint(GetTeam());

		//アイテム削除
		if (auto itemBag = other->GetComponent<ItemBag>(false)) {
			auto hideItem = itemBag->GetHideItem();
			auto item = hideItem->GetGameObject()->GetComponent<Item>();
			itemBag->RemoveItem(hideItem->GetGameObject()->GetComponent<Item>(false));
			auto onlineController = other->GetComponent<Online::PlayerOnlineController>();

			//再配置場所の取得
			auto hidePlaces = maru::Utility::FindComponents<HidePlace>();
			auto hidePlace = maru::MyRandom::RandomArray(hidePlaces);

			//アイテム再配置イベント
			auto itemHiderEvent = [hideItem, hidePlace]() {
				auto hider = hideItem->GetObjectHider();
				hider->Appear(hidePlace->GetHidePosition());
			};

			auto data = OnlineGoalData(m_param.team, onlineController->GetPlayerNumber(), item->GetItemId(), hidePlace->GetHidePosition());
			Online::OnlineManager::RaiseEvent(false, (std::uint8_t*)&data, sizeof(OnlineGoalData), EXECUTE_GOAL_EVENT_CODE);

			//カウントダウンスタート
			StartCountDown(itemHiderEvent);
		}

		Debug::GetInstance()->Log(L"SuccessGoal");
	}

	void Goal::SuccessGoal(Team team, int playerNumber, int itemId, const Vec3& hidePosition)
	{
		if (GetTeam() != team) {
			return;
		}

		auto onlineController = Online::PlayerOnlineController::GetPlayerOnlineController(playerNumber);
		auto other = onlineController->GetGameObject();

		//ポイント加算
		AddPoint(GetTeam());

		//アイテム削除
		if (auto itemBag = other->GetComponent<ItemBag>(false)) {
			auto item = itemBag->GetItem(itemId);
			auto hideItem = item->GetGameObject()->GetComponent<HideItem>();
			itemBag->RemoveItem(item);

			//アイテムの配置イベント
			auto itemHiderEvent = [hideItem, hidePosition]() {
				auto hider = hideItem->GetObjectHider();
				hider->Appear(hidePosition);
			};

			//カウントダウン開始
			StartCountDown(itemHiderEvent);
		}

		Debug::GetInstance()->Log(L"SuccessGoal");
	}

	void Goal::AddPoint(const Team& team) {
		if (auto& pointManager = PointManager::GetInstance()) {
			pointManager->AddPoint(team);
		}
	}

	void Goal::StartCountDown(const std::function<void()>& endEvent) {
		m_timer->ResetTimer(m_param.itemHiderTime, endEvent);
	}

	void Goal::PlayAnimation(const CollisionPair& pair) {
		auto other = pair.m_Dest.lock()->GetGameObject();
		auto animator = other->GetComponent<PlayerAnimator>();
		if (!animator) {
			return;
		}

		//ゴールを決めるアニメーションに変更
		animator->ChangePlayerAnimation(PlayerAnimationState::State::Goal1);
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

		if (!Online::OnlineManager::GetLocalPlayer().getIsMasterClient()) {
			return false;
		}

		return true;
	}

	void Goal::OnCollisionEnter(const CollisionPair& pair) {
		if (!IsCollision(pair)) {	//判定を取らないなら処理を飛ばす。
			return;
		}

		//アニメーションの再生
		PlayAnimation(pair);

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