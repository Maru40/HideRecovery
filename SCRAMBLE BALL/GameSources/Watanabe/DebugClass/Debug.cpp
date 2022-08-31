/*!
@file   Debug.cpp
@brief  デバッグクラス実体
*/

#include "stdafx.h"
#include "Debug.h"
#include <time.h>

namespace basecross {
	Debug::Debug(const shared_ptr<Stage>& stage)
		:BaseSingletonGameObject(stage), m_displayLogFirstIndex(0),
		// 以下設定
		m_maxLogCount(18),
		m_isDisplayForCount(true),
		m_isActive(false),
		m_defaultRect(10, 10, 500, 400)
	{}

	// 静的メンバ変数の実体
	shared_ptr<Debug> Debug::m_ownInstance = nullptr;

	void Debug::OnCreate() {
		// インスタンスを初期化して自身のインスタンスを代入
		CreateInstance();

		// 前面に描画
		SetDrawLayer(200);
		// 文字列表示用コンポーネント
		m_ssComp = AddComponent<StringSprite>();
		// 文字列表示領域のサイズを変更
		m_ssComp->SetTextRect(m_defaultRect);
		m_ssComp->SetText(L"");
		SetUpdateActive(m_isActive);
		SetDrawActive(m_isActive);
	}

	void Debug::OnUpdate() {
		// ログの数を取得
		int logCount = static_cast<int>(m_logData.size());
		// ログの数が最大表示数より多い場合は
		// 最新のログから（最新のログ - 最大表示数）までのログを表示する
		if (logCount > m_maxLogCount) {
			int diff = logCount - m_maxLogCount;
			m_displayLogFirstIndex = diff;
		}

		wstring wstr = L"";
		wstr += GetSystemInfo(); // システム情報
		wstr += BorderInsertion(); // 罫線
		if (m_isDisplayForCount) {
			// ログの数
			wstr += L"LogCount : " + Util::SizeTToWStr(m_logData.size()) + L"\n";
			wstr += BorderInsertion(); //罫線
		}
		// ログの表示
		for (int i = m_displayLogFirstIndex; i < logCount; i++) {
			wstr += m_logData[i];
		}
		m_ssComp->SetText(wstr);
	}

	void Debug::Log(const wstring& text) {
		RegisterLog(text);
	}

	void Debug::Log(int value) {
		RegisterLog(Util::IntToWStr(value));
	}

	void Debug::Log(float value) {
		RegisterLog(Util::FloatToWStr(value));
	}

	void Debug::Log(const Vec2& value) {
		wstring text(
			L"X : " + Util::FloatToWStr(value.x) + L" " +
			L"Y : " + Util::FloatToWStr(value.y));
		RegisterLog(text);
	}

	void Debug::Log(const Vec3& value) {
		wstring text(
			L"X : " + Util::FloatToWStr(value.x) + L" " +
			L"Y : " + Util::FloatToWStr(value.y) + L" " +
			L"Z : " + Util::FloatToWStr(value.z));
		RegisterLog(text);
	}

	void Debug::Log(const Vec4& value) {
		wstring text(
			L"X : " + Util::FloatToWStr(value.x) + L" " +
			L"Y : " + Util::FloatToWStr(value.y) + L" " +
			L"Z : " + Util::FloatToWStr(value.z) + L" " +
			L"W : " + Util::FloatToWStr(value.w));
		RegisterLog(text);
	}

	void Debug::RegisterLog(const wstring& text) {
		// 無駄なデータを増やさないためにも無効時は追加しない
		if (m_isActive)
			m_logData.push_back(GetNowTimeString() + L" " + text + L"\n");
	}

	void Debug::ClearLog() {
		m_logData.clear();
	}

	wstring Debug::GetNowTimeString() {
		time_t now = time(NULL);
		struct tm pnow;
		localtime_s(&pnow, &now);
		wstring hour = Util::IntToWStr(pnow.tm_hour);
		wstring min = Util::IntToWStr(pnow.tm_min);
		wstring sec = Util::IntToWStr(pnow.tm_sec);
		return L"[" + hour + L":" + min + L":" + sec + L"]";
	}

	wstring Debug::GetSystemInfo() {
		wstring systemInfo = L"";

		// オブジェクト数
		auto ObjCount = GetStage()->GetGameObjectVec().size();
		systemInfo += L"ObjectCount: " + Util::UintToWStr((UINT)ObjCount) + L"\n";
		// FPS
		auto fps = App::GetApp()->GetStepTimer().GetFramesPerSecond();
		systemInfo += L"FPS: " + Util::UintToWStr(fps);
		systemInfo += L"\nElapsedTime: ";
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		systemInfo += Util::FloatToWStr(ElapsedTime) + L"\n";

		// フレームカウント
		auto frameCount = App::GetApp()->GetFrameCount();
		systemInfo += L"FrameCount: " + Util::UintToWStr(frameCount) + L"\n";

		// ビューサイズ
		auto viewport = GetStage()->GetView()->GetTargetViewport();
		systemInfo += L"<View>\n";
		systemInfo += L"Width=" + Util::FloatToWStr(viewport.Width, 6, Util::FloatModify::Fixed) + L"\n";
		systemInfo += L"Height=" + Util::FloatToWStr(viewport.Height, 6, Util::FloatModify::Fixed) + L"\n";

		return systemInfo;
	}

	wstring Debug::BorderInsertion() {
		wstring border = L"";
		for (int i = 0; i < 45; i++) {
			border += L"-";
		}
		return border + L"\n";
	}

	void Debug::SetFontSize(float size) {
		auto oldFontSize = m_ssComp->GetFontSize();
		m_ssComp->SetFont(L"ＭＳゴシック", size);
		auto sizeRate = size / oldFontSize;
		auto rect = Rect2D<float>(
			m_defaultRect.left,
			m_defaultRect.top,
			m_defaultRect.right * sizeRate,
			m_defaultRect.bottom * sizeRate
			);
		m_ssComp->SetTextRect(rect);
	}

	void Debug::IsShowBackground(bool flg) {
		auto alpha = flg ? 0.2f : 0.0f;
		// 文字列の表示領域の背景色を変更
		m_ssComp->SetBackColor(Col4(0.0f, 0.0f, 0.0f, alpha));
	}
}
//end basecross