
/*!
@file Motion_KnockBack.h
@brief Motion_KnockBackなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	namespace Metaball {
		class MotionManager;

		namespace Task {
			struct SeekOffsetMove_Parametor;
		}
	}

	namespace Enemy {

		class EnemyBase;

		namespace Motion {

			//--------------------------------------------------------------------------------------
			/// ノックバックモーションのパラメータ
			//--------------------------------------------------------------------------------------
			struct KnockBack_Parametor {
				KnockBackData data;   //ノックバックデータ
				float range;          //広がる最大数。
				float transitionTime; //遷移時間
				float avoidRange;     //他のメタボールとの回避距離
				Vec3 maxRangeVec;     //広がる最大距離、Vec3版

				/// <summary>
				/// コンストラクタ
				/// </summary>
				KnockBack_Parametor();

				/// <summary>
				/// コンストラクタ
				/// </summary>
				/// <param name="range">広がる範囲</param>
				KnockBack_Parametor(const float& range);

				/// <summary>
				/// コンストラクタ
				/// </summary>
				/// <param name="data">ノックバックデータ</param>
				/// <param name="range">広がる範囲</param>
				KnockBack_Parametor(const KnockBackData& data, const float& range);

				/// <summary>
				/// 
				/// </summary>
				/// <param name="data">ノックバックデータ</param>
				/// <param name="range">広がる範囲</param>
				/// <param name="transitionTime">遷移時間</param>
				KnockBack_Parametor(const KnockBackData& data, const float& range, const float& transitionTime);
			};

			//--------------------------------------------------------------------------------------
			/// ノックバックモーション
			//--------------------------------------------------------------------------------------
			class KnockBack : public maru::MetaballMotionBase<EnemyBase>
			{
			public:
				using Parametor = KnockBack_Parametor;

			private:
				ex_weak_ptr<Parametor> m_paramPtr;   //パラメータ
				std::vector<Vec3> m_defaultOffsets;  //デフォルトのオフセット値

			public:
				/// <summary>
				/// コンストラクタ
				/// </summary>
				/// <param name="owner">このクラスを所有するクラス</param>
				/// <param name="paramPtr">パラメータ</param>
				KnockBack(const std::shared_ptr<EnemyBase>& owner, const std::shared_ptr<KnockBack::Parametor>& paramPtr);

			private:
				/// <summary>
				/// モーションパラメータを取得する。
				/// </summary>
				/// <returns>モーションパラメータ</returns>
				vector<Metaball::Task::SeekOffsetMove_Parametor> CalculateMotionParametors();

				/// <summary>
				/// 中心からオフセット方向の、回転を考慮した方向データを取得する。
				/// </summary>
				/// <param name="centerPosition">中心位置</param>
				/// <param name="forward">フォワード</param>
				/// <param name="offset">オフセット</param>
				/// <returns>中心から回転を考慮した方向データ</returns>
				Vec3 ConvertRotationCenterToOffset(const Vec3& centerPosition, const Vec3& forward, const Vec3& offset);

				/// <summary>
				/// 互いのメタボールの重なりを回避するベクトルを計算
				/// </summary>
				/// <param name="baseDirect">ベースとなる方向</param>
				/// <param name="currentIndex">現在のインデックス</param>
				/// <param name="offsets">オフセット群</param>
				/// <returns>互いのメタボールの回避するベクトル</returns>
				Vec3 CalculateAvoidVec(const Vec3& baseDirect, const int currentIndex, const std::vector<Vec3>& offsets);

				/// <summary>
				/// 潰すベクトルを返す。
				/// </summary>
				/// <returns>潰すベクトル</returns>
				Vec3 CalculatePressedVec();

				/// <summary>
				/// デフォルト値からどれだけの距離を動いたか返す。
				/// </summary>
				/// <returns>デフォルト値から動いた距離</returns>
				float GetRange() const;

				/// <summary>
				/// 吸収されたオブジェクトの数を取得
				/// </summary>
				/// <returns>吸収されたオブジェクトの数</returns>
				int GetNumAbsrobed() const;

				/// <summary>
				/// ノックバックで動く最大距離を取得
				/// </summary>
				/// <returns>ノックバックで動く最大距離</returns>
				Vec3 GetMaxRangeVec() const;

				/// <summary>
				/// 吸収されている状態かどうか
				/// </summary>
				/// <returns>吸収されていたらtrue</returns>
				bool IsAbsrobed() const;

				/// <summary>
				/// 吸収されているオブジェクトのオフセット群を取得
				/// </summary>
				/// <returns>吸収されているオブジェクトのオフセット群</returns>
				std::vector<Vec3> GetAbsorbedOffsets() const;
			};

		}

	}
}