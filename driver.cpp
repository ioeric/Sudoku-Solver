#include <iostream>
#include <cstdlib>
#include "sudoku.h"
using namespace std;

Sudoku* solve(Sudoku* puzzle){
	// If puzzle is NULL, can't be solved, return NULL 
	if (puzzle==NULL) return NULL;

	puzzle->horSolve(); // Method 2
	puzzle->verSolve(); // Method 3
	puzzle->boxSolve(); // Method 4
	int flag = puzzle->finalize(); // Mothod 1
	if (flag == -1) { // If flag=-1, failure, return NULL
		return NULL;	
	// If changes are made and puzzle is solved, return puzzle
	}else if (flag == 1 && puzzle->isSolved()) {
		return puzzle;
	// No change was made, or changes are made, but puzzle has not been solved
	// Guess!
	} else if (flag == 0 || (flag == 1 && !puzzle->isSolved())){
		Sudoku* newpuzzle = puzzle->guess();
		if (newpuzzle == NULL){
			return NULL;	
		}
		Sudoku* result = solve(newpuzzle);
		// If newpuzzle be solved successfully
		if (result!=NULL){
			if (newpuzzle != result && newpuzzle!=NULL)
				delete newpuzzle;
			return result;
		}
		delete newpuzzle;
		// Continue solve puzzle	
		return solve(puzzle);
	}
	// If flag is unknown number, return NULL
	return NULL;
}
int main(){
	char origin[100]; // Original puzzle
	
	for (int i=0;i<9;i++)
		for (int j=0;j<9;j++)
			cin >> origin[i*9 + j];
	origin[81] = '\0';
	Sudoku* puzzle = new Sudoku(origin);
	cout << *puzzle << endl; // Output original puzzle
	Sudoku* result = solve(puzzle); // Solve puzzle
	if (result!=NULL){
		if (result!=puzzle)
			delete puzzle;
		cout << *result;
		delete result;
	}else{
		if (puzzle!=NULL)
			delete puzzle;
		cerr << "ERROR" << endl;
		exit(0);
	}	
	return 0;
}
