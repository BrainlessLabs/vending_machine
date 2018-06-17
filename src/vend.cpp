// vend.cpp : Defines the entry point for the console application.
//

#include "coin.hpp"
#include "coin_manager.hpp"
#include "vending_machine.hpp"
#include "application.hpp"
#include <iostream>
#include <CppConsoleTable.hpp>
#include <rang.hpp>

namespace vm {
	using ConsoleTable = samilton::ConsoleTable;
	class ConsoleApplication : ApplicationI {
	public:
		using VendingMachineType = VendingMachineManager<CoinManager<vm::Coin<>>>;
	private:
		VendingMachineType _vending_machine;
		const std::string _spaces;
		
		void printHeader() const {
			std::cout << rang::style::bold << rang::bg::blue
				<< "No" << _spaces
				<< "Unit Name" << _spaces
				<< "Quantity" << _spaces
				<< "Price" << _spaces
				<< "Description" << _spaces
				<< rang::style::reset << std::endl;
		}

		void printSKU(const std::uint32_t unit_no,
			std::string const& unit_name,
			const std::uint32_t quantity,
			const VendingMachineType::CoinValueType price,
			std::string const& description) const {
			std::cout << 
				rang::fg::yellow
				<< unit_no << _spaces
				<< rang::style::reset << rang::fg::green
				<< unit_name << _spaces
				<< quantity << _spaces
				<< price << _spaces
				<< description << _spaces
				<< rang::style::reset << std::endl;
		}

		void printSKUs(std::string const& skus) {
			const json j = json::parse(skus);
			const auto items = j["items"];
			for (const auto item : items) {
				printSKU(item["number"], item["item"], item["quantity"], item["price"], item["description"]);
			}
		}
	public:
		ConsoleApplication() : _vending_machine({1, 2, 5, 10}), _spaces("     "){
			initianize();
		}

		virtual void initianize() {
			_vending_machine.addSKU("Coke", 12.5, 1000, "Coke Bottle");
			_vending_machine.addSKU("Sprite", 11, 1000, "Sprite Bottle");
			_vending_machine.addSKU("ThumbsUp", 20, 1000, "ThumbsUp Bottle");
		}

		virtual void run() {
			VendingMachineType::CoinBucketType coinBucket = { { 1, 2 },{ 2, 4 },{ 5, 1 },{ 10, 5 } };
			std::cout << _vending_machine.getSKUs() << std::endl;
			std::cout << _vending_machine.canPurchase("Coke", 11) << std::endl;
			std::cout << _vending_machine.purchase("Coke", coinBucket) << std::endl;
			printHeader();
			printSKUs(_vending_machine.getSKUs());
		}
		
		virtual void uninitialize() {

		}
	};
}

int main() {
	vm::CoinManager<vm::Coin<>> cp;
	vm::CoinManager<vm::Coin<>> cp1 = {1, 2, 5, 10};
	vm::ConsoleApplication console_app;
	console_app.run();
    return 0;
}
