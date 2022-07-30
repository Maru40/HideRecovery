/*!
@file LaserBase.cpp
@brief LaserBaseのクラス実体
担当：丸山裕喜
*/

#include "stdafx.h"
#include "Project.h"

#include "LaserBase.h"
#include "GameStageBase.h"

#include "TimeHelper.h"

namespace basecross {

	//パラメータ----------------------------------------------------------------------------------

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

		//ベクター配列のため、このような代入方法がとれる。
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
		drawComp->SetDepthStencilState(DepthStencilState::Read);  //重なり合いの不具合をなくす。
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
		//IsSmaller出なくMaxLengthでないなら
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

		//weightより小さくしたい。
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

		//カメラの位置によって、頂点の位置を算出する。
		//レーザーの中心座標を取得(それぞれの切れ目分用意)
		//ローカル座標での頂点の位置決めを行う。
		std::vector<Vec3> points = {  //進行方向に対して後ろに頂点を配置する。
			forward * 0.0f,  //傾いても計算できるようにする。
			forward * -width,
			forward * (-length + width),
			forward * -length,
		};

		auto camera = GetGameStage()->GetCamera<Camera>();
		int index = 0;  //カウンタを使わないけどカウンタを使いたい。
		for (auto& point : points)  //中心点の左右に頂点を配置する。
		{
			//ワールド座標の原点にいるとは限らないから、
			//ポイントを[ワールド座標に移動させた位置]から見たカメラの方向。
			auto pointToCamera = camera->GetEye() - (point + transform->GetPosition());  //ポイントをワールド座標に移動させる。
			//外積を使って、「進行方向」と「カメラへのベクトル」に「垂直なベクトル」を求めることができる。
			auto newCross = pointToCamera.cross(forward);
			//外積は平行四辺形分の大きさのため、nomalizeする。
			newCross.normalize();  //ベクトルの向きだけ使いたいので正規化する。

			//それぞれ垂直な方向に、頂点を配置する。
			m_vertices[index++].position = point + +newCross * width;  //右
			m_vertices[index++].position = point + -newCross * width;  //左  //crossの順番によって変わる
		}

		auto drawComp = GetGameObject()->GetComponent<DrawComp>();
		drawComp->UpdateVertices(m_vertices);
	}

	void LaserBaseDraw::EndProcess() {
		GetStage()->RemoveGameObject<GameObject>(GetGameObject());
	}
}