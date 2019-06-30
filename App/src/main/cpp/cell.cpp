//#include <iostream>

#include "cell.h"

const int CELLS_PER_ROW = 5; //(5x5 grid by default)

cell::cell()
{
	voltorb = true;
	one = true;
	two = true;
	three = true;
	known = false;
}


cell::~cell()
{
}

void cell::Set(int val)
{
	switch (val)
	{
	case 0:
		one = false;
		two = false;
		three = false;
		break;
	case 1:
		voltorb = false;
		two = false;
		three = false;
		break;
	case 2:
		voltorb = false;
		one = false;
		three = false;
		break;
	case 3:
		voltorb = false;
		one = false;
		two = false;
		break;
	default:
		break;
	}
}

bool cell::IsStatic()
{
	if (CountPossibleVals() == 1) {
		return true;
	}
	else {
		return false;
	}
}

bool cell::CanHavePoints()
{
	if (two || three) {
		return true;
	}
	else {
		return false;
	}
}

bool cell::IsKnown()
{
	return known;
}

bool cell::IsNoPoints()
{
	return noPoints;
}

void cell::MakeKnown()
{
	known = true;
}

void cell::MakeNoPoints()
{
	noPoints = true;
}

bool cell::CanBe(int val)
{
	switch (val)
	{
	case 0:
		return voltorb;
	case 1:
		return one;
	case 2:
		return two;
	case 3:
		return three;
	default:
		return false;
	}
}

void cell::Disable(int val)
{
	switch (val)
	{
	case 0: 
		voltorb= false;
		break;
	case 1:
		one = false;
		break;
	case 2:
		two = false;
		break;
	case 3:
		three = false;
		break;
	default:
		break;
	}
}

int cell::GetValue()
{
	if (voltorb) {
		return 0;
	}
	else if (one) {
		return 1;
	}
	else if (two) {
		return 2;
	}
	else if (three) {
		return 3;
	}
}

//void cell::PrintCell()
//{
//	if (voltorb) {
//		std::cout << "V";
//	}
//	else {
//		std::cout << " ";
//	}
//	if (one) {
//		std::cout << "1";
//	}
//	else {
//		std::cout << " ";
//	}
//	if (two) {
//		std::cout << "2";
//	}
//	else {
//		std::cout << " ";
//	}
//	if (three) {
//		std::cout << "3";
//	}
//	else {
//		std::cout << " ";
//	}
//}

void cell::FindPossibleVals(int rowTotal, int rowVoltorbs, int colTotal, int colVoltorbs, int rowRemainingCells, int colRemainingCells) {

	FindPossibleRowVals(rowTotal, rowVoltorbs, rowRemainingCells);
	FindPossibleRowVals(colTotal, colVoltorbs, colRemainingCells);
}

void cell::FindPossibleRowVals(int total, int voltorbs, int remainingCells) {

	// Rule 1: All Voltorb
	if (voltorbs == remainingCells) {
		one = false;
		two = false;
		three = false;
	}

	// Rule 2: No Voltorb
	if (voltorbs == 0) {
		voltorb = false;
	}

	// Rule 3: total 5
	if ((total + voltorbs) == remainingCells) {
		two = false;
		three = false;
	}

	// Rule 4: 4 Voltorb
	if (voltorbs == (remainingCells - 1)) {
		switch (total) {
		case 1:
			two = false;
			three = false;
			break;
		case 2:
			one = false;
			three = false;
			break;
		case 3:
			one = false;
			two = false;
			break;
		default:
			break;
		}
	}

	// Rule 5: Total 6
	if ((total + voltorbs) == (remainingCells + 1)) {
		three = false;
	}

	// Rule 6: Too High for 2's
	int numberCells = remainingCells - voltorbs;
	int addlPoints = total - numberCells;
	if (addlPoints == (numberCells * 2)) {
		// All cells must be 3 or 0
		one = false;
		two = false;
	}

	// Rule 7: Too High for 1's
	if ((addlPoints + 1) == (numberCells * 2)) {
		// 1's not possible
		one = false;
	}
}

int cell::CountPossibleVals()
{
	int numValues = 0;
	if (voltorb) {
		numValues++;
	}
	if (one) {
		numValues++;
	}
	if (two) {
		numValues++;
	}
	if (three) {
		numValues++;
	}
	return numValues;
}
