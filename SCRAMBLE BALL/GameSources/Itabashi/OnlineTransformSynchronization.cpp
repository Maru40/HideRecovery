#include "stdafx.h"
#include "OnlineTransformSynchronization.h"
#include <float.h>

struct UseDataFlags
{
	bool a;
	bool b;
};

enum class OnlineTransformDataType : std::uint8_t
{
	None         = 0b00000000,
	UsePosition  = 0b00000001,
	UseRotation  = 0b00000010
};

template <class T, std::enable_if_t<std::is_enum_v<T>, std::nullptr_t> = nullptr>
T operator|(T left, T right)
{
	return static_cast<T>(static_cast<std::underlying_type_t<T>>(left) | static_cast<std::underlying_type_t<T>>(right));
}

template <class T, std::enable_if_t<std::is_enum_v<T>, std::nullptr_t> = nullptr>
T& operator|=(T& left, T right)
{
	return left = left | right;
}

template <class T, std::enable_if_t<std::is_enum_v<T>, std::nullptr_t> = nullptr>
T operator&(T L, T R)
{
	return static_cast<T>(static_cast<std::underlying_type_t<T>>(L) & static_cast<std::underlying_type_t<T>>(R));
}

template <class T, std::enable_if_t<std::is_enum_v<T>, std::nullptr_t> = nullptr>
bool HasEnumerator(T left, T right)
{
	return (left & right) == right;
}

template <class T>
void AddByteData(std::vector<std::uint8_t>& data, int& offset, const T& addData)
{
	data.resize(data.size() + sizeof(T));
	std::memcpy(data.data() + offset, &addData, sizeof(T));
	offset += sizeof(T);
}

namespace basecross
{
namespace Online
{
	bool OnlineTransformSynchronization::OnlineTransformData::operator==(const OnlineTransformData& other) const
	{
		return position == other.position && rotation == other.rotation;
	}

	OnlineTransformSynchronization::OnlineTransformSynchronization(const std::shared_ptr<GameObject>& owner) :
		OnlineComponent(owner)
	{
	}

	std::vector<std::uint8_t> OnlineTransformSynchronization::CreateTransformDeltaCompressionData() const
	{
		int offset = 0;

		std::vector<std::uint8_t> data;
		AddByteData(data, offset, m_onlinePlayerNumber);
		
		auto transform = m_transform.lock();

		auto position = transform->GetWorldPosition();
		auto rotation = transform->GetWorldQuaternion();

		OnlineTransformDataType dataType = OnlineTransformDataType::None;

		if (m_isSynchronizePosition && m_beforeData.position != position)
		{
			dataType |= OnlineTransformDataType::UsePosition;
			m_beforeData.position = position;
		}

		if (m_isSynchronizeRotation && m_beforeData.rotation != rotation)
		{
			dataType |= OnlineTransformDataType::UseRotation;
			m_beforeData.rotation = rotation;
		}

		AddByteData(data, offset, dataType);

		if (HasEnumerator(dataType, OnlineTransformDataType::UsePosition))
		{
			AddByteData(data, offset, position);
		}

		if (HasEnumerator(dataType, OnlineTransformDataType::UseRotation))
		{
			AddByteData(data, offset, rotation);
		}

		return data;
	}

	void OnlineTransformSynchronization::UpdateTransformData(const std::uint8_t* bytes)
	{
		auto transform = m_transform.lock();

		OnlineTransformDataType dataType = *reinterpret_cast<const OnlineTransformDataType*>(bytes);

		bytes += sizeof(OnlineTransformDataType);

		if ((dataType & OnlineTransformDataType::UsePosition) == OnlineTransformDataType::UsePosition)
		{
			Vec3 position = *reinterpret_cast<const Vec3*>(bytes);
			transform->SetWorldPosition(position);
			m_beforeData.position = position;
			bytes += sizeof(Vec3);
		}

		if ((dataType & OnlineTransformDataType::UseRotation) == OnlineTransformDataType::UseRotation)
		{
			Quat rotation = *reinterpret_cast<const Quat*>(bytes);
			transform->SetWorldQuaternion(rotation);
			m_beforeData.rotation = rotation;
			bytes += sizeof(Quat);
		}
	}

	void OnlineTransformSynchronization::OnCreate()
	{
		m_transform = GetGameObject()->GetComponent<Transform>();
	}

	void OnlineTransformSynchronization::OnUpdate2()
	{
		m_countSpan += App::GetApp()->GetElapsedTime();

		if (m_countSpan <= m_synchronizeSpan)
		{
			return;
		}

		m_countSpan -= m_synchronizeSpan;

		auto transform = m_transform.lock();

		if (!transform || !m_isMaster || m_onlinePlayerNumber == OnlineManager::INVALID_ONLINE_PLAYER_NUMBER)
		{
			return;
		}

		auto data = CreateTransformDeltaCompressionData();

		OnlineManager::RaiseEvent(false, data.data(), static_cast<int>(data.size()), SYNCHRONIZE_TRANSFORM_ONLINE_EVENT_CODE);
	}

	void OnlineTransformSynchronization::OnCustomEventAction(int playerNumber, std::uint8_t eventCode, const std::uint8_t* bytes)
	{
		if (eventCode != SYNCHRONIZE_TRANSFORM_ONLINE_EVENT_CODE)
		{
			return;
		}

		int otherNumber = *reinterpret_cast<const int*>(bytes);

		if (m_onlinePlayerNumber != otherNumber)
		{
			return;
		}

		bytes += sizeof(int);

		UpdateTransformData(bytes);
	}

}
}