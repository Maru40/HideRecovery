/*!
@file LaserBase.cpp
@brief LaserBase�̃N���X����
�S���F�ێR�T��
*/

#include "stdafx.h"
#include "Project.h"

#include "LaserBase.h"
#include "GameStageBase.h"

#include "TimeHelper.h"

namespace basecross {

	//�p�����[�^----------------------------------------------------------------------------------

	LaserBaseDraw_Parametor::LaserBaseDraw_Parametor():
		LaserBaseDraw_Parametor(
			0.25f, 3.0f,
			1.0f, 5.0f,
			10.0f, 10.0f,
			Vec3(0.0f, 0.0, 1.0f), Col4(0.1f, 0.1f, 0.8f, 1.0f),
			L"Point_TX"
		)
	{ }

	LaserBaseDraw_Parametor::LaserBaseDraw_Parametor(
		const float& width, const float& maxWidth,
		const float& length, const float& maxLength,
		const float& longerSpeed, const float& smallerSpeed,
		const Vec3& forward, const Col4& color,
		const wstring& texture
	) :
		width(width), maxWidth(maxWidth),
		length(length), maxLength(maxLength),
		longerSpeed(longerSpeed), smallerSpeed(smallerSpeed),
		forward(forward), color(color),
		texture(texture),
		isSmaller(false),
		longerType(ScaleChangeType::Normal),
		smallerType(ScaleChangeType::Normal)
	{ }

	//--------------------------------------------------------------------------------------------

	LaserBaseDraw::LaserBaseDraw(const std::shared_ptr<GameObject>& objPtr)
		: LaserBaseDraw(objPtr, Parametor())
	{}

	LaserBaseDraw::LaserBaseDraw(const std::shared_ptr<GameObject>& objPtr, const Parametor& param)
		: Component(objPtr), m_param(param)
	{}

	void LaserBaseDraw::OnCreate() {
		const float& width = m_param.width;
		const float& maxWidth = m_param.maxWidth;
		const float& length = m_param.length;
		const Col4& color = m_param.color;

		//�x�N�^�[�z��̂��߁A���̂悤�ȑ�����@���Ƃ��B
		m_vertices = {
			{Vec3(-width,0.0f,+0.0f),color,Vec2(0.0f,0.0f)},	//0   
			{Vec3(+width,0.0f,+0.0f),color,Vec2(1.0f,0.0f)},	//1

			{Vec3(-width,0.0f,-width),color,Vec2(0.0f,0.5f)},   //2
			{Vec3(+width,0.0f,-width),color,Vec2(1.0f,0.5f)},   //3
			{Vec3(-width,0.0f,-length + width),color,Vec2(0.0f,0.5f)}, //4
			{Vec3(+width,0.0f,-length + width),color,Vec2(1.0f,0.5f)}, //5

			{Vec3(-width,0.0f,-length),color,Vec2(0.0f,1.0f)},  //6
			{Vec3(+width,0.0f,-length),color,Vec2(1.0f,1.0f)}	//7
		};

		std::vector<uint16_t> indices = {
			0,1,2,
			2,1,3,

			2,3,4,
			4,3,5,

			4,5,6,
			6,5,7,
		};

		auto drawComp = GetGameObject()->AddComponent<DrawComp>();
		drawComp->CreateOriginalMesh(m_vertices, indices);
		drawComp->SetOriginalMeshUse(true);
		drawComp->SetTextureResource(m_param.texture);
		drawComp->SetDepthStencilState(DepthStencilState::Read);  //�d�Ȃ荇���̕s����Ȃ����B
		drawComp->SetBlendState(BlendState::Additive);

		GetGameObject()->SetAlphaActive(true);
	}

	void LaserBaseDraw::OnUpdate() {
		LongerUpdate();
		SmallerUpdate();

		VerticesUpdate();
	}

	void LaserBaseDraw::LongerUpdate() {
		auto delta = maru::TimeHelper::GetElapsedTime(maru::DeltaType::Normal);

		const float& maxLength = m_param.maxLength;
		float longerSpeed = m_param.longerSpeed * delta;
		//IsSmaller�o�Ȃ�MaxLength�łȂ��Ȃ�
		if (!IsSmaller() && m_param.length < maxLength) {
			m_param.length += longerSpeed;
		}
	}

	void LaserBaseDraw::SmallerUpdate() {
		if (!IsSmaller()) {
			return;
		}

		auto delta = maru::TimeHelper::GetElapsedTime(maru::DeltaType::Normal);
		m_param.length -= m_param.smallerSpeed * delta;

		//weight��菬�����������B
		const float DestSize = 1.0f;
		if (m_param.length <= m_param.width + DestSize) {
			m_param.length = m_param.width + DestSize;
			EndProcess();
		}
	}

	void LaserBaseDraw::VerticesUpdate() {
		const auto& forward = GetLongerForward().GetNormalized();
		
		const float& width = m_param.width;
		const float& length = m_param.length;

		//�J�����̈ʒu�ɂ���āA���_�̈ʒu���Z�o����B
		//���[�U�[�̒��S���W���擾(���ꂼ��̐؂�ڕ��p��)
		//���[�J�����W�ł̒��_�̈ʒu���߂��s���B
		std::vector<Vec3> points = {  //�i�s�����ɑ΂��Č��ɒ��_��z�u����B
			forward * 0.0f,  //�X���Ă��v�Z�ł���悤�ɂ���B
			forward * -width,
			forward * (-length + width),
			forward * -length,
		};

		auto camera = GetGameStage()->GetCamera<Camera>();
		int index = 0;  //�J�E���^���g��Ȃ����ǃJ�E���^���g�������B
		for (auto& point : points)  //���S�_�̍��E�ɒ��_��z�u����B
		{
			//���[���h���W�̌��_�ɂ���Ƃ͌���Ȃ�����A
			//�|�C���g��[���[���h���W�Ɉړ��������ʒu]���猩���J�����̕����B
			auto pointToCamera = camera->GetEye() - (point + transform->GetPosition());  //�|�C���g�����[���h���W�Ɉړ�������B
			//�O�ς��g���āA�u�i�s�����v�Ɓu�J�����ւ̃x�N�g���v�Ɂu�����ȃx�N�g���v�����߂邱�Ƃ��ł���B
			auto newCross = pointToCamera.cross(forward);
			//�O�ς͕��s�l�ӌ`���̑傫���̂��߁Anomalize����B
			newCross.normalize();  //�x�N�g���̌��������g�������̂Ő��K������B

			//���ꂼ�ꐂ���ȕ����ɁA���_��z�u����B
			m_vertices[index++].position = point + +newCross * width;  //�E
			m_vertices[index++].position = point + -newCross * width;  //��  //cross�̏��Ԃɂ���ĕς��
		}

		auto drawComp = GetGameObject()->GetComponent<DrawComp>();
		drawComp->UpdateVertices(m_vertices);
	}

	void LaserBaseDraw::EndProcess() {
		GetStage()->RemoveGameObject<GameObject>(GetGameObject());
	}
}