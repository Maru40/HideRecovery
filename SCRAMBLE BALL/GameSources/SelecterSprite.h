/*!
@file SelecterSprite.h
@brief SelecterSpriteヘッダ　
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	class SelectSprite;

	//--------------------------------------------------------------------------------------
	/// スプライトの選択管理
	//--------------------------------------------------------------------------------------
	class SelecterSprite : public Component
	{
		vector<ex_weak_ptr<SelectSprite>> m_targets;
		int m_selectIndex;          //選択中のインデックス
		int m_maxIndex;             //最大インデックス

		float m_speed;              //移動スピード
		float m_baseSize;           //基本サイズ
		float m_maxSize;            //最大サイズ
		float m_sizeElapsed = 0.0f; //サイズの現在値

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		SelecterSprite(const std::shared_ptr<GameObject>& objPtr):
			Component(objPtr),
			m_selectIndex(0),
			m_maxIndex(0),
			m_speed(2.0f),
			m_baseSize(1.0f),
			m_maxSize(0.1f)
		{}

		void OnCreate() override;
		void OnUpdate() override;

	private:
		/// <summary>
		/// スプライトエフェクト
		/// </summary>
		void SpriteEffect();

		/// <summary>
		/// 入力に合わせてどれを選択中かを判断する
		/// </summary>
		void InputSelect();

		/// <summary>
		/// 決定入力をする。
		/// </summary>
		void InputDecision();

		/// <summary>
		/// 選択先が変更された事を、現在選択中のUIに伝える。
		/// </summary>
		void EndSelect();

		/// <summary>
		/// 選択先を変更する。
		/// </summary>
		void ChangeSelect();

		/// <summary>
		/// インデックスの範囲内かどうかを判断する。
		/// </summary>
		void CheckSelectIndex();

	public:
		//--------------------------------------------------------------------------------------
		/// アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// ターゲット配列の設定
		/// </summary>
		/// <param name="targets">ターゲット配列</param>
		void SetTargets(const vector<ex_weak_ptr<SelectSprite>>& targets);

		/// <summary>
		/// ターゲットの追加
		/// </summary>
		/// <param name="target">ターゲットの追加</param>
		void AddTarget(const std::shared_ptr<SelectSprite>& target) {
			m_targets.push_back(target);
		}

		/// <summary>
		/// 選択中のインデックスの取得
		/// </summary>
		/// <returns></returns>
		int GetSelectIndex() const {
			return m_selectIndex;
		}

		/// <summary>
		/// 選択中のスプライトの取得
		/// </summary>
		/// <returns></returns>
		std::shared_ptr<SelectSprite> GetSelectObject() const {
			return m_targets[m_selectIndex].GetShard();
		}
	};

}

//endbasecross