#include "PasswordTextUI.h"
#include "Watanabe/UI/Numbers.h"

/// <summary>
/// 数字文字を増やす(9を超えたら0に戻す)
/// </summary>
/// <param name="c">現在の文字</param>
/// <returns>結果文字</returns>
wchar_t UpRollChar(wchar_t c)
{
	++c;

	if (c > L'9')
	{
		c = L'0';
	}

	return c;
}

/// <summary>
/// 数字文字を減らす(0を下回ったら0に戻す)
/// </summary>
/// <param name="c">現在の文字</param>
/// <returns>結果文字</returns>
wchar_t DownRollChar(wchar_t c)
{
	--c;

	if (c < L'0')
	{
		c = L'9';
	}

	return c;
}

namespace basecross
{
namespace UI
{
	void PasswordTextUI::UpdateTrianglePosition(UIMoveDirection direction)
	{
		if (direction == UIMoveDirection::Left)
		{
			m_targetPasswordDisitNum = std::max(--m_targetPasswordDisitNum, 0);
		}

		if (direction == UIMoveDirection::Right)
		{
			m_targetPasswordDisitNum = std::min(++m_targetPasswordDisitNum, m_passwordDisitNum - 1);
		}

		auto number = m_numbers.lock();

		if (!number)
		{
			return;
		}
		auto numberSprite = number->GetNumberSprite(m_targetPasswordDisitNum);
		auto numberPosition = numberSprite->GetRectTransform()->GetPosition();

		auto upTriangle = m_upTriangle.lock();
		auto downTriangle = m_downTriangle.lock();

		if (upTriangle)
		{
			upTriangle->GetComponent<RectTransform>()->SetPosition(numberPosition.x, 120);
		}

		if (downTriangle)
		{
			downTriangle->GetComponent<RectTransform>()->SetPosition(numberPosition.x, -120);
		}
	}

	void PasswordTextUI::ChangeNumber(UIMoveDirection direction)
	{
		wchar_t& changeChar = m_passwordText[m_targetPasswordDisitNum];

		auto RollFunc = direction == UIMoveDirection::Up ? &UpRollChar : &DownRollChar;

		changeChar = RollFunc(changeChar);

		auto numbers = m_numbers.lock();

		if (!numbers)
		{
			return;
		}

		numbers->SetNumber(std::stoi(m_passwordText));
	}

	PasswordTextUI::PasswordTextUI(const std::shared_ptr<GameObject>& owner) :
		SelectableComponent(owner)
	{
		m_passwordText.resize(m_passwordDisitNum, L'0');
		m_targetPasswordDisitNum = 0;
	}

	void PasswordTextUI::OnLateStart()
	{
		UpdateTrianglePosition(UIMoveDirection::None);
	}

	void PasswordTextUI::Clear()
	{
		m_passwordText.clear();
		m_passwordText.resize(m_passwordDisitNum, L'0');
		m_targetPasswordDisitNum = 0;
		UpdateTrianglePosition(UIMoveDirection::None);

		auto numbers = m_numbers.lock();

		if (numbers)
		{
			numbers->SetNumber(0);
		}
	}

	void PasswordTextUI::OnSubmit()
	{
		if (!IsComplete())
		{
			return;
		}

		for (const auto& submitEvent : m_submitEvents)
		{
			submitEvent(m_passwordText);
			SimpleSoundManager::OnePlaySE(m_decisionSEKey);
		}
	}

	void PasswordTextUI::OnMove(UIMoveDirection direction)
	{
		SelectableComponent::OnMove(direction);


		if (direction == UIMoveDirection::None)
		{
			return;
		}

		if (direction == UIMoveDirection::Left || direction == UIMoveDirection::Right)
		{
			UpdateTrianglePosition(direction);
			return;
		}

		if (direction == UIMoveDirection::Up || direction == UIMoveDirection::Down)
		{
			ChangeNumber(direction);
		}
	}
}
}