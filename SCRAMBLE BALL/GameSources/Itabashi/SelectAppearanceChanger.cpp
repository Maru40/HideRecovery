#include "SelectAppearanceChanger.h"

namespace basecross
{
	SelectAppearanceChanger::SelectAppearanceChanger(const std::shared_ptr<GameObject>& owner) :
		Component(owner)
	{

	}

	void SelectAppearanceChanger::UpdateAppearanceChanger(const Vec3& size, const Col4& color)
	{
		auto transform = m_transform.lock();
		auto baseDraw = m_baseDraw.lock();
		auto spriteDraw = m_spriteDraw.lock();

		if (transform)
		{
			transform->SetScale(size);
		}

		if (baseDraw)
		{
			baseDraw->SetDiffuse(color);
		}

		if (spriteDraw)
		{
			spriteDraw->SetDiffuse(color);
		}
	}

	void SelectAppearanceChanger::OnLateStart()
	{
		auto owner = GetGameObject();

		m_transform = owner->GetComponent<Transform>(false);
		m_baseDraw = owner->GetComponent<SmBaseDraw>(false);
		m_spriteDraw = owner->GetComponent<SpriteBaseDraw>(false);

		auto& size = m_isSelected ? m_selectedSize : m_deselectedSize;
		auto& color = m_isSelected ? m_selectedColor : m_deselectedColor;

		UpdateAppearanceChanger(size, color);
	}

	void SelectAppearanceChanger::OnSelect()
	{
		m_isSelected = true;

		UpdateAppearanceChanger(m_selectedSize, m_selectedColor);
	}

	void SelectAppearanceChanger::OnDeselect()
	{
		m_isSelected = false;

		UpdateAppearanceChanger(m_deselectedSize, m_deselectedColor);
	}

	void SelectAppearanceChanger::SetSelectedSize(const Vec3& size)
	{
		m_selectedSize = size;

		if (m_isSelected)
		{
			UpdateAppearanceChanger(m_selectedSize, m_selectedColor);
		}
	}

	void SelectAppearanceChanger::SetDeselectedSize(const Vec3& size)
	{
		m_deselectedSize = size;

		if (!m_isSelected)
		{
			UpdateAppearanceChanger(m_deselectedSize, m_deselectedColor);
		}
	}

	void SelectAppearanceChanger::SetSelectedColor(const Col4& color)
	{
		m_selectedColor = color;

		if (m_isSelected)
		{
			UpdateAppearanceChanger(m_selectedSize, m_selectedColor);
		}
	}

	void SelectAppearanceChanger::SetDeselectedColor(const Col4& color)
	{
		m_deselectedColor = color;

		if (!m_isSelected)
		{
			UpdateAppearanceChanger(m_deselectedSize, m_deselectedColor);
		}
	}
}