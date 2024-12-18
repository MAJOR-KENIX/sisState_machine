## **Conditions**

The Condition type is a function object representing a condition to be evaluated. It controls state transitions in a state machine by determining whether a specific condition is satisfied.

**Implementations**

```cpp
namespace sisState
{
	using Condition = std::function<bool()>;
}
```

<br/>

**Enum class `ConditionMode`**

The `ConditionMode` enum class defines two evaluation modes for conditions in a state machine:

* `OnePass`: Evaluates each condition individually. If any of the conditions is true, the state transition occurs.
* `AllPass`: Evaluates all conditions. The state transition only occurs if all conditions are true.

These two modes allow control over the behavior of the state machine based on the combination of required conditions for a state change.

The default value is `AllPass`.

**Implementations**

```cpp
namespace sisState
{
	enum class ConditionMode : uint8_t { OnePass, AllPass };
}
```

<br/>

**Constructor**

Without `using namespace sisState;`

```cpp
sisState::Condition ConditionName_0 = [] { /* ... */ };
sisState::Condition ConditionName_1 = [&] { /* ... */ };
```

<br/>

With `using namespace sisState;`

```cpp
Condition ConditionName_0 = [] { /* ... */ };
Condition ConditionName_1 = [&] { /* ... */ };
```

<br/>

**Examples**

Without `using namespace sisState;`

```cpp
sisState::Condition isConnected = [] { /* ... */ };
sisState::Condition isInMainMenu = [&] { /* ... */ };
sisState::Condition isInSelectionMode = [&] { /* ... */ };
```

<br/>

With `using namespace sisState;`

```cpp
Condition isConnected = [] { /* ... */ };
Condition isInMainMenu = [&] { /* ... */ };
Condition isInSelectionMode = [&] { /* ... */ };
```

<br/>

*Conditions must be declared at the end of the state machine definition, but prior to any actions.*

```cpp
struct MachineName : sisState::Machine<MachineName>
{
	...

	sisState::Condition isConnected  = [] { /* ... */ };
	sisState::Condition isInSelectionMode = [&] { /* ... */ };
	sisState::Condition isInMainMenu = [&] { /* ... */ };

	...
}
``` 