#pragma once
#include<memory>
#include<vector>
#include"ex_weak_ptr.h"

namespace itbs
{
	namespace Utility
	{
		template<class T, class... Types>
		class I_Func
		{
		public:
			virtual T operator()(Types... args) = 0;
			virtual bool IsValid() const = 0;

			virtual bool operator==(const I_Func<T, Types...>& func) const = 0;

			bool operator!=(const I_Func<T, Types...>& func) const
			{
				!this->operator==(func);
			}
		};

		template<class Object, class T, class... Args>
		class MemberFunction : public I_Func<T, Args...>
		{
		public:
			using func_type = T(Object::*)(Args...);

			struct FunctionSet
			{
				itbs::Utility::ex_weak_ptr<Object> ptr;
				func_type fn;

				FunctionSet(std::shared_ptr<Object>& ptr, func_type fn) :
					ptr(ptr),
					fn(fn)
				{

				}

				bool operator==(const FunctionSet& functionSet) const
				{
					return ptr.get() == functionSet.ptr.get() &&
						fn == fn;
				}

				bool operator!=(const FunctionSet& functionSet) const
				{
					return !*this == functionSet;
				}
			};
		private:
			FunctionSet m_functionSet;

		public:
			MemberFunction(std::shared_ptr<Object>& ptr, func_type fn) :
				m_functionSet(ptr, fn)
			{

			}

			T operator()(Args... args) override
			{
				auto* objectPtr = m_functionSet.ptr.get();
				return (objectPtr->*(m_functionSet.fn))(args...);
			}

			bool IsValid() const override
			{
				return m_functionSet.ptr;
			}

			FunctionSet GetFunctionSet() const
			{
				return m_functionSet;
			}

			bool operator==(const I_Func<T, Args...>& func) const override
			{
				auto memberFunc = dynamic_cast<const MemberFunction&>(func);

				try
				{
					return m_functionSet == memberFunc.GetFunctionSet();
				}
				catch (...)
				{
					return false;
				}
			}
		};

		template<class T, class... Args>
		class GlobalFunction : public I_Func<T, Args...>
		{
		public:
			using func_type = T(*)(Args...);

		private:
			func_type fn;

		public:
			GlobalFunction(func_type fn) :
				fn(fn)
			{

			}

			T operator()(Args... args) override
			{
				return (*fn)(args...);
			}

			bool IsValid() const override
			{
				return true;
			}

			func_type GetPtr() const
			{
				return fn;
			}

			bool operator==(const I_Func<T, Args...>& func) const
			{
				auto globalFunc = dynamic_cast<const GlobalFunction&>(func);

				try
				{
					return fn == globalFunc.GetPtr();
				}
				catch (...)
				{
					return false;
				}
			}
		};

		template<class T>
		class Delegate;

		template<class T, class... Args>
		class Delegate<T(Args...)>
		{
			using I_func = I_Func<T, Args...>;
			std::vector<std::shared_ptr<I_func>> functions;
		public:

			template<class Obj>
			void AddFunc(std::shared_ptr<Obj>& object, T(Obj::* fn)(Args...))
			{
				auto memberFunc = std::make_shared<MemberFunction<Obj, T, Args...>>(object, fn);
				functions.push_back(std::move(memberFunc));
			}

			void AddFunc(T(*fn)(Args...))
			{
				auto globalFunc = std::make_unique<GlobalFunction<T, Args...>>(fn);
				functions.push_back(std::move(globalFunc));
			}

			Delegate() {}

			template<class Obj>
			Delegate(std::shared_ptr<Obj>& object, T(Obj::* fn)(Args...))
			{
				AddFunc(object, fn);
			}

			Delegate(T(*fn)(Args...))
			{
				AddFunc(fn);
			}

			template<class Obj>
			void RemoveFunc(std::shared_ptr<Obj>& object, T(Obj::* fn)(Args...))
			{
				auto memberFunc = MemberFunction<Obj, T, Args...>(object, fn);

				auto it = functions.begin();

				while (it != functions.end())
				{
					auto& func = (*it);

					if (func->operator==(memberFunc))
					{
						it = functions.erase(it);
						return;
					}

					it++;
				}
			}

			void RemoveFunc(T(*fn)(Args...))
			{
				auto globalFunc = GlobalFunction<T, Args...>(fn);

				auto it = functions.begin();

				while (it != functions.end())
				{
					auto& func = (*it);

					if (func->operator==(globalFunc))
					{
						it = functions.erase(it);
						return;
					}

					it++;
				}
			}

			const std::vector<std::shared_ptr<I_func>>& GetFunction() const
			{
				return functions;
			}

			void Clear()
			{
				functions.clear();
			}

			void operator()(Args... args)
			{
				auto it = functions.begin();

				while (it != functions.end())
				{
					auto& func = (*it);
					if (!func->IsValid())
					{
						it = functions.erase(it);
						continue;
					}

					(*func)(args...);
					it++;
				}
			}

			bool operator==(const Delegate& other) const
			{
				auto& otherFunction = other.GetFunction();

				auto check = [](const std::shared_ptr<I_func>& func1, const std::shared_ptr<I_func>& func2)
				{
					return func1->operator==(*func2);
				};

				return std::equal(std::begin(functions), std::end(functions),
					std::begin(otherFunction), std::end(otherFunction), check);
			}

			bool operator!=(const Delegate& other) const
			{
				return !(*this == other);
			}
		};

		template<class Friend, class T, class... Args>
		class EventDelegate;
	}
}