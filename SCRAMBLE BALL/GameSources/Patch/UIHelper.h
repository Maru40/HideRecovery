﻿#pragma once

namespace basecross
{
	class GameObject;
	class Component;
	class Stage;

	/// <summary>
	/// 表示する際のアンカータイプ
	/// </summary>
	enum class AnchorType
	{
		Center,
		Up,
		Down,
		Left,
		LeftUp,
		LeftDown,
		Right,
		RightUp,
		RightDown
	};

	/// <summary>
	/// UI描画用トランスフォーム
	/// </summary>
	class RectTransform : public Component
	{
		/// <summary>
		/// アンカーに対応するクライアント領域左上の頂点のオフセットをもつマップ
		/// </summary>
		static const std::unordered_map<AnchorType, Vec2> m_anchorOffsetTable;

		/// <summary>
		/// ローカル座標
		/// </summary>
		Vec2 m_position = Vec2(0, 0);
		/// <summary>
		/// ローカル回転
		/// </summary>
		float m_rotation = 0.0f;
		/// <summary>
		/// ローカルスケール
		/// </summary>
		Vec2 m_scale = Vec2(1.0f, 1.0f);

		/// <summary>
		/// 四角の幅
		/// </summary>
		float m_width;
		/// <summary>
		/// 四角の高さ
		/// </summary>
		float m_height;

		/// <summary>
		/// 四角のアンカータイプ
		/// </summary>
		AnchorType m_anchorType = AnchorType::Center;

		/// <summary>
		/// ピボット
		/// </summary>
		Vec2 m_pivot = Vec2(0.0f, 0.0f);

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="owner">ゲームオブジェクト</param>
		RectTransform(std::shared_ptr<GameObject>& owner);

		/// <summary>
		/// UI用座標をウィンドウ座標に変換する
		/// </summary>
		/// <param name="uiPoint">UI用座標</param>
		/// <returns>ウィンドウ座標</returns>
		static Vec2 ToWindowPoint(const Vec2& uiPoint);

		/// <summary>
		/// アンカータイプを設定する
		/// </summary>
		/// <param name="anchorType">アンカータイプ</param>
		void SetAnchor(const AnchorType anchorType);

		/// <summary>
		/// アンカータイプを取得する
		/// </summary>
		/// <returns>アンカータイプ</returns>
		AnchorType GetAnhor() const;

		/// <summary>
		/// ピボットを設定する
		/// </summary>
		/// <param name="pivot">ピボット</param>
		void SetPivot(Vec2 pivot);
		/// <summary>
		/// ピボットを設定する
		/// </summary>
		/// <param name="x">ピボットX座標</param>
		/// <param name="y">ピボットY座標</param>
		void SetPivot(float x, float y);

		/// <summary>
		/// ピボットを取得する
		/// </summary>
		/// <returns>ピボット</returns>
		Vec2 GetPivot() const;

		/// <summary>
		/// 四角の幅を設定する
		/// </summary>
		/// <param name="width">幅</param>
		void SetWidth(const float width);
		/// <summary>
		/// 四角の高さを設定する
		/// </summary>
		/// <param name="height">高さ</param>
		void SetHeight(const float height);
		/// <summary>
		/// 四角のサイズを設定する
		/// </summary>
		/// <param name="width">幅</param>
		/// <param name="height">高さ</param>
		void SetRectSize(const float width, const float height);

		/// <summary>
		/// 四角の幅を取得する
		/// </summary>
		/// <returns>幅</returns>
		float GetWidth() const;
		/// <summary>
		/// 四角の高さを取得する
		/// </summary>
		/// <returns>高さ</returns>
		float GetHeight() const;

		/// <summary>
		/// ローカル座標を設定する
		/// </summary>
		/// <param name="position">ローカル座標</param>
		void SetPosition(const Vec2& position);
		/// <summary>
		/// ローカル座標を設定する
		/// </summary>
		/// <param name="x">ローカルX座標</param>
		/// <param name="y">ローカル座標</param>
		void SetPosition(const float x, const float y);

		/// <summary>
		/// ローカル座標を取得する
		/// </summary>
		/// <returns>ローカル座標</returns>
		Vec2 GetPosition() const;

		Vec2 GetWorldPosition() const;

		/// <summary>
		/// ローカル回転を設定する
		/// </summary>
		/// <param name="radian">ローカルZ回転</param>
		void SetRotation(const float radian);

		/// <summary>
		/// ローカル回転を取得する
		/// </summary>
		/// <returns>ローカルZ回転</returns>
		float GetRotation() const;

		/// <summary>
		/// ローカルZ回転を追加する
		/// </summary>
		/// <param name="radian">追加するローカルZ回転</param>
		void Rotate(const float radian);

		/// <summary>
		/// ローカルスケールを設定する
		/// </summary>
		/// <param name="scale">ローカルスケール</param>
		void SetScale(const Vec2& scale);
		/// <summary>
		/// ローカルスケールを設定する
		/// </summary>
		/// <param name="x">ローカルXスケール</param>
		/// <param name="y">ローカルYスケール</param>
		void SetScale(const float x, const float y);

		/// <summary>
		/// ローカルスケールを取得する
		/// </summary>
		/// <returns>ローカルスケール</returns>
		Vec2 GetScale() const;

		/// <summary>
		/// ワールドスケールを取得する
		/// </summary>
		/// <returns>ワールドスケール</returns>
		Vec2 GetWorldScale() const;

		/// <summary>
		/// ピボットから受け取ったアンカータイプまでの長さを返す
		/// </summary>
		/// <param name="anchorType">アンカータイプ</param>
		/// <param name="isScaleble">スケールの影響を受けるか</param>
		/// <returns>ピボットからアンカータイプの距離</returns>
		Vec2 GetAnchorOffset(const AnchorType anchorType,const bool isScaleble = true) const;

		/// <summary>
		/// ワールド行列を取得する
		/// </summary>
		/// <returns></returns>
		D2D1::Matrix3x2F GetWorldMatrix() const;

		/// <summary>
		/// 親のRectTransformを取得する
		/// </summary>
		/// <returns>親のRectTransform</returns>
		std::shared_ptr<RectTransform> GetParentRectTransform() const;

		/// <summary>
		/// 親のアンカーオフセットを取得する
		/// </summary>
		/// <param name="anchorType">アンカータイプ</param>
		/// <returns>アンカーオフセット</returns>
		Vec2 GetParentAnchorOffset(const AnchorType anchorType) const;

		void OnUpdate() override;

		virtual void OnDraw() override {}
	};

	enum class ImageFillType
	{
		None,
		Horizontal,
		Vertical
	};

	/// <summary>
	/// 画像を表示するコンポーネント
	/// </summary>
	class Image : public PCTSpriteDraw
	{
		/// <summary>
		/// Verticesに必要な頂点数
		/// </summary>
		static constexpr int VERTICES_POINT_NUM = 4;

		/// <summary>
		/// 頂点データ配列
		/// </summary>
		static const std::vector<uint16_t> m_indices;

		/// <summary>
		/// 四角トランスフォーム
		/// </summary>
		ex_weak_ptr<RectTransform> rectTransform;

		/// <summary>
		/// 画像の切り取り領域の頂点データ
		/// </summary>
		std::array<Vec2, VERTICES_POINT_NUM> m_UVPoints;

		/// <summary>
		/// 画像の塗りつぶしタイプ
		/// </summary>
		ImageFillType m_fillType = ImageFillType::None;

		/// <summary>
		/// 塗りつぶされる量 (1.0f ～ 2.0f)
		/// </summary>
		float m_fillAmount = 1.0f;

		/// <summary>
		/// 現在の状態のverticesデータを作成する
		/// </summary>
		/// <returns>verticesデータ</returns>
		std::vector<VertexPositionColorTexture> CreateVertices() const;

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="owner">ゲームオブジェクト</param>
		Image(std::shared_ptr<GameObject>& owner);


		/// <summary>
		/// 画像の切り取り領域を設定する
		/// </summary>
		/// <param name="leftUpPoint">切り取り領域の左上の頂点の座標</param>
		/// <param name="width">切り取り領域の幅</param>
		/// <param name="height">切り取り領域の高さ</param>
		/// <param name="imageSize">元画像の幅と高さ</param>
		void SetUVRect(const Vec2& leftUpPoint,const float width,const float height,Vec2& imageSize);

		/// <summary>
		/// Alpha値を設定する
		/// </summary>
		/// <param name="alpha">Alpha値</param>
		void SetAlpha(const float alpha);

		/// <summary>
		/// Alpha値を取得する
		/// </summary>
		/// <returns>Alpha値</returns>
		float GetAlpha() const;

		/// <summary>
		/// 塗りつぶしタイプを設定する
		/// </summary>
		/// <param name="fillType">塗りつぶしタイプ</param>
		void SetFillType(const ImageFillType fillType);

		/// <summary>
		/// 塗りつぶしタイプを取得する
		/// </summary>
		/// <returns></returns>
		ImageFillType GetFillType() const;

		/// <summary>
		/// 塗りつぶし量を設定する
		/// </summary>
		/// <param name="fillAmount">塗りつぶし量</param>
		void SetFillAmount(const float fillAmount);

		/// <summary>
		/// 塗りつぶし量を取得する
		/// </summary>
		/// <returns>塗りつぶし量</returns>
		float GetFillAmount() const;

		virtual void OnCreate() override;

		void OnUpdate() override;
	};

	/// <summary>
	/// テキストを表示するコンポーネント
	/// </summary>
	class TextBox : public Component
	{
	public:

		/// <summary>
		/// Boxの縦方向に対して、Textがどの位置にあるか
		/// </summary>
		enum class TextVerticalAlignment
		{
			/// <summary>
			/// 上詰め
			/// </summary>
			Up,
			/// <summary>
			/// 中央寄せ
			/// </summary>
			Center,
			/// <summary>
			/// 下詰め
			/// </summary>
			Down
		};

		enum class TextSideAlignment
		{
			Left,
			Center,
			Right
		};

		enum class TextStyle
		{
			Normal,
			Italic,
			Oblique
		};

	private:
		/// <summary>
		/// テキストスタイルに対してDWRITE_FONT_STYLEに変換するテーブル
		/// </summary>
		static const std::unordered_map<TextStyle, DWRITE_FONT_STYLE> m_textStypeToDWriteStyleTable;

		/// <summary>
		/// 四角トランスフォーム
		/// </summary>
		ex_weak_ptr<RectTransform> rectTransform;
		/// <summary>
		/// テキストの横に対する位置
		/// </summary>
		TextSideAlignment m_textSideAlignment = TextSideAlignment::Left;
		/// <summary>
		/// テキストの縦に対する位置
		/// </summary>
		TextVerticalAlignment m_textVerticalAlignment = TextVerticalAlignment::Up;

		/// <summary>
		/// ボックスの色
		/// </summary>
		Col4 m_boxColor = Col4();
		/// <summary>
		/// ボックス描画用色ブラシ
		/// </summary>
		ComPtr<ID2D1SolidColorBrush> m_boxBrush;

		/// <summary>
		/// フォントの色
		/// </summary>
		Col4 m_fontColor = Col4(0,0,0,1);
		/// <summary>
		/// フォントの描画用色ブラシS
		/// </summary>
		ComPtr<ID2D1SolidColorBrush> m_fontBrush;

		/// <summary>
		/// ステートブロック
		/// </summary>
		ComPtr<ID2D1DrawingStateBlock> m_stateBlock;

		/// <summary>
		/// フォント名
		/// </summary>
		std::wstring m_fontName = L"ＭＳ ゴシック";

		/// <summary>
		/// フォントサイズ
		/// </summary>
		float m_fontSize = 16.0f;

		/// <summary>
		/// テキスト
		/// </summary>
		std::wstring m_text;

		/// <summary>
		/// テキストフォーマット
		/// </summary>
		ComPtr<IDWriteTextFormat> m_textFormat;
		/// <summary>
		/// テキストレイアウト
		/// </summary>
		ComPtr<IDWriteTextLayout> m_textLayout;

		/// <summary>
		/// テキストメトリクスs
		/// </summary>
		DWRITE_TEXT_METRICS m_textMetrics;

		/// <summary>
		/// テキストスタイル
		/// </summary>
		TextStyle m_textStyle = TextStyle::Normal;

		/// <summary>
		/// テキストのフォーマットを更新する
		/// </summary>
		void TextFormatUpdate();
		/// <summary>
		/// テキストのレイアウトを更新する
		/// </summary>
		void TextLayoutUpdate();
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="owner">ゲームオブジェクト</param>
		TextBox(std::shared_ptr<GameObject>& owner);

		/// <summary>
		/// ボックスの色を設定する
		/// </summary>
		/// <param name="color">ボックスの色</param>
		void SetBoxColor(const Col4& color);
		/// <summary>
		/// ボックスの色を設定する
		/// </summary>
		/// <param name="red">赤</param>
		/// <param name="green">緑</param>
		/// <param name="blue">青</param>
		/// <param name="alpha">透明度</param>
		void SetBoxColor(const float red, const float green, const float blue, const float alpha);

		/// <summary>
		/// ボックスの色を取得する
		/// </summary>
		/// <returns>ボックスの色</returns>
		Col4 GetBoxColor() const;

		/// <summary>
		/// フォントの色を設定する
		/// </summary>
		/// <param name="color">フォントの色</param>
		void SetFontColor(const Col4& color);
		/// <summary>
		/// フォントの色を設定する
		/// </summary>
		/// <param name="red">赤</param>
		/// <param name="green">緑</param>
		/// <param name="blue">青</param>
		/// <param name="alpha">透明度</param>
		void SetFontColor(const float red, const float green, const float blue, const float alpha);

		/// <summary>
		/// フォントの色を取得する
		/// </summary>
		/// <returns>フォントの色</returns>
		Col4 GetFontColor() const;

		/// <summary>
		/// フォントの字体を設定する
		/// </summary>
		/// <param name="fontName">フォント名</param>
		void SetFontName(const std::wstring& fontName);

		/// <summary>
		/// フォントのサイズを設定する
		/// </summary>
		/// <param name="fontSize">フォントサイズ</param>
		void SetFontSize(const float fontSize);

		/// <summary>
		/// TextBox全体のAlpha値を設定する
		/// </summary>
		/// <param name="alpha">Alpha値</param>
		void SetAlpha(const float alpha);

		/// <summary>
		/// テキストを設定する
		/// </summary>
		/// <param name="text">設定するテキスト</param>
		void SetText(const std::wstring& text);

		/// <summary>
		/// テキストを追加する
		/// </summary>
		/// <param name="text">追加するテキスト</param>
		void AddText(const std::wstring& text);

		/// <summary>
		/// テキストを取得する
		/// </summary>
		/// <returns>テキスト</returns>
		std::wstring GetText() const;

		/// <summary>
		/// テキストの横に対する位置を設定する
		/// </summary>
		/// <param name="textSideAlignment">横に対する位置</param>
		void SetTextSideAlignment(const TextSideAlignment textSideAlignment);

		/// <summary>
		/// テキストの横に対する位置を取得する
		/// </summary>
		/// <returns>横に対する位置</returns>
		TextSideAlignment GetTextSideAlignment() const;

		/// <summary>
		/// テキストの縦に対する位置を設定する
		/// </summary>
		/// <param name="textVerticalAlignment">縦に対する位置</param>
		void SetTextVerticalAlignment(const TextVerticalAlignment textVerticalAlignment);

		/// <summary>
		/// テキストの縦に対する位置を取得する
		/// </summary>
		/// <returns>縦に対する位置</returns>
		TextVerticalAlignment GetTextVerticalAlignment() const;

		/// <summary>
		/// テキストスタイルを設定する
		/// </summary>
		/// <param name="textStyle">テキストスタイル</param>
		void SetTextStyle(const TextStyle textStyle);

		/// <summary>
		/// テキストスタイルを取得する
		/// </summary>
		/// <returns>テキストスタイル</returns>
		TextStyle GetTextStyle() const;

		void OnUpdate() override {}

		void OnDraw() override;
	};

	struct ChoicesObjectAndEvent
	{
		std::wstring text;
		ex_weak_ptr<GameObject> choicesObject;
		std::function<void()> eventFunction;

		ChoicesObjectAndEvent();

		ChoicesObjectAndEvent(const std::wstring& text, std::shared_ptr<GameObject>& choicesObject, const std::function<void()>& eventFunction);
	};

	class ChoicesList : public Component
	{

		ex_weak_ptr<GameObject> m_upTextBox;
		ex_weak_ptr<GameObject> m_centerTextBox;
		ex_weak_ptr<GameObject> m_downTextBox;

		std::vector<ex_weak_ptr<ChoicesObjectAndEvent const>> m_choiceObjectAndEvents;

		bool m_isEmpty = true;

		int m_index = 0;

		void UpdateChoices();

		std::shared_ptr<GameObject> CreateChildTextBoxObject();
	public:
		itbs::Utility::Delegate<void()> selectChangeEvent;
		itbs::Utility::Delegate<void()> pushEvent;

		ChoicesList(std::shared_ptr<GameObject>& owner);

		void AddChoice(const std::shared_ptr<ChoicesObjectAndEvent const>& choicesObjectAndEvent);

		void RemoveChoice(const std::shared_ptr<GameObject>& gameObject);

		void AddIndex(const int addIndex);

		void SetIndex(const int index);

		void Invoke();

		void OnStart() override;

		void OnUpdate2() override;
	};

	/// <summary>
	/// 移動方向列挙
	/// </summary>
	enum class UIMoveDirection
	{
		None, // 無移動
		Left, // 左方向
		Right, // 右方向
		Up, // 上方向
		Down // 下方向
	};

	/// <summary>
	/// 決定用インターフェース
	/// </summary>
	class I_Submitable
	{
	public:
		virtual void OnSubmit() = 0;
	};

	/// <summary>
	/// キャンセル用インターフェース
	/// </summary>
	class I_Cancelable
	{
	public:
		virtual void OnCancel() = 0;
	};

	/// <summary>
	/// 移動用インターフェース
	/// </summary>
	class I_Selectable
	{
	public:
		virtual void OnSelect() = 0;
		virtual void OnDeselect() = 0;
	};

	class I_Movable
	{
	public:
		virtual void OnMove(UIMoveDirection direction) = 0;
	};

	/// <summary>
	/// 選択することができるコンポーネント
	/// </summary>
	class SelectableComponent : public Component, public I_Selectable, public I_Movable
	{
		std::weak_ptr<SelectableComponent> m_leftSelectable; // 左で選択されるSelectableComponent
		std::weak_ptr<SelectableComponent> m_rightSelectable; // 右で選択されるSelectableComponent
		std::weak_ptr<SelectableComponent> m_upSelectable; // 上で選択されるSelectableComponent
		std::weak_ptr<SelectableComponent> m_downSelectable; // 下で選択されるSelectableComponent

		bool m_isSelectedLock = false;

	public:

		SelectableComponent(const std::shared_ptr<GameObject>& owner);

		void SetIsSelectedLock(const bool isSelectedLock);

		bool GetIsSelectedLock() const;

		virtual void OnSelect() override {}

		virtual void OnDeselect() override {}

		virtual void OnMove(UIMoveDirection direction) override;

		void SetLeftSelectable(const std::shared_ptr<SelectableComponent>& selectable) noexcept { m_leftSelectable = selectable; }
		void SetRightSelectable(const std::shared_ptr<SelectableComponent>& selectable) noexcept { m_rightSelectable = selectable; }
		void SetUpSelectable(const std::shared_ptr<SelectableComponent>& selectable) noexcept { m_upSelectable = selectable; }
		void SetDownSelectable(const std::shared_ptr<SelectableComponent>& selectable) noexcept { m_downSelectable = selectable; }

		_NODISCARD std::shared_ptr<SelectableComponent> GetLeftSelectable() const noexcept { return m_leftSelectable.lock(); }
		_NODISCARD std::shared_ptr<SelectableComponent> GetRightSelectable() const noexcept { return m_rightSelectable.lock(); }
		_NODISCARD std::shared_ptr<SelectableComponent> GetUpSelectable() const noexcept { return m_upSelectable.lock(); }
		_NODISCARD std::shared_ptr<SelectableComponent> GetDownSelectable() const noexcept { return m_downSelectable.lock(); }

		bool IsSelected() const;
	};

	class itbs::Input::I_BasicInputer;

	class EventSystem : public Component
	{
	public:
		static const std::wstring EVENTSYSTEM_OBJECT_KEY;

	private:
		static std::weak_ptr<EventSystem> m_eventSystem;

		std::shared_ptr<itbs::Input::I_BasicInputer> m_basicInputer;

		std::weak_ptr<GameObject> m_nowSelectableObject;
		std::stack<std::weak_ptr<GameObject>> m_stackSelectableObject;

		UIMoveDirection GetMoveInput() const;

	public:
		EventSystem(std::shared_ptr<GameObject>& owner);

		void SetNowSelectableObject(const std::shared_ptr<GameObject>& selectableObject);

		void PushSelectableObject(const std::shared_ptr<GameObject>& selectableObject, bool oldObjectEnable = false);

		void PopSelectableObject(bool oldObjectActive = false);

		_NODISCARD std::shared_ptr<GameObject> GetNowSelectableObject() const noexcept { return m_nowSelectableObject.lock(); }

		static std::shared_ptr<EventSystem> GetInstance(const std::shared_ptr<Stage>& stage);

		void SetBasicInputer(const std::shared_ptr<itbs::Input::I_BasicInputer>& basicInputer);

		void OnCreate() override;

		void OnUpdate() override;

		virtual void OnDraw() override {}

		void Clear();
	};

	/// <summary>
	/// ボタンUI
	/// </summary>
	class Button : public SelectableComponent, public I_Submitable
	{
		std::weak_ptr<Image> m_image;

		std::weak_ptr<TextureResource> m_normalButtonTexture;
		std::weak_ptr<TextureResource> m_selectedButtonTexture;

		std::vector<std::function<void()>> m_pushEvents;

	public:

		Button(std::shared_ptr<GameObject>& owner);

		void SetNormalButtonTexture(const std::shared_ptr<TextureResource>& normalButtonTexture);
		void SetNormalButtonTexture(const std::wstring& normalButtonTextureKey) { SetNormalButtonTexture(App::GetApp()->GetResource<TextureResource>(normalButtonTextureKey)); }

		void SetSelectedButtonTexture(const std::shared_ptr<TextureResource>& selectedButtonTexture);
		void SetSelectedButtonTexture(const std::wstring& selectedButtonTextureKey) { SetSelectedButtonTexture(App::GetApp()->GetResource<TextureResource>(selectedButtonTextureKey)); }

		void SetAllButtonTexture(const std::shared_ptr<TextureResource>& allButtonTexture);
		void SetAllButtonTexture(const std::wstring& allButtonTextureKey) { SetAllButtonTexture(App::GetApp()->GetResource<TextureResource>(allButtonTextureKey)); }

		void SetImage(const std::shared_ptr<Image>& image);
		std::shared_ptr<Image> GetImage() const { return m_image.lock(); }

		void OnSelect() override;

		void OnDeselect() override;

		void OnSubmit() override;

		void OnCreate() override;

		void AddPushEvent(const std::function<void()>& pushEvent) { m_pushEvents.push_back(pushEvent); }
	};

	/// <summary>
	/// UI用GameObject
	/// </summary>
	class UIObject : public GameObject
	{
	protected:
		ex_weak_ptr<RectTransform> rectTransform;

	public:
		UIObject(const std::shared_ptr<Stage>& stage);

		void OnPreCreate() override;
	};
}