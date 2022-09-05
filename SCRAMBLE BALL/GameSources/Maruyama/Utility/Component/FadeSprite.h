/*!
@file FadeSpriteManager.h
@brief FadeSpriteManagerヘッダ　
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

	//--------------------------------------------------------------------------------------
	/// フェードタイプ
	//--------------------------------------------------------------------------------------
	enum class FadeType
	{
		In,   //フェードイン
		Out,  //フェードアウト
	};

	//--------------------------------------------------------------------------------------
	/// フェードスピライト管理
	//--------------------------------------------------------------------------------------
	class FadeSpriteManager : public Component
	{
		ex_weak_ptr<SpriteObject> m_fadeSprite = nullptr; //フェードさせるSpriteObject

		std::function<void()> m_endFunction = nullptr;    //フェード終了時に呼び出すイベント
		FadeType m_type = FadeType::In;                   //フェードタイプ

		std::unique_ptr<GameTimer> m_timer = nullptr;     //タイマー管理クラス。

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		FadeSpriteManager(const std::shared_ptr<GameObject>& objPtr);

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		/// <param name="spriteObject">スプライトオブジェクト</param>
		FadeSpriteManager(const std::shared_ptr<GameObject>& objPtr, const std::shared_ptr<SpriteObject>& spriteObject);

		void OnCreate() override;
		void OnUpdate() override;

	private:
		/// <summary>
		/// フェード更新処理
		/// </summary>
		void FadeUpdate();

		/// <summary>
		/// アルファ値を更新する。
		/// </summary>
		/// <param name="alpha">更新したいアルファ値</param>
		void UpdateAlpha(const float& alpha);

		/// <summary>
		/// フェード終了時に呼び出す処理
		/// </summary>
		void FadeEnd();

	public:
		/// <summary>
		/// フェードスタート
		/// </summary>
		/// <param name="type">フェードタイプ</param>
		/// <param name="endFunction">フェード終了時に呼び出すイベント</param>
		/// <param name="fadeTime">フェードに掛ける時間</param>
		void FadeStart(const FadeType& type, const std::function<void()>& endFunction = nullptr, const float& fadeTime = 0.5f);
		
		/// <summary>
		/// 終了判断
		/// </summary>
		/// <returns>フェードが終了したらtrue</returns>
		bool IsEnd() const;

	};

}