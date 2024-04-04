#include "../headers/Manager.h"
#include "limits.h"
#include "list"


double Manager::FarthestAugmentingPath(list<Edge*>& biggestPath,
                                       Vertex*& last) {
    // the values for the final flow
    double finalFlow = 0;
    int actualTopDistance = 0;

    // Go through each reservoir and detect more distant path
    for (const auto& [code, reservoir] : reservoirs) {

        // Reset the values for that reservoir
        int topDistance = 0;
        double newFlow = 0;
        Vertex* fakeLast;

        // find reservoir
        Vertex* source = network.findVertex(reservoir);
        // Calculate already sending flow
        double alreadySending=0;
        for (Edge* e : source->getAdj()){
            alreadySending+=e->getFlow();
        }

        // Create queue with reservoir and remaining sending flow
        queue<pair<Vertex*, double>> q;
        q.push({source, reservoir->getMaxDelivery()-alreadySending});

        // Clean the graph 
        for (Vertex* v : network.getVertexSet()) {
            v->setPath(nullptr);
            v->setDist(1);
        }

        while (!q.empty()) {
            Vertex* v = q.front().first;
            double flow = q.front().second;
            q.pop();
            if (!flow) continue;

            // Save info of the last visited City
            if (v->getInfo()->getCode()[0] == 'C') {
                double totalIncoming = 0;
                for (Edge* e : v->getIncoming()){
                    totalIncoming+=e->getFlow();
                }
                double availableSpace = dynamic_cast<City*>(v->getInfo())->getDemand()-totalIncoming;
                if (availableSpace){
                    fakeLast = v;
                    topDistance = v->getDist();
                    newFlow = min(flow, availableSpace);
                }
            }

            // See outgoing edges
            for (Edge* e : v->getAdj()) {
                Vertex* d = e->getDest();
                Edge* reverseEdge = e->getReverse();
                // Only follow direct path if there is space and no reverse edge
                // Or no flow in reverse Edge
                if ((d->getPath() == nullptr) &&
                    (e->getFlow() < e->getWeight()) && (!reverseEdge || reverseEdge->getFlow()==0)) {
                    d->setPath(e);
                    double possibleFlow = std::min(flow, e->getWeight() - e->getFlow());
                    d->setDist(v->getDist() + 1);
                    q.push({d, possibleFlow});
                }
            }


            for (Edge* e : v->getIncoming()) {
                Vertex* o = e->getOrig();
                // Only if path has already flow 
                // So its possible to redirect water
                if ((o->getPath() == nullptr) && (e->getFlow() > 0)) {
                    double possibleFlow;
                    o->setPath(e);

                    if (e->getReverse()){
                        possibleFlow = std::min(flow, e->getWeight()+e->getFlow());
                        o->setDist(v->getDist());
                    }else{
                        possibleFlow = std::min(flow, e->getFlow());
                        o->setDist(v->getDist() -1);
                    }
                    q.push({o, possibleFlow});
                }
            }
        }

        // Check if distant path from reservoir is the biggest
        if (topDistance > actualTopDistance || (topDistance == actualTopDistance && newFlow > finalFlow)) {
            // std::cout << " --- NEW PATH --- \n";
            // cout << topDistance << " -- " << biggestPath.size() << "\n";
            list<Edge*> newBiggestPath;
            Vertex* node = fakeLast;
            // std::cout << "LAST INSIDE BEFORE: " << last->getInfo()->getCode() << '\n';
            while (node != source) {
                // std::cout << node->getInfo()->getCode() << " - " << std::flush;
                Edge* e = node->getPath();
                newBiggestPath.push_back(e);
                if (e->getOrig() == node){
                    // Backedge
                    node = e->getDest();
                    if (e->getReverse() && newFlow < e->getFlow()){
                        // Edge direction is not reversed then flow in that pipe is decreased
                        // So update distance because it increaseses empty space
                        topDistance--;
                    }
                }else{
                    node = e->getOrig();
                }
            }

            if (topDistance > actualTopDistance || (topDistance == actualTopDistance && newFlow > finalFlow)) {
                // update the values
                biggestPath = newBiggestPath;
                actualTopDistance = topDistance;
                last = fakeLast;
                finalFlow = newFlow;
            }
            // Check if changed anything
            // std::cout << source->getInfo()->getCode();
            // std::cout << "\n";
            // std::cout << "LAST INSIDE AFTER: " << last->getInfo()->getCode() << '\n';
        }
    }
    // std::cout << "BEFORE LAST :" << last->getInfo()->getCode() << '\n';
    return finalFlow;
}

void Manager::balanceNetwork() {
    CalculateMaxFlow();
    tuple<double, double, double> result = AnalyzeBalance();
    std::cout << std::get<0>(result) << " - " << std::get<1>(result) << " - "
              << std::get<2>(result) << "\n";

    // Clean graph
    for (Vertex* v : network.getVertexSet()) {
        for (Edge* e : v->getAdj()) {
            e->setFlow(0);
        }
    }
    unordered_set<string> reservoirs;
    list<Edge*> biggestPath;
    Vertex* last;
    double flow;
    while (flow = FarthestAugmentingPath(biggestPath, last)) {
        // std::cout << "AFTER LAST :" << last->getInfo()->getCode() << '\n';
        // std::cout << "\n\n\n SELECTED \n";
        // std::cout << "From: ";
        for (Edge* e : biggestPath) {
            // std::cout << last->getInfo()->getCode() << " - ";
            // std::cout << last->getInfo()->getCode() << " - ";
            if (last == e->getDest()) {
                last = e->getOrig();
                e->setFlow(e->getFlow() + flow);
            } else {
                last = e->getDest();
                if (flow > e->getFlow()){
                    e->getReverse()->setFlow(flow-e->getFlow());
                    e->setFlow(0);
                }else{
                    // Detect if flow in bidirected changes direction
                    e->setFlow(e->getFlow() - flow);
                }
            }
        }
        reservoirs.insert(last->getInfo()->getCode());
        // std::cout << " -- To: " << last->getInfo()->getCode() << " with size: " << biggestPath.size()+1 << " and flow: " << flow << "\n\n\n";
        biggestPath.clear();
        // std::cout << '\n';
    }

    result = AnalyzeBalance();
    std::cout << std::get<0>(result) << " - " << std::get<1>(result) << " - "
              << std::get<2>(result) << "\n";

    double total = 0;
    for (const auto& [code, city] : cities){
        for (Edge* e : network.findVertex(city)->getIncoming()){
            total += e->getFlow();
        }
    }
    std::cout << "With flow: " << total << '\n';
}

tuple<double, double, double> Manager::AnalyzeBalance() {
    double biggest = numeric_limits<double>::min();
    double smallest = numeric_limits<double>::max();
    double sum = 0;
    double numEdges = 173;
    double totalPeso = 0;
    int count = 0;
    double totalEmpty = 0;

    for (Vertex* v : network.getVertexSet()) {
        for (Edge* e : v->getAdj()) {
            if (e->getFlow() < 0) count++;
            if (e->getFlow() == 0) totalEmpty++;
            totalPeso += e->getWeight();
            double diff = e->getWeight() - e->getFlow();
            sum += diff;
            biggest = max(biggest, diff);
            if (!e->getReverse() || !e->getReverse()->getFlow()) {
                smallest = min(smallest, diff);
            }
        }
    }

    double totalOutput = 0;
    for (const auto& [c, r] : reservoirs){
        for (Edge* e : network.findVertex(r)->getAdj()){
            totalOutput+=e->getFlow();
        }
    }
    
    std::cout << "smallest: " << smallest << '\n';
    std::cout << "Pipes negativos: " << count << '\n';
    std::cout << "Total weight: " << totalPeso << '\n';
    std::cout << "Total empty: " << totalEmpty << '\n';
    std::cout << "Total output: " << totalOutput << '\n';
    return {sum / numEdges, biggest - smallest, biggest};
}