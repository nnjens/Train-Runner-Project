#ifndef TRAIN_H
#define	TRAIN_H
#include "StackAr.h"
#include "TrainRunner.h"
#include <iostream>
class Train
{
public:
  Station *stats;
  int numstations, numcars;
  int currentstat;
  unsigned short **graph;
  unsigned short **cost;
  //int **dist;
  //int **pred;
  //int **visited;
  //int *count;
  Train(Station *stations, int numStations, int numCars);
  void run(Car *cars, Operation *operations, int *numOperations);
  void dij(Operation *operations,int startnode,int endnode,int &numOps
      ,int &stime, int optype, int carID);
  
}; // class train 

#endif	// TRAIN_H

