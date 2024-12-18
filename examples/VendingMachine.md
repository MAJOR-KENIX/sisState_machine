## **Example VendingMachine**

```cpp
#include <iostream>
#include <sisState.hpp>
#include <string>

using namespace sisState;

struct VendingMachine : Machine<VendingMachine>
{
	bool hasMoney = false;
	bool productSelected = false;

	struct MoneyInserted : Event {};
	struct ProductSelected : Event {};
	struct ProductDispensed : Event {};
	struct ResetMachine : Event {};

	struct Idle : State<VendingMachine>
	{
		void enter() override { std::cout << "Vending Machine is Idle. Please insert money." << std::endl; }

		void event(const MoneyInserted&) { this->machine->goState<AwaitingSelection>(); }
	};

	struct AwaitingSelection : State<VendingMachine>
	{
		void enter() override { std::cout << "Money inserted. Please select a product." << std::endl; }

		void event(const ProductSelected&) { this->machine->goState<DispensingProduct>(); }
	};

	struct DispensingProduct : State<VendingMachine>
	{
		void enter() override
		{
			std::cout << "Dispensing your product..." << std::endl;
			this->machine->dispenseProduct();
		}

		void event(const ProductDispensed&) { this->machine->goState<Idle>(); }
	};

	struct ErrorState : State<VendingMachine>
	{
		void enter() override { std::cout << "Error detected! Please reset the machine." << std::endl; }

		void event(const ResetMachine&) { this->machine->goState<Idle>(); }
	};

	Condition isMoneyInserted = [&]() { return hasMoney; };
	Condition isProductChosen = [&]() { return productSelected; };

	std::variant<Idle, AwaitingSelection, DispensingProduct, ErrorState> state{};

	void insertMoney()
	{
		hasMoney = true;
		std::cout << "Money inserted." << std::endl;
		this->event<MoneyInserted>();
	}

	void selectProduct()
	{
		if (hasMoney)
		{
			productSelected = true;
			std::cout << "Product selected." << std::endl;
			this->event<ProductSelected>();
		}
		else { std::cout << "Insert money first!" << std::endl; }
	}

	void dispenseProduct()
	{
		if (productSelected)
		{
			std::cout << "Product dispensed. Thank you for your purchase!" << std::endl;
			hasMoney = false;
			productSelected = false;
			this->event<ProductDispensed>();
		}
		else
		{
			std::cout << "No product selected to dispense." << std::endl;
			this->goState<ErrorState>();
		}
	}

	void resetMachine()
	{
		std::cout << "Resetting machine..." << std::endl;
		hasMoney = false;
		productSelected = false;
		this->event<ResetMachine>();
	}
};

int main()
{
	VendingMachine vendingMachine = VendingMachine::initialState<VendingMachine::Idle>(true);

	std::cout << "--- Insert Money ---" << std::endl;
	vendingMachine.insertMoney();
	std::cout << std::endl;

	std::cout << "--- Select Product ---" << std::endl;
	vendingMachine.selectProduct();
	std::cout << std::endl;

	std::cout << "--- Reset Machine ---" << std::endl;
	vendingMachine.resetMachine();

	return 0;
}

```

<br/>

Output

```
Vending Machine is Idle. Please insert money.
--- Insert Money ---
Money inserted.
Money inserted. Please select a product.

--- Select Product ---
Product selected.
Dispensing your product...
Product dispensed. Thank you for your purchase!
Vending Machine is Idle. Please insert money.

--- Reset Machine ---
Resetting machine...
```