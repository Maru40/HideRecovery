#pragma once
#include <random>
#include <unordered_set>

namespace basecross
{
	template<class T = std::uint64_t, std::enable_if_t<std::is_unsigned_v<T>, std::nullptr_t> = nullptr>
	class UniqueIdCreater
	{
		std::mt19937_64 m_randomEngine;
		std::unordered_set<T> m_uniqueIds;

		T m_min = std::numeric_limits<T>::min();
		T m_max = std::numeric_limits<T>::max();

	public:

		UniqueIdCreater() : m_randomEngine(std::random_device()()) {}

		UniqueIdCreater(std::uint64_t seed) : m_randomEngine(seed) {}

		UniqueIdCreater(std::uint64_t seed, T min, T max) : m_randomEngine(seed), m_min(min), m_max(max) {}

		_NODISCARD T CreateId()
		{
			T id = 0;

			while (true)
			{
				id = std::uniform_int_distribution<T>(m_min, m_max)(m_randomEngine);

				auto find = m_uniqueIds.find(id);

				if (find == m_uniqueIds.end())
				{
					break;
				}
			}

			m_uniqueIds.insert(id);

			return id;
		}

		bool RemoveId(T id)
		{
			auto find = m_uniqueIds.find(id);

			if (find == m_uniqueIds.end())
			{
				return false;
			}

			m_uniqueIds.erase(id);

			return true;
		}

		bool ChangeId(T beforeId, T afterId)
		{
			auto find = m_uniqueIds.find(afterId);

			if (find != m_uniqueIds.end())
			{
				return false;
			}

			m_uniqueIds.insert(afterId);
			m_uniqueIds.erase(beforeId);

			return true;
		}

		void SetMinId(T min) { m_min = min; }
		void SetMaxId(T max) { m_max = max; }

		T GetMinId() const { return m_min; }
		T GetMaxId() const { return m_max; }
	};
}