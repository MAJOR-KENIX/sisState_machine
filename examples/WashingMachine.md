## **Example WashingMachine**

```cpp
#include <iostream>
#include <sisState.hpp>

struct WashingMachine : sisState::Machine<WashingMachine>
{
	bool doorIsClosed = false;

	struct StartButtonPressed : sisState::Event {};
	struct DoorClosed : sisState::Event {};
	struct CycleComplete : sisState::Event {};
	struct ErrorDetected : sisState::Event {};

	struct Idle : sisState::State<WashingMachine>
	{
		void enter() override { std::cout << "Machine is now Idle." << std::endl; }

		void event(const StartButtonPressed&) { this->machine->goState<Preparing>(); }
	};

	struct Preparing : sisState::State<WashingMachine>
	{
		void enter() override
		{
			std::cout << "Preparing for the washing cycle." << std::endl;
			this->machine->displayStartMessage();
		}

		void event(const DoorClosed&)
		{
			this->machine->doorIsClosed = true;
			this->machine->goState<Washing>();
		}
	};

	struct Washing : sisState::State<WashingMachine>
	{
		void enter() override { std::cout << "Washing in progress..." << std::endl; }

		void event(const CycleComplete&) { this->machine->goState<Spinning>({ this->machine->getDoorIsClosed }); }
		void event(const ErrorDetected&) { this->machine->goState<ErrorState>({ this->machine->checkForErrors }); }
	};

	struct Spinning : sisState::State<WashingMachine>
	{
		void enter() override { std::cout << "Spinning cycle started." << std::endl; }

		void event(const CycleComplete&) { this->machine->goState<Idle>(); }
	};

	struct ErrorState : sisState::State<WashingMachine>
	{
		void enter() override { std::cout << "Error detected! Please check the machine." << std::endl; }
	};

	sisState::Condition getDoorIsClosed = [&] { return doorIsClosed; };

	sisState::Action displayStartMessage = [] { std::cout << "Starting washing machine..." << std::endl; };
	sisState::Action checkForErrors = [] { std::cout << "Checking for errors..." << std::endl; };
	sisState::Action notifyUser = [] { std::cout << "Notifying user..." << std::endl; };

	std::variant<Idle, Preparing, Washing, Spinning, ErrorState> state{};
};

int main()
{
	WashingMachine washingMachine = WashingMachine::initialState<WashingMachine::Idle>();

	washingMachine.event<WashingMachine::StartButtonPressed>();
	washingMachine.event<WashingMachine::DoorClosed>();
	washingMachine.event<WashingMachine::CycleComplete>();

	return 0;
}
```

<br/>

Output

```
Preparing for the washing cycle.
Starting washing machine...
Washing in progress...
Spinning cycle started.
```