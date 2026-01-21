#include <iostream>
#include <chrono>

#include "restaurant.hpp"
#include "runnerstn.hpp"
#include "bill.hpp"
#include "cashier.hpp"
#include "payment.hpp"
#include "station.hpp" 
#include "walkin.hpp"

using namespace pos;

int main() {

	std::cout << "=== POS Project Test ===\n";
	
	
	Restaurant rest(1, "Hello Bistro");

	rest.reservations().createReservation(100, "Alice", 2, std::chrono::system_clock::now());
	rest.reservations().createReservation(101, "Bob", 4, std::chrono::system_clock::now());
	
	rest.tables().createTable(1, "T1", 4); 
	rest.tables().createTable(2, "T2", 2);


	rest.reservations().assignTable(100, 1);


    rest.walkins().addWalkIn(200, "Charlie", 2, std::chrono::system_clock::now(), false);
    rest.walkins().addWalkIn(201, "Dana", 3, std::chrono::system_clock::now(), true);


    rest.orders().addOrder(Order{"ORD-001", "1"}); // table 1
    rest.orders().findOrder("ORD-001")->get().addItem("Burger", 2);
    rest.orders().findOrder("ORD-001")->get().addItem("Fries", 1);

    rest.orders().addOrder(Order{"ORD-002", "2"}); // table 2
    rest.orders().findOrder("ORD-002")->get().addItem("Salad", 1);


    rest.tickets().addTicket(KitchenTicket{300, "ORD-001",{{"Burger",2},{"Fries",1}}});
    rest.tickets().addTicket(KitchenTicket{301, "ORD-002",{{"Salad",1}}});

    Station grill(10, "Grill Station");
    Station salad(11, "Salad Station");
    rest.expeditor().registerStation(grill);
    rest.expeditor().registerStation(salad);

    rest.expeditor().routeTicket(300, 10);
    rest.expeditor().routeTicket(301, 11);


    RunnerStation runner(400, "Bob Runner");
    runner.dispatch(300);
    runner.dispatch(301);


    Bill bill1(500, "ORD-001");
    bill1.addItem("Burger", 12.50);
    bill1.addItem("Fries", 4.00);
    bill1.calculateTotals(0.10);
    rest.cashier().addBill(bill1);

    Bill bill2(501, "ORD-002");
    bill2.addItem("Salad", 6.00);
    bill2.calculateTotals(0.10);
    rest.cashier().addBill(bill2);


    Payment pay1(600, bill1.id(), "Card", bill1.total());
    pay1.process();
    rest.cashier().settleBill(bill1.id());

    Payment pay2(601, bill2.id(), "Cash", bill2.total());
    pay2.process();
    rest.cashier().settleBill(bill2.id());


    std::cout << "\n=== Restaurant Summary ===\n" << rest.summary() << "\n";
    std::cout << "\n=== Runner Summary ===\n" << runner.summary() << "\n";
    std::cout << "\n=== Bill 1 Summary ===\n" << bill1.summary() << "\n";
    std::cout << "\n=== Bill 2 Summary ===\n" << bill2.summary() << "\n";
    std::cout << "\n=== Payment 1 Summary ===\n" << pay1.summary() << "\n";
    std::cout << "\n=== Payment 2 Summary ===\n" << pay2.summary() << "\n";

    std::cout << "=== End of Test ===\n";
    return 0;
}