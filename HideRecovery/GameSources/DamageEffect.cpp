/*!
@file DamageEffect.cpp
@brief DamageEffect�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "EffectBase.h"
#include "DamageEffect.h"

#include "BuilderVertexPCT.h"
#include "BoardObject.h"

#include "BillBoard.h"

#include "ParticleManager.h"

namespace basecross {

	namespace Effect {

		Damage::Damage(const std::shared_ptr<GameObject>& objPtr)
			:Base(objPtr)
		{}
		
		void Damage::OnCreate() {
			Base::OnCreate();

			//�f�[�^�̐���
			std::vector<ParticleData> datas = {
				CreateLineVData(),
				CretaePointData(),
			};

			CreateParticles(datas);
		}

		ParticleData Damage::CreateLineVData() {
			//�o�[�e�b�N�X�p�����[�^�̐���
			auto vertexPCTParam = Builder::VertexPCTParametor(Vec3(0.25f, 0.0f, 0.25f), 1.0f, Col4(1.0f, 0.0f, 0.0f, 1.0f), Vec2(512.0f), L"LineV_TX");

			//�p�[�e�B�N���p�����[�^�̐���
			auto particleParam = maru::ParticleManager::Parametor(vertexPCTParam);
			particleParam.powerRandomVec = Vec2(5.0f, 10.0f);                  //�����_���i�s�͂̕�
			particleParam.shotDirectRandomVec = Vec3(360.0f, 360.0f, 360.0f);  //�����_���i�s�����̕�
			particleParam.sizeRandomVec = Vec2(1.5f, 2.5f);                    //�����_���T�C�Y�̕�
			particleParam.liveTime = 0.6f;           //�p�[�e�B�N���̐�������
			particleParam.numOneFrameCreate = 25;    //1�t���[���ɐ������鐔
			particleParam.isLoop = false;            //���[�v���邩�ǂ���
			particleParam.isBillBoard = false;       //�r���{�[�h���邩�ǂ���
			particleParam.isForwardRotation = true;  //Shot������Forward��������B
			particleParam.isAlphaFade = true;        //AlphaFade�����邩�ǂ���

			return ParticleData(particleParam);
		}

		ParticleData Damage::CretaePointData() {
			//�o�[�e�b�N�X�p�����[�^�̐���
			auto vertexPCTParam = Builder::VertexPCTParametor(Vec3(0.25f, 0.0f, 0.25f), 1.0f, Col4(1.0f, 0.8f, 0.0f, 1.0f), Vec2(512.0f), L"LineV_TX");

			//�p�[�e�B�N���p�����[�^�̐���
			auto particleParam = maru::ParticleManager::Parametor(vertexPCTParam);
			particleParam.powerRandomVec = Vec2(0.5f, 3.0f);                    //�����_���i�s�͂̕�
			particleParam.shotDirectRandomVec = Vec3(360.0f, 360.0f, 360.0f);   //�����_���i�s�����̕�
			particleParam.sizeRandomVec = Vec2(1.0f, 2.0f);                     //�����_���T�C�Y�̕�
			particleParam.liveTime = 0.6f;            //�p�[�e�B�N���̐�������
			particleParam.numOneFrameCreate = 40;     //1�t���[���ɐ������鐔
			particleParam.isLoop = false;             //���[�v���邩�ǂ���
			particleParam.isBillBoard = false;        //�r���{�[�h���邩�ǂ���
			particleParam.isForwardRotation = true;   //Shot������Forward��������B
			particleParam.isAlphaFade = true;         //AlphaFade�����邩�ǂ���

			return ParticleData(particleParam);
		}
	}

}