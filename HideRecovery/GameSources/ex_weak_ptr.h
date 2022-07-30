#pragma once
#include<memory>

namespace itbs
{
	namespace Utility
	{
		template<class T>
		class ex_weak_ptr
		{
			std::weak_ptr<T> ptr;

		public:
			ex_weak_ptr() noexcept = default;

			ex_weak_ptr(const std::shared_ptr<T>& shardPtr)
			{
				ptr = shardPtr;
			}

			ex_weak_ptr& operator=(const std::shared_ptr<T>& shardPtr)
			{
				ptr = shardPtr;
				return *this;
			}

			std::shared_ptr<T> operator->() const
			{
				return ptr.lock();
			}

			std::shared_ptr<T> GetShard() const
			{
				return ptr.lock();
			}

			operator bool() const
			{
				return !ptr.expired();
			}

			T* get() const
			{
				return ptr.lock().get();
			}

			bool operator==(const ex_weak_ptr& other) const
			{
				return get() == other.get();
			}

			bool operator!=(const ex_weak_ptr& other) const
			{
				return !(*this == other);
			}
		};
	}
}