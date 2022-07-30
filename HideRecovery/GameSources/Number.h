
#pragma once
#include "stdafx.h"

namespace basecross {

	class Number : public GameObject
	{
		int number;
		std::vector<VertexPositionColorTexture> vertices;

	public:
		Number(const std::shared_ptr<Stage>& stage, int number)
			:GameObject(stage), number(number)
		{
		}

		void OnCreate() override;
		void SetValue(int value)
		{
			number = value;
			float fNumber = static_cast<float>(number);
			vertices[0].textureCoordinate = Vec2((fNumber + 0) * 90.0f / 995.0f, 0.0f);
			vertices[1].textureCoordinate = Vec2((fNumber + 1) * 90.0f / 995.0f, 0.0f);
			vertices[2].textureCoordinate = Vec2((fNumber + 0) * 90.0f / 995.0f, 1.0f);
			vertices[3].textureCoordinate = Vec2((fNumber + 1) * 90.0f / 995.0f, 1.0f);
			auto drawComp = GetComponent<PCTSpriteDraw>();
			drawComp->UpdateVertices(vertices);
		}

	};

	class HideRank : public GameObject
	{
		int m_rankNumber;
		std::vector<VertexPositionColorTexture> vertices;

	public:
		HideRank(const std::shared_ptr<Stage>& stage, int number)
			:GameObject(stage), m_rankNumber(number)
		{

		}

		void OnCreate() override;
		void SetValue(int value)
		{
			m_rankNumber = value;
			float fNumber = static_cast<float>(m_rankNumber);
			vertices[0].textureCoordinate = Vec2((fNumber + 0) * 100.0f / 512.0f, 0.0f);
			vertices[1].textureCoordinate = Vec2((fNumber + 1) * 100.0f / 512.0f, 0.0f);
			vertices[2].textureCoordinate = Vec2((fNumber + 0) * 100.0f / 512.0f, 1.0f);
			vertices[3].textureCoordinate = Vec2((fNumber + 1) * 100.0f / 512.0f, 1.0f);
			auto drawComp = GetComponent<PCTSpriteDraw>();
			drawComp->UpdateVertices(vertices);
		}

	};
}