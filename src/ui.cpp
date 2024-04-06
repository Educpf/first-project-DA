#include "../headers/Ui.h"

#ifdef _WIN32
#include "windows.h"
#endif

int main(int argc, char **argv)
{
#ifdef _WIN32
	SetConsoleOutputCP(CP_UTF8);
#endif
	(void)argv;
    UI(argc > 1);
    return 0;
}

UI::UI(bool useSmallSet) : manager(Manager(useSmallSet))
{
	auto start = std::chrono::high_resolution_clock::now();
    manager.loadCities();
	manager.loadReservoirs();
	manager.loadStations();
	manager.loadPipes();
	manager.maxFlowCities();
	manager.citiesInDeficit();
	manager.maintenancePipes();
	manager.maintenancePS();
	auto end = std::chrono::high_resolution_clock::now();
	loadtime = std::chrono::duration<double>(end - start).count();
	mainMenu();
}

void UI::mainMenu()
{
	while (1)
    {
        CLEAR;
        std::cout 
		<< "Flows\n"
        << "\n"
#ifdef _linux__
        << "Welcome " << getenv("USER") << "!\n"
#else
        << "Welcome!\n"
#endif
		<< "The system was loaded in " << loadtime << "s.\n"
		<< "Select an option:\n"
        << "\n"
		<< ">> Basic Service Metrics\n"
        << " [1] Max Flow\n"
		<< " [2] Cities in water deficit\n"
		<< " [3] Balance Network\n"
        << "\n"
		<< ">> Reliability\n"
		<< " [4] Reservoir removal\n"
		<< " [5] Pump Station removal\n"
		<< " [6] Pipeline removal\n"
		<< "\n"
		<< "[Q] Exit\n"
		<< "\n"
        << "$> ";
        std::string str;
		std::getline(std::cin, str);

		if (str.size() == 1)
		{
			switch (str[0])
			{
				case 'q':
				case 'Q':
					CLEAR;
					exit(0);
					break;
				case '1':
					maxFlowMenu();
					break;
				case '2':
					meetDemandsMenu();
					break;
				case '3':
					balancedFlowMenu();
					break;
				case '4':
					removeReservoirMenu();
					break;
				case '5':
					removePSMenu();
					break;
				case '6':
					removePipeMenu();
					break;
				case 't':
				case 'T':
					testMenu();
					break;
			}
			continue;
		}
    }
}

void UI::helpMsg(std::string error, std::string usage)
{
	CLEAR;
	std::cout << "Flows\n\n";
	if (error != "" && usage != "") {
		std::cout << "Invalid operation!\n"
				  << "\n Problem: " << error
				  << "\n Usage: " << usage
				  << "\n\nPress ENTER to continue...";
	} else {
		std::cout << "No help available for this page."
				  << "\n\nPress ENTER to continue...";
	}
	while (std::cin.get() != '\n') { }
}

void UI::testMenu()
{
	CLEAR;
	std::cout << "Testing load\n";
	int count = 0;
	for (auto i : manager.allElements)
	{
		std::cout << i.first << " -> " << i.second->getCode() << " + " << i.second->getId() << "\n";
		count++;
	}
	std::cout << "\nGot " << count << " entries\nPress ENTER to continue...";
	while (std::cin.get() != '\n') { }
}