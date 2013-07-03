#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include "math/Vector3.h"
#include "math/Matrix3.h"

int main()
{
	using namespace std;
	Vector3 Pref1 ( 1, 1, 0 );
	Vector3 Pref2 ( 0, 1, 0 );
	Vector3 Pref3 ( 1, 1, 2 );
	Vector3 Pref = ( Pref1+Pref2+Pref3 ) /3;
	cout << "Pref " << Pref << endl;
	Vector3 Ppat1 ( 1, 0, 0 );
	Vector3 Ppat2 ( 0, 1, 0 );
	Vector3 Ppat3 ( 0, 1, 2 );
	Vector3 Ppat = ( Ppat1+Ppat2+Ppat3 ) /3;
	cout << "Ppat " << Ppat << endl;
	// get move vector
	Vector3 patToref = Pref - Ppat;
	Vector3 PrefXup = ( Pref1 - Pref ).crossProduct ( Pref2 - Pref ).normaliseV() ;
	Vector3 PrefYup = ( Pref2 - Pref ).crossProduct ( Pref1 - Pref ).normaliseV() ;
	Vector3 PrefZup = ( Pref3 - Pref ).crossProduct ( Pref1 - Pref ).normaliseV() ;
	cout << "PrefXup " << PrefXup << endl;
	cout << "PrefYup " << PrefYup << endl;
	cout << "PrefZup " << PrefZup << endl;
	Vector3 PpatXup = ( Ppat1 - Ppat ).crossProduct ( Ppat2 - Ppat ).normaliseV();
	Vector3 PpatYup = ( Ppat2 - Ppat ).crossProduct ( Ppat1 - Ppat ).normaliseV();
	Vector3 PpatZup = ( Ppat3 - Ppat ).crossProduct ( Ppat1 - Ppat ).normaliseV();
	cout << "PpatXup " << PpatXup << endl;
	cout << "PpatYup " << PpatYup << endl;
	cout << "PpatZup " << PpatZup << endl;
	Matrix3 Mref;
	Mref.FromAxes2 ( PrefXup, PrefYup, PrefZup );
	cout << "Mref " << Mref << endl;
	Matrix3 Mpat;
	Mpat.FromAxes2 ( PpatXup, PpatYup, PpatZup );
	cout << "Mpat " << Mpat << endl;
	Matrix3 res = Mref * Mpat;
	cout << "Mref * Mpat " << Mref* Mpat << endl;
	cout << "PrefXup * res " << ( PrefXup * res ).normaliseV() << endl;
	cout << "PrefYup * res " << ( PrefYup * res ).normaliseV() << endl;
	cout << "PrefZup * res " << ( PrefZup * res ).normaliseV() << endl;
	cout << "PrefXup * res " << ( ( PrefXup * res ).normaliseV().distance ( PpatXup ) < 0.01 ) << endl;
	cout << "PrefYup * res " << ( ( PrefYup * res ).normaliseV().distance ( PpatYup ) < 0.01 ) << endl;
	cout << "PrefZup * res " << ( ( PrefZup * res ).normaliseV().distance ( PpatZup ) < 0.01 ) << endl;
}
