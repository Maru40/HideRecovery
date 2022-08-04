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

		float m_armRange = 10.0f;

		std::vector<std::wstring> m_hitTags;

		Vec3 m_direction;
	public:
		SpringArmComponent(std::shared_ptr<GameObject>& owner);

		void SetArmRange(const float armRange);

		float GetArmRange() const;

		void AddHitTag(const std::wstring& hitTag);

		void RemoveHitTag(const std::wstring& hitTag);

		void SetChildObject(const std::shared_ptr<GameObject>& childObject);

		void OnCreate() override;
		void OnUpdate2() override;

	private:
		Vec3 CalculateDirect();

		void InputYVec();
		void InputRXVec();
		void CheckRimitY();
	};
}