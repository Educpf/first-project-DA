#include "../headers/Manager.h"
#include <forward_list>

void Manager::removeReservoir(Reservoir* reservoir){

    CalculateMaxFlow();

    // Clean all the edges
    for (Vertex* v : network.getVertexSet()){
        for (Edge* e : v->getAdj()){
            e->setSelected(false);
        }
        v->setVisited(false);
    }

    // Do BFS and store the affected pumping stations
    unordered_set<Vertex*> affectedStations;

    queue<Vertex*> q;
    q.push(network.findVertex(reservoir));
    network.findVertex(reservoir)->setVisited(true);

    while (!q.empty()){
        Vertex* v = q.front();
        q.pop();

        for (Edge* e : v->getAdj()){
            // Record which pipes are the ones affected
            e->setSelected(true);
            Vertex* d = e->getDest();
            if (!d->isVisited()){
                d->setVisited(true);
                affectedStations.insert(d);
                q.push(d);
            }
        }
    }


    // Detect which stations/cities are the connecting ones
    for (Vertex* station : affectedStations){
        for (Edge* e : station->getAdj()){
            e->setFlow(0);
        }
        bool connectingStation = false;
        for (Edge* e : station->getIncoming()){
            // If has not selected incoming edge then is connecting
            if (!e->isSelected()){
                connectingStation = true;
            }else{
                // Reset the flow of the selected edges
                e->setFlow(0);
            }
        }
        if (connectingStation) q.push(station);
    }

    // Use BFS to get the separating vertices
    unordered_set<Vertex*> borderVertices;

    while (!q.empty()){
        Vertex* v = q.front();
        q.pop();

        if (v->getIncoming().size()){
            for (Edge* e : v->getIncoming()){
                Vertex* o = e->getOrig();

                affectedStations.insert(o);
                double flow = e->getFlow();
                e->setFlow(0);
                if (o->isVisited()){
                    continue;
                }

                // If edge is full
                if (flow == e->getWeight()){
                    borderVertices.insert(o);
                }else{
                    // If not remove it from being border
                    if (borderVertices.find(o)!=borderVertices.end()) borderVertices.erase(o);
                    // And add it to queue
                    o->setVisited(true);
                    q.push(o);
                }
            }
        }else{
            // Its a reservoir and then a border Vertice
            borderVertices.insert(v);
        }

        for (Edge* e : v->getAdj()){
            Vertex* d = e->getDest();
            double flow = e->getFlow();
            e->setFlow(0);
            if (d->isVisited()){
                continue;
            }
            if (flow ||(d->getInfo()->getCode()[0] == 'C') ){
                affectedStations.insert(d);
                d->setVisited(true);
                q.push(d);
            }
        }
    }
    
    // Super source for edmondsKarp Algorithm
    Element* superSource = new Element(0, "SS");
    network.addVertex(superSource);

    // Add super sink
    Element* superSink = new Element(0, "ST");
    network.addVertex(superSink);

    for (const auto& [code, sink]  : cities){
        network.addEdge(sink, superSink, sink->getDemand());
    }

    // Connect superSource to the different border Vertices
    for (Vertex* v: borderVertices){

        // TODO:  Maybe substitute for function inside Vertex that returns its type
        string code = v->getInfo()->getCode();
        double capacity;
        switch (code[0])
        {
        case 'P':
            for (Edge* e : v->getIncoming()){
                capacity += e->getFlow();
            }
            break;
        case 'R':
            capacity = dynamic_cast<Reservoir*>(v->getInfo())->getMaxDelivery();
            break;
        default:
            break;
        }
        network.addEdge(superSource, v->getInfo(), capacity);
    }

    affectedStations.insert(network.findVertex(superSink));
    affectedStations.insert(network.findVertex(superSource));

    EdmondsKarp(superSource, superSink, affectedStations);


    // Calculate the total flow in another function
    double total = 0;
    Vertex* superSinkVertex = network.findVertex(superSink);
    for (const auto& [code, c] : cities){
        for (Edge* e :network.findVertex(c)->getIncoming()){
            total+= e->getFlow();
        }
    }

    // For testing porpuses
    std::cout << "Max flow obtained was: " << total << std::endl;

    // Clean network
    network.removeVertex(superSource);
    network.removeVertex(superSink);
    delete superSink;
    delete superSource;
}