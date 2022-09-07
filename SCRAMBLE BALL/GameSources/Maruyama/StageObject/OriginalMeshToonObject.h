/*!
@file OriginalMeshToonObject.h
@brief OriginalMeshToonObjectクラス
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

#include "StageObjectBase.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	オリジナルメッシュトゥーンオブジェクト
	//--------------------------------------------------------------------------------------
	class OriginalMeshToonObject : public StageObjectBase
	{
		bool m_isStart = false;    //開始処理をしたかどうか
		bool m_isCollision = true; //衝突判定を生成するかどうか
		Vec3 m_scaleOffset;        //大きさオフセット

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="stage">このクラスが所属するステージ</param>
		/// <param name="name">オブジェクト名</param>
		/// <param name="textrue">テクスチャ名</param>
		OriginalMeshToonObject(const std::shared_ptr<Stage>& stage, const wstring& name, const wstring& textrue);

		void OnCreate() override;
		void OnUpdate() override;

	private:
		/// <summary>
		/// 衝突判定オブジェクトを生成
		/// </summary>
		/// <param name="scale">大きさ</param>
		void CreateCollisionObject(const Vec3& scale);

	public:
		/// <summary>
		/// 衝突判定をとるかどうか設定する
		/// </summary>
		/// <param name="isCollision">衝突判定をとるかどうか</param>
		void SetIsCollision(const bool isCollision) {
			m_isCollision = isCollision;
		}

	};

}

//endbasecross