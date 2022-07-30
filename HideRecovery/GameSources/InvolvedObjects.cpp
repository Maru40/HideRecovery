/*!
@file InvolvedObjects.cpp
@brief 巻き込まれるオブジェクトなど実体
*/

#include "stdafx.h"

#include "StageObjectBase.h"
#include "InvolvedObjects.h"
#include "InvolvedManager.h"

#include "Player.h"
#include "Pusher.h"

#include "GameTimer.h"
#include "TimerDestroy.h"
#include "VelocityManager.h"

#include "CollisionObject.h"

namespace basecross
{
	void InvolvedObjects::OnCreate()
	{
		// 描画用コンポーネントの設定
		auto draw = AddComponent<BcPNTStaticModelDraw>();
		draw->SetMeshResource(L"tree_dead2"); // 見た目を設定

		//メッシュの調整用Mat
		Mat4x4 spanMat;
		spanMat.affineTransformation(
			Vec3(0.2f, 1.0f, 1.0f),
			Vec3(0.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f)
		);

		draw->SetMeshToTransformMatrix(spanMat); // メッシュの設定

		// 初期位置の設定
		auto transComp = GetComponent<Transform>();
		transComp->SetPosition(m_StartPos);


		// 衝突判定の設定
		auto collComp = AddComponent<CollisionObb>();
		collComp->SetMakedSize(1.25f);

		// コンポーネントの追加
		AddComponent<Gravity>();
		AddComponent<InvolvedManager>(GetThis<InvolvedObjects>());
		AddComponent<Pusher>();
		AddComponent<VelocityManager>();

		SetAlphaActive(true); // 透明度を適用する
	}

	void InvolvedObjects::OnUpdate()
	{
		transform->SetScale(Vec3(1.0f / 0.2f, 1.0f, 1.0f));
	}

	void InvolvedObjects::OnDestroy()
	{
		if (m_ColObj)
		{
			// 相手を消去
			GetStage()->RemoveGameObject<GameObject>(m_ColObj.GetShard());
		}
	}

	void InvolvedObjects::Damaged(int damage)
	{
		m_Endurance -= damage; // ダメージ分体力を減らす
		Breaking(); // 破壊する
	}

	void InvolvedObjects::Breaking()
	{
		if (m_Endurance <= 0) // 体力が0以下なら
		{
			// 1秒後に自身を消去する
			AddComponent<TimerDestroy>(1.0f);
		}
	}
}