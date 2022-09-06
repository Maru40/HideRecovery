#include"CameraHelper.h"

namespace basecross
{
	// CameraData ------------------------------

	CameraBlendData::CameraBlendData() :
		CameraBlendData(Vec3(), Vec3(0, 0, 1))
	{

	}

	CameraBlendData::CameraBlendData(const Vec3& position, const Vec3& rotation) :
		position(position),
		rotation(rotation)
	{

	}

	// CameraBlendCut --------------------------

	CameraBlendData CameraBlendCut::GetCameraBlendData(const CameraBlendData& startData, const CameraBlendData& endData, const float time) const
	{
		return endData;
	}

	// CameraBlendLinear -----------------------

	CameraBlendLinear::CameraBlendLinear(const float linearTime) :
		m_linearTime(linearTime)
	{

	}

	CameraBlendData CameraBlendLinear::GetCameraBlendData(const CameraBlendData& startData, const CameraBlendData& endData, const float time) const
	{
		if (time >= m_linearTime)
		{
			return endData;
		}

		CameraBlendData cameraBlendData;

		cameraBlendData.position = startData.position + (endData.position - startData.position) * time / m_linearTime;
		cameraBlendData.rotation = startData.rotation + (endData.rotation - startData.rotation) * time / m_linearTime;

		return cameraBlendData;
	}

	// CameraBrain -----------------------------

	CameraBrain::CameraBrain(std::shared_ptr<GameObject>& owner) :
		Component(owner)
	{

	}

	std::shared_ptr<VirtualCamera> CameraBrain::GetMaximunPriorityVirtualCamera() const
	{
		auto it = m_virtualCameraMap.rbegin();

		while (it != m_virtualCameraMap.rend())
		{
			auto& cameraDataPair = (*it);

			if (!cameraDataPair.second)
			{
				it++;
				continue;
			}

			if (!cameraDataPair.second->GetUpdateActive())
			{
				it++;
				continue;
			}

			return cameraDataPair.second.GetShard();
		}

		return nullptr;
	}

	void CameraBrain::AddVirtualCamera(const std::shared_ptr<VirtualCamera>& virtualCamera)
	{
		int priority = virtualCamera->GetPriority();

		auto it = m_virtualCameraMap.find(priority);

		if (it != m_virtualCameraMap.end())
		{
			std::wstringstream wss;
			wss << L"衝突した順位 " << priority;

			throw BaseException(
				L"カメラの優先順位は既に登録されています",
				wss.str().c_str(),
				L"CameraBrain::AddVirtualCamera"
			);

			return;
		}

		m_virtualCameraMap[priority] = virtualCamera;

		m_isChanged = true;
	}

	void CameraBrain::RemoveVirtualCamera(const std::shared_ptr<VirtualCamera>& virtualCamera)
	{
		auto it = m_virtualCameraMap.find(virtualCamera->GetPriority());

		if (it != m_virtualCameraMap.end())
		{
			m_virtualCameraMap.erase(it);

			m_isChanged = true;
		}
	}

	void CameraBrain::ChangeVirtualCamera()
	{
		m_isChanged = true;
	}

	void CameraBrain::SetCameraBlend(const std::shared_ptr<I_CameraBlend>& cameraBlend)
	{
		m_cameraBlend = cameraBlend;
	}

	void CameraBrain::OnStart()
	{
		m_camera = GetStage()->GetView()->GetTargetCamera();

		int i = 0;
	}

	void CameraBrain::OnUpdate2()
	{
		if (m_isChanged)
		{
			auto virtualCamera = GetMaximunPriorityVirtualCamera();

			if (m_endCamera != virtualCamera)
			{
				m_startCamera = m_endCamera;
				m_endCamera = virtualCamera;
				m_timer = 0.0f;
			}

			m_isChanged = false;
		}

		if (!m_endCamera)
		{
			return;
		}

		m_timer += App::GetApp()->GetElapsedTime();

		if (m_startCamera)
		{
			m_startCameraData = m_startCamera->GetCameraBlendData();
		}

		auto forward = transform->GetForward();

		auto cameraBlendData = m_cameraBlend->GetCameraBlendData(m_startCameraData, m_endCamera->GetCameraBlendData(), m_timer);

		transform->SetWorldPosition(cameraBlendData.position);
		transform->SetRotation(cameraBlendData.rotation);

		forward = transform->GetForward();

		int i = 0;
	}

	// VirtualCamera ---------------------------

	VirtualCamera::VirtualCamera(std::shared_ptr<GameObject>& owner, const int priority) :
		Component(owner),
		m_priority(priority)
	{

	}

	int VirtualCamera::GetPriority() const
	{
		return m_priority;
	}

	CameraBlendData VirtualCamera::GetCameraBlendData() const
	{
		return CameraBlendData(transform->GetWorldPosition(), transform->GetWorldQuaternion().toRotVec());
	}

	void VirtualCamera::OnCreate()
	{
		for (auto& gameobject : GetStage()->GetGameObjectVec())
		{
			auto cameraBrain = gameobject->GetComponent<CameraBrain>(false);

			if (!cameraBrain)
			{
				continue;
			}

			m_cameraBrain = cameraBrain;
			m_cameraBrain->AddVirtualCamera(GetThis<VirtualCamera>());

			return;
		}

		throw BaseException(
			L"ステージにまだCameraBrainがついたオブジェクトが存在しません",
			L"",
			L"VirtualCamera::OnStart()"
		);
	}

	void VirtualCamera::OnEnable()
	{
		m_cameraBrain->ChangeVirtualCamera();
	}

	void VirtualCamera::OnDisable()
	{
		m_cameraBrain->ChangeVirtualCamera();
	}

	void VirtualCamera::OnDestroy()
	{
		if (!m_cameraBrain)
		{
			return;
		}

		m_cameraBrain->RemoveVirtualCamera(GetThis<VirtualCamera>());
	}

	// CameraComponent ------------------------

	CameraComponent::CameraComponent(std::shared_ptr<GameObject>& owner) :
		Component(owner)
	{
	}

	void CameraComponent::SetCameraAtForward()
	{
		m_camera->SetAt(transform->GetWorldPosition() + transform->GetForward());
	}

	void CameraComponent::OnCreate()
	{
		m_camera = GetStage()->GetView()->GetTargetCamera();

		auto cameraObject = m_camera->GetCameraObject();

		if (cameraObject)
		{
			m_isTarget = false;
			return;
		}

		m_isTarget = true;

		SetCameraAtForward();

		m_camera->SetCameraObject(GetGameObject());
	}

	void CameraComponent::OnUpdate2()
	{
		if (!m_isTarget)
		{
			return;
		}

		SetCameraAtForward();
	}

	void CameraComponent::OnEnable()
	{
		m_isTarget = true;

		m_camera->SetCameraObject(GetGameObject());
	}

	void CameraComponent::OnDisable()
	{
		if (m_camera->GetCameraObject() != nullptr && m_camera->GetCameraObject() == GetGameObject())
		{
			m_isTarget = false;
			m_camera->SetCameraObject(nullptr);
		}
	}

	// CameraObject ------------------------------

	CameraObject::CameraObject(std::shared_ptr<Stage>& stage) :
		GameObject(stage)
	{

	}

	void CameraObject::OnCreate()
	{
		m_cameraBrain = AddComponent<CameraBrain>();
		m_camera = AddComponent<CameraComponent>();
	}
}