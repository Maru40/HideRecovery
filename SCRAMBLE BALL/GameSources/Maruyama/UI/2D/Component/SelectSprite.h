/*!
@file SelectSprite.h
@brief SelectSpriteなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 選択できるスプライト
	//--------------------------------------------------------------------------------------
	class SelectSprite : public Component
	{
	private:
		//選択中アクション
		maru::Action<void()> m_selectAction;
		//選択解除
		maru::Action<void()> m_unSelectAction;
		//決定したときのアクション
		maru::Action<void()> m_decisionAction;

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		SelectSprite(const std::shared_ptr<GameObject>& objPtr);
		
	public:
		//--------------------------------------------------------------------------------------
		/// アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// 選択時のイベントを登録
		/// </summary>
		/// <param name="isSelect">選択した時か、選択解除した時か</param>
		/// <param name="function">呼び出したいイベント</param>
		void AddSelectAction(const bool isSelect, const std::function<void()>& function);

		/// <summary>
		/// 登録したイベントを呼び出す。
		/// </summary>
		/// <param name="isSelect">選択した時か、選択解除した時か</param>
		void SelectInvoke(const bool isSelect);

		/// <summary>
		/// 決定時に呼び出したいイベント
		/// </summary>
		/// <param name="function">呼び出すイベント</param>
		void AddDecisionAction(const std::function<void()>& function);

		/// <summary>
		/// 登録した決定時に呼ぶイベントを呼び出す。
		/// </summary>
		void DecisionInvoke();

	};

}