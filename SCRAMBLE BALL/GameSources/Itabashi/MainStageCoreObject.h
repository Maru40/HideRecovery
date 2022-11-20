#pragma once

namespace basecross
{
namespace StageObject
{
	class MainStageCoreObject : public GameObject
	{
	public:
		MainStageCoreObject(const std::shared_ptr<Stage>& stage);

		void OnCreate() override;
	};
}
}