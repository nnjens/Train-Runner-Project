// Author: Nicholas Jens
#include <iostream>
#include "TrainRunner.h"
#include "StackAr.h"
#include "train.h"
#include <string.h>
#include <stdio.h>
#define INFINITY 9999
using namespace std;

Train::Train(Station *stations, int numStations, int numCars)
{
  int i;
  stats = new Station[numStations];
  memcpy(stats, stations, numStations*sizeof(Station));
  //stats = stations;
  numstations = numStations;
  numcars = numCars;
  currentstat = 0;
  cost = new unsigned short*[numStations];
  for(i = 0; i < numStations; i++)
    {cost[i] = new unsigned short[numStations];}
  graph = new unsigned short*[numStations];
  for(i = 0; i < numStations; i++)
    {graph[i] = new unsigned short[numStations];}
} // Train()
void Train::dij(Operation *operations,int startnode,int endnode,int &numOps,int &stime,
     int optype, int carID)
{
  StackAr<int> stack(numstations);
  int v = numstations;
  int dist[v], pred[v];
  int visited[v], mindist, next;
  int i,count;
  int from, to, stackcount = 0;

  for (i = 0; i < v; i++)
  {
    dist[i] = cost[startnode][i];
    pred[i] = startnode;
    visited[i] = 0;
    //cout<<"pred["<<i<<"] = " <<pred[i]<<endl;
  }

  dist[startnode] = 0;
  visited[startnode] = 1;
  count = 1;

  while(count < v-1)
  {
    mindist = INFINITY;
    for(i=0;i < v; i++)
      if(dist[i] < mindist && !visited[i])
      {
        mindist = dist[i];
        next = i;
      }
      visited[next] = 1;
      for(i = 0; i < v; i++)
      {
        if(!visited[i])
          if(mindist+cost[next][i] < dist[i])
          {
            dist[i] = mindist+cost[next][i];
            pred[i] = next;
            if (i == endnode) { i = v; count = v;}
          }
      }
      count++;
  }

  i = endnode;
  do
  {
    stack.push(i);
    stackcount++;
    //i = pred[j];
    //cout <<"pushing " << i << " onto le stackzor :^)" <<endl;
    i = pred[i];
  }
  while(i!=startnode);
  //cout << "stack count: " << stackcount << endl;
  //numOps += stackcount;
  from = startnode;
  for(i = 0; i < stackcount; i++)
  {
    to = stack.topAndPop();
    //cout << "operating from " << from  << " to " << to << endl;
    stime += cost[from][to];
    //cout << "at time = " <<stime<<endl;
    if(endnode != startnode){
      operations[numOps].time = stime;
      operations[numOps].ID = to;
      operations[numOps].operation = 'M';
      numOps++;
    }
    else if(endnode == startnode && optype)
    {
      operations[numOps].time = stime;
      operations[numOps].ID = carID;
      operations[numOps].operation = 'P';
      numOps++;
      i = stackcount + 1;
    }
    else if(endnode == startnode && !optype)
    {
      operations[numOps].time = stime;
      operations[numOps].ID = carID;
      operations[numOps].operation = 'D';
      numOps++;
      i = stackcount + 1;
    }

    //operations[numOps].ID = to;
    //if(i != (stackcount - 1))
    //{
     // operations[numOps].ID = to;
      //operations[numOps].operation = 'M';
    //}
    if(i == (stackcount - 1 ) && optype == 1)
    {
      operations[numOps].time = stime;
      operations[numOps].ID = carID;
      operations[numOps].operation = 'P';
      numOps++;
    }
    else if(i == (stackcount - 1) && optype == 0)
    {
      operations[numOps].time = stime;
      operations[numOps].ID = carID;
      operations[numOps].operation = 'D';
      numOps++;
    }
    //set up for next operation
    from = to;
    //numOps++;
  }
}  //dij()

void Train::run(Car *cars, Operation *operations, int *numOperations)
{

  int v = numstations;
  int i, j, in, dist, currentstat, dest;
  int numOps = 0;
  //int numDelivered = 0;
  int stime = 0;
  //INITIALIZE GRAPH TO 0's
  for(i = 0; i < v; i++)
  {
    for(j = 0; j < v; j++)
    {
      graph[i][j] = 0;
    }
  }
  //i = 0; j = 0;
  // CREATE GRAPH

  for(i = 0; i < v; i++)
  {
    for(j = 0; j < stats[i].adjCount; j++)
    {
      //cout << "i = "<< i << "adj:" <<stats[i].adjCount <<endl;
      dist = stats[i].distances[j];
      in = stats[i].adjacent[j];
      //cout << "for station "<< i << ": inserting " << in <<" distance: "<< dist << endl;
      graph[i][in] = dist;
    } //for j
  }//for i
  for(i = 0; i < v; i++)
  {
    for(j = 0; j < v; j++)
    {
      if(graph[i][j] == 0)
        cost[i][j] = INFINITY;
      else
        cost[i][j] = graph[i][j];
    }
  }



  currentstat = 0; //initialize currentstation
  for(i = 0; i < numcars; i++)
  {
    dest = cars[i].origin;
    //cout << "picking up from " << currentstat << "->" << dest << endl;
    dij(operations,currentstat,dest,numOps,stime,1,i);

    currentstat = dest;
    dest = cars[i].destination;
    //cout << "delivering from " << currentstat << "->" << dest << endl;
    dij(operations,currentstat,dest,numOps,stime,0,i);
    currentstat = dest;

  }
  *numOperations = numOps;
  // Students must fill the operations array, and set numOperations.
} // run()