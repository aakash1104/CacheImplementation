#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

class cLineLine
{
public:
  int tag;
  char data[8];
  bool dirty;
  cLineLine();
};

cLineLine::cLineLine()
{
  tag = 0;
  dirty = false;
  for(int i = 0; i < 8; i++) 
    data[i] = 0;
}

int main(int argc, char** argv)
{
  int add, opcode, data, whichLine, tag;
  bool hit, dirty;
  char RAM[0xFFFF];
  cLineLine cLine[32];
  ifstream inf(argv[1]);
  ofstream outf("dm-out.txt");
  outf << hex << setfill('0') << uppercase;
  
  while(inf >> hex >>  add >> opcode >> data)
  {
//    cout << "-----------" << hex << add << " " << opcode << " " << data << " " << endl; 
    whichLine = (add / 8) % 32;
    tag = add / 8;

    
    if(opcode == 0xFF) // write
    {
      if(cLine[whichLine].dirty == true)
        for(int i = 0; i < 8; i++)
          (&RAM[cLine[whichLine].tag * 8])[i] = cLine[whichLine].data[i];

      cLine[whichLine].tag = tag;
      cLine[whichLine].data[add & 7] = data;
      cLine[whichLine].dirty = true;
    } // if a write

    if(opcode == 0x00) // read
    {
      if(cLine[whichLine].tag == tag)
        hit = true;
      else
        hit = false;

      if(hit == false)
      {
        if(cLine[whichLine].dirty == true)
          for(int i = 0; i < 8; i++)
            (&RAM[cLine[whichLine].tag * 8])[i] = cLine[whichLine].data[i];
      
        cLine[whichLine].tag = tag;
        for(int i = 0; i < 8; i++)
          cLine[whichLine].data[i] = (&RAM[tag * 8])[i];
      }
      else
      {
      }
      
      outf << setw(2) << data << ' ' << hit << ' ' << cLine[whichLine].dirty << endl;     


     if(hit == false)
        cLine[whichLine].dirty = false;
     
     }

  } 
  
  inf.close();
  outf.close();
  return 0;
} // main()
