#include "sudoku.h"

// Default Constructor
Sudoku::Sudoku (){
	for (int i=0;i<9;i++){
		for (int j=0;j<9;j++){
			puzzle[i][j] = new Cell();
		}
	}
}

//Initializes a Sudoku with the translated string
Sudoku::Sudoku (std::string input){
	int tmp;
	SolvedCell* tmpSolved;
	for (int i=0;i<9;i++){
		for (int j=0;j<9;j++){
			if (input[i*9+j] == '.'){
				puzzle[i][j] = new Cell();
			}
			else{
				tmp = input[i*9+j]-'0';
				puzzle[i][j] = new Cell(tmp);
				tmpSolved = new SolvedCell(i,j,tmp);
				// Push the solved cell into stack 
				finalStack.push(tmpSolved);
			}
		}
	}
}
	
//Copy Constructor
Sudoku::Sudoku (const Sudoku& other){
	while (!finalStack.empty()){
		finalStack.pop();
	}
	for (int i=0;i<9;i++){
		for (int j=0;j<9;j++){
			puzzle[i][j] = new Cell;
			*puzzle[i][j] = *other.puzzle[i][j];
		}
	}	
}

//Destructor
Sudoku::~Sudoku (){
	for (int i=0;i<9;i++){
		for (int j=0;j<9;j++){
			delete puzzle[i][j];
		}
	}	
	SolvedCell* tmp;
	while (!finalStack.empty()){	
		tmp= finalStack.top();
		delete tmp;
		finalStack.pop();
	}
}

//Removes a candidate from the cell with the least
//number of possible candidates. Returns a pointer
//to a Sudoku object for which the target cell is
//finalized to the removed candidate. If no guess
//can be made NULL is returned
Sudoku* Sudoku::guess (){
	int pos_i, pos_j; // Record the position of cell to be guessed
	int min = 10;
	bool found = false;
	int poss;
	for (int i=0;i<9;i++){
		for (int j=0;j<9;j++){
			poss = puzzle[i][j]->possibilities();
			// Guess if more than one choices
			// and replace the potential cell if poss is smaller
			if (poss>1 && poss < min){
				min = poss;
				pos_i = i;
				pos_j = j;
				found = true;
			}
		}
	}
	
	// Can't guess, return NULL
	if (found == false) return NULL;
	// Create a new puzzle
	Sudoku *tmpSudoku = new Sudoku(*this);
	
	int Smallest = tmpSudoku->puzzle[pos_i][pos_j]->first();
	
	// Remove this candidate in chosen cell in old puzzle
	puzzle[pos_i][pos_j]->removeCandidate(Smallest);
	// If only one candidate in this cell, finalize it
	if (puzzle[pos_i][pos_j]->possibilities() == 1){
		SolvedCell* svdCell = new SolvedCell(pos_i,pos_j, puzzle[pos_i][pos_j]->first());
		finalStack.push(svdCell);
	}

	// Finalize this chosen cell in temporary puzzle
	for (int k=1;k<=9;k++){
		if (k!=Smallest){
			tmpSudoku->puzzle[pos_i][pos_j]->removeCandidate(k);
		}
	}	
	SolvedCell* solved = new SolvedCell(pos_i, pos_j, Smallest);
	tmpSudoku->finalStack.push(solved);
	return tmpSudoku;
}

//Adds horizontally solvable cells to stack
void Sudoku::horSolve (){
	int record[10]; // record: record the number of cells who have candidate k
			// (eg, record[ k ] is the number of cells who have candidate k)
	int num[10][9]; // num: record the column index of cells 
	bool flags[10]; // If a cell is solved, its flag is true
	int tmpValue;
	for (int i=0;i<9;i++){
	//	memset(record, 0, sizeof(record));
	//	memset(num,0,sizeof(num));
	//	memset(flags, 0, sizeof(flags));
	
		for (int t=0;t<10;t++) record[t] = 0;
		for (int t=0;t<10;t++) flags[t] = false;
		for (int t=0;t<10;t++)
			for (int tt=0;t<9;t++)
				num[t][tt] = 0;
		// Find all solved cells that have only one candidate,
		// and make the candidate's flag true so that other cells 
		// can't choose this candidate
		for (int j=0;j<9;j++){
			if (puzzle[i][j]->possibilities() == 1) {
				tmpValue = puzzle[i][j]->first();
				flags[tmpValue] = true;
			}	
		}
		
		// Check every cell of ith row
		for (int j=0;j<9;j++){
			// If cell has one candidate, skip
			if (puzzle[i][j]->possibilities() == 1) continue; 
			
			// Record all possible candidates	
			for (int k=1;k<=9;k++){
				if (!flags[k] && puzzle[i][j]->at(k)){
					num[k][ record[k] ] = j;
					record[k]++;	
				}
			}

		}	
		
		// If a candidate can only be exists in some cell, then this cell is solveable
		for (int k=1;k<=9;k++){
			if (record[k] == 1){
				SolvedCell *tmpCell = new SolvedCell(i, num[k][0], k);

				finalStack.push(tmpCell);
			}
		}
	}

			
}

//Adds vertically solvable cells to stack
void Sudoku::verSolve (){
	int record[10]; // record: record the number of cells who have candidate k
			// (eg, record[ k ] is the number of cells who have candidate k)
	int num[10][9]; // num: record the row index of cells 
	bool flags[10]; // If a cell is solved, its flag is true
	int tmpValue;

	for (int j=0;j<9;j++){
		
	//	memset(record, 0, sizeof(record));
//		memset(num,0,sizeof(num));
//		memset(flags, 0, sizeof(flags));
		for (int t=0;t<10;t++) record[t] = 0;
		for (int t=0;t<10;t++) flags[t] = false;
		for (int t=0;t<10;t++)
			for (int tt=0;t<9;t++)
				num[t][tt] = 0;

		// Find all solved cells who have only one candidate,
		// and make the candidate's flag true so that other cells 
		// can't choose this candidate
		for (int i=0;i<9;i++){
			if (puzzle[i][j]->possibilities() == 1){
				tmpValue = puzzle[i][j]->first();
				flags[tmpValue] = true;
			}

		}	
		
		// Check every cell in this column
		for (int i=0;i<9;i++){
			// If cell is solved, skip
			if (puzzle[i][j]->possibilities() == 1) continue;
			
			// Record all possible candidates
			for (int k=1;k<=9;k++){
				if (!flags[k] && puzzle[i][j]->at(k)){
					num[k][record[k]] = i;
					record[k]++;
				}
			}
		}
		
		// If a candidate can only be exists in some cell, then this cell is solveable
		for (int k=0;k<9;k++){
			if (record[k] == 1){
				SolvedCell *tmpCell = new SolvedCell(num[k][0], j, k);
				finalStack.push(tmpCell);

			}
		}
	}


}

//Adds solvable cells by box to stack
void Sudoku::boxSolve (){
	int record[10]; // record: record the number of cells who have candidate k
			// (eg, record[ k ] is the number of cells who have candidate k)
	int num[10][9]; // num: record the column index of cells 
	bool flags[10]; // If a cell is solved, its flag is true
	int tmpValue;	
	// Divide the sudoku into 9 box
	int R,C; // R,C is the row number and colume number of a box
	int now_R, now_C; // col and colume of cell

	// Check every box
	for (int box=0;box<9;box++){ 
		R = box / 3;
		C = box % 3;
	//	memset(record, 0, sizeof(record));
//		memset(num, 0, sizeof(num));
//		memset(flags, 0, sizeof(flags));
		for (int t=0;t<10;t++) record[t] = 0;
		for (int t=0;t<10;t++) flags[t] = false;
		for (int t=0;t<10;t++)
			for (int tt=0;t<9;t++)
				num[t][tt] = 0;

		// Check every cell in the box, and mark solved cells
		for (int x=0;x<9;x++){
			now_R = R*3 + x / 3;
			now_C = C*3 + x % 3;
			if (puzzle[now_R][now_C]->possibilities() == 1){
				tmpValue = puzzle[now_R][now_C]->first();
				flags[tmpValue] = true;
				continue;
			}

		
		}
		
		// Check every cell
		for (int x=0;x<9;x++){
			now_R = R*3 + x / 3;
			now_C = C*3 + x % 3;
			// If a cell is solved, skip
			if (puzzle[now_R][now_C]->possibilities()==1) continue;
			// Record all possible candidates of a cell
			for (int k=1;k<=9;k++){
				if (!flags[k] && puzzle[now_R][now_C]->at(k)){
					num[k][record[k]] = x;
					record[k]++;
				}
			}
		}

		// Push cell, whose candidate cannot be put into other cells, into stack
		for (int k=1;k<=9;k++){
			if (record[k]==1){
				now_R = R*3 + num[k][0] / 3;
				now_C = C*3 + num[k][0] % 3;	
				SolvedCell *tmpCell = new SolvedCell(now_R, now_C, k);
				finalStack.push(tmpCell);
			}
		}


	}


}


//Removes val as a possible candidate from all cells
//in the same row, colomn, and box.
//All solvable cells get added to the stack
bool Sudoku::eliminate(int row, int col, int val){
	int box_num = (row /3) * 3 + col / 3;
	int poss;
	SolvedCell* solved;
	bool mark;
	for (int j=0;j<9;j++){
		if (j!=col && puzzle[row][j]->at(val)){	
			poss = puzzle[row][j]->possibilities();
			puzzle[row][j]->removeCandidate(val);
			if (poss == 1 || poss == 0){
			       	return false;
			}
			if (poss == 2){
				solved = new SolvedCell(row, j, puzzle[row][j]->first());
				finalStack.push(solved);	
			}
		}else if (puzzle[row][j]->possibilities() == 0)
			return  false;
	}

	// Eliminate col
	for (int i=0;i<9;i++){
		if (i!=row && puzzle[i][col]->at(val)){
			poss = puzzle[i][col]->possibilities();
			mark = puzzle[i][col]->removeCandidate(val);
			if (poss == 1 || poss == 0) return  false;
			if (poss == 2){
				solved = new SolvedCell(i, col, puzzle[i][col]->first());	
				finalStack.push(solved);	
			}
		}else if(puzzle[i][col]->possibilities()==0)
			return false;
	}

	int now_R, now_C;
	int box_R = box_num / 3;
	int box_C = box_num % 3;
	// Eliminate box
	for (int x=0;x<9;x++){
		now_R = box_R * 3 + x / 3;
		now_C = box_C * 3 + x % 3;
		if (!(now_R == row && now_C == col) && puzzle[now_R][now_C]->at(val)){
			poss = puzzle[now_R][now_C]->possibilities();
			mark = puzzle[now_R][now_C]->removeCandidate(val);
			if (poss == 1 || poss == 0) return false;
			if (poss == 2){
				solved = new SolvedCell(now_R, now_C, puzzle[now_R][now_C]->first());
				finalStack.push(solved);
			}
		}else if (puzzle[now_R][now_C]->possibilities()==0)
			return false;
	}

	return true;
}

//Finalizes cells in stack
//-1 failure in puzzle
//0 nothing on stack
//1 changes made
int Sudoku::finalize (){
	bool flag = true;
	// Nothing on stackm return 0
	if (finalStack.empty()) return 0;
	SolvedCell* solvedCell;
	while (true){
		// Finilized all solvedCells, break
		if (finalStack.empty()) break;

		solvedCell = finalStack.top();
		finalStack.pop();
		// Remove all other candidate in this cell
		for (int i=1;i<=9;i++){
			if (i!=solvedCell->val){
				puzzle[solvedCell->row][solvedCell->col]->removeCandidate(i);
			}
		}
		if (puzzle[solvedCell->row][solvedCell->col]->possibilities()==0){
		
			delete solvedCell; // Delete this solvedCell from heap
			return -1;
		}
		flag = eliminate(solvedCell->row, solvedCell->col, solvedCell->val);
		delete solvedCell;
		if (!flag) 
			return -1;

	}

	return 1;


}

//true if puzzle is solved, false otherwise
bool Sudoku::isSolved (){
	// If every cell has only one candidate, return true
	for (int i=0;i<9;i++){
		for (int j=0;j<9;j++){
			if (puzzle[i][j]->possibilities() != 1)
				return false;
		}
		
	}
	
	return true;

}

//Output
std::ostream & operator<< (std::ostream & stream, const Sudoku & puzzleState){
	for (int i=0;i<9;i++){
		for (int j=0;j<9;j++){
			if (puzzleState.puzzle[i][j]->possibilities() == 1)
				stream << puzzleState.puzzle[i][j]->first() << ' ';
			else
				stream << ". ";  
		
		}
		stream << std::endl;
	
	}
	return stream;

}


