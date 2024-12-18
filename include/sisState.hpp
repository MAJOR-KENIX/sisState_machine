/**********************************************\
				• MAJOR-KENIX •
			  • SIS STATE V 1.3 •
				 • MIT 2024 •
\**********************************************/

#pragma once

#include <memory>
#include <variant>
#include <algorithm>
#include <functional>
#include <initializer_list>

namespace sisState
{
	struct Event { virtual ~Event() = default; };

	using Condition = std::function<bool()>;
	enum class ConditionMode : uint8_t { OnePass, AllPass };

	using Action = std::function<void()>;
	enum class ActionPosition : uint8_t { Before, Between, After };

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