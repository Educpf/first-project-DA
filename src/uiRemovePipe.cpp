#include "../headers/Ui.h"
#include <algorithm>
#include <fstream>
#include <chrono>
#include <iomanip>

void UI::removePipeMenu()
{
	RmResult lst = manager.rmPipelines;

	size_t count = 0;
	std::string str;
	int totalPages = (lst.size() + 9 - (lst.size() - 1) % 10) / 10;

	while (1)
    {
        CLEAR;
        std::cout 
		<< "Reliability - Temporary removal of a pipe line\n"
		<< "\n"
		<< "By removing X -- Y pipeline, the following cities don't have their demands met:\n\n";
		if (!lst.empty())
		{
			for (size_t i = count; i < std::min(count + 10, lst.size()); i++)
			{
				auto it = lst.begin();
				std::advance(it, i);
				std::cout << it->first << "\n\n";
				for(auto n : it->second)
            		std::cout << "  " << n.first << " - " << manager.maxFlows[n.first] << " --> " << n.second << "\n";
				std::cout << "\n";
			}
			std::cout << "Page " << (count + 10 - count % 10) / 10 << " of " 
						<< totalPages << "\n";
			std::cout << "Total count: " << lst.size() << "\n";
		} 
		else
		{
			std::cout << "This can't be empty. Was the dataset loaded correctly?\n";
		}

		std::cout
		<< "\n"
		<< (lst.empty() ? "" : "[back] - Previous page\t[next] - Next page\n")
		<< (lst.empty() ? "" : "[page (integer)] - Select a specific page\n")
		<< "[B] - Back \t\t[Q] - Exit\n"
		<< "\n"
		<< "You can use one of the commands above\n"
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