## **Example HeatingSystem**

```cpp
#include <iostream>
#include <sisState.hpp>

struct HeatingSystem : sisState::Machine<HeatingSystem>
{
	uint8_t currentTemperature = 20;

	struct TemperatureLow : sisState::Event { uint8_t value; };
	struct TemperatureComfortable : sisState::Event { uint8_t value; };
	struct TemperatureHigh : sisState::Event { uint8_t value; };

	struct Off : sisState::State<HeatingSystem>
	{
		void enter() override { std::cout << "Heating system is Off." << std::endl; }

		void event(const TemperatureLow&) { machine->goState<Heating>(); }
	};

	struct Heating : sisState::State<HeatingSystem>
	{
		void enter() override { std::cout << "Heating system is On. Increasing temperature." << std::endl; }
		void exit() override { std::cout << "Heating system is turning off." << std::endl; }

		void event(const TemperatureComfortable&) { machine->goState<EnergySaving>(); }
	};

	struct EnergySaving : sisState::State<HeatingSystem>
	{
		void enter() override { std::cout << "Energy-saving mode activated." << std::endl; }

		void event(const TemperatureLow&) { machine->goState<Heating>(); }
		void event(const TemperatureHigh&) { machine->goState<Off>(); }
	};

	std::variant<Off, Heating, EnergySaving> state{};

	sisState::Condition isTemperatureLow = [&]() { return currentTemperature < 18; };
	sisState::Condition isTemperatureComfortable = [&]() { return currentTemperature >= 18 && currentTemperature <= 22; };
	sisState::Condition isTemperatureHigh = [&]() { return currentTemperature > 22; };

	void handleTemperature(uint8_t temp)
	{
		currentTemperature = temp;

		if (isTemperatureLow()) { this->event<TemperatureLow>(); }
		else if (isTemperatureComfortable()) { this->event<TemperatureComfortable>(); }
		else if (isTemperatureHigh()) { this->event<TemperatureHigh>(); }
	}
};

int main()
{
	HeatingSystem heatingSystem = HeatingSystem::initialState<HeatingSystem::Off>(true);

	heatingSystem.handleTemperature(16);
	heatingSystem.handleTemperature(20);
	heatingSystem.handleTemperature(23);
	heatingSystem.handleTemperature(17);

	return 0;
}
```

<br/>

Output

```
Heating system is Off.
Heating system is On. Increasing temperature.
Heating system is turning off.
Energy-saving mode activated.
Heating system is Off.
Heating system is On. Increasing temperature.
```