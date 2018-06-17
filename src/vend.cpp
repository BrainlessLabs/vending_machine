// vend.cpp : Defines the entry point for the console application.
//

#include "coin.hpp"
#include "coin_manager.hpp"
#include "vending_machine.hpp"
#include "application.hpp"
#include <iostream>

namespace vm {
	class ConsoleApplication : ApplicationI {
	private:
		VendingMachineManager<CoinManager<vm::Coin<>>> _vending_machine;
	public:
		ConsoleApplication() : _vending_machine({1, 2, 5, 10}) {
			initianize();
		}

		virtual void initianize() {
			_vending_machine.addSKU("Coke", 12.5, 1000, "Coke Bottle");
			_vending_machine.addSKU("Sprite", 11, 1000, "Sprite Bottle");
			_vending_machine.addSKU("ThumbsUp", 20, 1000, "ThumbsUp Bottle");
		}

		virtual void run() {
		}
		
		virtual void uninitialize() {

		}
	};
}

int main() {
	vm::CoinManager<vm::Coin<>> cp;
	vm::CoinManager<vm::Coin<>> cp1 = {1, 2, 5, 10};
	using VendingMachineType = vm::VendingMachineManager<vm::CoinManager<vm::Coin<>>>;
	VendingMachineType::CoinBucketType coinBucket = { {1, 2}, {2, 4}, {5, 1}, {10, 5} };
	VendingMachineType vending_machine = { 1, 2, 5, 10 };
	vending_machine.addSKU("Coke", 12.5, 1000, "Coke Bottle");
	vending_machine.addSKU("Sprite", 11, 1000, "Sprite Bottle");
	vending_machine.addSKU("ThumbsUp", 20, 1000, "ThumbsUp Bottle");
	std::cout << vending_machine.canPurchase("Coke", 11) << std::endl;
	std::cout << vending_machine.purchase("Coke", coinBucket) << std::endl;
	vm::ConsoleApplication console_app;
    return 0;
}
