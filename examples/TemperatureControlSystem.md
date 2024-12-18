## **Example TemperatureControlSystem**

```cpp
#include <iostream>
#include <sisState.hpp>

using namespace sisState;

struct TemperatureControlSystem : Machine<TemperatureControlSystem>
{
	uint8_t currentTemperature = 22;

	bool isOccupied = true;
	bool isEcoModeEnabled = false;

	struct TemperatureLow : Event {};
	struct TemperatureComfortable : Event {};
	struct TemperatureHigh : Event {};

	struct Idle : State<TemperatureControlSystem>
	{
		void enter() override { std::cout << "System is now Idle." << std::endl; }

		void event(const TemperatureLow&)
		{
			if (this->machine->isEcoModeEnabled) { this->machine->goState<Heating>({ this->machine->isEcoModeEnabledCondition }); }
			else { this->machine->goState<Heating>({ this->machine->isOccupiedCondition }); }
		}

		void event(const TemperatureHigh&)
		{
			if (this->machine->isEcoModeEnabled) { this->machine->goState<Cooling>({ this->machine->isEcoModeEnabledCondition }); }
			else { this->machine->goState<Cooling>({ this->machine->isOccupiedCondition }); }
		}
	};

	struct Heating : State<TemperatureControlSystem>
	{
		void enter() override
		{
			if (this->machine->isEcoModeEnabled) { std::cout << "Eco Heating in progress: Reducing energy usage..." << std::endl; }
			else { std::cout << "Heating in progress..." << std::endl; }
		}

		void exit() override { std::cout << "Stopping heating..." << std::endl; }

		void event(const TemperatureComfortable&) { this->machine->goState<Idle>(); }
	};

	struct Cooling : State<TemperatureControlSystem>
	{
		void enter() override
		{
			if (this->machine->isEcoModeEnabled) { std::cout << "Eco Cooling in progress: Optimizing energy efficiency..." << std::endl; }
			else { std::cout << "Cooling in progress..." << std::endl; }
		}

		void exit() override { std::cout << "Stopping cooling..." << std::endl; }

		void event(const TemperatureComfortable&) { this->machine->goState<Idle>(); }
	};

	struct EcoMode : State<TemperatureControlSystem>
	{
		void enter() override { std::cout << "Eco Mode Activated: Prioritizing energy-saving measures." << std::endl; }

		void event(const TemperatureHigh&) { this->machine->goState<Cooling>({ this->machine->isEcoModeEnabledCondition }); }
		void event(const TemperatureLow&) { this->machine->goState<Heating>({ this->machine->isEcoModeEnabledCondition }); }
		void event(const TemperatureComfortable&) { std::cout << "Temperature within eco-comfort range. Remaining in Eco Mode." << std::endl; }
	};

	Condition isOccupiedCondition = [&]() { return isOccupied; };
	Condition isEcoModeEnabledCondition = [&]() { return isEcoModeEnabled; };
	Condition isEcoModeDisabledCondition = [&]() { return !isEcoModeEnabled; };

	std::variant<Idle, Heating, Cooling, EcoMode> state{ Idle{} };

	void adjustTemperature(int temp)
	{
		currentTemperature = temp;

		if (currentTemperature < 20) { this->event<TemperatureLow>(); }

		else if (currentTemperature > 24) { this->event<TemperatureHigh>(); }
		else { this->event<TemperatureComfortable>(); }
	}

	void toggleEcoMode(bool enable)
	{
		isEcoModeEnabled = enable;

		if (enable) { this->goState<EcoMode>(); }
		else { this->goState<Idle>(); }
	}

	void setOccupancy(bool occupied)
	{
		isOccupied = occupied;
		if (occupied) { std::cout << "Room is now occupied." << std::endl; }
		else { std::cout << "Room is now unoccupied." << std::endl; }
	}
};

int main()
{
	auto climateControl = TemperatureControlSystem::initialState<TemperatureControlSystem::Idle>(true);

	std::cout << "--- Adjust Temperature to 18 (Heating Expected) ---" << std::endl;
	climateControl.adjustTemperature(18);
	std::cout << std::endl;

	std::cout << "--- Enable Eco Mode ---" << std::endl;
	climateControl.toggleEcoMode(true);
	std::cout << std::endl;

	std::cout << "--- Adjust Temperature to 26 (Cooling Expected) ---" << std::endl;
	climateControl.adjustTemperature(26);
	std::cout << std::endl;

	std::cout << "--- Disable Eco Mode ---" << std::endl;
	climateControl.toggleEcoMode(false);
	std::cout << std::endl;

	std::cout << "--- Adjust Temperature to 22 (Idle Expected) ---" << std::endl;
	climateControl.adjustTemperature(22);

	return 0;
}
```

<br/>

Output

```
System is now Idle.
--- Adjust Temperature to 18 (Heating Expected) ---
Heating in progress...

--- Enable Eco Mode ---
Stopping heating...
Eco Mode Activated: Prioritizing energy-saving measures.

--- Adjust Temperature to 26 (Cooling Expected) ---
Eco Cooling in progress: Optimizing energy efficiency...

--- Disable Eco Mode ---
Stopping cooling...
System is now Idle.

--- Adjust Temperature to 22 (Idle Expected) ---
```