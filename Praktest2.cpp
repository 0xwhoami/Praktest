#include <iostream>

using namespace std;

int main () {
	int temp;

	cin >> temp;

	switch (temp) {
		case 1:
			cout << "satu" << endl;
			break;

		case 2:
			cout << "dua" << endl;
			break;

		case 3:
			cout << "tiga" << endl;
			break;

		default:
			cout << "Bilangan lebih besar dari tiga" << endl;
	}
}