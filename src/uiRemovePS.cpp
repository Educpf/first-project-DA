#include "../headers/Ui.h"
#include <algorithm>
#include <fstream>
#include <chrono>
#include <iomanip>

std::unordered_map<std::string, int> getSearchVertexes(Manager &manager, std::string searchTerm)
{
	std::unordered_map<std::string, int> result;

	if (searchTerm.empty())
		return result;
	for (auto city : manager.cities)
	{
		if (UI::strFind(city.second->getName(), searchTerm))
			result[city.first] = manager.maxFlows[city.first];
	}
	return result;
}

void UI::removePSMenu()
{
	RmResult lst = manager.rmPS;

	size_t count = 0;
	std::string str;
	std::string search;
	int totalPages = (lst.size() + 9 - (lst.size() - 1) % 10) / 10;

	while (1)
    {
        CLEAR; 
		std::cout 
		<< "Reliability - Temporary removal of a pump station \n"
		<< "\n"
		<< "Affected locations for all pump stations" << (search.empty() ? "" : " containing \"" + search + "\"") << ":\n\n";
		if (!lst.empty())
		{
			for (size_t i = count; i < std::min(count + 10, lst.size()); i++)
			{
				auto it = lst.begin();
				std::advance(it, i);
				Element *x = manager.network.findVertexByCode(it->first)->getInfo();
				City *city = dynamic_cast<City *>(x);
				std::cout << x->getCode();
				if (city != nullptr) 
					std::cout << " (" << city->getName() << ")";
			}
			std::cout << "\nPage " << (count + 10 - count % 10) / 10 << " of " 
						<< totalPages << "\n";
			std::cout << "Total count: " << lst.size() << "\n";
		}
		else
		{
			std::cout << "The search for " << str << " returned no results.\n";
		}

		std::cout
		<< "\n"
		<< (lst.empty() ? "" : "[back] - Previous page\t[next] - Next page\n")
		<< (lst.empty() ? "" : "[page (integer)] - Select a specific page\n")
		<< "[B] - Back \t\t[Q] - Exit\n"
		<< "\n"
		<< "You can search the max flow for a specific city\n"
		<< "or use one of the commands above\n"
		<< "\n"
        << "$> ";

		getline(std::cin, str);

        if (str == "Q" || str == "q")
		{
			CLEAR;
            exit(0);
		}
		if (str == "B" || str == "b")
			break;

		if (str == "next" && !lst.empty())
		{
			count = count + 10 < lst.size() + (lst.size() + 1 / 10) % 10 ? count + 10 : count;
			continue;
		}

		if (str == "back" && !lst.empty())
		{
			count = count < 10 ? 0 : count - 10;
			continue;
		}

		if (str.substr(0, 4) == "page")
		{
			if (str.size() <= 5 || lst.empty()) {
				helpMsg("There is no page to change to!", "page [num] if there is results");
				continue;
			}
			int page = atoi(str.substr(5).c_str());
			if (page <= 0 || page > totalPages) {
				helpMsg("That page does not exist!", "page [num] if there is results");
				continue;
			}
			count = (page - 1) * 10;
			continue;
		}

		helpMsg("Invalid command!", "[next/back/b/q]");
    }
}
