#include "StackMazeGenerator.h"


Maze* StackMazeGenerator::generateMaze(uint32_t width, uint32_t height, uint32_t tileSize, uint32_t pathWidth)
{
   Maze* maze = new Maze(width, height, tileSize, pathWidth, 10);

   this->stack.push(std::make_pair(0, 0));
   maze->mazeArray[0][0] = Maze::CELL_VISITED;
   this->numberOfVisitedCells++;
   while(false == maze->isGeneratingFinished())
   {
      generateStep(maze);
   }

   this->numberOfVisitedCells = 0;

   return maze;
}

Maze* StackMazeGenerator::generateMazeStepByStep(uint32_t width, uint32_t height, uint32_t tileSize, uint32_t pathWidth)
{
   Maze* maze = new Maze(width, height, tileSize, pathWidth, 10);
   if(true == this->stack.empty() || true == maze->isGeneratingFinished())
   {
      this->numberOfVisitedCells = 0;

      this->stack.push(std::make_pair(0, 0));
      maze->mazeArray[0][0] = Maze::CELL_VISITED;
      this->numberOfVisitedCells++;
   }

   return maze;
}

void StackMazeGenerator::nextMazeGenerationStep(Maze* maze)
{
   if(false == maze->isGeneratingFinished())
   {
      generateStep(maze);
   }
}

void StackMazeGenerator::generateStep(Maze* maze)
{
   maze->removeMarkers();
   if(numberOfVisitedCells < maze->getHeight() * maze->getWidth())
   {
      //Establish unvisited neighbours
      std::vector<int> neighbours;

      uint32_t x = stack.top().first;
      uint32_t y = stack.top().second;

      //North
      if(y > 0)
      {
         if((maze->mazeArray[y - 1][x] & Maze::CELL_VISITED) == 0)
         {
            neighbours.push_back(0);
         }
      }
      //South
      if(y < maze->getHeight() - 1)
      {
         if((maze->mazeArray[y + 1][x] & Maze::CELL_VISITED) == 0)
         {
            neighbours.push_back(2);
         }
      }
      //West
      if(x > 0)
      {
         if((maze->mazeArray[y][x - 1] & Maze::CELL_VISITED) == 0)
         {
            neighbours.push_back(3);
         }
      }
      //East
      if(x < maze->getWidth() - 1)
      {
         if((maze->mazeArray[y][x + 1] & Maze::CELL_VISITED) == 0)
         {
            neighbours.push_back(1);
         }
      }

      if(!neighbours.empty())
      {
         int next_cell_dir = neighbours[this->rand.rand(0, neighbours.size() - 1)];
         //create path
         switch(next_cell_dir)
         {
         case 0: //North
            maze->mazeArray[y][x] |= Maze::CELL_PATH_N;
            maze->mazeArray[y - 1][x] |= Maze::CELL_PATH_S | Maze::CELL_VISITED;
            stack.push(std::make_pair((x + 0), (y - 1)));
            break;
         case 1: //East
            maze->mazeArray[y][x] |= Maze::CELL_PATH_E;
            maze->mazeArray[y][x + 1] |= Maze::CELL_PATH_W | Maze::CELL_VISITED;
            stack.push(std::make_pair((x + 1), (y)));
            break;
         case 2: //South
            maze->mazeArray[y][x] |= Maze::CELL_PATH_S;
            maze->mazeArray[y + 1][x] |= Maze::CELL_PATH_N | Maze::CELL_VISITED;
            stack.push(std::make_pair((x + 0), (y + 1)));
            break;
         case 3:
            maze->mazeArray[y][x] |= Maze::CELL_PATH_W;
            maze->mazeArray[y][x - 1] |= Maze::CELL_PATH_E | Maze::CELL_VISITED;
            stack.push(std::make_pair((x - 1), (y)));
            break;
         }
         numberOfVisitedCells++;

         maze->markers.push_back(Marker(stack.top().first, stack.top().second));
      }
      else {
         //pop out of stack
         maze->markers.push_back(Marker(stack.top().first, stack.top().second));
         stack.pop();
      }
   }
   else
   {
      maze->finishedGenerating();
      maze->mazeArray[0][0] |= Maze::CELL_START;
      maze->mazeArray[maze->getHeight() - 1][maze->getWidth() - 1] |= Maze::CELL_END;
   }
}
