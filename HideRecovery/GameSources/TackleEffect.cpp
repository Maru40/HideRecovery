/*!
@file TackleEffect.cpp
@brief TackleEffect�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "EffectBase.h"
#include "TackleEffect.h"

#include "AuraBase.h"
#include "PlayerInputer.h"

namespace basecross {

	namespace Effect {

		Tackle::Tackle(const std::shared_ptr<GameObject>& objPtr)
			:Base(objPtr)
		{}

		void Tackle::OnCreate() {
			std::vector<CreateAuraData> datas = {
				CreateDefaultAura(),
				CreateMoveAura(),
			};

			CreateAuraObjects(datas);
			SetEffectsDrawActive(false);
		}

		CreateAuraData Tackle::CreateDefaultAura() {
			auto param = Aura_Parametor();

			param.sides = 30;        //���_�̐�(��������Y��ȃ��b�V��)
			param.height = 0.6f;     //����
			param.topRadius = 0.0f;  //��̔��a
			param.topColor = Col4(0.0f, 0.5f, 0.0f, 0.5f); //��̐F
			param.bottomRadius = 2.5f;  //���̔��a
			param.bottomColor = Col4(0.0f, 0.5f, 0.0f, 0.0f); //���̐F
			param.texture = L"SquareSprite_TX";  //�e�N�X�`���l�[��
			param.updateParam.speed = 0.0f; //UV�X�s�[�h
			param.updateParam.direct = Vec2(0.0f, -1.0f); //UVMove����

			//�I�t�Z�b�g�f�[�^
			OffsetData offsetData;
			offsetData.position = Vec3(0.0f, 0.5f, 1.0f);
			offsetData.rotation = Vec3(XM_PIDIV2, 0.0f, 0.0f);
			offsetData.scale = Vec3(1.0f, 1.0f, 1.0f);

			return CreateAuraData(param, offsetData);
		}

		CreateAuraData Tackle::CreateMoveAura() {
			auto param = Aura_Parametor();

			param.sides = 30;        //���_�̐�(��������Y��ȃ��b�V��)
			param.height = 0.6f;     //����
			param.topRadius = 0.0f;  //��̔��a
			param.topColor = Col4(0.0f, 0.5f, 0.0f, 1.0f); //��̐F
			param.bottomRadius = 2.5f;  //���̔��a
			param.bottomColor = Col4(0.0f, 0.5f, 0.0f, 0.0f); //���̐F
			param.texture = L"LineH_TX";  //�e�N�X�`���l�[��
			param.updateParam.speed = 1.0f; //UV�X�s�[�h
			param.updateParam.direct = Vec2(0.0f, -1.0f); //UVMove����

			//�I�t�Z�b�g�f�[�^
			OffsetData offsetData;
			offsetData.position = Vec3(0.0f, 0.5f, 1.0f);
			offsetData.rotation = Vec3(XM_PIDIV2, 0.0f, 0.0f);
			offsetData.scale = Vec3(1.0f, 1.0f, 1.0f);

			return CreateAuraData(param, offsetData);
		}

	}
}