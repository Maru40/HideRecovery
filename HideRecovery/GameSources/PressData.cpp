/*!
@file PressData.cpp
@brief PressDataのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "PressData.h"
#include "Pusher.h"

namespace basecross {

	/// <summary>
	/// 潰してきた相手が死ぬときに呼び出したいイベントの追加
	/// </summary>
	/// <param name="data">潰しデータ</param>
	void AddPusherDestroyEvent(PressData* data) {
		if (auto pusherComp = data->pusher->GetComponent<Pusher>(false)) {
			//削除時に呼び出したい処理。
			pusherComp->AddDestroyAction([data](Pusher& pusher) {
				data->lostPusherPosition = pusher.GetGameObject()->GetComponent<Transform>()->GetPosition();
			});
		}
	}

	//--------------------------------------------------------------------------------------
	/// 潰しデータ本体
	//--------------------------------------------------------------------------------------

	PressData::PressData()
		:PressData(nullptr, nullptr, nullptr)
	{}

	PressData::PressData(
		const std::shared_ptr<GameObject>& pusher,
		const std::shared_ptr<GameObject>& pressed,
		const std::shared_ptr<GameObject>& obstacle
	) :
		PressData(pusher, pressed, obstacle, SlowTimeParametor(), MissData(), PressType::Miss, CollisionPair())
	{}

	PressData::PressData(
		const std::shared_ptr<GameObject>& pusher,
		const std::shared_ptr<GameObject>& pressed,
		const std::shared_ptr<GameObject>& obstacle,
		const SlowTimeParametor& slowParam,
		const MissData& missData,
		const PressType pressType,
		const CollisionPair& collisionPair
	) :
		pusher(pusher),
		pressed(pressed),
		obstacle(obstacle),
		slowParam(slowParam),
		missData(missData),
		pressType(pressType),
		collisionPair(collisionPair),
		pressedVec(Vec3(0.0f)),
		lostPusherPosition(Vec3(0.0f))
	{
		if (this->pusher) {
			AddPusherDestroyEvent(this);
		}

		//変更された場合に呼び出したい処理
		this->pusher.AddSubscribe(
			[]() { return true; },
			[&]() { 
				if (this->pusher) {
					//Pusherが削除時に呼びたいイベントの登録
					AddPusherDestroyEvent(this);
				}
			} 
		);
	}

	std::shared_ptr<Pusher> PressData::GetPusher() const {
		return pusher ? pusher->GetComponent<Pusher>(false) : nullptr;
	}

	float PressData::CalculatePressPower() const {
		return pressedVec.length();
	}

	Vec3 PressData::GetPressedToPuserVec() const {
		return pusher->GetComponent<Transform>()->GetPosition() - pressed->GetComponent<Transform>()->GetPosition();
	}

	Vec3 PressData::GetPusherToPressedVec() const {
		return pressed->GetComponent<Transform>()->GetPosition() - pusher->GetComponent<Transform>()->GetPosition();
	}

	Vec3 PressData::GetHitPointToPressedVec() const {
		return pressed->GetComponent<Transform>()->GetPosition() - collisionPair.m_CalcHitPoint;
	}

	Vec3 PressData::GetPusherPosition() const {
		return pusher ? pusher->GetComponent<Transform>()->GetPosition() : lostPusherPosition;
	}

}

//endbasecross
