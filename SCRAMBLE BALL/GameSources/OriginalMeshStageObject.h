/*!
@file OriginalMeshStageObejct.h
@brief OriginalMeshStageObejctクラス
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "StageObjectBase.h"
#include "CollisionObject.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	オリジナルメッシュオブジェクトのインターフェース
	//--------------------------------------------------------------------------------------
	class I_OriginalMeshObject {
	public:
		/// <summary>
		/// 衝突判定の設定
		/// </summary>
		/// <param name="isCollision">当たり判定を生成するかどうか</param>
		virtual void SettingCollisionObject(const bool isCollision) = 0;
	};

	//--------------------------------------------------------------------------------------
	///	オリジナルメッシュオブジェクト
	//--------------------------------------------------------------------------------------
	template<class DrawType>
	class OriginalMeshStageObject : public StageObjectBase, public I_OriginalMeshObject
	{
		Vec3 m_scaleOffset; //大きさのオフセット

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="stage">このクラスが所属するステージ</param>
		/// <param name="name">オブジェクト名</param>
		/// <param name="texture">テクスチャ名</param>
		OriginalMeshStageObject(const std::shared_ptr<Stage>& stage, const wstring& name, const wstring& texture)
			: StageObjectBase(stage, name, texture)
		{}

		void OnCreate() override {
			StageObjectBase::OnCreate();

			m_scaleOffset = Vec3(0.25f);
			//メッシュの調整用Mat
			Mat4x4 spanMat;
			spanMat.affineTransformation(
				Vec3(m_scaleOffset),
				Vec3(0.0f),
				Vec3(0.0f, XM_PI, 0.0f),
				Vec3(0.0f, 0.0f, 0.0f)
			);

			auto draw = AddComponent<DrawType>();
			auto name = GetName();
			draw->SetMeshResource(GetName());
			draw->SetMeshToTransformMatrix(spanMat);

			//影付け
			auto shadow = AddComponent<Shadowmap>();
			shadow->SetMeshResource(GetName());
			shadow->SetMeshToTransformMatrix(spanMat);

			auto offset = Vec3(1.0f / m_scaleOffset.x, 1.0f / m_scaleOffset.y, 1.0f / m_scaleOffset.z);
			transform->SetScale(Vec3(offset));

			SetAlphaActive(true);
			AddTag(L"T_Obstacle");
		}

	protected:
		/// <summary>
		/// 衝突判定生成パラメータの取得
		/// </summary>
		/// <param name="scale">大きさ</param>
		/// <returns>衝突判定生成パラメータ</returns>
		CollisionObjectManager_Parametor GetCollisionObjectParametor(const Vec3& scale) {
			auto param = CollisionObjectManager::Parametor();
			param.objName = m_name;
			param.folderName = L"MapCollisionOffsetData/";
			param.fileName = L"MapCollisionOffsetData.csv";
			param.baseScale = scale;

			return param;
		}

	public:
		/// <summary>
		/// 衝突判定をするかどうか設定
		/// </summary>
		/// <param name="isCollision">衝突判定をするかどうか</param>
		void SetIsCollision(const bool isCollision) {
			m_isCollision = isCollision;
		}

		/// <summary>
		/// 衝突判定オブジェクトの設定
		/// </summary>
		/// <param name="isCollision"></param>
		void SettingCollisionObject(const bool isCollision) override {
			auto offset = Vec3(1.0f / m_scaleOffset.x, 1.0f / m_scaleOffset.y, 1.0f / m_scaleOffset.z);
			auto scale = Vec3(transform->GetScale() * offset);
			transform->SetScale(scale);

			//コリジョンを生成するなら。
			if (isCollision) {
				auto param = GetCollisionObjectParametor(scale);
				auto collisionObjectManager = AddComponent<CollisionObjectManager>(param);
				collisionObjectManager->SettingCollisionObject(param);

				//collisionObjectManager->SetCollisionDrawActive(true);
			}
		}
	};

}

//endbasecross