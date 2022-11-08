
/*!
@file Random.h
@brief Randomクラス
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"
#include <random>

namespace basecross {

	namespace maru {
		class MyRandom
		{
			static std::mt19937 m_mt;

		public:

			/// <summary>
			/// 渡された値から、ランダムな値を返す。
			/// </summary>
			template<class T>
			static T Random(const T& value)
			{
				auto absValue = std::abs(value);
				return Random(-absValue, absValue);
			}

			/// <summary>
			/// 渡された最大と最少の値から、ランダムな値を返す。
			/// </summary>
			template<class T>
			static T Random(const T& min, const T& max)
			{
				std::uniform_real_distribution<> dist(min, max);

				auto value = static_cast<T>(dist(m_mt));

				return value;
			}

			/// <summary>
			/// ベクトルの要素を絶対値に直す
			/// </summary>
			/// <param name="vec">直したいベクトル</param>
			/// <returns>ベクトルの要素を絶対値に直す。</returns>
			static Vec3 AbsVec(const Vec3& vec) {
				auto resultVec = Vec3(0.0f);

				resultVec.x = abs(vec.x);
				resultVec.y = abs(vec.y);
				resultVec.z = abs(vec.z);

				return resultVec;
			}

			/// <summary>
			/// ランダムなベクトルを返す。
			/// </summary>
			/// <param name="randomVec">ランダム範囲</param>
			/// <returns>ランダムなベクトル</returns>
			static Vec3 RandomVector(const Vec3& randomVec) {
				auto absVec = AbsVec(randomVec);
				return RandomVector(-absVec, absVec);
			}

			/// <summary>
			/// ランダムなベクトルを返す
			/// </summary>
			/// <param name="min">最小値</param>
			/// <param name="max">最大値</param>
			/// <returns>ランダムなベクトル</returns>
			static Vec3 RandomVector(const Vec3& min, const Vec3& max) {
				Vec3 randomPosition(0.0f);

				randomPosition.x = Random(min.x, max.x);
				randomPosition.y = Random(min.y, max.y);
				randomPosition.z = Random(min.z, max.z);

				return randomPosition;
			}

			/// <summary>
			/// 何割の確率化変数で受け取る。
			/// 0～100までの間の数値を引き数で受け取る。
			/// </summary>
			/// <param name="prob">確率</param>
			/// <returns>確率でtrueかfalseを返す</returns>
			static bool RandomProbability(const float prob) {
				auto random = MyRandom::Random(0.0f, 100.0f);
				if (random <= prob) {
					return true;
				}
				else {
					return false;
				}
			}

			/// <summary>
			/// 配列の中からランダムな要素を返す。
			/// </summary>
			template<class T>
			static T RandomArray(const vector<T>& vec) {
				auto ran = MyRandom::Random(0, (int)vec.size());

				return vec[ran];
			}

			/// <summary>
			/// ランダムなポジションを返す。
			/// </summary>
			/// <param name="maxRangeBase">最大値のベース値</param>
			/// <returns>ランダムなポジション</returns>
			static Vec3 RandomPosition(const Vec3& maxRangeBase) {
				auto maxRange = Vec3(abs(maxRangeBase.x), abs(maxRangeBase.y), abs(maxRangeBase.z));

				return RandomVector(-maxRange, maxRange);
			}

		};
	}

}

//endbasecross