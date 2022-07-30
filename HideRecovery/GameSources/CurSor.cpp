/*!
@file Corsor.cpp
@brief カーソルなど実体
*/

#include "stdafx.h"
#include "Cursor.h"
#include "PolygonTexture.h"

namespace basecross
{
	void Cursor::OnCreate()
	{
		//板ポリの定義
		Col4 color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
		std::vector<VertexPositionColorTexture> vertices = {
			{Vec3(-16.0f, +16.0f, 0.0f), color, Vec2(0.0f, 0.0f)},
			{Vec3(+16.0f, +16.0f, 0.0f), color, Vec2(1.0f, 0.0f)},
			{Vec3(-16.0f, -16.0f, 0.0f), color, Vec2(0.0f, 1.0f)},
			{Vec3(+16.0f, -16.0f, 0.0f), color, Vec2(1.0f, 1.0f)}
		};
		std::vector<uint16_t> indices = {
			0, 1, 2,
			2, 1, 3
		};

		auto drawComp = AddComponent<PCTSpriteDraw>(vertices, indices); //描画用コンポーネントの設定
		drawComp->SetTextureResource(L"Cursor"); //テクスチャの設定

		SetDrawActive(true); // 描画状態を設定
		SetAlphaActive(true); // 透明度を適用
		SetDrawLayer(0); // 描画レイヤーを設定

	}

	void Cursor::AddUiTexture(const CursorUIData& data)
	{
		m_UiObj.push_back(data); // 管理するUIデータを追加
		m_cursorObj.push_back(GetStage()->AddGameObject<PolygonTexture>(data.uiData)); // ステージにUIを追加
	}

	void Cursor::ClearUITexture()
	{
		m_cursorObj.clear();
		m_UiObj.clear();
	}

	void Cursor::Select()
	{
		if (m_UiObj[m_CursorPos].func.operator bool()) // 関数が実行可能なら
		{
			m_UiObj[m_CursorPos].func(); // 実行
		}
	}

	void Cursor::MoveCursor(int pos)
	{
		m_CursorPos += pos; //ポジションを更新

		PositionCorrection(pos); // ポジションを補正

		PositionCorrection(-pos); // 配列外に出ている可能性があるので補正

		// ここからトランスフォームの変更

		auto cursorTrans = GetComponent<Transform>(); // トランスフォームを取得　
		auto cursorScale = cursorTrans->GetScale(); // スケールを取得

		auto uiData = m_UiObj[m_CursorPos].uiData; // UIのデータを取得

		auto newPos = uiData.position; // UIの位置を取得
		newPos.x -= uiData.vertexPos.x / 2 * uiData.scale.x + cursorScale.x * m_PositionOffset; // UIの左横に配置されるように修正
		cursorTrans->SetPosition(newPos); // ポジションを更新
	}

	void Cursor::PositionCorrection(int pos)
	{
		// 選択不可なら飛ばす
		while (!m_UiObj[m_CursorPos].isSelectable)
		{
			m_CursorPos += pos;
		}

		// 配置されているUIの数を取得
		int size = (int)m_UiObj.size();

		// ポジションが配列の範囲外なら範囲内に入れる
		if (m_CursorPos >= size)
		{
			m_CursorPos = size - 1;
		}
		if (m_CursorPos < 0)
		{
			m_CursorPos = 0;
		}
	}
}