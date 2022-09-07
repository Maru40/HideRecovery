
/*!
@file EffectManager.cpp
@brief EffectManager�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "Maruyama/Utility/SingletonComponent/SingletonComponent.h"
#include "EffectManager.h"

#include "EffectBase.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�p�[�e�B�N�������p�̑O���錾�֐�
	//--------------------------------------------------------------------------------------

	/// <summary>
	/// �X���C�����ׂꂽ���̃p�[�e�B�N���f�[�^
	/// </summary>
	Effect::ParticleData GetSlimeCrushData();
	/// <summary>
	/// �W�F�b�g�X�p�[�N�f�[�^
	/// </summary>
	Effect::ParticleData GetJetSpakeData();


	//--------------------------------------------------------------------------------------
	///	�G�t�F�N�g�Ǘ��N���X
	//--------------------------------------------------------------------------------------

	std::weak_ptr<EffectManager>  maru::SingletonComponent<EffectManager>::sm_instance;

	EffectManager::EffectManager(const std::shared_ptr<GameObject>& objPtr)
		:SingletonComponent(objPtr)
	{}

	void EffectManager::OnCreate() {
		SettingEffectMap();
	}

	void EffectManager::SettingEffectMap() {
		//m_effectMap[ID::TackleDamage] = [&](const Vec3& position, const Quat& quat) { return CreateEffect<Effect::Damage>(position, quat); };
		//m_effectMap[ID::JetSpake] = [&](const Vec3& position, const Quat& quat) { return CreatePrticle(GetJetSpakeData(), position, quat); };
		//m_effectMap[ID::Vernier] = [&](const Vec3& position, const Quat& quat) { return CreatePrticles(GetVernierData(), position, quat); };
		//m_effectMap[ID::SlimeCrush] = [&](const Vec3& position, const Quat& quat) { return CreatePrticle(GetSlimeCrushData(), position, quat); };
	}

	std::shared_ptr<GameObject> EffectManager::Play(const ID id, const Vec3& position, const Quat& quat) {
		if (!m_effectMap.count(id)) {  //particle�����݂��Ȃ��Ȃ�
			DebugObject::AddString(L"id�����݂��܂���");
			return nullptr;
		}

		auto& func = m_effectMap[id];
		return func ? func(position, quat) : nullptr;
	}


	std::shared_ptr<GameObject> EffectManager::CreatePrticle(const Effect::ParticleData& data, const Vec3& position, const Quat& quat) {
		auto object = GetStage()->Instantiate<GameObject>(position, quat);
		auto particleManager = object->AddComponent<maru::ParticleManager>(data.particleParam);

		return object;
	}

	std::shared_ptr<GameObject> EffectManager::CreatePrticles(const std::vector<Effect::ParticleData>& data, const Vec3& position, const Quat& quat) {
		auto object = GetStage()->Instantiate<GameObject>(position, quat);
		auto effect = object->AddComponent<Effect::Base>();
		effect->CreateParticles(data);

		return object;
	}


	//--------------------------------------------------------------------------------------
	///	�p�[�e�B�N���̐����p�f�[�^�擾�֐�
	//--------------------------------------------------------------------------------------

	//�X���C�����ׂ��Ƃ��̃p�[�e�B�N���f�[�^----------------------------------------------

	Effect::ParticleData GetSlimeCrushData() {
		//�o�[�e�b�N�X�p�����[�^�̐���
		auto vertexPCTParam =
			Builder::VertexPCTParametor(Vec3(0.5f, 0.5f, 0.0f), 1.0f, Col4(0.1f, 0.9f, 0.1f, 1.0f), Vec2(512.0f), L"Point_TX");

		//�p�[�e�B�N���p�����[�^�̐���
		auto particleParam = maru::ParticleManager::Parametor(vertexPCTParam);
		particleParam.powerRandomVec = Vec2(2.0f, 2.0f);                   //�����_���i�s�͂̕�
		particleParam.shotDirectRandomVec = Vec3(360.0f, 360.0f, 360.0f);      //�����_���i�s�����̕�
		particleParam.sizeRandomVec = Vec2(0.5f, 1.0f);                    //�����_���T�C�Y�̕�
		particleParam.liveTime = 0.5f;            //�p�[�e�B�N���̐�������
		particleParam.numOneFrameCreate = 20;     //1�t���[���ɐ������鐔
		particleParam.isLoop = false;             //���[�v���邩�ǂ���
		particleParam.isBillBoard = true;         //�r���{�[�h���邩�ǂ���
		particleParam.isForwardRotation = false;  //Shot������Forward��������B
		particleParam.isAlphaFade = true;         //AlphaFade�����邩�ǂ���

		return Effect::ParticleData(particleParam);
	}

	//--------------------------------------------------------------------------------------

	//�W�F�b�g�X�p�[�N�f�[�^----------------------------------------------------------------

	Effect::ParticleData GetJetSpakeData() {
		//�o�[�e�b�N�X�p�����[�^�̐���
		auto vertexPCTParam = 
			Builder::VertexPCTParametor(Vec3(0.25f, 0.0f, 0.25f), 1.0f, Col4(1.0f, 0.5f, 0.5f, 1.0f), Vec2(512.0f), L"LineV_TX");

		//�p�[�e�B�N���p�����[�^�̐���
		auto particleParam = maru::ParticleManager::Parametor(vertexPCTParam);
		particleParam.powerRandomVec = Vec2(2.0f, 6.0f);                  //�����_���i�s�͂̕�
		particleParam.shotDirectRandomVec = Vec3(45.0f, 0.0f, 45.0f);  //�����_���i�s�����̕�
		particleParam.sizeRandomVec = Vec2(1.0f, 1.0f);                    //�����_���T�C�Y�̕�
		particleParam.liveTime = 1.0f;           //�p�[�e�B�N���̐�������
		particleParam.numOneFrameCreate = 40;    //1�t���[���ɐ������鐔
		particleParam.isLoop = false;            //���[�v���邩�ǂ���
		particleParam.isBillBoard = false;       //�r���{�[�h���邩�ǂ���
		particleParam.isForwardRotation = true;  //Shot������Forward��������B
		particleParam.isAlphaFade = true;        //AlphaFade�����邩�ǂ���

		return Effect::ParticleData(particleParam);
	}

	//--------------------------------------------------------------------------------------

	//�o�[�j�A�f�[�^------------------------------------------------------------------------

	Effect::ParticleData EffectManager::GetVernierVLineData() {
		//�o�[�e�b�N�X�p�����[�^�̐���
		auto vertexPCTParam =
			Builder::VertexPCTParametor(Vec3(0.25f, 0.0f, 0.25f), 1.0f, Col4(0.0f, 0.5f, 1.0f, 1.0f), Vec2(512.0f), L"LineV_TX");

		//�p�[�e�B�N���p�����[�^�̐���
		auto particleParam = maru::ParticleManager::Parametor(vertexPCTParam);
		particleParam.powerRandomVec = Vec2(2.0f, 5.0f);                  //�����_���i�s�͂̕�
		particleParam.shotDirectRandomVec = Vec3(45.0f, 0.0f, 45.0f);  //�����_���i�s�����̕�
		particleParam.sizeRandomVec = Vec2(5.0f, 5.0f);                    //�����_���T�C�Y�̕�
		particleParam.liveTime = 0.5f;           //�p�[�e�B�N���̐�������
		particleParam.numOneFrameCreate = 40;    //1�t���[���ɐ������鐔
		particleParam.isLoop = false;            //���[�v���邩�ǂ���
		particleParam.isBillBoard = false;       //�r���{�[�h���邩�ǂ���
		particleParam.isForwardRotation = true;  //Shot������Forward��������B
		particleParam.isAlphaFade = true;        //AlphaFade�����邩�ǂ���
		particleParam.isGravity = false;         //�d�͂��g�p���邩�ǂ���
		particleParam.gravityScale = Vec3(0.0f, -9.8f, 0.0f);  //�d�͂̑傫��

		return Effect::ParticleData(particleParam);
	}

	Effect::ParticleData EffectManager::GetVernierPointData() {
		//�o�[�e�b�N�X�p�����[�^�̐���
		auto vertexPCTParam = 
			Builder::VertexPCTParametor(Vec3(0.5f, 0.5f, 0.0f), 1.0f, Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(512.0f), L"Point_TX");

		//�p�[�e�B�N���p�����[�^�̐���
		auto particleParam = maru::ParticleManager::Parametor(vertexPCTParam);
		particleParam.powerRandomVec = Vec2(0.3f, 0.3f);                   //�����_���i�s�͂̕�
		particleParam.shotDirectRandomVec = Vec3(15.0f, 0.0f, 15.0f);      //�����_���i�s�����̕�
		particleParam.sizeRandomVec = Vec2(0.3f, 0.7f);                    //�����_���T�C�Y�̕�
		particleParam.liveTime = 0.5f;            //�p�[�e�B�N���̐�������
		particleParam.numOneFrameCreate = 20;     //1�t���[���ɐ������鐔
		particleParam.isLoop = false;             //���[�v���邩�ǂ���
		particleParam.isBillBoard = true;         //�r���{�[�h���邩�ǂ���
		particleParam.isForwardRotation = false;  //Shot������Forward��������B
		particleParam.isAlphaFade = true;         //AlphaFade�����邩�ǂ���

		return Effect::ParticleData(particleParam);
	}

	std::vector<Effect::ParticleData> EffectManager::GetVernierData() {
		std::vector<Effect::ParticleData> datas = {
			GetVernierVLineData(),
			GetVernierPointData(),
		};

		return datas;
	}

	//--------------------------------------------------------------------------------------
}