#pragma once
class cell
{
public:
	cell();
	~cell();
	void Set(int val);
	bool IsStatic();
	bool CanHavePoints();
	bool IsKnown();
	bool IsNoPoints();
	void MakeKnown();
	void MakeNoPoints();
	bool CanBe(int val);
	void Disable(int val);
	int GetValue();			//ONLY USE IF VALUE IS KNOWN (only one option)
//	void PrintCell();
	void FindPossibleVals(int rowTotal, int rowVoltorb, int colTotal, int colVoltorbs, int rowRemainingCells, int colRemainingCells);
	void FindPossibleRowVals(int total, int voltorbs, int remainingCells);
	int CountPossibleVals();
private:
	bool voltorb;
	bool one;
	bool two;
	bool three;
	bool known = false;
	bool noPoints = false;
};

