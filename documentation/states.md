## **States**

The State structure provides a basic model for managing states within a finite state machine. It defines the behavior of a state, including entry, exit, and event handling.

| TYPENAME    | Description                                                                 |
|:-----------:|-----------------------------------------------------------------------------|
| **M**       | Is a structure `machine` that derives from the `struct sisState::Machine`.  |

**Implementations**

```cpp
template<typename M>
struct State
{
	virtual ~State() = default;

	virtual void enter() {}
	virtual void exit() {}
	
	virtual void event(const Event&) {}

	M* m = nullptr;
	M* machine = nullptr;

	void setMachine(M* _m)
	{
		m = _m;
		machine = m;
	}
};
```

- **`enter`**: Invoked upon entering the state; its implementation is optional.
- **`exit`**: Invoked upon exiting the state; its implementation is optional.
- **`event`**: Handles events specific to the state, allowing custom logic during its active phase; its implementation is optional.

<br/>

**Note** : 
You can use the `machine` variable in the enter, exit, and event methods to access the variables present in the machine's state. You can use `m` instead.

<br/>

**Constructor**

Without `using namespace sisState;`

```cpp
struct StateName : sisState::State<MachineName>
{
	void enter() override { /* ... */ } // optional
	void exit() override { /* ... */ } // optional

	void event(const EventName&) { /* ... */ } // optional
};
```

<br/>

With `using namespace sisState;`

```cpp
struct StateName : State<MachineName>
{
	void enter() override { /* ... */ } // optional
	void exit() override { /* ... */ } // optional

	void event(const EventName&) { /* ... */ } // optional
};
```

<br/>

**Examples**

Without `using namespace sisState;`

```cpp
struct Idle : sisState::State<MachineName>
{
	void enter() override { std::cout << "Enter the Idle state" << std::endl; }

	void event(const evRunningPress&) { this->machine->goState<Running>(); } 
};

struct Running : sisState::State<MachineName>
{
	void enter() override { std::cout << "Enter the Running state" << std::endl; }
	void exit() override { std::cout << "Exit the Running state" << std::endl; }

	void event(const evJumpPress&) { this->machine->goState<Jump>(); } 
};

struct Jump : sisState::State<MachineName>
{
	void exit() override { std::cout << "Exit the Jump state" << std::endl; }

	void event(const evOnGround&) { this->m->goState<Idle>(); } 
};

std::variant<Idle, Running, Jump> state{};
```

<br/>

With `using namespace sisState;`

```cpp
struct Idle : State<MachineName>
{
	void enter() override { std::cout << "Enter the Idle state" << std::endl; }

	void event(const evRunningPress&) { this->machine->goState<Running>(); } 
};

struct Running : State<MachineName>
{
	void enter() override { std::cout << "Enter the Running state" << std::endl; }
	void exit() override { std::cout << "Exit the Running state" << std::endl; }

	void event(const evJumpPress&) { this->machine->goState<Jump>(); } 
};

struct Jump : State<MachineName>
{
	void exit() override { std::cout << "Exit the Jump state" << std::endl; }

	void event(const evOnGround&) { this->m->goState<Idle>(); } 
};

std::variant<Idle, Running, Jump> state{};
```

<br/>

*All state-specific structures defined for a finite state machine must be declared prior to the definition of the Machine or positioned inside the Machine's definition, before the declaration of the state variable and after the declaration of any other variables or member functions.*

**Furthermore, all states must be listed in a `std::variant` named `state`.**

```cpp
struct MachineName : sisState::Machine<MachineName>
{
	struct Idle : sisState::State<MachineName>
	{
		void enter() override { std::cout << "Machine is now Idle." << std::endl; }

		void event(const StartButtonPressed&) { this->machine->goState<Preparing>(); }
	};

	struct Preparing : sisState::State<MachineName>
	{
		void enter() override
		{
			std::cout << "Preparing for the washing cycle." << std::endl;
			this->machine->displayStartMessage();
		}

		void event(const DoorClosed&) { this->machine->goState<Washing>(); }
	};

	struct Washing : sisState::State<MachineName>
	{
		void enter() override { std::cout << "Washing in progress..." << std::endl; }

		void event(const CycleComplete&) { this->machine->goState<Spinning>(); }
		void event(const ErrorDetected&) { this->machine->goState<ErrorState>(); }
	};

	struct Spinning : sisState::State<MachineName>
	{
		void enter() override { std::cout << "Spinning cycle started." << std::endl; }

		void event(const CycleComplete&) { this->machine->goState<Idle>(); }
	};

	struct ErrorState : sisState::State<MachineName>
	{
		void enter() override { std::cout << "Error detected! Please check the machine." << std::endl; }
	};
};
```