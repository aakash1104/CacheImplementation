#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

class cacheLine
{
public:
  int Least;
  int tag;
  char data[4];
  bool dirty;
  cacheLine();
}; // class Line

cacheLine :: cacheLine()
{ 
  Least = 0;
  tag = 0;
  dirty = false;
  for(int i = 0; i < 4; i++)
    data[i] = 0;
}


int main(int argc, char** argv)
{
  int i, add, data, whichLine, tag, whichSet, opcode;
  bool hit, dirty;
  char RAM[0xFFFF];
  cacheLine cLine[10][6];
  
  ifstream inf(argv[1]);
  ofstream outf("sa-out.txt");
  
  
  while(inf >> hex >>  add >> opcode >> data)
  {
    whichSet = (add / 4) % 10;
    tag = (add / 4);
    
    for(int i = 0; i < 6; i++)
      cLine[whichSet][i].Least++;

    if(opcode == 0xFF) 
    {
      hit = false;
      for(whichLine = 0; whichLine < 6; whichLine++)
        if(cLine[whichSet][whichLine].tag == tag)
        {
          hit = true;
          break;
        }  

      if(hit == false)
      {
        whichLine = 0;

        for(int i = 0; i < 6; i++)
          if(cLine[whichSet][i].Least > cLine[whichSet][whichLine].Least)
            whichLine = i;
      
        if(cLine[whichSet][whichLine].dirty)
          for(int i = 0; i < 4; i++)
            (&RAM[cLine[whichSet][whichLine].tag * 4])[i] = cLine[whichSet][whichLine].data[i];

        for(int i = 0; i < 4; i++)
          cLine[whichSet][whichLine].data[i] = (&RAM[tag * 4])[i];

        cLine[whichSet][whichLine].dirty = false;
      } 

      cLine[whichSet][whichLine].tag = tag;
      cLine[whichSet][whichLine].Least = 0;
      cLine[whichSet][whichLine].data[add & 3] = data;
      cLine[whichSet][whichLine].dirty = true;
    }

    if(opcode == 0x00) 
    {
      hit = false;

      for(whichLine = 0; whichLine < 6; whichLine++)
      {
        if(cLine[whichSet][whichLine].tag == tag)
        {
          cLine[whichSet][whichLine].Least = 0;
          hit = true;
          dirty = cLine[whichSet][whichLine].dirty;
          break;
        }  
      } 

      if(hit == false)
      {
        int max = 0;
        whichLine = 0;
      
        for(int i = 0; i < 6; i++)
          if(cLine[whichSet][i].Least > max)
          {
            max = cLine[whichSet][i].Least;
            whichLine = i;
          } 
      
        if(cLine[whichSet][whichLine].dirty)
         for(int i = 0; i < 4; i++)
          (&RAM[cLine[whichSet][whichLine].tag * 4])[i] = cLine[whichSet][whichLine].data[i];

      for(int i = 0; i < 4; i++)
        cLine[whichSet][whichLine].data[i] = (&RAM[tag * 4])[i];

      cLine[whichSet][whichLine].tag = tag;
      cLine[whichSet][whichLine].Least = 0;
    }

    outf << hex << setfill('0') << uppercase << setw(2) << data << ' ' << hit << ' ' << cLine[whichSet][whichLine].dirty << endl;
    
    if(hit == false) 
      cLine[whichSet][whichLine].dirty = false;
    } 
  } 
  return 0;
} 

