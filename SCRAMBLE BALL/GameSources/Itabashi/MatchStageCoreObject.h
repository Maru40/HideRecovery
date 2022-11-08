#pragma once

namespace basecross
{
namespace StageObject
{
	/// <summary>
	/// MatchStageの重要なコンポーネントを持ったオブジェクト
	/// </summary>
	class MatchStageCoreObject : public GameObject
	{
	public:
		MatchStageCoreObject(const std::shared_ptr<Stage>& stage);

		void OnCreate() override;
	};
}
}