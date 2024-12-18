## **Actions**

The Action type is a function object that represents an action to be performed. It is used to encapsulate a block of code that can be executed when a specific event occurs.

**Implementations**

```cpp
namespace sisState
{
	using Action = std::function<void()>;
}
```

<br/>

**Enum class `ActionPosition`**

The `ActionPosition` enum class is used to specify the position of an action within a state transition in a finite state machine. It defines three possible positions:

* `Before`: The action will be executed before the state transition occurs.
* `Between`: The action will be executed during the state transition, after the old state has been exited but before the new state has been entered.
* `After`: The action will be executed after the state transition has occurred.

This enum class allows for more fine-grained control over the execution of actions within a state machine, enabling developers to specify exactly when an action should occur during a state transition.

The default value is `Between`.

<br/>

**Implementations**

```cpp
namespace sisState
{
	enum class ActionPosition : uint8_t { Before, Between, After };
}
```

<br/>

**Constructor**

Without `using namespace sisState;`

```cpp
sisState::Action ActionName_0 = [] { /* ... */ };
sisState::Action ActionName_1 = [&] { /* ... */ };
```

<br/>

With `using namespace sisState;`

```cpp
Action ActionName_0 = [] { /* ... */ };
Action ActionName_1 = [&] { /* ... */ };
```

<br/>

**Examples**

Without `using namespace sisState;`

```cpp
sisState::Action resetValue = [&] { /* ... */ };
sisState::Action updateCustomerDatabase = [] { /* ... */ };
sisState::Action sendConfirmationEmail = [] { /* ... */ };
```

<br/>

With `using namespace sisState;`

```cpp
Action resetValue = [&] { /* ... */ };
Action updateCustomerDatabase = [] { /* ... */ };
Action sendConfirmationEmail = [] { /* ... */ };
```

<br/>

*All state structures defined for a finite state machine must be placed within the machine definition, following the declaration of the variables and events, but preceding the actions and/or conditions.*

```cpp
struct MachineName : sisState::Machine<MachineName>
{
	...

	sisState::Action resetValue = [&] { /* ... */ };
	sisState::Action updateCustomerDatabase = [] { /* ... */ };
	sisState::Action sendConfirmationEmail = [] { /* ... */ };
}
```