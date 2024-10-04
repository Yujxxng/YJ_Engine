#pragma once
#include "BaseLevel.h"

namespace Levels
{
	class MainLevel : public GSM::BaseLevel
	{
	public:
		~MainLevel() override;
		void Init() override;
		void Update() override;
		void Exit() override;

		void ReLoad();
		void Stop(float t);
	};
}