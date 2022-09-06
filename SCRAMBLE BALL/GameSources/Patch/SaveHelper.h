#pragma once
#include"StructIOFileStream.h"

namespace basecross
{
	template<class SaveStruct>
	class SavableBase
	{
		SaveStruct& m_saveStruct;
		std::wstring m_filePath;
	public:
		SavableBase(SaveStruct& saveStruct, const std::wstring& filePath) :
			m_saveStruct(saveStruct),
			m_filePath(filePath)
		{

		}

		bool Load()
		{
			auto loadSaveStruct = m_saveStruct;

			auto isLoadSuccess = itbs::Utility::StructIOFileStream::LoadFile(loadSaveStruct, App::GetApp()->GetDataDirWString() + m_filePath);

			if (!isLoadSuccess)
			{
				return false;
			}

			m_saveStruct = loadSaveStruct;

			return true;
		}

		bool Save()
		{
			return itbs::Utility::StructIOFileStream::WriteFile(m_saveStruct, App::GetApp()->GetDataDirWString() + m_filePath);
		}
	};
}