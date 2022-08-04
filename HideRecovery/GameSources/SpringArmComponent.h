#pragma once

namespace basecross
{

	class SpringArmComponent : public Component
	{
	public:
		struct Parametor
		{
			float armRange;		//�A�[���̒���
			Vec3  armVec;		//�A�[���̕���

			float radY;			//y���̊p�x
			float radXZ;		//xz���̊p�x

			float speedY;		//y���̑��x
			float speedXZ;		//xz���̑��x

			float maxY;			//y���̍ő�
			float minY;			//y���̍ŏ�

			float lerpSpeed;	//��Ԉړ����x

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			Parametor();

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="armRange">�A�[���̒���</param>
			/// <param name="armVec">�A�[���̕���</param>
			/// <param name="radY">y���̊p�x</param>
			/// <param name="radXZ">zy���̊p�x</param>
			/// <param name="speedY">y���̑��x</param>
			/// <param name="speedXZ">xz���̑��x</param>
			/// <param name="maxY">y���̍ő�p�x</param>
			/// <param name="minY">z���̍ő�p�x</param>
			/// <param name="lerpSpeed">�⊮�ړ����x</param>
			/// <param name="seekType">�Ǐ]�^�C�v</param>
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