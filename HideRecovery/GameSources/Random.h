#pragma once
#include<random>

namespace itbs
{
	namespace Math
	{
		class Random
		{
			static std::mt19937 mt;

		public:
			/// <summary>
			/// 最小値と最大値の間をランダムで返す
			/// </summary>
			/// <param name="min">最小値</param>
			/// <param name="max">最大値</param>
			/// <returns>最小値と最大値の間のランダムな値</returns>
			static float Range(const float min, const float max);
			/// <summary>
			/// 一定確率でtrueを返す
			/// </summary>
			/// <param name="probability">確率</param>
			/// <returns>当たればtrue,はずれればfalse</returns>
			static bool Probability(float probability);
		};
	}
}