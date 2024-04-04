#include "../headers/Ui.h"

std::vector<Vertex *> getSearchVertexes(Graph &graph, std::string searchTerm)
{
	std::vector<Vertex *> result;

	if (searchTerm.empty())
		return result;
	std::transform(searchTerm.begin(), searchTerm.end(), searchTerm.begin(), ::toupper);
	for (auto vtx : graph.getVertexSet())
	{
		Element *elem = vtx->getInfo();
		std::string code = elem->getCode();
		if (code == searchTerm)
			return {vtx};
		if (std::find(code.begin(), code.end(), searchTerm) != code.end())
		{
			result.push_back(vtx);
			continue;
		}
		City *city = dynamic_cast<City *>(elem);
		if (city != nullptr)
		{
			std::string name = city->getName();
			std::transform(name.begin(), name.end(), name.begin(), ::toupper);
			if (std::find(name.begin(), name.end(), searchTerm) != name.end())
			{
				result.push_back(vtx);
				continue;
			}
		}
		Reservoir *reserv = dynamic_cast<Reservoir *>(elem);
		if (reserv != nullptr)
		{
			std::string mName = reserv->getName();
			std::transform(mName.begin(), mName.end(), mName.begin(), ::toupper);
			if (std::find(mName.begin(), mName.end(), searchTerm) != mName.end())
			{
				result.push_back(vtx);
				continue;
			}
			std::string munic = reserv->getMunicipality();
			std::transform(munic.begin(), munic.end(), munic.begin(), ::toupper);
			if (std::find(munic.begin(), munic.end(), searchTerm) != munic.end())
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
	std::vector<Vertex *> lst = graph.getVertexSet();
	size_t count = 0;
	std::string str;
	int totalPages = (lst.size() + 9 - (lst.size() - 1) % 10) / 10;

	while (1)
    {
        CLEAR;
        std::cout 
		<< "Service Metrics\n"
		<< "\n"
		<< "The total max flow for the network is:" << totalFlow << "\n\n";
		if (!lst.empty())
		{
			for (size_t i = count; i < std::min(count + 10, lst.size()); i++) {
				auto it = lst.begin();
				std::advance(it, i);
				auto w = *it;
				std::cout << w->getInfo()->getCode() << " - " <<  getFlow(w) << ")\n";
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
			std::vector<Vertex *> result = getSearchVertexes(graph, str);
			lst = result;
			continue;
		}
		helpMsg("Invalid command!", "[next/back/b/q/(integer)]");
    }
}

double CalculateMaxFlow();