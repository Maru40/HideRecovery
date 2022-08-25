/*!
@file GameStageBase.h
@brief GameStageBase
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	/// 前方宣言
	//--------------------------------------------------------------------------------------
	class PlayerObject;
	class MainCamera;
	class MainCameraObject;
	class StartCamera;
	class StageObjectBase;
	class SlimeRenderTarget;

	class GameStageBase : public Stage {
	protected:
		//--------------------------------------------------------------------------------------
		/// よく使われるオブジェクト
		//--------------------------------------------------------------------------------------

		ex_weak_ptr<PlayerObject> m_player;
		std::vector<std::shared_ptr<GameObject>> m_floors; // 床に対応するオブジェクト

		//--------------------------------------------------------------------------------------
		/// カメラ
		//--------------------------------------------------------------------------------------

		//メインカメラ
		//std::shared_ptr<SingleView> m_mainView;                    //ビュー
		//std::shared_ptr<MainCamera> m_mainCamera;                  //カメラ
		//std::shared_ptr<MainCameraObject> m_mainCameraObject;      //オブジェクト
		std::shared_ptr<SingleView> m_mainView;						//ビュー
		std::shared_ptr<Camera> m_mainCamera;						//カメラ

		//スタートカメラ
		std::shared_ptr<SingleView> m_startView;					//ビュー
		std::shared_ptr<StartCamera> m_startCamera;					//カメラ

		//レンダーターゲット
		std::shared_ptr<SlimeRenderTarget> m_slimeRenderTarget;		//スライム用のレンダーターゲット

		/// <summary>
		/// メインカメラの生成
		/// </summary>
		virtual void CreateMainCamera();

		/// <summary>
		/// スタートカメラの生成
		/// </summary>
		/// <param name="stageName">ステージ名</param>
		virtual void CreateStartCamera(const wstring& stageName);

		/// <summary>
		/// マップの生成
		/// </summary>
		/// <param name="fileName">マップファイルの名前</param>
		virtual void CreateMap(const wstring& fileName);

		/// <summary>
		/// UIの生成
		/// </summary>
		/// <param name="fileName">UIレイアウトファイルの名前</param>
		virtual void CreateUI(const wstring& fileName);

		/// <summary>
		/// 外側のコリジョンを複数生成する関数
		/// </summary>
		/// <param name="objects">生成するベースとなる床オブジェクト</param>
		template<class T>
		void CreateMapOutCollisions(const std::vector<std::shared_ptr<T>>& objects) {
			for (const auto& object : objects) {
				auto objectTrans = object->GetComponent<Transform>();
				auto position = objectTrans->GetPosition();
				auto scale = objectTrans->GetScale();
				auto halfScale = scale * 0.5f;

				constexpr float Width = 0.5f;
				//奥行生成
				auto forwardPosition = position + (Vec3::Forward() * halfScale.z);
				CreateMapOutCollision(forwardPosition, Vec3::Forward(), scale.x, Width);

				//手前側生成
				auto backPosition = position + (-Vec3::Forward() * halfScale.z);
				CreateMapOutCollision(backPosition, -Vec3::Forward(), scale.x, Width);

				//右側
				auto rightPosition = position + (Vec3::Right() * halfScale.x);
				CreateMapOutCollision(rightPosition, Vec3::Right(), scale.z, Width);

				//左側
				auto leftPosition = position + (-Vec3::Right() * halfScale.x);
				CreateMapOutCollision(leftPosition, -Vec3::Right(), scale.z, Width);
			}
		}

		/// <summary>
		/// 外側のコリジョンを生成する
		/// </summary>
		/// <param name="startPosition">生成スタート場所</param>
		/// <param name="forward">向き</param>
		/// <param name="length">長さ</param>
		/// <param name="width">横幅</param>
		/// <param name="height">高さ</param>
		void CreateMapOutCollision(const Vec3& startPosition, const Vec3& forward, const float& length, const float& width, const float& height = 15.0f);

	public:
		GameStageBase();
		virtual ~GameStageBase() {}

		virtual void OnCreate() override;
		virtual void RenderStage() override;

		//--------------------------------------------------------------------------------------
		/// アクセッサ
		//--------------------------------------------------------------------------------------

		/// <summary>
		/// プレイヤーの取得
		/// </summary>
		/// <returns>プレイヤー</returns>
		std::shared_ptr<PlayerObject> GetPlayer() const;

		/// <summary>
		/// カメラの取得
		/// </summary>
		template<class T>
		std::shared_ptr<T> GetCamera() const
		{
			auto view = GetView();
			auto camera = view->GetTargetCamera();
			auto tCam = dynamic_pointer_cast<T>(camera);
			return tCam;
		}

		/// <summary>
		/// メインカメラに切り替え
		/// </summary>
		/// <returns>メインカメラ</returns>
		std::shared_ptr<Camera> ChangeMainCamera();

		/// <summary>
		/// メインカメラオブジェクトの取得
		/// </summary>
		/// <returns>メインカメラオブジェクト</returns>
		//std::shared_ptr<MainCameraObject> GetMainCameraObject() const;

		/// <summary>
		/// スタートカメラに切り替え
		/// </summary>
		/// <returns>スタートカメラ</returns>
		std::shared_ptr<StartCamera> ChangeStartCamera();

		/// <summary>
		/// 床に対応するオブジェクトを追加
		/// </summary>
		/// <param name="obj">オブジェクト</param>
		void AddFloorObject(std::shared_ptr<GameObject> obj) {
			m_floors.push_back(obj);
		}
	};
}
//end basecross