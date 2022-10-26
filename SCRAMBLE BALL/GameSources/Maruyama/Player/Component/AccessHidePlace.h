
/*!
@file AccessHidePlace.h
@brief AccessHidePlaceなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	class HidePlace;

	class AccessHidePlace : public Component
	{
		float m_range;				//近くに来たと判断する距離
		bool m_isUpdateDrawBoxUI;	//ボックスのUI更新をするかどうか

		std::vector<std::weak_ptr<HidePlace>> m_allHidePlaces;	//フィールドの全ての隠すオブジェクト

	public:
		AccessHidePlace(const std::shared_ptr<GameObject>& objPtr);

		void OnLateStart() override;
		void OnUpdate() override;

	private:
		void UpdateDrawBoxUI();

	public:
		void Access();

		//--------------------------------------------------------------------------------------
		/// アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// 近くに来たと判断する距離の設定
		/// </summary>
		/// <param name="range">近くに来たと判断する距離</param>
		void SetRange(const float range) noexcept { m_range = range; }

		/// <summary>
		/// 近くに来たと判断する距離を取得
		/// </summary>
		/// <returns>近くに来たと判断する距離</returns>
		float GetRange() const noexcept { return m_range; }

		/// <summary>
		/// UIの表示を更新するかどうかを設定
		/// </summary>
		/// <param name="isUpdate">UIの表示を更新するかどうか</param>
		void SetIsUpdateDrawBoxUI(const bool isUpdate) noexcept { m_isUpdateDrawBoxUI = isUpdate; }

		/// <summary>
		/// UIの更新をするかどうかを取得
		/// </summary>
		/// <returns>UIを更新するならtrue</returns>
		bool IsUpdateDrawBoxUI() const noexcept { return m_isUpdateDrawBoxUI; }
	};

}