#include"UIHelper.h"
#include"WindowSetting.h"
#include"FontHelper.h"
#include"MathHelper.h"

using itbs::Math::MyMath;

namespace basecross
{
	// RectTransform ----------------------------
	
	const std::unordered_map<AnchorType, Vec2> RectTransform::m_anchorOffsetTable =
	{
		std::make_pair(AnchorType::Center, Vec2(0.0f, 0.0f)),
		std::make_pair(AnchorType::Up, Vec2(0.0f, 0.5f)),
		std::make_pair(AnchorType::Down, Vec2(0.0f, -0.5f)),
		std::make_pair(AnchorType::Left, Vec2(-0.5f, 0.0f)),
		std::make_pair(AnchorType::LeftUp, Vec2(-0.5f, 0.5f)),
		std::make_pair(AnchorType::LeftDown, Vec2(-0.5f, -0.5f)),
		std::make_pair(AnchorType::Right, Vec2(0.5f, 0.0f)),
		std::make_pair(AnchorType::RightUp, Vec2(0.5f, 0.5f)),
		std::make_pair(AnchorType::RightDown, Vec2(0.5f, -0.5f))
	};

	RectTransform::RectTransform(std::shared_ptr<GameObject>& owner) :
		Component(owner)
	{
		
	}

	Vec2 RectTransform::ToWindowPoint(const Vec2& uiPoint)
	{
		Vec2 windowPoint;

		windowPoint.x = uiPoint.x + WindowSetting::WINDOW_WIDTH * 0.5f;
		windowPoint.y = -uiPoint.y + WindowSetting::WINDOW_HEIGHT * 0.5f;

		return windowPoint;
	}

	Vec2 RectTransform::GetParentAnchorOffset(const AnchorType anchorType) const
	{
		auto parentTransform = GetParentRectTransform();

		if (parentTransform)
		{
			return parentTransform->GetAnchorOffset(anchorType, false);
		}

		Vec2 anchorOffset = m_anchorOffsetTable.at(anchorType);

		return Vec2(WindowSetting::WINDOW_WIDTH * anchorOffset.x, WindowSetting::WINDOW_HEIGHT * anchorOffset.y);
	}


	void RectTransform::SetAnchor(const AnchorType anchorType)
	{
		m_anchorType = anchorType;
	}

	AnchorType RectTransform::GetAnhor() const
	{
		return m_anchorType;
	}

	void RectTransform::SetPivot(Vec2 pivot)
	{
		pivot.x = std::fmaxf(-0.5f, pivot.x);
		pivot.x = std::fminf( 0.5f, pivot.x);
		pivot.y = std::fmaxf(-0.5f, pivot.y);
		pivot.y = std::fminf( 0.5f, pivot.y);

		m_pivot = pivot;
	}

	void RectTransform::SetPivot(float x, float y)
	{
		SetPivot(Vec2(x, y));
	}

	Vec2 RectTransform::GetPivot() const
	{
		return m_pivot;
	}

	void RectTransform::SetWidth(const float width)
	{
		m_width = width;
	}

	void RectTransform::SetHeight(const float height)
	{
		m_height = height;
	}

	void RectTransform::SetRectSize(const float width, const float height)
	{
		m_width = width;
		m_height = height;
	}

	float RectTransform::GetWidth() const
	{
		return m_width;
	}

	float RectTransform::GetHeight() const
	{
		return m_height;
	}

	void RectTransform::SetPosition(const Vec2& position)
	{
		m_position = position;
	}

	void RectTransform::SetPosition(const float x, const float y)
	{
		SetPosition(Vec2(x, y));
	}

	Vec2 RectTransform::GetPosition() const
	{
		return m_position;
	}

	Vec2 RectTransform::GetWorldPosition() const
	{
		auto worldMatrix = GetWorldMatrix();
		return Vec2(worldMatrix._31, -worldMatrix._32);
	}

	void RectTransform::SetRotation(const float radian)
	{
		m_rotation = radian;
	}

	float RectTransform::GetRotation() const
	{
		return m_rotation;
	}

	void RectTransform::Rotate(const float radian)
	{
		m_rotation += radian;
	}

	void RectTransform::SetScale(const Vec2& scale)
	{
		m_scale = scale;
	}

	void RectTransform::SetScale(const float x, const float y)
	{
		SetScale(Vec2(x, y));
	}

	Vec2 RectTransform::GetScale() const
	{
		return m_scale;
	}

	Vec2 RectTransform::GetWorldScale() const
	{
		auto scale = GetScale();

		auto parentRectTransform = GetParentRectTransform();

		if (parentRectTransform)
		{
			auto parentScale = parentRectTransform->GetWorldScale();

			scale.x *= parentScale.x;
			scale.y *= parentScale.y;

			return scale;
		}

		return scale;
	}

	Vec2 RectTransform::GetAnchorOffset(const AnchorType anchorType,const bool isScaleble) const
	{
		Vec2 anchorOffset = m_anchorOffsetTable.at(anchorType);

		Vec2 offset = anchorOffset - m_pivot;

		Vec2 scale = isScaleble ? GetWorldScale() : Vec2(1, 1);

		return Vec2(m_width * scale.x * offset.x, m_height * scale.y * offset.y);
	}

	D2D1::Matrix3x2F RectTransform::GetWorldMatrix() const
	{
		auto point2F = D2D1::Point2F();
		
		auto offsetPosition = GetParentAnchorOffset(m_anchorType);
		auto windowPosition = Vec2(m_position.x, m_position.y) + GetParentAnchorOffset(m_anchorType);
		auto positionMatrix = D2D1::Matrix3x2F::Translation(D2D1::SizeF(windowPosition.x, -windowPosition.y));

		auto rotationMatrix = D2D1::Matrix3x2F::Rotation(XMConvertToDegrees(-m_rotation), point2F);

		auto scaleMatrix = D2D1::Matrix3x2F::Scale(D2D1::SizeF(m_scale.x, m_scale.y), point2F);

		auto worldMatrix = scaleMatrix * rotationMatrix * positionMatrix;

		auto parentRectTransform = GetParentRectTransform();

		if (parentRectTransform)
		{
			return worldMatrix * parentRectTransform->GetWorldMatrix();
		}

		return worldMatrix;
	}

	std::shared_ptr<RectTransform> RectTransform::GetParentRectTransform() const
	{
		auto parent = GetGameObject()->GetParent();

		if (!parent)
		{
			return nullptr;
		}

		return parent->GetComponent<RectTransform>(false);
	}

	void RectTransform::OnUpdate()
	{

		auto position = GetPosition();

		auto parentRectTransform = GetParentRectTransform();

		if (parentRectTransform)
		{
			auto parentScale = parentRectTransform->GetScale();
			position.x *= parentScale.x;
			position.y *= parentScale.y;

			auto offset = parentRectTransform->GetAnchorOffset(GetAnhor());

			position += offset;
		}

		transform->SetPosition(position.x, position.y, 0);

		float rotation = GetRotation();

		transform->SetRotation(Vec3(0, 0, rotation));

		auto scale = GetWorldScale();

		transform->SetScale(scale.x, scale.y, 1);

	}
	// Image ------------------------------------

	const std::vector<uint16_t> Image::m_indices = { 0,1,2, 2,1,3 };

	Image::Image(std::shared_ptr<GameObject>& owner) :
		PCTSpriteDraw(owner),
		m_UVPoints({
			Vec2(0.0f,0.0f),
			Vec2(1.0f,0.0f),
			Vec2(0.0f,1.0f),
			Vec2(1.0f,1.0f)
			})
	{
		SetBlendState(BlendState::AlphaBlend);
		rectTransform = owner->GetComponent<RectTransform>();
	}

	std::vector<VertexPositionColorTexture> Image::CreateVertices() const
	{
		Vec2 pivot = rectTransform->GetPivot();
		Col4 color = GetDiffuse();

		float width = rectTransform->GetWidth();
		float height = rectTransform->GetHeight();

		Vec3 leftUpPoint, rightUpPoint, leftDownPoint, rightDownPoint;

		switch (m_fillType)
		{
		case basecross::ImageFillType::None:
			leftUpPoint = Vec3(width * (-0.5f - pivot.x), height * (0.5f - pivot.y), 0);
			rightUpPoint = Vec3(width * (0.5f - pivot.x), height * (0.5f - pivot.y), 0);
			leftDownPoint = Vec3(width * (-0.5f - pivot.x), height * (-0.5f - pivot.y), 0);
			rightDownPoint = Vec3(width * (0.5f - pivot.x), height * (-0.5f - pivot.y), 0);
			break;
		case basecross::ImageFillType::Horizontal:
			leftUpPoint = Vec3(width * (-0.5f - pivot.x), height * (0.5f - pivot.y), 0);
			rightUpPoint = Vec3(width * (0.5f - pivot.x) - (width * (1.0f - m_fillAmount)), height * (0.5f - pivot.y), 0);
			leftDownPoint = Vec3(width * (-0.5f - pivot.x), height * (-0.5f - pivot.y), 0);
			rightDownPoint = Vec3(width * (0.5f - pivot.x) - (width * (1.0f - m_fillAmount)), height * (-0.5f - pivot.y), 0);
			break;
		case basecross::ImageFillType::Vertical:
			leftUpPoint = Vec3(width * (-0.5f - pivot.x), height * (0.5f - pivot.y) - (height * (1.0f - m_fillAmount)), 0);
			rightUpPoint = Vec3(width * (0.5f - pivot.x), height * (0.5f - pivot.y) - (height * (1.0f - m_fillAmount)), 0);
			leftDownPoint = Vec3(width * (-0.5f - pivot.x), height * (-0.5f - pivot.y), 0);
			rightDownPoint = Vec3(width * (0.5f - pivot.x), height * (-0.5f - pivot.y), 0);
			break;
		}

		Vec2 uvLeftUp = m_UVPoints[0];
		Vec2 uvRightUp = m_UVPoints[1];
		Vec2 uvLeftDown = m_UVPoints[2];
		Vec2 uvRightDowm = m_UVPoints[3];

		switch (m_fillType)
		{
		case basecross::ImageFillType::None:
			break;
		case basecross::ImageFillType::Horizontal:
			uvRightUp.x -= (1.0f - m_fillAmount);
			uvRightDowm.x -= (1.0f - m_fillAmount);
			break;
		case basecross::ImageFillType::Vertical:
			uvLeftUp.y += (1.0f - m_fillAmount);
			uvRightUp.y += (1.0f - m_fillAmount);
			break;
		default:
			break;
		}

		std::vector<VertexPositionColorTexture> vertices = {
			{leftUpPoint, color, uvLeftUp},
			{rightUpPoint, color, uvRightUp},
			{leftDownPoint, color, uvLeftDown} ,
			{rightDownPoint, color, uvRightDowm}
		};

		return vertices;

	}

	void Image::OnCreate()
	{
		PCTSpriteDraw::OnCreate();

		UpdateVertices(CreateVertices());
	}

	void Image::SetUVRect(const Vec2& leftUpPoint, const float width, const float height, Vec2& imageSize)
	{
		for (auto& point : m_UVPoints)
		{
			point = leftUpPoint;
		}

		m_UVPoints[1].x += width;
		m_UVPoints[2].y += height;
		m_UVPoints[3] += Vec2(width, height);

		for (auto& point : m_UVPoints)
		{
			point.x /= imageSize.x;
			point.y /= imageSize.y;
		}

		UpdateVertices(CreateVertices());
	}

	void Image::SetAlpha(const float alpha)
	{
		auto color = GetDiffuse();
		color.w = alpha;
		SetDiffuse(color);
	}

	float Image::GetAlpha() const
	{
		auto color = GetDiffuse();
		return color.w;
	}

	void Image::SetFillType(const ImageFillType fillType)
	{
		m_fillType = fillType;
	}

	ImageFillType Image::GetFillType() const
	{
		return m_fillType;
	}

	void Image::SetFillAmount(const float fillAmount)
	{
		float fill = fillAmount;
		fill = std::fmaxf(fill, 0.0f);
		fill = std::fminf(fill, 1.0f);
		m_fillAmount = fill;
	}

	float Image::GetFillAmount() const
	{
		return m_fillAmount;
	}

	void Image::OnUpdate()
	{
		UpdateVertices(CreateVertices());
	}
	// TextBox ------------------------

	const std::unordered_map<TextBox::TextStyle, DWRITE_FONT_STYLE> TextBox::m_textStypeToDWriteStyleTable = {
		std::make_pair(TextBox::TextStyle::Normal,DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL),
		std::make_pair(TextBox::TextStyle::Italic,DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_ITALIC),
		std::make_pair(TextBox::TextStyle::Oblique,DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_OBLIQUE)
	};

	TextBox::TextBox(std::shared_ptr<GameObject>& owner) :
		Component(owner)
	{
		rectTransform = owner->GetComponent<RectTransform>();

		// デバイスに依存するリソースを作成します。
		auto deviceResources = App::GetApp()->GetDeviceResources();
		auto D2DFactory = deviceResources->GetD2DFactory();

		SetFontColor(m_fontColor);

		SetBoxColor(m_boxColor);

		TextFormatUpdate();

		TextLayoutUpdate();

		D2DFactory->CreateDrawingStateBlock(&m_stateBlock);
	}

	void TextBox::TextFormatUpdate()
	{
		auto deviceResources = App::GetApp()->GetDeviceResources();
		auto DWriteFactory = deviceResources->GetDWriteFactory();

		DWriteFactory->CreateTextFormat(
			m_fontName.c_str(),
			NULL,
			DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_NORMAL,
			m_textStypeToDWriteStyleTable.at(m_textStyle),
			DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL,
			m_fontSize,
			L"ja",
			&m_textFormat
		);
	}

	void TextBox::TextLayoutUpdate()
	{
		auto deviceResources = App::GetApp()->GetDeviceResources();
		auto DWriteFactory = deviceResources->GetDWriteFactory();

		DWriteFactory->CreateTextLayout(
			m_text.c_str(),
			(uint32)m_text.length(),
			m_textFormat.Get(),
			rectTransform->GetWidth(), // 入力テキストの最大幅。
			rectTransform->GetHeight(), // 入力テキストの最大高さ。
			&m_textLayout
		);

		m_textLayout->GetMetrics(&m_textMetrics);

		SetTextSideAlignment(m_textSideAlignment);
	}

	void TextBox::SetBoxColor(const Col4& color)
	{
		m_boxColor = color;

		auto colorF = D2D1::ColorF(color.x, color.y, color.z, color.w);

		auto deviceResources = App::GetApp()->GetDeviceResources();
		auto D2DDeviceContext = deviceResources->GetD2DDeviceContext();

		D2DDeviceContext->CreateSolidColorBrush(colorF, &m_boxBrush);
	}

	void TextBox::SetBoxColor(const float red, const float green, const float blue, const float alpha)
	{
		SetBoxColor(Col4(red, green, blue, alpha));
	}

	Col4 TextBox::GetBoxColor() const
	{
		return m_boxColor;
	}

	void TextBox::SetFontColor(const Col4& color)
	{
		m_fontColor = color;

		auto colorF = D2D1::ColorF(color.x, color.y, color.z, color.w);

		auto deviceResources = App::GetApp()->GetDeviceResources();
		auto D2DDeviceContext = deviceResources->GetD2DDeviceContext();

		D2DDeviceContext->CreateSolidColorBrush(colorF, &m_fontBrush);
	}

	void TextBox::SetFontColor(const float red, const float green, const float blue, const float alpha)
	{
		TextBox::SetFontColor(Col4(red, green, blue, alpha));
	}

	Col4 TextBox::GetFontColor() const
	{
		return m_fontColor;
	}

	void TextBox::SetFontName(const std::wstring& fontName)
	{
		m_fontName = fontName;

		TextFormatUpdate();
	}

	void TextBox::SetFontSize(const float fontSize)
	{
		m_fontSize = fontSize;

		TextFormatUpdate();
	}

	void TextBox::SetAlpha(const float alpha)
	{
		Col4 color = GetFontColor();
		color.w = alpha;
		SetFontColor(color);

		color = GetBoxColor();
		color.w = alpha;
		SetBoxColor(color);
	}

	void TextBox::SetText(const std::wstring& text)
	{
		m_text = text;
	}

	void TextBox::AddText(const std::wstring& text)
	{
		SetText(m_text + text);
	}

	std::wstring TextBox::GetText() const
	{
		return m_text;
	}

	void TextBox::SetTextSideAlignment(const TextSideAlignment textSideAlignment)
	{
		m_textSideAlignment = textSideAlignment;

		DWRITE_TEXT_ALIGNMENT alignment;

		switch (m_textSideAlignment)
		{
		case basecross::TextBox::TextSideAlignment::Left:
			alignment = DWRITE_TEXT_ALIGNMENT_LEADING;
			break;
		case basecross::TextBox::TextSideAlignment::Center:
			alignment = DWRITE_TEXT_ALIGNMENT_CENTER;
			break;
		case basecross::TextBox::TextSideAlignment::Right:
			alignment = DWRITE_TEXT_ALIGNMENT_TRAILING;
			break;
		}

		m_textLayout->SetTextAlignment(alignment);
	}

	TextBox::TextSideAlignment TextBox::GetTextSideAlignment() const
	{
		return m_textSideAlignment;
	}

	void TextBox::SetTextVerticalAlignment(const TextVerticalAlignment textVerticalAlignment)
	{
		m_textVerticalAlignment = textVerticalAlignment;
	}

	TextBox::TextVerticalAlignment TextBox::GetTextVerticalAlignment() const
	{
		return m_textVerticalAlignment;
	}

	void TextBox::SetTextStyle(const TextStyle textStyle)
	{
		m_textStyle = textStyle;
		TextFormatUpdate();
	}

	TextBox::TextStyle TextBox::GetTextStyle() const
	{
		return m_textStyle;
	}

	void TextBox::OnDraw()
	{
		auto deviceResources = App::GetApp()->GetDeviceResources();
		auto D2DDeviceContext = deviceResources->GetD2DDeviceContext();

		auto defaultTranslation = D2D1::Matrix3x2F::Translation(D2D1::SizeF(
			WindowSetting::WINDOW_WIDTH * 0.5f,
			WindowSetting::WINDOW_HEIGHT * 0.5f
		));

		auto worldMatrix = rectTransform->GetWorldMatrix();

		D2DDeviceContext->SetTransform(worldMatrix * defaultTranslation);

		Vec2 offset = rectTransform->GetPivot();

		float width = rectTransform->GetWidth();
		float height = rectTransform->GetHeight();

		D2D1_RECT_F boxRectangle = D2D1::RectF(
			width * (-0.5f - offset.x), height * (-0.5f + offset.y),
			width * ( 0.5f - offset.x), height * ( 0.5f + offset.y)
		);

		D2DDeviceContext->SaveDrawingState(m_stateBlock.Get());
		D2DDeviceContext->BeginDraw();

		//バックグラウンドの描画
		D2DDeviceContext->FillRectangle(&boxRectangle, m_boxBrush.Get());

		float heightOffset = 0;

		switch (m_textVerticalAlignment)
		{
		case basecross::TextBox::TextVerticalAlignment::Up:
			heightOffset = 0;
			break;
		case basecross::TextBox::TextVerticalAlignment::Center:
			heightOffset = rectTransform->GetHeight() * 0.5f - m_textMetrics.height * 0.5f;
			break;
		case basecross::TextBox::TextVerticalAlignment::Down:
			heightOffset = rectTransform->GetHeight() - m_textMetrics.height;
			break;
		}

		TextLayoutUpdate();

		D2DDeviceContext->DrawTextLayout(
			D2D1::Point2F(boxRectangle.left, boxRectangle.top + heightOffset),
			m_textLayout.Get(),
			m_fontBrush.Get()
		);

		D2DDeviceContext->EndDraw();

		D2DDeviceContext->RestoreDrawingState(m_stateBlock.Get());

	}

	// ChoicesObjectAndEvent --------------------

	ChoicesObjectAndEvent::ChoicesObjectAndEvent() :
		ChoicesObjectAndEvent(L"a", std::shared_ptr<GameObject>(), []() {})
	{

	}

	ChoicesObjectAndEvent::ChoicesObjectAndEvent(const std::wstring& text, std::shared_ptr<GameObject>& choicesObject,
		const std::function<void()>& eventFunction) :
		text(text),
		choicesObject(choicesObject),
		eventFunction(eventFunction)
	{

	}
	// ChoicesList ------------------------------

	ChoicesList::ChoicesList(std::shared_ptr<GameObject>& owner) :
		Component(owner)
	{

	}

	void ChoicesList::UpdateChoices()
	{
		m_index = MyMath::Clamp(m_index, 0, static_cast<int>(m_choiceObjectAndEvents.size() - 1));

		if (m_choiceObjectAndEvents.empty())
		{
			m_upTextBox->SetActive(false);
			m_centerTextBox->SetActive(false);
			m_downTextBox->SetActive(false);

			return;
		}

		std::wstring kari = m_choiceObjectAndEvents[m_index]->text;

		auto te = m_centerTextBox->GetComponent<TextBox>();
		m_centerTextBox->GetComponent<TextBox>()->SetText(m_choiceObjectAndEvents[m_index]->text);
		m_centerTextBox->SetActive(true);
		
		if (m_index - 1 >= 0)
		{
			m_upTextBox->GetComponent<TextBox>()->SetText(m_choiceObjectAndEvents[m_index - 1]->text);
			m_upTextBox->SetActive(true);
		}
		else
		{
			m_upTextBox->SetActive(false);
		}

		if (m_index + 1 < m_choiceObjectAndEvents.size())
		{
			m_downTextBox->GetComponent<TextBox>()->SetText(m_choiceObjectAndEvents[m_index + 1]->text);
			m_downTextBox->SetActive(true);
		}
		else
		{
			m_downTextBox->SetActive(false);
		}
	}

	std::shared_ptr<GameObject> ChoicesList::CreateChildTextBoxObject()
	{
		auto uiObject = dynamic_pointer_cast<UIObject>(GetGameObject());

		auto textBoxObject = GetStage()->Instantiate<UIObject>(Vec3(), Quat::Identity(), uiObject);

		auto rect = textBoxObject->GetComponent<RectTransform>();
		rect->SetRectSize(200, 50);

		auto textBox = textBoxObject->AddComponent<TextBox>();

		textBox->SetBoxColor(0, 0, 0, 1);
		textBox->SetFontColor(1, 1, 1, 1);
		textBox->SetFontSize(32);
		textBox->SetTextSideAlignment(TextBox::TextSideAlignment::Center);
		textBox->SetTextVerticalAlignment(TextBox::TextVerticalAlignment::Center);

		textBoxObject->SetDrawLayer(10);

		return textBoxObject;
	}

	void ChoicesList::AddChoice(const std::shared_ptr<ChoicesObjectAndEvent const>& choicesObjectAndEvent)
	{
		m_choiceObjectAndEvents.push_back(choicesObjectAndEvent);

		UpdateChoices();
	}

	void ChoicesList::RemoveChoice(const std::shared_ptr<GameObject>& choicesObject)
	{
		auto it = m_choiceObjectAndEvents.begin();

		while (it != m_choiceObjectAndEvents.end())
		{
			auto& choicesObjectAndEvent = (*it);

			if (!choicesObjectAndEvent)
			{
				it = m_choiceObjectAndEvents.erase(it);

				continue;
			}

			if (choicesObjectAndEvent->choicesObject == choicesObject)
			{
				it = m_choiceObjectAndEvents.erase(it);
				
				continue;
			}

			it++;
		}

		UpdateChoices();
	}

	void ChoicesList::AddIndex(const int addIndex)
	{
		SetIndex(m_index + addIndex);
	}

	void ChoicesList::SetIndex(const int index)
	{
		auto kariIndex = MyMath::Clamp(index, 0, static_cast<int>(m_choiceObjectAndEvents.size() - 1));

		if (kariIndex != m_index)
		{
			selectChangeEvent();
		}

		m_index = kariIndex;

		UpdateChoices();
	}

	void ChoicesList::Invoke()
	{
		if (m_choiceObjectAndEvents.empty())
		{
			return;
		}

		pushEvent();
		auto& choicesObjectAndEvent = m_choiceObjectAndEvents[m_index];
		choicesObjectAndEvent->eventFunction();
	}

	void ChoicesList::OnStart()
	{
		auto uiObject = dynamic_pointer_cast<UIObject>(GetGameObject());

		m_upTextBox = CreateChildTextBoxObject();
		m_centerTextBox = CreateChildTextBoxObject();
		m_downTextBox = CreateChildTextBoxObject();

		auto rect = m_upTextBox->GetComponent<RectTransform>();
		rect->SetPosition(0, 50);
		rect->SetScale(Vec2(0.75f));

		rect = m_downTextBox->GetComponent<RectTransform>();
		rect->SetPosition(0, -50);
		rect->SetScale(Vec2(0.75f));

		m_upTextBox->SetActive(false);
		m_centerTextBox->SetActive(false);
		m_downTextBox->SetActive(false);
	}

	void ChoicesList::OnUpdate2()
	{
		auto it = m_choiceObjectAndEvents.begin();

		while (it != m_choiceObjectAndEvents.end())
		{
			if (!(*it) || !(*it)->choicesObject)
			{
				it = m_choiceObjectAndEvents.erase(it);

				continue;
			}

			it++;
		}

		UpdateChoices();

	}

	// SelectableComponent ----------------------

	SelectableComponent::SelectableComponent(std::shared_ptr<GameObject>& owner) :
		Component(owner)
	{

	}

	void SelectableComponent::SetIsSelectedLock(const bool isSelectedLock)
	{
		m_isSelectedLock = isSelectedLock;
	}

	bool SelectableComponent::GetIsSelectedLock() const
	{
		return m_isSelectedLock;
	}

	void SelectableComponent::OnSelect()
	{
		selectEvent();
	}

	void SelectableComponent::OnPush()
	{
		pushEvent();
	}

	void SelectableComponent::OnOutSelect()
	{
		outSelectEvent();
	}

	void SelectableComponent::SetVerticalBeforeSelectable(const std::shared_ptr<I_Selectable>& verticalBeforeSelectable)
	{
		m_verticalBeforeSelectable = verticalBeforeSelectable;
	}

	void SelectableComponent::SetVerticalNextSelectable(const std::shared_ptr<I_Selectable>& verticalNextSelectable)
	{
		m_verticalNextSelectable = verticalNextSelectable;
	}

	void SelectableComponent::SetHorizontalBeforeSelectable(const std::shared_ptr<I_Selectable>& horizontalBeforeSelectable)
	{
		m_horizontalBeforeSelectable = horizontalBeforeSelectable;
	}

	void SelectableComponent::SetHorizontalNextSelectable(const std::shared_ptr<I_Selectable>& horizontalNextSelectable)
	{
		m_horizontalNextSelectable = horizontalNextSelectable;
	}

	std::shared_ptr<I_Selectable> SelectableComponent::GetVerticalBeforeSelectable() const
	{
		return m_verticalBeforeSelectable.GetShard();
	}

	std::shared_ptr<I_Selectable> SelectableComponent::GetVerticalNextSelectable() const
	{
		return m_verticalNextSelectable.GetShard();
	}

	std::shared_ptr<I_Selectable> SelectableComponent::GetHorizontalBeforeSelectable() const
	{
		return m_horizontalBeforeSelectable.GetShard();
	}

	std::shared_ptr<I_Selectable> SelectableComponent::GetHorizontalNextSelectable() const
	{
		return m_horizontalNextSelectable.GetShard();
	}

	// EventSystem ------------------------------

	ex_weak_ptr<EventSystem> EventSystem::m_eventSystem;

	EventSystem::EventSystem(std::shared_ptr<GameObject>& owner) :
		Component(owner)
	{
	}

	void EventSystem::MoveCheck(bool(itbs::Input::I_BasicInputer::*isDown)() const,std::shared_ptr<I_Selectable>(I_Selectable::*func)() const)
	{
		if (!m_nowSelectable || m_nowSelectable->GetIsSelectedLock())
		{
			return;
		}

		auto nextSelectable = (m_nowSelectable.get()->*func)();

		if (!nextSelectable)
		{
			return;
		}

		if ((m_basicInputer.get()->*isDown)())
		{
			if (m_nowSelectable)
			{
				m_nowSelectable->OnOutSelect();
			}

			m_nowSelectable = nextSelectable;

			if (m_nowSelectable)
			{
				m_nowSelectable->OnSelect();
			}
		}
	}

	void EventSystem::SetNowSelectable(const std::shared_ptr<I_Selectable>& nowSelectable)
	{
		if (m_nowSelectable)
		{
			m_nowSelectable->OnOutSelect();
		}
		m_nowSelectable = nowSelectable;

		if (m_nowSelectable)
		{
			m_nowSelectable->OnSelect();
		}
	}

	void EventSystem::PushSelectable(const std::shared_ptr<I_Selectable>& nowSelectable)
	{
		m_stackSelectable.push(m_nowSelectable);

		m_nowSelectable = nowSelectable;

		if (m_nowSelectable)
		{
			m_nowSelectable->OnSelect();
		}
	}

	void EventSystem::PopSelectable()
	{
		if (m_nowSelectable)
		{
			m_nowSelectable->OnOutSelect();
		}

		m_nowSelectable = m_stackSelectable.top().GetShard();

		m_stackSelectable.pop();
	}

	std::shared_ptr<I_Selectable> EventSystem::GetNowSelectable() const
	{
		return m_nowSelectable.GetShard();
	}

	ex_weak_ptr<EventSystem> EventSystem::GetInstance(const std::shared_ptr<Stage>& stage)
	{
		if (!m_eventSystem || m_eventSystem->GetStage() != stage)
		{
			m_eventSystem = stage->Instantiate<GameObject>()->AddComponent<EventSystem>();
		}

		return m_eventSystem;
	}

	void EventSystem::SetBasicInputer(const std::shared_ptr<itbs::Input::I_BasicInputer>& basicInputer)
	{
		m_basicInputer = basicInputer;
	}

	void EventSystem::OnCreate()
	{
		if (!m_eventSystem)
		{
			m_eventSystem = GetThis<EventSystem>();
		}
	}

	void EventSystem::OnUpdate()
	{
		if (!m_basicInputer)
		{
			return;
		}

		MoveCheck(&itbs::Input::I_BasicInputer::IsLeftDown, &I_Selectable::GetHorizontalBeforeSelectable);
		MoveCheck(&itbs::Input::I_BasicInputer::IsRightDown, &I_Selectable::GetHorizontalNextSelectable);
		MoveCheck(&itbs::Input::I_BasicInputer::IsUpDown, &I_Selectable::GetVerticalBeforeSelectable);
		MoveCheck(&itbs::Input::I_BasicInputer::IsDownDown, &I_Selectable::GetVerticalNextSelectable);

		if ((m_basicInputer.get()->* & itbs::Input::I_BasicInputer::IsDesitionDown)())
		{
			if (m_nowSelectable)
			{
				m_nowSelectable->OnPush();
			}
		}
	}

	// Button -----------------------------------

	Button::Button(std::shared_ptr<GameObject>& owner) :
		SelectableComponent(owner)
	{

	}

	void Button::SetNormalButtonImage(const std::wstring& normalButtonImageKey)
	{
		m_normalButtonImageKey = normalButtonImageKey;

		if (!m_image->GetTextureResource())
		{
			m_image->SetTextureResource(m_normalButtonImageKey);
		}
	}

	void Button::SetSelectedButtonImage(const std::wstring& selectedButtonImageKey)
	{
		m_selectedButtonImageKey = selectedButtonImageKey;
	}

	void Button::SetAllButtonImage(const std::wstring& allButtonImageKey)
	{
		SetNormalButtonImage(allButtonImageKey);
		SetSelectedButtonImage(allButtonImageKey);
	}


	void Button::OnCreate()
	{
		m_image = GetGameObject()->GetComponent<Image>(false);

		if (!m_image)
		{
			m_image = GetGameObject()->AddComponent<Image>();
		}

		EventSystem::GetInstance(GetStage());
	}

	void Button::OnSelect()
	{
		SelectableComponent::OnSelect();

		m_image->SetTextureResource(m_selectedButtonImageKey);
	}

	void Button::OnOutSelect()
	{
		SelectableComponent::OnOutSelect();

		m_image->SetTextureResource(m_normalButtonImageKey);
	}

	// UIObject ---------------------------------

	UIObject::UIObject(std::shared_ptr<Stage>& stage) :
		GameObject(stage)
	{

	}

	void UIObject::OnPreCreate()
	{
		GameObject::OnPreCreate();

		rectTransform = AddComponent<RectTransform>();
	}
}