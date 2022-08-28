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
#include "Watanabe/Interface/I_Performable.h"
#include "Watanabe/StageObject/FireworksObject.h"
#include "Watanabe/Component/BallAnimator.h"

#include "GameTimer.h"

#include "TackleAttack.h"
#include "Maruyama/Player/Component/GoalAnimationController.h"
#include "Maruyama/Interface/I_TeamMember.h"

#include "PlayerInputer.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	/// オンライン用データ
	//--------------------------------------------------------------------------------------
	struct OnlineGoalData
	{
		team::TeamType team;
		int playerNumber;
		int itemId;
		Vec3 hidePosition;

		OnlineGoalData(team::TeamType team, int playerNumber, int itemId, const Vec3& hidePosition) :
			team(team),
			playerNumber(playerNumber),
			itemId(itemId),
			hidePosition(hidePosition)
		{}
	};

	//--------------------------------------------------------------------------------------
	/// ゴール管理クラスのパラメータ
	//--------------------------------------------------------------------------------------

	Goal_Parametor::Goal_Parametor(const team::TeamType& team) :
		team(team),
		itemHiderTime(3.0f),
		timeDrawPosition(Vec3(0.0f, 0.0f, 0.0f)),
		dunkPositionOffset(Vec3(0.0f, 3.0f, 0.0f)),
		dunkBallPositionOffset(Vec3(0.0f, 1.85f, -0.5f))
	{
		constexpr float fOffset = +1.0f;
		constexpr float fOffsetY = -0.5f;
		firePositionOffsets = {
			Vec3(+fOffset, fOffsetY, +fOffset),
			Vec3(+fOffset, fOffsetY, -fOffset),
			Vec3(-fOffset, fOffsetY, +fOffset),
			Vec3(-fOffset, fOffsetY, -fOffset),
		};
	}

	//--------------------------------------------------------------------------------------
	/// ゴール管理クラス本体
	//--------------------------------------------------------------------------------------

	Goal::Goal(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor) :
		OnlineComponent(objPtr),
		m_param(parametor),
		m_timer(new GameTimer(0)),
		m_goalEffectSoundClip(L"GoalEffectSE", false, 0.5f)
	{}

	void Goal::OnLateStart() {
		SettingPerformable();
		m_soundEmitter = GetGameObject()->GetComponent<SoundEmitter>();
	}

	void Goal::OnUpdate() {
		if (!m_timer->IsTimeUp()) {
			TimerUpdate();
		}
	}

	void Goal::SettingPerformable() {
		for (auto offset : m_param.firePositionOffsets) {
			auto fire = GetStage()->Instantiate<FireworksObject>(transform->GetPosition() + offset, Quat::Identity());
			m_fireEffets.push_back(fire);
		}
	}

	void Goal::TimerUpdate() {
		m_timer->UpdateTimer();

		//残り時間を渡す。
		float leftTime = m_timer->GetLeftTime();
	}

	Vec3 Goal::GoalProcess(const std::shared_ptr<GameObject>& other, const std::shared_ptr<Item>& item)
	{
		auto otherTrans = other->GetComponent<Transform>();

		//タックル状態なら状態をリセット
		if (auto tackle = other->GetComponent<TackleAttack>(false)) {
			tackle->ForceTaskReset();
		}

		//ゴールアニメーションの設定
		if (auto goalAnimationController = other->GetComponent<GoalAnimationController>(false)) {
			goalAnimationController->SetDunkPosition(GetDunkPosition());
			goalAnimationController->SetDunkBallPosition(transform->GetPosition() + m_param.dunkBallPositionOffset);
			goalAnimationController->SetGoal(GetThis<Goal>());
		}

		//ポイント加算
		if (auto teamMember = other->GetComponent<I_TeamMember>(false)) {
			AddPoint(teamMember->GetTeam());
		}

		//再配置場所の取得
		auto hidePlaces = maru::Utility::FindComponents<HidePlace>();
		auto hidePlace = maru::MyRandom::RandomArray(hidePlaces);
		std::weak_ptr<Operator::ObjectHider> weakObjectHider = item->GetGameObject()->GetComponent<Operator::ObjectHider>();

		//アイテム再配置イベント
		auto itemHiderEvent = [weakObjectHider, hidePlace]() {
			auto hider = weakObjectHider.lock();
			hider->Appear(hidePlace->GetHidePosition());

			hidePlace->SetHideItem(hider->GetGameObject()->GetComponent<HideItem>(false));
		};

		//カウントダウンスタート
		StartCountDown(itemHiderEvent);

		Debug::GetInstance()->Log(L"SuccessGoal");

		return hidePlace->GetHidePosition();
	}

	void Goal::SuccessGoal(const CollisionPair& pair) {
		auto other = pair.m_Dest.lock()->GetGameObject();
		auto itemBag = other->GetComponent<ItemBag>();
		auto hideItem = itemBag->GetHideItem();
		auto item = hideItem->GetGameObject()->GetComponent<Item>();
		auto onlineController = other->GetComponent<Online::PlayerOnlineController>();

		itemBag->RemoveItem(item);

		auto hidePosition = GoalProcess(other, item);

		auto data = OnlineGoalData(m_param.team, onlineController->GetPlayerNumber(), item->GetItemId(), hidePosition);
		Online::OnlineManager::RaiseEvent(false, (std::uint8_t*)&data, sizeof(OnlineGoalData), EXECUTE_GOAL_EVENT_CODE);
	}

	void Goal::SuccessGoal(team::TeamType team, int playerNumber, int itemId, const Vec3& hidePosition)
	{
		if (GetTeam() != team) {
			return;
		}

		auto onlineController = Online::PlayerOnlineController::GetPlayerOnlineController(playerNumber);
		auto other = onlineController->GetGameObject();
		auto itemBag = other->GetComponent<ItemBag>();
		auto item = itemBag->GetItem(itemId);
		itemBag->RemoveItem(item);

		GoalProcess(other, item);

		PlayAnimation(other);
	}

	void Goal::AddPoint(const team::TeamType& team) {
		if (auto& pointManager = PointManager::GetInstance()) {
			pointManager->AddPoint(team);
		}
	}

	void Goal::StartCountDown(const std::function<void()>& endEvent) {
		m_timer->ResetTimer(m_param.itemHiderTime, endEvent);
	}

	void Goal::PlayAnimation(const std::shared_ptr<GameObject>& other) {
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

		SuccessGoal(pair);

		//アニメーションの再生
		PlayAnimation(pair.m_Dest.lock()->GetGameObject());
	}

	void Goal::PlayFireEffects() {
		for (auto weakFire : m_fireEffets) {
			if (auto fire = weakFire.lock()) {
				fire->Start();
			}
		}
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