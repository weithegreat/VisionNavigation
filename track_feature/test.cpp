#include <iostream>
#include <vector>

using namespace std;

int main ()
{
  vector<int> myvector(2);   // 10 zero-initialized ints

  // assign some values:
  myvector.at(0)=0;
    myvector.at(1)=1;

  cout << "myvector contains:";
  for (int i=0; i<myvector.size(); i++)
    cout << ' ' << myvector.at(i);
  cout << '\n';

  return 0;
}