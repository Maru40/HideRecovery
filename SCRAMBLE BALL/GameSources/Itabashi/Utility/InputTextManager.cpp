#include "InputTextManager.h"

namespace itbs
{
namespace Input
{
	std::unordered_set<I_InputText*> InputTextManager::m_inputTexts;

	void InputTextManager::Push(wchar_t wc)
	{
		for (auto& inputText : m_inputTexts)
		{
			inputText->Push(wc);
		}
	}
}
}