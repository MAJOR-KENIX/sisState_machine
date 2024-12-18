## **Events**

The structure Event provides a basic model for managing events, enabling the propagation and processing of information within a system.

**Implementations**

```cpp
namespace sisState
{
	struct Event { virtual ~Event() = default; };
}
```

<br/>

**Constructor**

Without `using namespace sisState;`

```cpp
struct EventName : sisState::Event {};
```

<br/>

With `using namespace sisState;`

```cpp
struct EventName : Event {};
```

<br/>

**Examples**

Without `using namespace sisState;`

```cpp
struct onDrop : sisState::Event {};
struct onError : sisState::Event {};
struct onSuccess : sisState::Event {};
```

<br/>

With `using namespace sisState;`

```cpp
struct onDrop : Event {};
struct onError : Event {};
struct onSuccess : Event {};
```

<br/>

*All state-specific Event structures defined for a finite state machine must be declared prior to the definition of the Machine or positioned at the beginning of the Machine's definition (After the variables, if there are any).*

```cpp
struct onDrop : sisState::Event {};
struct onError : sisState::Event {};
struct onSuccess : sisState::Event {};

struct DropFile : sisState::Machine<DropFile>
{
	...
};
```

-OR-

```cpp
struct DropFile : sisState::Machine<DropFile>
{
	struct onDrop : sisState::Event {};
	struct onError : sisState::Event {};
	struct onSuccess : sisState::Event {};

	...
};
```