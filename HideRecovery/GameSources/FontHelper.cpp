#include"FontHelper.h"

namespace basecross
{
	std::vector<std::wstring> FontHelper::m_systemFontNames;

	std::vector<std::wstring> FontHelper::m_customFontNames;

	IDWriteFontCollection* FontHelper::m_customFontCollectionPtr = nullptr;

	//IDWriteFontCollectionLoader* FontHelper::m_fontCollectionLoaderPtr = new(std::nothrow) IDWriteFontCollectionLoader();

	const std::vector<std::wstring>& FontHelper::GetSystemFontNames()
	{
		//システムフォントが一つ以上(一度でも取得した)いたら返す
		if (!m_systemFontNames.empty())
		{
			return m_systemFontNames;
		}

		auto deviceResources = App::GetApp()->GetDeviceResources();
		auto DWriteFactory = deviceResources->GetDWriteFactory();

		IDWriteFontCollection* fontCollectionPtr = nullptr;

		DWriteFactory->GetSystemFontCollection(&fontCollectionPtr);

		int nameCount = fontCollectionPtr->GetFontFamilyCount();

		for (int i = 0; i < nameCount; i++)
		{
			IDWriteFontFamily* fontFamilyPtr = nullptr;

			fontCollectionPtr->GetFontFamily(i, &fontFamilyPtr);

			IDWriteLocalizedStrings* localizedFamilyName = nullptr;
			fontFamilyPtr->GetFamilyNames(&localizedFamilyName);

			UINT32 index = 0;
			BOOL exists = false;

			wchar_t localeName[LOCALE_NAME_MAX_LENGTH]{};

			int defaultLocaleSuccess = GetUserDefaultLocaleName(localeName, LOCALE_NAME_MAX_LENGTH);

			if (defaultLocaleSuccess > 0)
			{
				localizedFamilyName->FindLocaleName(localeName, &index, &exists);
			}

			if (!exists)
			{
				localizedFamilyName->FindLocaleName(L"en-us", &index, &exists);
			}

			if (!exists)
			{
				index = 0;
			}

			UINT32 length = 0;

			localizedFamilyName->GetStringLength(index, &length);

			wchar_t* fontName = new (std::nothrow) wchar_t[length + 1];

			if (fontName != nullptr)
			{
				localizedFamilyName->GetString(index, fontName, length + 1);

				std::wstring name = fontName;

				delete[] fontName;

				m_systemFontNames.push_back(name);
			}
		}

		return m_systemFontNames;
	}

	bool FontHelper::AddCustomFont(const std::wstring& fontPath)
	{
		auto deviceResources = App::GetApp()->GetDeviceResources();
		auto DWriteFactory = deviceResources->GetDWriteFactory();

		//DWriteFactory->CreateCustomFontCollection();

		return true;
	}
}