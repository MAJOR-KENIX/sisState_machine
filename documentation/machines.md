## **Machines**

The Machine struct serves as the central component of the finite state machine, managing states, transitions, and events. It should be declared as a templated class and must inherit from Machine\<MachineName\>, where MachineName represents the specific name of the machine being implemented.

| TYPENAME    | Description                                                                 |
|:-----------:|-----------------------------------------------------------------------------|
| **M**       | Is a structure `machine` that derives from the `struct sisState::Machine`.  |
| **S**       | Is a structure `state` that derives from the `struct sisState::State`.      |
| **E**       | Is a struct `event` that derives from the `struct sisState::Event`.         |
| **A**       | Is a struct `action` that derives from the `struct sisState::Action`.       |
| **C**       | Is a struct `condition` that derives from the `struct sisState::Condition`. |

**Implementations**

```cpp
namespace sisState
{
template<typename M>
struct Machine
{
	Machine<M>() = default;

	template<typename S>
	static M initialState(const bool& executeEnter = false)
	{
		M machine;
		machine.template setState<S>();
		if (executeEnter) { machine.enter(); }
		return machine;
	}

	template<typename S>
	bool isState()
	{
		return std::visit([](const auto& currentState) { return std::is_same_v<std::decay_t<decltype(currentState)>, S>; }, static_cast<const M*>(this)->state);
	}

	template<typename E>
	void event() { std::visit([](auto& state) { if constexpr (requires { state.event(E {}); }) { state.event(E {}); } }, static_cast<M*>(this)->state); }

protected:
	void exit() { std::visit([&](auto& state) { if (state.m) { state.exit(); } }, static_cast<M*>(this)->state); }
	void enter() { std::visit([&](auto& state) { if (state.m) { state.enter(); } }, static_cast<M*>(this)->state); }

	template <typename S>
	void setState()
	{
		auto s = S{};
		s.setMachine(static_cast<M*>(this));
		static_cast<M*>(this)->state = s;
	}

	template<typename S> requires std::derived_from<S, State<M>>
	void goState()
	{
		exit();
		setState<S>();
		enter();
	}

	template<typename S, typename C> requires std::derived_from<S, State<M>>&& std::derived_from<C, Condition>
	void goState(const std::initializer_list<C>& conditions, const ConditionMode& conditionMode = ConditionMode::AllPass)
	{
		if (!processConditions(conditions, conditionMode)) { return; }
		goState<S>();
	}

	template<typename S, typename A> requires std::derived_from<S, State<M>>&& std::derived_from<A, Action>
	void goState(const std::initializer_list<A>& actions, const ActionPosition& actionPosition = ActionPosition::Between)
	{
		if (actionPosition == ActionPosition::Before) { processActions(actions); }
		exit();
		if (actionPosition == ActionPosition::Between) { processActions(actions); }
		setState<S>();
		enter();
		if (actionPosition == ActionPosition::After) { processActions(actions); }
	}

	template<typename S, typename A, typename C> requires std::derived_from<S, State<M>>&& std::derived_from<A, Action>&& std::derived_from<C, Condition>
	void goState(const std::initializer_list<C>& conditions, const std::initializer_list<A>& actions, const ConditionMode& conditionMode = ConditionMode::AllPass, const ActionPosition& actionPosition = ActionPosition::Between)
	{
		if (!processConditions(conditions, conditionMode)) { return; }

		if (actionPosition == ActionPosition::Before) { processActions(actions); }
		exit();
		if (actionPosition == ActionPosition::Between) { processActions(actions); }
		setState<S>();
		enter();
		if (actionPosition == ActionPosition::After) { processActions(actions); }
	}

private:
	template<typename C>
	bool processConditions(const std::initializer_list<C>& conditions, const ConditionMode& conditionMode)
	{
		switch (conditionMode)
		{
		case ConditionMode::AllPass: return std::all_of(conditions.begin(), conditions.end(), [](const auto& c) { return c(); });
		case ConditionMode::OnePass: return std::any_of(conditions.begin(), conditions.end(), [](const auto& c) { return c(); });
		}
	}

	template<typename A>
	void processActions(const std::initializer_list<A>& actions) { for (const auto& a : actions) { a(); } }
};
}
```

#### **Public**  
- **`Machine<M>()`**: Default constructor for the `Machine`. (do not use it)
- **`initialState<S>(executeEnter)`**: Initializes the machine to state `S`. If `executeEnter` is `true`, calls the `enter` method of the state.
- **`isState<S>()`**: Checks if the current state matches the type `S`.
- **`event<E>()`**: Sends an event of type `E` to the current state, invoking its `event` handler if defined.

#### **Protected**  
- **`exit()`**: Invokes the `exit` method of the current state during a transition.
- **`enter()`**: Invokes the `enter` method of the new state during a transition.
- **`setState<S>()`**: Transitions the machine to state `S`, associating it with the machine instance.
- **`goState<S>()`**: Transitions the machine to state `S`, calling `exit` on the current state and `enter` on the new state.
- **`goState<S, C>(conditions, conditionMode)`**: Transitions to state `S` if conditions are met, based on the specified `conditionMode` (`AllPass` or `OnePass`).
- **`goState<S, A>(actions, actionPosition)`**: Transitions to state `S` and executes actions based on `actionPosition` (`Before`, `Between`, or `After`).
- **`goState<S, A, C>(conditions, actions, conditionMode, actionPosition)`**: Combines conditions and actions to transition to state `S`, evaluating conditions and executing actions as specified.

#### **Private**  
- **`processConditions<C>(conditions, conditionMode)`**: Evaluates a list of conditions based on the specified mode (`AllPass` or `OnePass`).
- **`processActions<A>(actions)`**: Executes a list of actions sequentially.

**Constructor**

Without `using namespace sisState;`

```cpp
struct MachineName : sisState::Machine<MachineName>
{
	...
}
```

<br/>

With `using namespace sisState;`

```cpp
struct MachineName : Machine<MachineName>
{
	...
}
```

<br/>

*Example of a state machine declaration.*

```cpp
struct SunMachine : sisState::Machine<SunMachine>
{
};
```