### **Actions in State Machines**

Actions can be executed at different points during a state transition. Here are the three main positions where actions can be applied:

| Position         | Description                                                                                           |
|------------------|-------------------------------------------------------------------------------------------------------|
| **Before**       | Actions are executed **before exiting the current state**.                                            |
| **Between**      | Actions are executed **after exiting the current state but before entering the next state**.          |
| **After**        | Actions are executed **after fully entering the new state**.                                          |

<br/>

```cpp
#include <iostream>
#include <sisState.hpp>
#include <vector>

using namespace sisState;

struct RobotController : Machine<RobotController>
{
	bool batteryLow = false;
	bool obstacleDetected = false;

	struct StartMission : Event {};
	struct TaskCompleted : Event {};
	struct ObstacleCleared : Event {};
	struct ChargeComplete : Event {};

	struct Idle : State<RobotController>
	{
		void enter() override { std::cout << "Robot is idle." << std::endl; }

		void event(const StartMission&)
		{
			std::cout << "Mission started." << std::endl;
			this->machine->goState<ExecutingMission>({ this->machine->logAction, this->machine->checkBattery }, ActionPosition::Before);
		}
	};

	struct ExecutingMission : State<RobotController>
	{
		void enter() override { std::cout << "Executing mission tasks..." << std::endl; }

		void event(const TaskCompleted&)
		{
			if (this->machine->obstacleDetected) { this->machine->goState<ObstacleDetected>({ this->machine->logAction, this->machine->sendAlert }, ActionPosition::Between); }
			else { this->machine->goState<Idle>({ this->machine->logAction, this->machine->notifyCompletion }, ActionPosition::After); }
		}
	};

	struct ObstacleDetected : State<RobotController>
	{
		void enter() override { std::cout << "Obstacle detected. Stopping mission." << std::endl; }

		void event(const ObstacleCleared&)
		{
			std::cout << "Obstacle cleared. Resuming mission." << std::endl;
			this->machine->goState<ExecutingMission>({ this->machine->logAction, this->machine->resumeTask });
		}
	};

	struct Charging : State<RobotController>
	{
		void enter() override { std::cout << "Charging robot's battery..." << std::endl; }

		void event(const ChargeComplete&)
		{
			std::cout << "Battery fully charged." << std::endl;
			this->machine->goState<Idle>({ this->machine->logAction });
		}
	};

	Action logAction = [] { std::cout << "[LOG]: Transition logged." << std::endl; };
	Action checkBattery = [&]() { std::cout << "[ACTION]: Checking battery level. Low: " << (batteryLow ? "Yes" : "No") << std::endl; };
	Action sendAlert = [] { std::cout << "[ACTION]: Sending obstacle alert to operator." << std::endl; };
	Action notifyCompletion = [] { std::cout << "[ACTION]: Mission completed. Notifying operator." << std::endl; };
	Action resumeTask = [] { std::cout << "[ACTION]: Resuming mission tasks." << std::endl; };

	std::variant<Idle, ExecutingMission, ObstacleDetected, Charging> state{};

	void startMission() { this->event<StartMission>(); }
	void completeTask() { this->event<TaskCompleted>(); }
	void clearObstacle() { this->event<ObstacleCleared>(); }
	void chargeBattery() { this->event<ChargeComplete>(); }
};

int main()
{
	auto robot = RobotController::initialState<RobotController::Idle>(true);

	std::cout << "--- Starting Mission ---" << std::endl;
	robot.startMission();
	std::cout << std::endl;

	std::cout << "--- Task Completed with No Obstacles ---" << std::endl;
	robot.completeTask();
	std::cout << std::endl;

	std::cout << "--- Task Completed with Obstacle ---" << std::endl;
	robot.obstacleDetected = true;
	robot.completeTask();
	std::cout << std::endl;

	std::cout << "--- Clearing Obstacle ---" << std::endl;
	robot.clearObstacle();
	std::cout << std::endl;

	std::cout << "--- Charging Battery ---" << std::endl;
	robot.batteryLow = true;
	robot.chargeBattery();

	return 0;
}
```