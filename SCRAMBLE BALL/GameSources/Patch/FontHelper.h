#pragma once

namespace basecross
{
	class FontHelper
	{
		static std::vector<std::wstring> m_systemFontNames;

		static std::vector<std::wstring> m_customFontNames;

		static IDWriteFontCollection* m_customFontCollectionPtr;

		//static IDWriteFontCollectionLoader* m_fontCollectionLoaderPtr;
	public:
		static const std::vector<std::wstring>& GetSystemFontNames();

		static bool AddCustomFont(const std::wstring& fontPath);
	};
}