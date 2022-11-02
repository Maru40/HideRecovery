#pragma once
#include <unordered_set>

namespace itbs
{
namespace Input
{
	class I_InputText
	{
	public:
		virtual bool Push(wchar_t wc) = 0;

		virtual const wchar_t* GetText() const = 0;

		virtual void Clear() = 0;
	};

	class InputTextManager
	{
		static std::unordered_set<I_InputText*> m_inputTexts;

	public:

		InputTextManager() = delete;

		static void Push(wchar_t wc);

		static void AddInputText(I_InputText* inputText) { m_inputTexts.insert(inputText); }
		static void RemoveInputText(I_InputText* inputText) { m_inputTexts.erase(inputText); }
	};
}
}