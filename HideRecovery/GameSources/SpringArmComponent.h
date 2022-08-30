#pragma once

namespace basecross
{

	class SpringArmComponent : public Component
	{
	public:
		struct Parametor
		{
			float armRange;		//アームの長さ
			Vec3  armVec;		//アームの方向

			float radY;			//y軸の角度
			float radXZ;		//xz軸の角度

			float speedY;		//y軸の速度
			float speedXZ;		//xz軸の速度

			float maxY;			//y軸の最大
			float minY;			//y軸の最少

			float lerpSpeed;	//補間移動速度

			float armSpeed;		//アームの縮小スピード
			float currentArmRange; //現在のアームの長さ

			/// <summary>
			/// コンストラクタ
			/// </summary>
			Parametor();

			/// <summary>
			/// コンストラクタ
			/// </summary>
			/// <param name="armRange">アームの長さ</param>
			/// <param name="armVec">アームの方向</param>
			/// <param name="radY">y軸の角度</param>
			/// <param name="radXZ">zy軸の角度</param>
			/// <param name="speedY">y軸の速度</param>
			/// <param name="speedXZ">xz軸の速度</param>
			/// <param name="maxY">y軸の最大角度</param>
			/// <param name="minY">z軸の最大角度</param>
			/// <param name="lerpSpeed">補完移動速度</param>
			/// <param name="seekType">追従タイプ</param>
			Parametor(const float& armRange,
				const Vec3& armVec,
				const float& radY,
				const float& radXZ,
				const float& speedY,
				const float& speedXZ,
				const float& maxY,
				const float& minY,
				const float& lerpSpeed);
		};

	private:
		Parametor m_param = Parametor();

		ex_weak_ptr<GameObject> m_childObject;

		ex_weak_ptr<Transform> m_childTransform;

		std::weak_ptr<GameObject> m_player;

		float m_armRange = 10.0f;
		Vec3 m_defaultCenterOffset = Vec3(0.0f);

		std::vector<std::wstring> m_hitTags;

		Vec3 m_direction;
	public:
		SpringArmComponent(std::shared_ptr<GameObject>& owner);

		void SetArmRange(const float armRange);

		float GetArmRange() const;

		void AddHitTag(const std::wstring& hitTag);

		void RemoveHitTag(const std::wstring& hitTag);

		void SetChildObject(const std::shared_ptr<GameObject>& childObject);

		std::shared_ptr<GameObject> GetChildObject() const { return m_childObject.GetShard(); }

		void OnCreate() override;
		void OnUpdate2() override;

		void SetCurrentArmRange(const float& range) { m_param.currentArmRange = range; }

		//float GetArmRange() const { return m_param.armRange; }

		void SetRadXZ(const float rad) noexcept {
			m_param.radXZ = rad;
			m_param.armVec.x = cosf(m_param.radXZ);
			m_param.armVec.z = sinf(m_param.radXZ);

			if (abs(m_param.radXZ) >= XM_2PI) {
				//一回転したら0に戻す
				m_param.radXZ = 0.0f;
			}
		}

		void SetSpeedXZ(const float speed) { m_param.speedXZ = speed; }
		float GetSpeedXZ() const { return m_param.speedXZ; }

		float GetRadXZ() const noexcept { return m_param.radXZ; }

		void SetPlayer(const std::shared_ptr<GameObject>& player) { m_player = player; }

	private:
		Vec3 CalculateDirect();
		Vec3 CalculatePosition(const float& targetLength);
		float CalculateArmRange(const float& targetRange);

		void InputYVec();
		void InputRXVec();
		void CheckRimitY();

		void CheckLookAt(const float armRange);
	};
}