/*!
@file CrushScore.cpp
@brief 潰した時の成功判定実体
*/

#include "stdafx.h"
#include "Scene.h"
#include "CrushScore.h"

#include "GameStage.h"
#include "PolygonTexture.h"

namespace basecross
{
	void CrushScore::OnUpdate()
	{
		// UIを移動させる
		MoveUi();
	}

	void CrushScore::DeployUi(const wstring& texture)
	{
		auto stage = GetStage(); // 現在のステージを取得
		// UIを配置
		auto data = UiData(texture, m_StartPos, Vec3(256.0 * 0.7f, 64.0f * 0.7, 0.0f), Vec2(1.0f, 1.0f));
		m_ScoreUi.push_back(stage->AddGameObject<PolygonTexture>(data));
	}

	void CrushScore::MoveUi()
	{
		for (auto& ui : m_ScoreUi)
		{
			auto delta = App::GetApp()->GetElapsedTime(); // デルタ時間を取得

			auto uiTrans = ui->GetComponent<Transform>(); // UIのトランスフォームを取得
			auto pos = uiTrans->GetPosition(); // UIのポジションを取得

			// ポジションを更新
			pos += m_MovePower * delta;
			uiTrans->SetPosition(pos);

			FadeOut(ui, pos, delta);
		}
	}

	void CrushScore::FadeOut(const std::shared_ptr<GameObject>& UI, const Vec3& position, float delta)
	{
		if (position.y >= 50) // Y方向が50以上なら
		{
			// フェードアウトさせる
			auto color = UI->GetComponent<PCTSpriteDraw>()->GetDiffuse();
			color.w -= 3.0f * delta;
			UI->GetComponent<PCTSpriteDraw>()->SetDiffuse(color);

			if (color.w <= 0) // フェードアウトし終わったら
			{
				// 描画と更新の処理を止める
				UI->SetUpdateActive(false);
				UI->SetDrawActive(false);
			}
		}
	}
}