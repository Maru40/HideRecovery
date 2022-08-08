/*!
@file   EfkComponent.h
@brief  Effekseerを使用するためのコンポーネント
*/

#pragma once
#include "stdafx.h"
#include "EfkEffect.h"

namespace basecross {
	/**
 * @brief 位置、大きさ、回転の情報を持つ構造体
 */
	struct TransformData {
		Vec3 Position;
		Vec3 Scale;
		Vec3 Rotation;
		TransformData() {
			this->Position = Vec3(0.0f);
			this->Scale = Vec3(1.0f);
			this->Rotation = Vec3(0.0f);
		}
		TransformData(const Vec3& position) {
			this->Position = position;
			this->Scale = Vec3(1.0f);
			this->Rotation = Vec3(0.0f);
		}
		TransformData(const Vec3& position, const Vec3& scale) {
			this->Position = position;
			this->Scale = scale;
			this->Rotation = Vec3(0.0f);
		}
		TransformData(const Vec3& position, const Vec3& scale, const Vec3& rotation) {
			this->Position = position;
			this->Scale = scale;
			this->Rotation = rotation;
		}
		TransformData operator +(const TransformData& data) {
			this->Position += data.Position;
			this->Scale += data.Scale;
			this->Rotation += data.Rotation;
			return *this;
		}
	};

	/**
	 * @brief 空間タイプ
	 */
	enum class SpaceType {
		World,	// ワールド
		Local	// ローカル
	};
	/**
	 * @brief 座標指定
	 */
	enum class CoordinateType {
		Absolute,	// 絶対
		Relative	// 相対
	};
	/**
	 * @brief エフェクトコンポーネント
	 */
	class EfkComponent : public Component {
	private:
		// エフェクトデータ構造体
		struct EfkData {
			// エフェクトデータのポインタ
			Effekseer::EffectRef EffectData;
			// エフェクトハンドル
			Effekseer::Handle Handle;
			// 再生時のオフセットデータ
			TransformData Offset;
			// 前回のエフェクトを停止せずに再生を開始するか
			bool NoStopLastEffect;
			// 位置を同期するか
			bool IsSync;
			// ループさせるか
			bool IsLoop;
			EfkData()
				:EfkData(nullptr, TransformData(), false, false, false)
			{}
			EfkData(const Effekseer::EffectRef& data, const TransformData& offset, bool flg, bool syncFlg, bool loopFlg) {
				this->EffectData = data;
				this->Handle = -1;
				this->Offset = offset;
				this->NoStopLastEffect = flg;
				this->IsSync = syncFlg;
				this->IsLoop = loopFlg;
			}
		};
		// キー,エフェクトデータのマップ
		map<wstring, EfkData> m_effectDataMap;
		// マネージャーのポインタ
		Effekseer::ManagerRef m_manager;
		// 再生速度
		float m_playSpeed;
		// 使用する空間
		SpaceType m_useSpace;
		// 座標指定
		CoordinateType m_coordinateType;
		// デバッグモードか
		bool m_isDebugMode;
		/**
		 * @brief ゲームオブジェクトの位置を取得
		 *
		 * @return ゲームオブジェクトの位置
		 */
		Vec3 GetGameObjectPosition();
		/**
		 * @brief ゲームオブジェクトの回転を取得
		 *
		 * @return ゲームオブジェクトの回転
		 */
		Vec3 GetGameObjectRotation();
		/**
		 * @brief ゲームオブジェクトと同期する
		 */
		void SyncGameObject(const wstring& key);
	public:
		EfkComponent(const shared_ptr<GameObject>& gameObjectPtr);
		EfkComponent(const shared_ptr<GameObject>& gameObjectPtr, SpaceType useSpace);
		~EfkComponent() {}

		void OnCreate() override {}
		void OnUpdate() override;
		void OnDraw()override {}

		/**
		 * @brief 使用するエフェクトの設定
		 *
		 * @param key エフェクトのキー
		 * @param offset オフセットデータ
		 * @param noStopLastEffect 前回のエフェクトを停止せずに再生を開始するか
		 */
		void SetEffectResource(const wstring& key, const TransformData& offset, bool noStopLastEffect);
		/**
		 * @brief 使用するエフェクトの設定
		 *
		 * @param key エフェクトのキー
		 * @param offset オフセットデータ
		 */
		void SetEffectResource(const wstring& key, const TransformData& offset);
		/**
		 * @brief 使用するエフェクトの設定
		 *
		 * @param key エフェクトのキー
		 */
		void SetEffectResource(const wstring& key);

		/**
		 * @brief エフェクトの再生
		 *
		 * @param key エフェクトのキー
		 */
		void Play(const wstring& key);

		/**
		 * @brief エフェクトの再生
		 *
		 * @param key エフェクトのキー
		 * @param isOwnerRotation trueなら再生するエフェクトをオーナーの回転と同じにする
		 */
		void Play(const wstring& key, bool isOwnerRotation);
		/**
		 * @brief エフェクトの停止
		 *
		 * @param key エフェクトのキー
		 */
		void Stop(const wstring& key);
		/**
		 * @brief すべてのエフェクトを停止
		 */
		void StopAll();
		/**
		 * @brief エフェクトの一時停止（すでに一時停止している場合はそこから再生）
		 *
		 * @param key エフェクトのキー
		 */
		void Pause(const wstring& key);

		void SetRotation(const wstring& key, const Vec3& rotation);

		/**
		 * @brief 位置を同期するか
		 *
		 * @param key エフェクトのキー
		 */
		void IsSyncGameObject(const wstring& key, bool flg);

		void SetPosition(const wstring& key, const Vec3& position);

		/**
		 * @brief キーのエフェクトが再生中か
		 *
		 * @param key エフェクトのキー
		 * @return 再生している:true/再生していない:false
		 */
		bool IsPlaying(const wstring& key);

		void PlayLoop(const wstring& key);

		/**
		 * @brief コンポーネントに登録されているすべてのエフェクトが再生中か
		 *
		 * @return 再生している:true/再生していない:false
		 */
		bool IsPlaying();

		void SetCoordinateType(CoordinateType coordinateType);

		/**
		 * @brief デバッグモードの設定
		 *
		 * @param flg trueなら有効
		 */
		void SetDebugMode(bool flg);
	};
}
