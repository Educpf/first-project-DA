#include "../headers/Manager.h"
#include <limits.h>
#include <list>

double Manager::CalculateMaxFlow()
{
    // Add super source
    Element* superSource = new Element(0, "SS");
    network.addVertex(superSource);

    for (const auto& [code, source]  : reservoirs){
        network.addEdge(superSource, source, source->getMaxDelivery());
    }

    // Add super sink
    Element* superSink = new Element(0, "ST");
    network.addVertex(superSink);

    for (const auto& [code, sink] : cities){
        network.addEdge(sink, superSink, sink->getDemand());
    }

    // Clean graph
    for (Vertex* v : network.getVertexSet()){
        for (Edge* e : v->getAdj()){
            e->setFlow(0);
        }
        v->setPath(nullptr);
    }

    std::unordered_set<Vertex*> coiso;
    for (auto e : allElements){
        coiso.insert(network.findVertex(e.second));
    }
    coiso.insert(network.findVertex(superSource));
    coiso.insert(network.findVertex(superSink));

    EdmondsKarp(superSource, superSink, coiso);

    double total = 0;
    Vertex* superSinkVertex = network.findVertex(superSink);
    for (Edge* e : superSinkVertex->getIncoming()){
        total += e->getFlow();
    }

    // Clean graph
    network.removeVertex(superSource);
    network.removeVertex(superSink);
    delete superSink;
    delete superSource;

    return total;
}

void Manager::EdmondsKarp(Element* source, Element* target, const std::unordered_set<Vertex*>& affected)
{
    double new_flow;

    auto sc = network.findVertex(source);
    auto tg = network.findVertex(target);

    while ((new_flow = EdmondsBFS(sc, tg, affected))){
        Vertex* node = tg;
        while (node != sc){
            Vertex* next;
            Edge* e = node->getPath();
            if (e->getDest() == node){
                next = e->getOrig();
                e->setFlow(e->getFlow() + new_flow);
            }else{
                next = e->getDest();
                e->setFlow(e->getFlow() - new_flow);
            }
            node = next;
        }
    }
}

double Manager::EdmondsBFS(Vertex* source, Vertex* target, const std::unordered_set<Vertex*>& affected)
{
    std::queue<std::pair<Vertex*, double>> q;
    q.push({source,std::numeric_limits<double>::max()});

    for (Vertex* v : network.getVertexSet()){
        v->setPath(nullptr);
    }

    while (!q.empty()){
        Vertex* v = q.front().first;
        double flow = q.front().second;
        q.pop();

        for (Edge* e : v->getAdj()){
            Vertex* d = e->getDest();
            if (affected.find(d) == affected.end()) continue;
            Edge* reverseEdge = e->getReverse();
            if ((d->getPath()== nullptr) && (e->getFlow() < e->getWeight()) && (reverseEdge ? reverseEdge->getFlow()==0:true)){
                d->setPath(e);
                double new_flow = std::min(flow, e->getWeight()-e->getFlow());
                if (d == target) return new_flow;
                q.push({d, new_flow});
            }
        }
        for (Edge* e : v->getIncoming()){
            Vertex* o = e->getOrig();
            if (affected.find(o) == affected.end()) continue;
            if ((o->getPath()== nullptr) && (e->getFlow() > 0)){
                o->setPath(e);
                double new_flow = std::min(flow, e->getFlow());
                if (o == target) return new_flow;
                q.push({o, new_flow});
            }
        }
    }
    return 0;
}

void Manager::maxflowcities(){ 
     CalculateMaxFlow();
     for(const auto& [code,city] : this->cities){
         int flow = 0;
        for(auto incoming : network.findVertex(city)->getIncoming()){
             flow += incoming->getFlow();
        }
        maxflows[code] = flow;
    }
    /**
    for(auto k : this->maxflows){
        cout << k.first << "          " << k.second <<endl;
    }

     cout<<"\n\n\n";
     */
}


void Manager::citiesindeficit() { 
    std::unordered_map<std::string, int> result;
    for(const auto& [code,city] : this->cities){
        int maxflowcity = maxflows[code];
        if(maxflowcity < city->getDemand()) deficitcities[code] = city->getDemand()-maxflowcity;
    }
    /**
    for(auto k : this->deficitcities){
        cout<< k.first << "    " << k.second<< endl;
    }
        cout<<"\n\n\n";
        */
}
