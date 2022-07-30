/*!
@file InvolvedManager.cpp
@brief 巻き込まれるオブジェクトの衝突判定
*/

#include "stdafx.h"
#include "InvolvedManager.h"
#include "InvolvedObjects.h"

#include "Player.h"
#include "Crusher.h"

namespace basecross
{
	void InvolvedManager::OnCollisionEnter(const CollisionPair& pair)
	{
		auto collObj = pair.m_Dest.lock()->GetGameObject(); // 衝突相手を取得
		auto player = dynamic_pointer_cast<Player>(collObj); // 相手がプレイヤーか判定
		if (player)
		{
			ObjectRotation(pair.m_DestCalcHitCenter); // 回転
		}

		auto crusher = collObj->GetComponent<Crusher>(false); // 破壊者かを調べる
		if (crusher)
		{
			HitDamage(pair.m_CalcHitPoint); // ダメージ判定
		}
	}

	void InvolvedManager::HitDamage(const Vec3& point)
	{
		auto myPos = transform->GetPosition(); // 自分の位置
		auto scale = transform->GetScale(); //大きさ

		auto range = m_InvolObj->GetRangeFactor(); // ダメージを受ける範囲を取得
		auto hitScope = Vec2(scale.x * range.x, scale.z * range.y); // 被ダメージ範囲
		hitScope *= 0.1;

		//　当たった位置が範囲の中なら
		if ((point.x <= myPos.x + hitScope.x && point.x >= myPos.x - hitScope.x) ||
			(point.z <= myPos.z + hitScope.y && point.z >= myPos.z - hitScope.y))
		{
			m_InvolObj->Damaged(1); // ダメージを与える
		}
	}

	void InvolvedManager::ObjectRotation(const Vec3& hitCenter)
	{
		auto position = transform->GetPosition(); //現在の位置
		auto right = transform->GetRight(); //右方向
		auto forward = transform->GetForward(); //前方向
		auto hitDir = hitCenter - position; //ヒットした方向

		auto hitRight = dot(hitDir.normalize(), right.normalize()); //当たった位置が右方向に対して+か-か
		auto hitForward = dot(hitDir.normalize(), forward.normalize()); //当たった位置が前方向に対して+か-か

		auto scope = m_InvolObj->GetRotationScope();

		//中心から当たった位置までの距離が一定以下なら
		if (hitDir.length() <= scope)
		{
			auto delta = App::GetApp()->GetElapsedTime();
			transform->Rotate(Vec3::Up() * m_RotationPow * hitRight * hitForward * delta); //回転させる
		}
	}
}