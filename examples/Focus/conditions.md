### **Conditions in State Machines**

Conditions are logical checks that determine whether a state transition can occur. They help control the flow of the state machine by validating specific requirements before transitioning. There are two primary modes for evaluating conditions during a transition.

| Mode               | Description                                                                                         |
|--------------------|-----------------------------------------------------------------------------------------------------|
| **All Pass**       | All specified conditions must evaluate to `true` for the transition to occur.                       |
| **One Pass**       | At least one of the specified conditions must evaluate to `true` for the transition to occur.        |

<br/>

```cpp
#include <iostream>
#include <sisState.hpp>

using namespace sisState;

struct AutomaticDoor : Machine<AutomaticDoor>
{
	bool sensorTriggered = false;
	bool powerAvailable = true;
	bool maintenanceMode = false;
	bool manualOverride = false;

	struct PersonDetected : Event {};
	struct NoPersonDetected : Event {};
	struct PowerFailure : Event {};
	struct PowerRestored : Event {};
	struct MaintenanceActivated : Event {};
	struct MaintenanceDeactivated : Event {};
	struct ManualOverrideActivated : Event {};
	struct ManualOverrideDeactivated : Event {};

	struct Closed : State<AutomaticDoor>
	{
		void enter() override { std::cout << "Door is now closed." << std::endl; }

		void event(const PersonDetected&) { this->machine->goState<Opening>({ this->machine->isPowerAvailableCondition, this->machine->isNotInMaintenanceCondition }, ConditionMode::AllPass); }
	};

	struct Opening : State<AutomaticDoor>
	{
		void enter() override { std::cout << "Door is opening..." << std::endl; }

		void event(const NoPersonDetected&) { this->machine->goState<Closing>({ this->machine->isPowerAvailableCondition }, ConditionMode::AllPass); }
		void event(const ManualOverrideActivated&) { this->machine->goState<ManualControl>(); }
	};

	struct Closing : State<AutomaticDoor>
	{
		void enter() override { std::cout << "Door is closing..." << std::endl; }

		void event(const PersonDetected&) { this->machine->goState<Opening>({ this->machine->isPowerAvailableCondition, this->machine->isNotInMaintenanceCondition }, ConditionMode::AllPass); }
		void event(const PowerFailure&) { this->machine->goState<ErrorState>(); }
	};

	struct ManualControl : State<AutomaticDoor>
	{
		void enter() override { std::cout << "Manual control activated. Operator in control." << std::endl; }

		void event(const ManualOverrideDeactivated&) { this->machine->goState<Closed>({ this->machine->isPowerAvailableCondition, this->machine->isNotInMaintenanceCondition }, ConditionMode::AllPass); }
	};

	struct ErrorState : State<AutomaticDoor>
	{
		void enter() override { std::cout << "Error: Door system failure!" << std::endl; }

		void event(const PowerRestored&) { this->machine->goState<Closed>({ this->machine->isNotInMaintenanceCondition, this->machine->manualOverrideCondition }, ConditionMode::OnePass); }
	};

	Condition isPowerAvailableCondition = [&]() { return powerAvailable; };
	Condition isNotInMaintenanceCondition = [&]() { return !maintenanceMode; };
	Condition manualOverrideCondition = [&]() { return manualOverride; };

	std::variant<Closed, Opening, Closing, ManualControl, ErrorState> state{};

	void detectPerson(bool detected)
	{
		sensorTriggered = detected;

		if (detected) { this->event<PersonDetected>(); }
		else { this->event<NoPersonDetected>(); }
	}

	void setPower(bool power)
	{
		powerAvailable = power;

		if (!power) { this->event<PowerFailure>(); }
		else { this->event<PowerRestored>(); }
	}

	void setMaintenanceMode(bool maintenance)
	{
		maintenanceMode = maintenance;

		if (maintenance) { this->event<MaintenanceActivated>(); }
		else { this->event<MaintenanceDeactivated>(); }
	}

	void setManualOverride(bool override)
	{
		manualOverride = override;

		if (override) { this->event<ManualOverrideActivated>(); }
		else { this->event<ManualOverrideDeactivated>(); }
	}
};


int main()
{
	AutomaticDoor door = AutomaticDoor::initialState<AutomaticDoor::Closed>();

	door.detectPerson(true);
	door.detectPerson(false);

	door.setManualOverride(true);
	door.setManualOverride(false);

	door.setPower(false);
	door.setPower(true);
}
```