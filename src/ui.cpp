#include "../headers/Ui.h"

int main()
{
    UI();
    return 0;
}

UI::UI() : manager(Manager()) {
	auto start = std::chrono::high_resolution_clock::now();
    manager.loadCities();
	manager.loadPipes();
	manager.loadReservoirs();
	manager.loadStations();
	auto end = std::chrono::high_resolution_clock::now();
	loadtime = std::chrono::duration<double>(end - start).count();
	mainMenu();
}

void UI::mainMenu()
{
	while (true)
	{
		CLEAR;

	}
}