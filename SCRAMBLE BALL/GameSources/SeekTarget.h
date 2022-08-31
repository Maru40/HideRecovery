/*!
@file SeekTarget.h
@brief SeekTargetLookなど
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	追従タイプ
	//--------------------------------------------------------------------------------------
	enum class SeekTarget_Type {
		Normal,                     //通常
		Lerp,                       //Lerp
		VelocitySeek,               //追従行動
		VelcoityArrive,             //到着行動
		VelocityNearSeekFarArrive,	//近くにいるときは追従、遠くにいるときは到着行動
	};

	//--------------------------------------------------------------------------------------
	///	追従パラメータ
	//--------------------------------------------------------------------------------------
	struct SeekTarget_Parametor {
		using SeekType = SeekTarget_Type;

		float speed;				//追従スピード
		float range;				//追従範囲
		Vec3 offset;				//追従オフセット
		SeekType seekType;			//追従タイプ
		bool isScaleParent;			//Scaleを親に合わせるかどうか。

		/// <summary>
		/// コンストラクタ
		/// </summary>
		SeekTarget_Parametor();

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="seekType">追従タイプ</param>
		SeekTarget_Parametor(const SeekType& seekType);

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="range">追従範囲</param>
		/// <param name="offset">追従オフセット</param>
		/// <param name="seekType">追従タイプ</param>
		SeekTarget_Parametor(const float& range, const Vec3& offset, const SeekType& seekType);

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="speed">追従スピード</param>
		/// <param name="range">追従範囲</param>
		/// <param name="offset">追従オフセット</param>
		/// <param name="seekType">追従タイプ</param>
		/// <param name="isScaleParent">追従オフセットに親オブジェクトのスケールを適応するかどうか</param>
		SeekTarget_Parametor(const float& speed, const float& range, const Vec3& offset, const SeekType& seekType, const bool isScaleParent = false);
	};

	//--------------------------------------------------------------------------------------
	///	ターゲットをターゲティングしたときの挙動を管理する
	//--------------------------------------------------------------------------------------
	class SeekTarget : public Component
	{
	public:
		using SeekType = SeekTarget_Type;
		using Parametor = SeekTarget_Parametor;

	private:
		Parametor m_param;                //パラメータ
	
		ex_weak_ptr<GameObject> m_target; //ターゲットのゲームオブジェクト

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		/// <param name="target">ターゲットのゲームオブジェクト</param>
		SeekTarget(const std::shared_ptr<GameObject>& objPtr, const std::shared_ptr<GameObject>& target);

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		/// <param name="target">ターゲットのゲームオブジェクト</param>
		/// <param name="parametor">パラメータ</param>
		SeekTarget(const std::shared_ptr<GameObject>& objPtr, const std::shared_ptr<GameObject>& target, const Parametor& parametor);

		void OnCreate() override;
		void OnUpdate() override;

	private:

		/// <summary>
		/// 計算したポジションを返す。
		/// </summary>
		/// <returns>計算したポジション</returns>
		Vec3 CalculatePosition();

		/// <summary>
		/// 追従アップデート
		/// </summary>
		void ChaseUpdate();

		/// <summary>
		/// 通常の追いかける処理
		/// </summary>
		/// <param name="basePosition">目的地</param>
		void NormalChase(const Vec3& basePosition);

		/// <summary>
		/// 目的地に補間を掛けた移動処理
		/// </summary>
		/// <param name="basePosition">目的地</param>
		void LerpChase(const Vec3& basePosition);

		/// <summary>
		/// 速度管理をする移動処理
		/// </summary>
		/// <param name="basePosition">目的地</param>
		void VelocityChase(const Vec3& basePosition);

		/// <summary>
		/// 追従先のオフセットを計算する
		/// </summary>
		/// <returns>追従先のオフセット</returns>
		Vec3 CalculateOffset() const;

	public:
		//--------------------------------------------------------------------------------------
		///	アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// ターゲットのセット
		/// </summary>
		/// <param name="target">ターゲットのゲームオブジェクト</param>
		/// <param name="isSetPositioin">すぐにターゲットの場所に合わせるかどうか</param>
		void SetTarget(const std::shared_ptr<GameObject>& target, bool isSetPositioin = false) {
			m_target = target;
			//すぐにターゲットのポジションに合わせる場合
			if (target && isSetPositioin) {
				transform->SetPosition(CalculatePosition());
			}
		}

		/// <summary>
		/// ターゲットの取得
		/// </summary>
		/// <returns>ターゲットのゲームオブジェクト</returns>
		std::shared_ptr<GameObject> GetTarget() const noexcept{ return m_target.GetShard(); }

		/// <summary>
		/// 追従タイプのセット
		/// </summary>
		/// <param name="type">追従タイプ</param>
		void SetSeekType(const SeekType& type) noexcept{ m_param.seekType = type; }

		/// <summary>
		/// 追従タイプの取得
		/// </summary>
		/// <returns>現在の追従タイプ</returns>
		SeekType GetSeekType() const noexcept{ return m_param.seekType; }

		/// <summary>
		/// パラメータのセット
		/// </summary>
		/// <param name="parametor">パラメータ</param>
		void SetParametor(const Parametor& parametor) noexcept { m_param = parametor; }

		/// <summary>
		/// パラメータの取得
		/// </summary>
		/// <returns>パラメータ</returns>
		Parametor GetParametor() const noexcept { return m_param; }

		/// <summary>
		/// オフセットのセット
		/// </summary>
		/// <param name="offset">オフセット値</param>
		void SetOffset(const Vec3& offset) noexcept{ m_param.offset = offset; }

		/// <summary>
		/// オフセットの取得
		/// </summary>
		/// <returns>オフセット値</returns>
		Vec3 GetOffset() const { return CalculateOffset(); }

		/// <summary>
		/// ローカルなオフセット値を取得
		/// </summary>
		/// <returns>ローカルなオフセット値</returns>
		Vec3 GetLocalOffset() const noexcept { return m_param.offset; }
		
		/// <summary>
		/// 追従範囲のセット
		/// </summary>
		/// <returns>追従範囲</returns>
		void SetRange(const float& range) noexcept { m_param.range = range; }

		/// <summary>
		/// 追従範囲の取得
		/// </summary>
		/// <returns>追従範囲</returns>
		float GetRange() const noexcept{ return m_param.range; }

		/// <summary>
		/// 移動スピードのセット
		/// </summary>
		/// <returns>移動スピード</returns>
		void SetSpeed(const float& speed) noexcept { m_param.speed = speed; }

		/// <summary>
		/// 移動スピードの取得
		/// </summary>
		/// <returns>移動スピード</returns>
		float GetSpeed() const noexcept { return m_param.speed; }

		/// <summary>
		/// ワールド座標を取得
		/// </summary>
		/// <returns>ワールド座標</returns>
		Vec3 GetWorldPosition() const {
			auto targetTrans = m_target->GetComponent<Transform>();

			Mat4x4 newMat;
			newMat.affineTransformation(
				transform->GetScale(),
				transform->GetPivot(),
				Quat::Identity(),
				m_param.offset
			);

			newMat *= targetTrans->GetWorldMatrix();
			auto position = newMat.transInMatrix();
			return position;
		}
	};

}

//endbasecross