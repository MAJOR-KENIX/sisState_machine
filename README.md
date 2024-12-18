# **SIS-State Machine**

## **Overview**

The SIS-State machine is a C++ library designed to simplify the implementation of finite state machines. It provides a flexible framework for creating state-based systems.

#### **Donation**
+ Bitcoin (BTC) : bc1qxgkrvzt6snvha23aqdah4v0d8r375597epu26e
+ Ethereum (ETH) : 0x3309C87AEd0b04d8A8c385eBA7FF6F858bE0fCD6
+ Solana (SOL) : E9PqQpmyiyT9EYYuFk4KzwkFy8RUWaY4P3SkbB9Su9s9

<br/>

### Namespace
The `sisState` namespace contains all the definitions and functions related to this library.

<br/>

## **Features**

*   **State Management**: The library provides a base struct `State` for defining states, which can be extended to implement custom behavior.
*   **Event Handling**: States can handle events using the `event` method, allowing for decoupling of event sources.
*   **Condition-Based Transitions**: Transitions between states can be conditioned on the outcome of one or more conditions, using the `goState` method with a list of conditions.
*   **Action Execution**: Actions can be executed before, between, or after state transitions using the `goState` method with a list of actions.

<br/>

## **Structures**

*   ### **Machine**

	A base structure representing a state machine, providing a common interface for managing states and transitions.

*   ### **State**

	An abstract base structure that represents a single state within the state machine. Derived structures must implement the necessary methods to define the behavior of each state.

*   ### **Event**

	An abstract base structure that represents an event that can occur in one or plus states. Derived structures can be used to define custom events with specific behaviors.

<br/>

## **Installation**

**Note: C++20 is the minimum version required.**

SIS-State machine is an header-only library, no special installation steps are needed. Just point your compiler to the "include" directory, and in your source files:

    #include <sisState.hpp>

<br/>

## **Documentation**

You can find the full documentation in the `documentation/` directory:

 - [Machine](/documentation/machine.md)
 - [Actions](/documentation/actions.md)
 - [Conditions](/documentation/conditions.md)
 - [Concepts](/documentation/events.md)
 - [States](/documentation/states.md)

<br/>

## **Simple use case (toggle)**

<div style="text-align: center;">
	<pre class="mermaid">
stateDiagram
    state LightMachine {
        [*] --> st_Off
        st_Off --> st_On : ev_PressSwitch
        st_On --> st_Off : ev_PressSwitch
    }
	</pre>
</div>

<br/>

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
	// Initialize the LightMachine in the 'Off' state
	LightMachine lightMachine = LightMachine::initialState<LightMachine::Off>();

	// Simulate a button press to transition, change the On state here.
	lightMachine.event<LightMachine::PressSwitch>();

	// Simulate a button press to transition,change the Off state here.
	lightMachine.event<LightMachine::PressSwitch>();

	return 0;
}
```

## **License**
SIS-State machine is [Open Source] software. It may be used for any purpose, including commercial purposes, at absolutely no cost. It is distributed under the terms of the [MIT license].

  [Open Source]: http://www.opensource.org/docs/definition.html
  [MIT license]: http://www.opensource.org/licenses/mit-license.html