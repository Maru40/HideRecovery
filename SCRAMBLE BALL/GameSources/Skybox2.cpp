/*!
@file Skybox.cpp
@brief �L�����N�^�[�Ȃǎ���
*/

#include "stdafx.h"
#include "SkyBox2.h"
#include "Plane.h"
#include <string>

namespace basecross{
	// �X�^�e�B�b�N�����o�ϐ��Ƃ��ď��������邱�ƂŁAGameStage.cpp�ł̃e�N�X�`���ǂݍ��݂̍ۂɗ��p���Ă���
	const std::map<std::wstring, Vec3> Skybox2::pairs = {
		{L"000_000_000", Vec3(0   ,  0,   +0.5f)},
		{L"000_090_000", Vec3(+0.5f, 0,    0)},
		{L"000_180_000", Vec3(0   ,  0,   -0.5f)},
		{L"000_270_000", Vec3(-0.5f, 0,    0)},
		{L"090_000_000", Vec3(0   , -0.5f, 0)},
		{L"270_000_000", Vec3(0   , +0.5f, 0)},
	};

	void Skybox2::OnCreate()
	{
		for (const auto& pair : pairs)
		{
			// �v���[���N���X�����ɃX�J�C�L���[�u���쐬����
			auto plane = ObjectFactory::Create<Plane>(GetStage());

			auto planeDrawComp = plane->GetComponent<PTStaticDraw>(); // ���C�e�B���O�͊|�������Ȃ��̂ŁAPNT�͎g��Ȃ��i���C�e�B���O�ŉe�����ƃL���[�u�`�󂪂΂��j
			planeDrawComp->SetTextureResource(pair.first);
			planeDrawComp->SetDepthStencilState(DepthStencilState::None); // Z�o�b�t�@�̉e�����󂯂Ȃ�����i�ŏ��ɕ`�悷��΁A���̃I�u�W�F�N�g���Ղ�Ȃ��j
			planeDrawComp->SetSamplerState(SamplerState::LinearClamp); // �e�N�X�`���̐؂�ڂ�ڗ����Ȃ�����i��������Ȃ��ƃv���[���̋��ڂɐ����������悤�Ɍ����Ă��܂��j

			auto planeTransComp = plane->GetComponent<Transform>();
			// �e�N�X�`���L�[�̕�������p�x�ɕϊ�����
			float rotX = XMConvertToRadians(std::stof(pair.first.substr(0, 3))); // 0�����ڂ���3�����؂�o��
			float rotY = XMConvertToRadians(std::stof(pair.first.substr(4, 3))); // 4�����ڂ���3�����؂�o��
			float rotZ = XMConvertToRadians(std::stof(pair.first.substr(8, 3))); // 8�����ڂ���3�����؂�o��
			// ����͈͂̌��E�܂Ŋg�傷��iZ�o�b�t�@���I�t�ɂ��Ă���̂ŕs�K�v���Ǝv�����A�O�̂��߁j
			const float scale = 707.0f;
			planeTransComp->SetPosition(pair.second * scale);
			planeTransComp->SetRotation(rotX, rotY , rotZ);
			planeTransComp->SetScale(Vec3(scale));
			planeTransComp->SetParent(GetThis<GameObject>()); // �X�J�C�L���[�u���\������v���[����Skybox2�I�u�W�F�N�g�̎q�ɂ��邱�Ƃō��W�̊Ǘ����ȗ����ł���

			m_planes.push_back(plane);
			SetDrawLayer(-1);
		}
	}

	void Skybox2::OnUpdate()
	{
		// �X�J�C�L���[�u����ɃJ�����̍��W�ƈ�v����悤�ɍX�V����i�Œ�J�����̏ꍇ��OnUpdate���ԕs�v�j
		auto transComp = GetComponent<Transform>();
		transComp->SetPosition(GetStage()->GetView()->GetTargetCamera()->GetEye());
	}

	void Skybox2::OnDraw()
	{
		// �Ǘ����Ă��邷�ׂĂ̖ʂ�`�悷��
		for (auto& plane : m_planes)
		{
			plane->OnDraw();
		}
	}
}
//end basecross
