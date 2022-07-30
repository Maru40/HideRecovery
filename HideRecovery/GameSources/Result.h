#pragma once
#include "stdafx.h"

namespace basecross
{
	class Result : public GameObject
	{
	public:
		Result(const std::shared_ptr<Stage>& stage)
			:GameObject(stage)
		{}

		 void OnCreate() override;
		 void OnUpdate() override;
		 void OnDraw() override;
	};

	class ResultNext : public GameObject
	{
	public:
		ResultNext(const std::shared_ptr<Stage>& stage)
			:GameObject(stage)
		{}

		void OnCreate() override;
		void OnUpdate() override;
		void OnDraw() override;

	};

	class ResultRetry : public GameObject
	{
	public:
		ResultRetry(const std::shared_ptr<Stage>& stage)
			:GameObject(stage)
		{}

		void OnCreate() override;
		void OnUpdate() override;
		void OnDraw() override;

	};

	class ABotton : public GameObject
	{
	public:
		ABotton(const std::shared_ptr<Stage>& stage)
			:GameObject(stage)
		{}

		void OnCreate() override;
		void OnUpdate() override;
		void OnDraw() override;

	};

	class BBotton : public GameObject
	{
	public:
		BBotton(const std::shared_ptr<Stage>& stage)
			:GameObject(stage)
		{}

		void OnCreate() override;
		void OnUpdate() override;
		void OnDraw() override;

	};
}