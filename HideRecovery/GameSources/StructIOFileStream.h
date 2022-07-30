#pragma once
#include<fstream>
#include<string>

namespace itbs
{
	namespace Utility
	{

		class StructIOFileStream
		{
			//à”ñ°ÇÃÇ»Ç¢ÉâÉìÉ_ÉÄÇ»êî
			static constexpr unsigned int eof_number = 0xfd897fad;

			static void CreateAllDirectory(const std::wstring& filePath)
			{
				std::wstringstream wss;

				for (const auto& c : filePath)
				{
					wss << c;

					if (c == L'/' || c == L'\\')
					{
						CreateDirectory(wss.str().c_str(), NULL);
					}
				}
			}

		public:
			template<class T>
			static bool LoadFile(T& object, const std::wstring& path)
			{
				std::ifstream ifs(path, std::ios::binary);

				if (ifs.fail())
				{
					ifs.close();
					return false;
				}

				ifs.read((char*)&object, sizeof(T));

				unsigned int eof_n = 0;
				ifs.read((char*)&eof_n, sizeof(eof_n));

				if (ifs.fail() || eof_n != eof_number)
				{
					return false;
				}

				ifs.close();
				return true;
			}

			template<class T>
			static bool WriteFile(const T& object, const std::wstring& path)
			{
				CreateAllDirectory(path);

				std::ofstream ofs;
				ofs.open(path, std::ios::binary);

				ofs.write((char*)&object, sizeof(T));
				ofs.write((char*)&eof_number, sizeof(eof_number));
				ofs.close();

				return true;
			}
		};
	}
}