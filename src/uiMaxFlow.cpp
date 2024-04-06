#include "../headers/Ui.h"
#include <algorithm>
#include <fstream>
#include <chrono>
#include <iomanip>

/**
 * Searches a string in another string.
 * This search is case-insensitive.
 * @param one The string to search
 * @param two The string to be searched
 * @return If there are any matches
*/
bool strFind(const std::string &one, const std::string &two) {
	auto it = std::search(one.begin(), one.end(), two.begin(), two.end(),
    	[](unsigned char a, unsigned char b) { return std::toupper(a) == std::toupper(b);}
  		);
  	return (it != one.end());
}

std::unordered_map<std::string, int> getSearchVertexes(Graph &graph, Manager &manager, std::string searchTerm)
{
	std::unordered_map<std::string, int> result;

	if (searchTerm.empty())
		return result;
	for (auto city : manager.getCities())
	{
		if (strFind(city.second->getName(), searchTerm))
			result[city.first] = manager.maxFlows[city.first];
	}
	return result;
}

int getFlow(Vertex *elem)
{
	int total = 0;
	for (auto i : elem->getIncoming())
		total += i->getFlow();
	return total;
}

void saveMaxFlow(Graph &graph, std::unordered_map<std::string, int> &lst, int maxFlow)
{
	std::ofstream out("./maxFlowOutput.txt", std::ofstream::trunc);

	auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

	out
	<< "Information saved at " << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X") << "\n"
	<< "The max flow for the network is: " << maxFlow << "\n"
	<< "\n"
	<< "The max flow for each element is the following:\n\n";

	for (auto w : lst)
	{
		City *city = dynamic_cast<City *>(graph.findVertexByCode(w.first)->getInfo());

		if (city == nullptr)
			continue;
		out << city->getCode() << " (" << city->getName() << ")" << " - " << w.second << "\n";
	}
}

void UI::maxFlowMenu()
{
	int totalFlow = manager.CalculateMaxFlow();
	Graph graph = manager.getNetwork();
	std::unordered_map<std::string, int> lst = manager.maxFlows;

	size_t count = 0;
	std::string str;
	std::string search;
	int totalPages = (lst.size() + 9 - (lst.size() - 1) % 10) / 10;

	while (1)
    {
        CLEAR;
        std::cout 
		<< "Service Metrics\n"
		<< "\n"
		<< "The total max flow for the network is: " << totalFlow << "\n\n"
		<< "Max flow for all cities" << (search.empty() ? "" : " containing \"" + search + "\"") << ":\n\n";
		if (!lst.empty())
		{
			for (size_t i = count; i < std::min(count + 10, lst.size()); i++)
			{
				auto it = lst.begin();
				std::advance(it, i);
				Element *x = graph.findVertexByCode(it->first)->getInfo();
				City *city = dynamic_cast<City *>(x);
				std::cout << x->getCode();
				if (city != nullptr) 
					std::cout << " (" << city->getName() << ")";
				std::cout << " - " << it->second << "\n";
			}
			std::cout << "\nPage " << (count + 10 - count % 10) / 10 << " of " 
						<< totalPages << "\n";
			std::cout << "Total count: " << lst.size() << "\n";
		} 
		else
		{
			std::cout << "The search returned for " << str << " no results.\n";
		}

		std::cout
		<< "\n"
		<< (lst.empty() ? "" : "[back] - Previous page\t[next] - Next page\n")
		<< (lst.empty() ? "" : "[page (integer)] - Select a specific page\n")
		<< "[reset] Reset search\n" 
		<< (lst.empty() ? "" : "[save] Save displayed information to a file.\n")
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
			count = count + 10 < lst.size() + lst.size() % 10 ? count + 10 : count;
			continue;
		}

		if (str == "back" && !lst.empty())
		{
			count = count < 10 ? 0 : count - 10;
			continue;
		}
		if (str == "save")
		{
			saveMaxFlow(graph, lst, totalFlow);
			CLEAR;
			std::cout << "Saved current search to \"./maxFlowOutput.txt\".\nPress ENTER to continue...";
			while (std::cin.get() != '\n') { }
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

		if (str == "reset")
		{
			search = "";
			lst = manager.maxFlows;
			continue;
		}

		if (!str.empty())
		{
			lst = getSearchVertexes(graph, manager, str);
			search = str;
			continue;
		}
		helpMsg("Invalid command!", "[next/back/b/q/(search term)]");
    }
}