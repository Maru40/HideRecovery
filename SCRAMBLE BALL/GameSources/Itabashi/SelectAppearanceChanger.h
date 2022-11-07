#pragma once

namespace basecross
{
	/// <summary>
	/// 選択状態で見た目を変更するコンポーネント
	/// </summary>
	class SelectAppearanceChanger : public Component, public I_Selectable
	{
		std::weak_ptr<Transform> m_transform; // トランスフォーム
		std::weak_ptr<SmBaseDraw> m_baseDraw; // 基本描画コンポーネント
		std::weak_ptr<SpriteBaseDraw> m_spriteDraw; // スプライト描画用コンポーネント

		Vec3 m_selectedSize = Vec3(1.0f, 1.0f, 1.0f); // 選択時のサイズ
		Vec3 m_deselectedSize = Vec3(1.0f, 1.0f, 1.0f); // 非選択時のサイズ

		Col4 m_selectedColor = Col4(1.0f, 1.0f, 1.0f, 1.0f); // 選択時の色
		Col4 m_deselectedColor = Col4(1.0f, 1.0f, 1.0f, 1.0f); // 非選択時の色

		bool m_isSelected = false; // 選択時true

		/// <summary>
		/// 見た目の更新
		/// </summary>
		/// <param name="size">サイズ</param>
		/// <param name="color">色</param>
		void UpdateAppearanceChanger(const Vec3& size, const Col4& color);

	public:
		SelectAppearanceChanger(const std::shared_ptr<GameObject>& owner);

		void OnLateStart() override;

		void OnDraw() override {}

		void OnSelect() override;

		void OnDeselect() override;

		/// <summary>
		/// 選択時のサイズを設定
		/// </summary>
		/// <param name="size">選択時サイズ</param>
		void SetSelectedSize(const Vec3& size);
		/// <summary>
		/// 選択時サイズを取得
		/// </summary>
		/// <returns>選択時サイズ</returns>
		const Vec3& GetSelectedSize() const { return m_selectedSize; }

		/// <summary>
		/// 非選択時サイズを設定
		/// </summary>
		/// <param name="size">非選択時サイズ</param>
		void SetDeselectedSize(const Vec3& size);
		/// <summary>
		/// 非選択時サイズを取得
		/// </summary>
		/// <returns>非選択時サイズ</returns>
		const Vec3& GetDeselectedSize() const { return m_deselectedSize; }

		/// <summary>
		/// 選択時色を設定
		/// </summary>
		/// <param name="color">選択時色</param>
		void SetSelectedColor(const Col4& color);
		/// <summary>
		/// 選択時色を取得
		/// </summary>
		/// <returns>選択時色</returns>
		const Col4& GetSelectedColor() const { return m_deselectedColor; }

		/// <summary>
		/// 非選択時色を設定
		/// </summary>
		/// <param name="color">非選択時色</param>
		void SetDeselectedColor(const Col4& color);
		/// <summary>
		/// 非選択時色を取得
		/// </summary>
		/// <returns>非選択時色</returns>
		const Col4& GetDeselectedColor() const { return m_deselectedColor; }
	};
}