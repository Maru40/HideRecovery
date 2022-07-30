#pragma once
#include<map>

namespace itbs
{
	namespace Math
	{
		/// <summary>
		/// ベジェグラフ上の点を表す構造体
		/// </summary>
		struct BezierPoint
		{
			/// <summary>
			/// 時間 (グラフ上のX座標)
			/// </summary>
			float time;
			/// <summary>
			/// 値 (グラフ上のY座標)
			/// </summary>
			float value;

		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			BezierPoint();
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="time">時間</param>
			/// <param name="value">値</param>
			BezierPoint(const float time, const float value);

			bool operator==(const BezierPoint& other) const;
			bool operator!=(const BezierPoint& other) const;

			BezierPoint operator+(const BezierPoint& other) const;
		};

		/// <summary>
		/// ベジェグラフのアンカーポイントを表す構造体
		/// </summary>
		struct BezierAnchorPoint
		{
			/// <summary>
			/// アンカーポイント自体の座標
			/// </summary>
			BezierPoint point;
			/// <summary>
			/// 左側のハンドルの座標 (自身との相対座標)
			/// </summary>
			BezierPoint leftHandle;
			/// <summary>
			/// 右側のハンドルの座標 (自身との相対座標)
			/// </summary>
			BezierPoint rightHandle;

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="point">アンカーポイント自体の座標</param>
			BezierAnchorPoint(const BezierPoint& point);

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="time">アンカーポイント自体の時間 (グラフ上のX座標)</param>
			/// <param name="value">アンカーポイント自体の値 (グラフ上の座標)</param>
			BezierAnchorPoint(const float time, const float value);

			/// <summary>
			/// 左ハンドルが有効かどうか
			/// </summary>
			/// <returns>有効ならtrue</returns>
			bool IsLeftValid() const;

			/// <summary>
			/// 右ハンドルが有効かどうか
			/// </summary>
			/// <returns>有効ならtrue</returns>
			bool IsRightVaild() const;

			/// <summary>
			/// 左のハンドル座標を、ハンドルの長さと角度から代入する (開始地点は左、回転は反時計回り)
			/// </summary>
			/// <param name="length">ハンドルの長さ</param>
			/// <param name="radian">ハンドルの角度</param>
			void SetLeftHandleFromRadian(const float length, const float radian);

			/// <summary>
			/// 右のハンドル座標を、ハンドルの長さと角度から代入する (開始地点は右、回転は時計回り)
			/// </summary>
			/// <param name="length">ハンドルの長さ</param>
			/// <param name="radian">ハンドルの角度</param>
			void SetRightHandleFromRadian(const float length, const float radian);

			/// <summary>
			///	左右に長さ1、角度無しのハンドルを作成する
			/// </summary>
			void SetDefaultHandle();

			/// <summary>
			/// 左のハンドルを削除する
			/// </summary>
			void DeleteLeftHandle();
			/// <summary>
			/// 右のハンドルを削除する
			/// </summary>
			void DeleteRightHandle();
		};

		/// <summary>
		/// ベジェグラフデータマネージャークラス
		/// </summary>
		class BezierManager
		{
			/// <summary>
			/// アンカーポイントデータ配列
			/// </summary>
			std::map<float, BezierAnchorPoint> m_anchorPoints;

			/// <summary>
			/// 点が二つの場合の値(y値)の計算
			/// </summary>
			/// <param name="time">時間 (x値)</param>
			/// <param name="p1">点１</param>
			/// <param name="p2">点２</param>
			/// <returns>値 (y値)</returns>
			float GetTime(const float time,const BezierPoint& p1, const BezierPoint& p2) const;
			/// <summary>
			/// 点が三つの場合の値(y値)の計算
			/// </summary>
			/// <param name="time">時間 (x値)</param>
			/// <param name="p1">点１</param>
			/// <param name="p2">点２</param>
			/// <param name="p3">点３</param>
			/// <returns>値 (y値)</returns>
			float GetTime(const float time, const BezierPoint& p1, const BezierPoint& p2, const BezierPoint& p3) const;
			/// <summary>
			/// 点が四つの場合の値(y値)の計算
			/// </summary>
			/// <param name="time">時間 (x値)</param>
			/// <param name="p1">点１</param>
			/// <param name="p2">点２</param>
			/// <param name="p3">点３</param>
			/// <param name="p4">点４</param>
			/// <returns>値 (y値)</returns>
			float GetTime(const float time, const BezierPoint& p1, const BezierPoint& p2, const BezierPoint& p3, const BezierPoint& p4) const;
		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="startValue">初期点(time = 0.0f)の値</param>
			BezierManager(const float startValue);

			/// <summary>
			/// アンカーポイントの追加
			/// </summary>
			/// <param name="bezierPoint">アンカーポイント自体の座標</param>
			/// <returns>追加したアンカーポイントの参照</returns>
			BezierAnchorPoint& AddAnchorPoint(const BezierPoint& bezierPoint);
			/// <summary>
			/// アンカーポイントの追加
			/// </summary>
			/// <param name="bezierPoint">アンカーポイントのデータ</param>
			/// <returns>追加したアンカーポイントの参照</returns>
			BezierAnchorPoint& AddAnchorPoint(const BezierAnchorPoint& bezierPoint);
			/// <summary>
			/// アンカーポイントの追加
			/// </summary>
			/// <param name="time">アンカーポイント自体の時間 (x値)</param>
			/// <param name="value">アンカーポイント自体の値 (y値)</param>
			/// <returns>追加したアンカーポイントの参照</returns>
			BezierAnchorPoint& AddAnchorPoint(const float time, const float value);

			/// <summary>
			/// 初期点(time = 0.0f)のアンカーポイントの取得
			/// </summary>
			/// <returns>初期点の参照</returns>
			BezierAnchorPoint& GetZeroAnchorPoint();

			/// <summary>
			/// 最後のアンカーポイントのtimeを取得する
			/// </summary>
			/// <returns>最後のアンカーポイントのtime</returns>
			float GetLastPointTime() const;

			/// <summary>
			/// 時間からグラフデータ上のvalueを取得する
			/// </summary>
			/// <param name="time">取得する時間</param>
			/// <returns>時間に対応した値</returns>
			float TimeToValue(const float time) const;
		};
	}

	namespace Utility
	{
		class TimeLineFloat
		{
			Math::BezierManager m_bezier;

			float m_nowTime = 0.0f;
			float m_maxTime = 1.0f;

			bool m_isReverse = false;

		public:
			TimeLineFloat(const Math::BezierManager& bezier, const float maxTime);

			void SetNowTime(const float nowTime);
			void SetMaxTime(const float maxTime);
			void SetBezierManager(const Math::BezierManager& bezier);

			void SetIsReverse(const bool isReverse);

			void AddTime(const float time);

			float GetValue() const;

			bool IsFinish() const;

			void TimeReset();
		};
	}
}