#pragma once

namespace basecross
{
	struct CameraBlendData
	{
		Vec3 position;
		Vec3 rotation;

		CameraBlendData();
		CameraBlendData(const Vec3& position, const Vec3& rotation);
	};

	class I_CameraBlend
	{
	public:

		virtual CameraBlendData GetCameraBlendData(const CameraBlendData& startData, const CameraBlendData& endData, const float time) const = 0;
	};

	class CameraBlendCut : public I_CameraBlend
	{
	public:

		CameraBlendData GetCameraBlendData(const CameraBlendData& startData, const CameraBlendData& endData, const float time) const override;
	};

	class CameraBlendLinear : public I_CameraBlend
	{
		const float m_linearTime;
	public:
		CameraBlendLinear(const float linearTime);

		CameraBlendData GetCameraBlendData(const CameraBlendData& startData, const CameraBlendData& endData, const float time) const override;
	};

	class VirtualCamera;

	class CameraBrain : public Component
	{
		ex_weak_ptr<Camera> m_camera;

		std::map<int, ex_weak_ptr<VirtualCamera>> m_virtualCameraMap;

		std::shared_ptr<I_CameraBlend> m_cameraBlend = std::make_shared<CameraBlendCut>();

		ex_weak_ptr<VirtualCamera> m_startCamera;
		ex_weak_ptr<VirtualCamera> m_endCamera;

		CameraBlendData m_startCameraData;
		CameraBlendData m_endCameraData;

		bool m_isChanged = false;

		float m_timer = 0.0f;

		std::shared_ptr<VirtualCamera> GetMaximunPriorityVirtualCamera() const;

	public:
		CameraBrain(std::shared_ptr<GameObject>& owner);

		void AddVirtualCamera(const std::shared_ptr<VirtualCamera>& virtualCamera);

		void RemoveVirtualCamera(const std::shared_ptr<VirtualCamera>& virtualCamera);

		void ChangeVirtualCamera();

		void SetCameraBlend(const std::shared_ptr<I_CameraBlend>& cameraBlend);

		void OnStart() override;

		void OnUpdate2() override;
	};

	class VirtualCamera : public Component
	{
		ex_weak_ptr<CameraBrain> m_cameraBrain;

		int m_priority;
	public:
		VirtualCamera(std::shared_ptr<GameObject>& owner, const int priority);

		int GetPriority() const;

		CameraBlendData GetCameraBlendData() const;

		void OnCreate() override;

		void OnEnable() override;

		void OnDisable() override;

		void OnDestroy() override;
	};

	/// <summary>
	/// カメラ用コンポーネント
	/// </summary>
	class CameraComponent : public Component
	{
		/// <summary>
		/// カメラのポインタ
		/// </summary>
		ex_weak_ptr<Camera> m_camera;

		/// <summary>
		/// カメラの視点を常に前にする
		/// </summary>
		void SetCameraAtForward();

		/// <summary>
		/// 今ターゲットにされているかどうか
		/// </summary>
		bool m_isTarget = false;
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="owner">ゲームオブジェクト</param>
		CameraComponent(std::shared_ptr<GameObject>& owner);

		void OnCreate() override;

		void OnUpdate2() override;

		void OnEnable() override;

		void OnDisable() override;

	};

	class CameraObject : public GameObject
	{
		ex_weak_ptr<CameraComponent> m_camera;
		ex_weak_ptr<CameraBrain> m_cameraBrain;

	public:
		CameraObject(std::shared_ptr<Stage>& stage);

		void OnCreate() override;
	};
}