/*!
@file WaterPressure.cpp
@brief ジェットポンプなど実体
*/

#include "stdafx.h"
#include "GameStage.h"
#include "WaterPressure.h"
#include "WaterCollisionManager.h"

#include "LaserBase.h"
#include "LaserCollider.h"

#include "WaterDraw.h"

namespace basecross
{
	void WaterPressure::OnCreate()
	{
		// 描画用コンポーネントを追加
		AddComponent<WaterDraw>();

		// タグを追加
		AddTag(L"Water");
	}

	void WaterPressure::Move()
	{
		// デルタ時間を取得
		auto delta = App::GetApp()->GetElapsedTime();

		auto transComp = GetComponent<Transform>(); // トランスフォームを取得
		auto myPos = transComp->GetPosition(); // ポジションを取得

		// ポジションを更新
		auto newPos = myPos + m_MoveDir * m_MovePow * delta;
		newPos.y = m_StartPos.y;

		transComp->SetPosition(newPos); // ポジションをセット
		transform->SetForward(m_MoveDir); // 向きを前方に回転
	}

	void WaterPressure::CapturedCamera()
	{
		std::vector<Vec3> points = { // 進行方向に対して後ろに頂点を配置する
			m_MoveDir * 0.0f,
			m_MoveDir * -m_Weight,
			m_MoveDir * (-m_Height + m_Weight),
			m_MoveDir * -m_Height
		};

		int index = 0;

		auto camera = GetStage()->GetView()->GetTargetCamera(); // カメラを取得
		std::vector<VertexPositionColorTexture> vertices;
		auto myPos = GetComponent<Transform>()->GetPosition();

		for (auto& point : points) // 中心点の左右に頂点を配置する
		{
			auto pointToCamera = camera->GetEye() - (point + myPos); // ポイントを「ワールド座標に移動させた位置」からみたカメラの方向
			auto cross = pointToCamera.cross(m_MoveDir); // 「カメラへのベクトル」と「進行方向」に「垂直なベクトル」を算出
			cross.normalize(); // ベクトルの向きだけ使いたいので正規化する
			vertices[index++].position = point + -cross * m_Weight; // 左
			vertices[index++].position = point + +cross * m_Weight; // 右
		}
		auto drawComp = GetComponent<PCTStaticDraw>();
		drawComp->UpdateVertices(vertices); // 位置を変更した頂点を適用する

	}

	void WaterPressure::Erasure()
	{
		// 描画と更新をしなくする
		SetDrawActive(false);
		SetUpdateActive(false);
	}
}