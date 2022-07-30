/*!
@file WaterCollisionManager.cpp
@brief 水の衝突判定実体
*/

#include "stdafx.h"
#include "WaterCollisionManager.h"
#include "WaterPressure.h"

#include "Player.h"
#include "I_PushedWater.h"

namespace basecross
{
	void WaterCollisionManager::OnCollisionEnter(const CollisionPair& pair)
	{
		auto collObj = pair.m_Dest.lock()->GetGameObject(); // 相手のオブジェクトを取得
		auto player = dynamic_pointer_cast<Player>(collObj);
		if (!player) // プレイヤーじゃなかったら
		{
			// 自分を消去
			auto water = dynamic_pointer_cast<WaterPressure>(this->GetGameObject());
			if (water) {
				water->Erasure();
			}
		}

		// 相手が水に押されるかを判定
		auto pushedWater = collObj->GetComponent<I_PushedWater>(false);
		if (pushedWater)
		{
			auto obj = this->GetGameObject(); // 自分を取得

			PushData data; // 押し出しのデータ

			data.direction = this->transform->GetForward(); // 前方方向に押す
			data.power = 10.0f; // 押す強さ
			data.hitPoint = pair.m_CalcHitPoint; // 当たった位置

			pushedWater->Push(data); // dataをもとに相手のことを押す
		}

	}

	void WaterCollisionManager::OnCollisionExcute(const CollisionPair& pair)
	{
		auto collObj = pair.m_Dest.lock()->GetGameObject(); // 衝突した相手を取得
		auto pushedWater = collObj->GetComponent<I_PushedWater>(false); // 相手が水に押されるかを取得
		if (pushedWater) // 押せるなら
		{
			auto obj = this->GetGameObject();

			PushData data; // 押し出しのデータ

			data.direction = this->transform->GetForward(); // 前方方向に押す
			data.power = 10.0f; // 押す強さ
			data.hitPoint = pair.m_CalcHitPoint; // 当たった位置

			pushedWater->Push(data); // 相手を押す
		}
	}
}