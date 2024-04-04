#include "../headers/Ui.h"
#include <algorithm>

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

std::vector<Vertex *> getSearchVertexes(Graph &graph, std::string searchTerm)
{
	std::vector<Vertex *> result;

	if (searchTerm.empty())
		return result;
	for (auto vtx : graph.getVertexSet())
	{
		Element *elem = vtx->getInfo();
		if (dynamic_cast<Reservoir *>(elem) != nullptr)
			continue;
		if (strFind(elem->getCode(), searchTerm))
		{
			result.push_back(vtx);
			continue;
		}
		City *city = dynamic_cast<City *>(elem);
		if (city != nullptr)
		{
			if (strFind(city->getName(), searchTerm))
			{
				result.push_back(vtx);
				continue;
			}
		}
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

void UI::maxFlowMenu()
{
	int totalFlow = manager.CalculateMaxFlow();
	Graph graph = manager.getNetwork();
	std::vector<Vertex *> lst;
	for (Vertex *vtx : graph.getVertexSet())
	{
		if (dynamic_cast<Reservoir *>(vtx->getInfo()) != nullptr)
			continue;
		lst.push_back(vtx);
	}
	size_t count = 0;
	std::string str;
	int totalPages = (lst.size() + 9 - (lst.size() - 1) % 10) / 10;

	while (1)
    {
        CLEAR;
        std::cout 
		<< "Service Metrics\n"
		<< "\n"
		<< "The total max flow for the network is: " << totalFlow << "\n\n"
		<< "Max flow for all/searched elemens:\n\n";
		if (!lst.empty())
		{
			for (size_t i = count; i < std::min(count + 10, lst.size()); i++)
			{
				auto it = lst.begin();
				std::advance(it, i);
				
				Vertex *w = *it;
				City *city = dynamic_cast<City *>(w->getInfo());
				Reservoir *reserv = dynamic_cast<Reservoir *>(w->getInfo());

				std::cout << w->getInfo()->getCode();
				if (city != nullptr) 
					std::cout << " (" << city->getName() << ")";
				if (reserv != nullptr) 
					std::cout << " (" << reserv->getName() << " / " << reserv->getMunicipality() << ")";
				std::cout << " - " << getFlow(w) << "\n";
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
		<< "[B] - Back \t\t[Q] - Exit\n"
		<< "\n"
		<< "You can search the max flow for a specific city by typing it below or use one of the commands above\n"
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
		if (!str.empty())
		{
			lst = getSearchVertexes(graph, str);
			continue;
		}
		helpMsg("Invalid command!", "[next/back/b/q/(search term)]");
    }
}

double CalculateMaxFlow();