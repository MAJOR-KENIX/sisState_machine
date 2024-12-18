## **Example LightMachine**

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

Output
```
'Off' state exited.
Entering 'On' state.
'On' state exited.
Entering 'Off' state.
```