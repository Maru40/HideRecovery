
/*!
@file FieldImpactMap.h
@brief FieldImpactMap�Ȃ�
�S���F�ێR�T��
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	namespace maru {

		//--------------------------------------------------------------------------------------
		///	�O���錾
		//--------------------------------------------------------------------------------------
		class ImpactMap;

		//--------------------------------------------------------------------------------------
		///	�t�B�[���h�e���}�b�v�̃p�����[�^
		//--------------------------------------------------------------------------------------
		struct FieldImpactMap_Parametor {
			float intervalRange;        //�e���}�b�v�m�[�h�̊Ԋu
			float createHeightOffset;   //�������鍂���̃I�t�Z�b�g

			FieldImpactMap_Parametor();
		};

		//--------------------------------------------------------------------------------------
		///	�t�B�[���h�e���}�b�v
		//--------------------------------------------------------------------------------------
		class FieldImpactMap : public SingletonComponent<FieldImpactMap>
		{
		public:
			using Parametor = FieldImpactMap_Parametor;

		private:
			Parametor m_param; //�p�����[�^

			std::shared_ptr<ImpactMap> m_impactMap = nullptr; //�e���}�b�v�{��
			std::vector<std::weak_ptr<GameObject>> m_floors;  //�e���}�b�v�W�J���鏰�f�[�^�z��

		public:
			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
			/// <param name="floors">�e���}�b�v��W�J���鏰�f�[�^�z��</param>
			FieldImpactMap(const std::shared_ptr<GameObject>& objPtr, const std::vector<std::shared_ptr<GameObject>>& floors);

			/// <summary>
			/// �R���X�g���N�^
			/// </summary>
			/// <param name="objPtr">���̃N���X�����L����Q�[���I�u�W�F�N�g</param>
			/// <param name="floors">�e���}�b�v��W�J���鏰�f�[�^�z��</param>
			/// <param name="parametor"></param>
			FieldImpactMap(const std::shared_ptr<GameObject>& objPtr, const std::vector<std::shared_ptr<GameObject>>& floors, const Parametor& parametor);
			
			void OnLateStart() override;
			void OnUpdate() override;

		public:

			/// <summary>
			/// �����������[�g��S�Ď擾����B
			/// </summary>
			/// <param name="selfPosition">�������g�̃|�W�V����</param>
			/// <param name="targetPosition">�ړI�n</param>
			/// <returns>���������|�W�V�����̔z��</returns>
			std::vector<Vec3> GetRoutePositons(const Vec3& selfPosition, const Vec3& targetPosition);

			/// <summary>
			/// �e���}�b�v�f�[�^���擾
			/// </summary>
			/// <returns>�e���}�b�v�f�[�^</returns>
			std::shared_ptr<ImpactMap> GetImpactMap() const;

		private:
			/// <summary>
			/// �e���}�b�v�̃Z�b�e�B���O
			/// </summary>
			/// <param name="objects">�e���}�b�v�𐶐�����x�[�X�ƂȂ鏰�I�u�W�F�N�g</param>
			/// <param name="intervalRange">���ꂼ��̃m�[�h�̊Ԋu����</param>
			template<class T>
			void SettingImpactMap(const vector<std::weak_ptr<T>>& objects, const float intervalRange) {
				for (const auto& weakObject : objects) {
					constexpr float WallWidth = 1.5f;

					auto object = weakObject.lock();
					auto objectTrans = object->GetComponent<Transform>();

					//�ʒu�ݒ�
					auto position = objectTrans->GetPosition();
					position += Vec3::Right()	* WallWidth;
					position += Vec3::Forward() * WallWidth;

					//�傫���ݒ�
					auto scale = objectTrans->GetScale();
					scale.x += -WallWidth;
					scale.z += -WallWidth;

					auto parametor = maru::ImpactMap::Factory_Parametor();
					parametor.rect = maru::Rect(position, scale.x, scale.z);
					parametor.intervalRange = intervalRange;
					parametor.createHeight = m_param.createHeightOffset;
					
					m_impactMap->AddImpactData(parametor);
				}
			}

		};

	}
}