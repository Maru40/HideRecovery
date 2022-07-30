/*!
@file ScreenFadeManager.h
@brief ScreenFadeManagerヘッダ　
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	class SpriteObject;
	class GameTimer;

	class FadeSpriteManager;
	enum class FadeType;

	//--------------------------------------------------------------------------------------
	/// スクリーンフェード管理
	//--------------------------------------------------------------------------------------
	class ScreenFadeManager : public maru::SingletonComponent<ScreenFadeManager>
	{
		ex_weak_ptr<FadeSpriteManager> m_fadeSpriteManager; //フェード管理

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		ScreenFadeManager(const std::shared_ptr<GameObject>& objPtr);

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		/// <param name="fadeSpriteManager">フェードスプライト管理クラス</param>
		ScreenFadeManager(const std::shared_ptr<GameObject>& objPtr, const std::shared_ptr<FadeSpriteManager>& fadeSpriteManager);

		void OnCreate() override;

		/// <summary>
		/// フェードスタート
		/// </summary>
		/// <param name="type">フェードタイプ</param>
		/// <param name="endFunction">フェード終了時イベント</param>
		/// <param name="fadeTime">フェードにかける時間</param>
		void FadeStart(const FadeType& type, const std::function<void()>& endFunction = nullptr, const float& fadeTime = 0.5f);

		/// <summary>
		/// 終了判断
		/// </summary>
		/// <returns>フェードが終了しているならtrue</returns>
		bool IsEnd() const;

	};

}