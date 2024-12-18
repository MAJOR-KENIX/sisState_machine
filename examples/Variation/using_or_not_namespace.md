## **Using namespace sisState;**
This section outlines the code modifications required depending on whether the `using namespace sisState;` is used or not.

**Without `using namespace sisState;`**

```cpp
#include <iostream>
#include <sisState.hpp>

struct LightMachine : sisState::Machine<LightMachine>
{
	struct PressSwitch : sisState::Event {};

	struct Off : sisState::State<LightMachine>
	{
		void enter() override { std::cout << "Entering 'Off' state." << std::endl; }
		void exit() override { std::cout << "'Off' state exited." << std::endl; }

		void event(const PressSwitch&) { this->m->goState<On>(); }
	};

	struct On : sisState::State<LightMachine>
	{
		void enter() override { std::cout << "Entering 'On' state." << std::endl; }
		void exit() override { std::cout << "'On' state exited." << std::endl; }

		void event(const PressSwitch&) { this->m->goState<Off>(); }
	};

	std::variant<Off, On> state{};
};

int main()
{
	LightMachine lightMachine = LightMachine::initialState<LightMachine::Off>();

	lightMachine.event<LightMachine::PressSwitch>();
	lightMachine.event<LightMachine::PressSwitch>();

	return 0;
}
```

<br/>

**With `using namespace sisState;`**

```cpp
#include <iostream>
#include <sisState.hpp>

using namespace sisState;

struct LightMachine : Machine<LightMachine>
{
	struct PressSwitch : Event {};

	struct Off : State<LightMachine>
	{
		void enter() override { std::cout << "Entering 'Off' state." << std::endl; }
		void exit() override { std::cout << "'Off' state exited." << std::endl; }

		void event(const PressSwitch&) { this->m->goState<On>(); }
	};

	struct On : State<LightMachine>
	{
		void enter() override { std::cout << "Entering 'On' state." << std::endl; }
		void exit() override { std::cout << "'On' state exited." << std::endl; }

		void event(const PressSwitch&) { this->m->goState<Off>(); }
	};

	std::variant<Off, On> state{};
};

int main()
{
	LightMachine lightMachine = LightMachine::initialState<LightMachine::Off>();

	lightMachine.event<LightMachine::PressSwitch>();
	lightMachine.event<LightMachine::PressSwitch>();

	return 0;
}
```





