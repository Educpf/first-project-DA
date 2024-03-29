#include "../headers/Manager.h"
#include <limits.h>

void Manager::CalculateMaxFlow(){
    // Add super source
    Element* superSource = new Element(0, "SS");
    network.addVertex(superSource);

    for (const auto& [code, source]  : reservoirs){
        network.addEdge(superSource, source, source->getMaxDelivery());
    }

    // Add super sink
    Element* superSink = new Element(0, "ST");
    network.addVertex(superSink);

    for (const auto& [code, sink]  : cities){
        network.addEdge(sink, superSink, sink->getDemand());
    }

    EdmondsKarp(superSource, superSink);


    // Clean network
    double total = 0;
    Vertex* superSinkVertex = network.findVertex(superSink);
    for (Edge* e : superSinkVertex->getIncoming()){
        total+=e->getFlow();
    }

    std::cout << "Max flow obtained was: " << total << std::endl;

    network.removeVertex(superSource);
    network.removeVertex(superSink);
    delete superSink;
    delete superSource;

    

}

void Manager::EdmondsKarp(Element* source, Element* target){

    double new_flow;

    auto sc = network.findVertex(source);
    auto tg = network.findVertex(target);

    for (Vertex* v : network.getVertexSet()){
        for (Edge* e : v->getAdj()){
            e->setFlow(0);
        }
        v->setPath(nullptr);
    }


    while ((new_flow = EdmondsBFS(sc, tg))){
        Vertex* node = tg;
        while (node != sc){
            Vertex* next;
            Edge* e = node->getPath();
            node->setPath(nullptr);
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

double Manager::EdmondsBFS(Vertex* source, Vertex* target){


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