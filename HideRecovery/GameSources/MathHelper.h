#pragma once
#include<cmath>
#include<type_traits>

namespace itbs
{
	namespace Math
	{
		class MyMath
		{
		public:
			/// <summary>
			/// 比べて最大を返す
			/// </summary>
			/// <param name="a">要素1</param>
			/// <param name="b">要素2</param>
			/// <returns>より大きい値</returns>
			template<class T, std::enable_if_t<std::is_arithmetic_v<T>, std::nullptr_t> = nullptr>
			static T Max(const T& a, const T& b)
			{
				return a > b ? a : b;
			}

			/// <summary>
			/// 比べて最小を返す
			/// </summary>
			/// <param name="a">要素1</param>
			/// <param name="b">要素2</param>
			/// <returns>より小さい値</returns>
			template<class T, std::enable_if_t<std::is_arithmetic_v<T>, std::nullptr_t> = nullptr>
			static T Min(const T& a, const T& b)
			{
				return a < b ? a : b;
			}

			/// <summary>
			/// 値を範囲制限して返す
			/// </summary>
			/// <param name="value">値</param>
			/// <param name="min">最小</param>
			/// <param name="max">最大</param>
			/// <returns>範囲制限した値</returns>
			template<class T, std::enable_if_t<std::is_arithmetic_v<T>, std::nullptr_t> = nullptr>
			static T Clamp(T value, const T& min, const T& max)
			{
				value = Min<T>(value, max);
				value = Max<T>(value, min);

				return value;
			}
		};
	}
}