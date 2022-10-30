/*!
@file Goal.cpp
@brief Goalクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "Goal.h"

#include "Maruyama/Player/Component/ItemBag.h"
#include "Maruyama/Interface/I_TeamMember.h"

#include "Maruyama/Item/HideItem.h"
#include "Itabashi/Item.h"

#include "Watanabe/DebugClass/Debug.h"

#include "Maruyama/Utility/Utility.h"
#include "HidePlace.h"
#include "Maruyama/Utility/Random.h"

#include "Itabashi/ObjectHider.h"
#include "Itabashi/OnlineManager.h"
#include "Itabashi/OnlinePlayerSynchronizer.h"

#include "Watanabe/Manager/PointManager.h"
#include "Watanabe/Component/PlayerAnimator.h"
#include "Watanabe/Interface/I_Performable.h"
#include "Watanabe/StageObject/FireworksObject.h"
#include "Watanabe/Component/BallAnimator.h"

#include "Maruyama/Utility/Timer/GameTimer.h"

#include "Maruyama/Player/Component/TackleAttack.h"
#include "Maruyama/Player/Component/GoalAnimationController.h"
#include "Maruyama/Interface/I_TeamMember.h"
#include "Watanabe/UI/SplashMessageUI.h"
#include "Watanabe/BoardPoly/RelocationCountBP.h"

#include "Watanabe/BoardPoly/GoalBP.h"

#include "Watanabe/Manager/PointManager.h"
#include "Watanabe/Manager/TimeManager.h"
#include "Watanabe/Manager/ScoreManager.h"

#include "Patch/PlayerInputer.h"
#include "MainStage.h"

#include "Itabashi/OnlineGameItemManager.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	/// オンライン用データ
	//--------------------------------------------------------------------------------------
	struct OnlineGoalData
	{
		team::TeamType team;
		int playerNumber;

		OnlineGoalData(team::TeamType team, int playerNumber) :
			team(team),
			playerNumber(playerNumber)
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
		dunkBallPositionOffset(Vec3(0.0f, 1.85f, -0.5f)),
		goalBPOffset(Vec3(0.0f, 3.0f, 0.0f)),
		countDrawPositionOffset(Vec3(0.0f, 5.0f, 0.0f))
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
		m_goalEffectSoundClip(L"GoalEffectSE", false, 0.5f),
		m_relocationBallSoundClip(L"RelocationBallSE", false, 8.0f)
	{}

	void Goal::OnCreate() {
		//カウントダウンの生成
		//auto relocationCount = GetStage()->Instantiate<RelocationCountBP>(m_param.countDrawPositionOffset, Quat::Identity());
		//m_relocationCount = relocationCount;
	}

	void Goal::OnLateStart() {
		if (auto mainStage = dynamic_pointer_cast<MainStage>(GetStage())) {
			auto goalBP = GetStage()->Instantiate<GoalBP>();
			goalBP->GetComponent<Transform>()->SetPosition(transform->GetPosition() + m_param.goalBPOffset);
			m_goalBP = goalBP;
		}

		SettingPerformable();
		m_soundEmitter = GetGameObject()->GetComponent<SoundEmitter>();

		m_onlineGameItemManager = maru::Utility::FindComponent<OnlineGameItemManager>(GetStage());
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
		auto relocationCount = m_relocationCount.lock();
		float leftTime = m_timer->GetLeftTime();
	}

	void Goal::GoalProcess(const std::shared_ptr<GameObject>& other, const std::shared_ptr<Item>& item)
	{
		auto otherTrans = other->GetComponent<Transform>();

		if (!m_splashMessageUI.lock()) {	//スプラッシュメッセージの取得
			m_splashMessageUI = maru::Utility::FindGameObject<SplashMessageUI>(GetStage());
		}

		auto splash = m_splashMessageUI.lock();	//スプラッシュの取得

		//タックル状態なら状態をリセット
		if (auto tackle = other->GetComponent<TackleAttack>(false)) {
			tackle->ForceTaskReset();
		}

		//ゴールアニメーションの設定
		if (auto goalAnimationController = other->GetComponent<GoalAnimationController>(false)) {
			goalAnimationController->SetDunkPosition(GetDunkPosition());
			goalAnimationController->SetDunkBallPosition(transform->GetPosition() + m_param.dunkBallPositionOffset);
			goalAnimationController->SetGoal(GetThis<Goal>());
			goalAnimationController->SetGoalPerson(other);
		}

		//ポイント加算
		if (auto teamMember = other->GetComponent<I_TeamMember>(false)) {
			AddPoint(teamMember->GetTeam());
		}

		//カウントダウンスタート
		if (Online::OnlineManager::GetLocalPlayer().getIsMasterClient())
		{
			std::weak_ptr<OnlineGameItemManager> weakGameItemManager = m_onlineGameItemManager;
			std::weak_ptr<HideItem> weakHideItem = item->GetGameObject()->GetComponent<HideItem>();

			auto itemHiderEvent = [this, weakHideItem]()
			{
				m_onlineGameItemManager.lock()->RandomHideItem(weakHideItem.lock());
				GoalItemRelocation();
			};

			StartCountDown(itemHiderEvent);
		}
		else
		{
			StartCountDown([this]() {GoalItemRelocation(); });
		}

		//ゴール通知
		auto playerController = other->GetComponent<OnlinePlayerSynchronizer>(false);
		ScoreManager::GetInstance()->AddGoalCount(playerController->GetGamePlayerNumber());
	}

	void Goal::SuccessGoal(const CollisionPair& pair) {
		auto other = pair.m_Dest.lock()->GetGameObject();
		auto itemBag = other->GetComponent<ItemBag>();
		auto hideItem = itemBag->GetHideItem();
		auto item = hideItem->GetGameObject()->GetComponent<Item>();
		auto onlinePlayerSynchronizer = other->GetComponent<OnlinePlayerSynchronizer>();

		itemBag->RemoveItem(item);

		auto hidePlaces = maru::Utility::FindComponents<HidePlace>(GetStage());
		auto hidePlace = maru::MyRandom::RandomArray(hidePlaces);

		GoalProcess(other, item);

		auto data = OnlineGoalData(m_param.team, onlinePlayerSynchronizer->GetOnlinePlayerNumber());
		Online::OnlineManager::RaiseEvent(false, (std::uint8_t*)&data, sizeof(OnlineGoalData), EXECUTE_GOAL_EVENT_CODE);
	}

	void Goal::SuccessGoal(team::TeamType team, int playerNumber)
	{
		if (GetTeam() != team) {
			return;
		}

		auto onlinePlayerSynchronizer = OnlinePlayerSynchronizer::GetOnlinePlayerSynchronizer(playerNumber);
		auto other = onlinePlayerSynchronizer->GetGameObject();
		auto itemBag = other->GetComponent<ItemBag>();
		auto hideItem = itemBag->GetHideItem();
		auto item = hideItem->GetItem();
		itemBag->RemoveItem(item);

		auto hidePlaces = maru::Utility::FindComponents<HidePlace>(GetStage());

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
		if (auto animator = other->GetComponent<PlayerAnimator>()) {
			animator->ChangePlayerAnimation(PlayerAnimationState::State::Goal1);
		}
	}

	void Goal::GoalItemRelocation() {

		auto splash = m_splashMessageUI.lock();
		splash->SetMessage(SplashMessageUI::MessageType::Relocation);		//

		m_soundEmitter.lock()->PlaySoundClip(m_relocationBallSoundClip);	//音の再生

		//HidePlaceをClose状態にする処理。
		for (auto& place : maru::Utility::FindComponents<HidePlace>(GetStage())) {	
			place->Close();
		}

		//アニメーションコントローラーをfalseにする。(無理やりだから、将来的に変える。)
		for (auto& animationController : maru::Utility::FindComponents<GoalAnimationController>(GetStage())) {
			//animationController->SetUpdateActive(false);
		}
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

		m_soundEmitter.lock()->PlaySoundClip(m_goalEffectSoundClip);
	}

	void Goal::OnCustomEventAction(int playerNumber, std::uint8_t eventCode, const std::uint8_t* bytes)
	{
		if (eventCode == EXECUTE_GOAL_EVENT_CODE)
		{
			auto data = *(OnlineGoalData*)bytes;
			SuccessGoal(data.team, data.playerNumber);
			return;
		}
	}
}

//endbasecross