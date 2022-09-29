#include <iostream>

using namespace std;

int main () {
	int inc, end;

	cin >> inc >> end;

	for (int i = inc; i <= end; i += inc)
		cout << i << " ";
}
