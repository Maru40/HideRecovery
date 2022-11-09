#include "UICancelEventer.h"

namespace basecross
{
	UICancelEventer::UICancelEventer(const std::shared_ptr<GameObject>& owner) :
		Component(owner)
	{

	}

	void UICancelEventer::OnCancel()
	{
		for (const auto& cancelEvent : m_cancelEvents)
		{
			cancelEvent();
		}
	}
}