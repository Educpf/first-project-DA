#include "../headers/Ui.h"

int main(int argc, char **argv)
{
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
		<< " [2] Menu 2\n"
        << "\n"
		<< "[T] Test\n"
		<< "[Q] Exit\n"
		<< "\n"
        << "$> ";
        std::string str;
		std::getline(std::cin, str);

        if (str == "Q" || str == "q")
		{
			CLEAR;
            exit(0);
		}

		if (str == "1")
		{
			maxFlowMenu();
			continue;
		}

		if (str.size() > 1)
		{
			helpMsg("", "");
			continue;
		}

		if (str == "T" || str == "t")
		{
			testMenu();
			continue;
		}

		switch (str[0] - '0')
		{
			case 1:
				// menu 1
				break;
			case 2:
				// menu 2
				break;
			default:
				helpMsg("Command not found!", "help - shows all commands");
				break;
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
	for (auto i : manager.getAllElements())
	{
		std::cout << i.first << " -> " << i.second->getCode() << " + " << i.second->getId() << "\n";
		count++;
	}
	std::cout << "\nGot " << count << " entries\nPress ENTER to continue...";
	while (std::cin.get() != '\n') { }
}