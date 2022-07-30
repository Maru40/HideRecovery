/*!
@file CollisionObject.h
@brief CollisionObject
担当：丸山裕喜
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	前方宣言
	//--------------------------------------------------------------------------------------
	struct PositionScaleRotation;

	//--------------------------------------------------------------------------------------
	///	衝突オブジェクト管理パラメータ
	//--------------------------------------------------------------------------------------
	struct CollisionObjectManager_Parametor
	{
		wstring objName;    //オブジェクトネーム
		wstring folderName; //フォルダーネーム
		wstring fileName;   //ファイルネーム
		Vec3 baseScale;     //基本スケール
	};

	//--------------------------------------------------------------------------------------
	///	衝突オブジェクト管理
	//--------------------------------------------------------------------------------------
	class CollisionObjectManager : public Component
	{
	public:
		using Parametor = CollisionObjectManager_Parametor;

	private:
		Parametor m_param;								//パラメータ

		std::vector<ex_weak_ptr<GameObject>> m_objects; //生成したオブジェクトの配列

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="objPtr">このクラスを所有するゲームオブジェクト</param>
		/// <param name="parametor">パラメータ</param>
		CollisionObjectManager(const std::shared_ptr<GameObject>& objPtr, const Parametor& parametor);

		void OnCreate() override;
		void OnUpdate() override;

	private:
		/// <summary>
		/// 衝突オブジェクトの生成
		/// </summary>
		/// <param name="baseScale">基本の大きさ</param>
		/// <param name="offsetData">オフセットデータ</param>
		/// <returns>生成した衝突オブジェクト</returns>
		std::shared_ptr<GameObject> CreateCollisionObject(const Vec3& baseScale, const PositionScaleRotation& offsetData);

		/// <summary>
		/// 衝突オブジェクトの削除
		/// </summary>
		void RemoveCollisionObject();

	public:
		//--------------------------------------------------------------------------------------
		///	アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// コリジョンのデバッグ表示のOn,Off
		/// </summary>
		/// <param name="isDrawActive">表示するかどうか</param>
		void SetCollisionDrawActive(const bool isDrawActive);

		/// <summary>
		/// コリジョンのUpdateの切り替え
		/// </summary>
		/// <param name="isUpdateActive">更新するかどうか</param>
		void SetCollisionUpdateActive(const bool isUpdateActive);

		/// <summary>
		/// 全てのコリジョンオブジェクトを取得
		/// </summary>
		/// <returns>全てのコリジョンオブジェクト</returns>
		std::vector<ex_weak_ptr<GameObject>> GetCollisionObjects() const;

		/// <summary>
		/// コリジョンオブジェクトのセッティング
		/// </summary>
		/// <param name="parametor">セッティング用パラメータ</param>
		void SettingCollisionObject(const Parametor& parametor);
	};

}