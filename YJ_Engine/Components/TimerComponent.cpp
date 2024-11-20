#include "TimerComponent.h"
#include "SpriteComponent.h"

TimerComponent::TimerComponent(GameObject* owner) : LogicComponent(owner)
{
	ID = "Timer";
}

void TimerComponent::Update()
{
	if (active)
	{
		auto now = std::chrono::steady_clock::now();
		std::chrono::duration<float> elapsedTime = now - startTime;

		if (elapsedTime >= timer)
		{
			//std::cout << "TimeOut" << std::endl;
			active = false;
			timeOut = true;
		}
	}
}

void TimerComponent::SetTimer(float second)
{
	timer = std::chrono::duration<float>(second);
}

void TimerComponent::ActiveTimer()
{
	active = true;
	timeOut = false;
	startTime = std::chrono::steady_clock::now();
}

bool TimerComponent::TimeOut()
{
	return timeOut;
}

void TimerComponent::CopyComponent(GameObject* owner)
{
	TimerComponent* tmp = new TimerComponent(owner);
	tmp->active = this->active;
	tmp->timeOut = this->timeOut;
	tmp->startTime = this->startTime;
	tmp->timer = this->timer;

	owner->AddComponent(tmp);
}

void TimerComponent::LoadFromJson(const json& data)
{
	auto timerData = data.find("Timer");
	if (timerData != data.end())
	{
		auto timeData = timerData.value().find("time");
		timer = std::chrono::duration<float>(timeData.value());
	}
}

json TimerComponent::SaveToJson()
{
	json timer;
	timer["time"] = this->timer.count();

	return timer;
}

ComponentSerializer* TimerComponent::CreateComponent(GameObject* owner)
{
	TimerComponent* tmp = new TimerComponent(owner);
	owner->AddComponent(tmp);

	return tmp;
}

