#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;

class HeapSort{
	private:
		int rootIndex;
		int fatherIndex;
		int leftKidIndex;
		int rightKidIndex;
		int minKidIndex;
		int numItems;
		int* heapAry;
		int* sortedAry;
	
	public:
		HeapSort(){
			rootIndex = 1;
			fatherIndex = 0;
			leftKidIndex = 0;
			rightKidIndex = 0;
			minKidIndex = 0;
			numItems = 9;
			heapAry = new int[numItems + 1]();
			heapAry[0] = 0;
			sortedAry = new int[numItems]();
		}
	
		void buildHeap(int data){
			int kidIndex;
			heapAry[0]++;
			heapAry[heapAry[0]] = data;			
			kidIndex = heapAry[0];
			bubbleUp(kidIndex);
		}
		
		int deleteHeap(){	
			int data = heapAry[rootIndex];
			replaceRoot();
			fatherIndex = rootIndex;
			bubbleDown(fatherIndex);
			return data;
		}
	
		void replaceRoot(){
			int lastIndex = heapAry[0];
			if(lastIndex != rootIndex)
				heapAry[rootIndex] = heapAry[lastIndex];
			heapAry[lastIndex] = 0;
			heapAry[0]--;
		}
	
		void bubbleUp(int kid_Index){
			if(kid_Index == 1)
				return;
			else{
				fatherIndex = kid_Index/2;
				if(heapAry[kid_Index] >= heapAry[fatherIndex])
					return;
				else{
					int temp = heapAry[kid_Index];
					heapAry[kid_Index] = heapAry[fatherIndex];
					heapAry[fatherIndex] = temp;
					bubbleUp(fatherIndex);
				}
			}
		}
	
		void bubbleDown(int father_Index){
			if(isLeaf(father_Index))
				return;
			else{
				leftKidIndex = father_Index * 2;
				rightKidIndex = father_Index * 2 + 1;
				minKidIndex = findMinIndex(leftKidIndex, rightKidIndex);
				if(heapAry[minKidIndex] >= heapAry[father_Index])
					return;
				else{
					int temp = heapAry[minKidIndex];
					heapAry[minKidIndex] = heapAry[father_Index];
					heapAry[father_Index] = temp;
					bubbleDown(minKidIndex);
				}
			}
			
		}
	
		bool isLeaf(int index){
			int leftIndex = index * 2;
			int rightIndex = index * 2 + 1;
			if (leftIndex > heapAry[0] && rightIndex > heapAry[0])
				return true;
			return false;
				
		}
	
		int findMinIndex(int leftIndex, int rightIndex){
			int minIndex = leftIndex;
			if(rightIndex <= heapAry[0] && heapAry[rightIndex] < heapAry[leftIndex])
				minIndex = rightIndex;
			return minIndex;
		}
	
		bool isHeapEmpty(){
			if(heapAry[0] == 0)
				return true;
			return false;
		}
	
};

class imageProcessing{
	private:
		int numRows;
		int numCols;
		int minVal;
		int maxVal;
		int newMin;
		int newMax;
		int** imgInAry;
		int** imgOutAry;
		int** mirrorFramedAry;
		int** tempAry;
		int* hist;
		int* neighborAry;

	public:
		imageProcessing(int rows, int cols, int min, int max){
			numRows = rows;
			numCols = cols;
			minVal = min;
			maxVal = max;
			newMin = max;
			newMax = min;
			imgInAry = new int*[numRows];
			imgOutAry = new int*[numRows];
			for(int i = 0; i < numRows; i++){
				imgInAry[i] = new int[numCols]();
				imgOutAry[i] = new int[numCols]();
			}
			
			mirrorFramedAry = new int*[numRows+2];
			tempAry = new int*[numRows+2];
			for(int i = 0; i < numRows+2; i++){
				mirrorFramedAry[i] = new int[numCols+2]();
				tempAry[i] = new int[numCols+2]();
			}
			
			hist = new int[maxVal+1]();
			neighborAry = new int[9]();		
		}
		
		void mirrorFramed(){
			for(int c = 0; c < numCols+2; c++){
				mirrorFramedAry[0][c] = mirrorFramedAry[1][c];
				mirrorFramedAry[numRows+1][c] = mirrorFramedAry[numRows][c];
			}
			for(int r = 0; r < numRows+2; r++){
				mirrorFramedAry[r][0] = mirrorFramedAry[r][1];
				mirrorFramedAry[r][numCols+1] = mirrorFramedAry[r][numCols];
			}
			
		}

		void loadImage(ifstream& Input1){
			int in;
			for (int i = 0; i < numRows; i++){
				for (int j = 0; j < numCols; j++){
					Input1 >> in;
					imgInAry[i][j] = in;
					mirrorFramedAry[i+1][j+1] = in;
				}
			}
		}
		
		void loadNeighbors(int i, int j){
			int n = 0;
			for(int row = i-1; row <= i+1; row++){
				for(int col = j-1; col <= j+1; col++){
					neighborAry[n] = mirrorFramedAry[row][col];
					n++;
				}
			}
		}
		
		void computeHistogram(){
			for(int i = 0; i < numRows; i++){
				for(int j = 0; j < numCols; j++){
					hist[imgInAry[i][j]]++;
				}
			}
		}
		
		void printHist(ofstream& Output1){
			Output1 << numRows << " " << numCols << " " << minVal << " " << maxVal << endl;
			for(int i = 0; i < maxVal+1; i++){
				Output1 << i << " " << hist[i] << endl;
			}
		}
		
		void computeAvg3x3(){
			for(int i = 1; i < numRows+1; i++){
				for(int j = 1; j < numCols+1; j++){
					loadNeighbors(i,j);
					tempAry[i][j] = Avg3x3();
					if(tempAry[i][j] < newMin)
						newMin = tempAry[i][j];
					if(tempAry[i][j] > newMax)
						newMax = tempAry[i][j];
				}
			}				
		}
		
		int Avg3x3(){
			int sum = 0;
			for (int i = 0; i < 9; i++){
				sum += neighborAry[i];
			}
			return sum/9;
		}
		
		void computeMedian3x3(){
			for(int i = 1; i < numRows+1; i++){
				for(int j = 1; j < numCols+1; j++){
					int p = mirrorFramedAry[i][j];
					loadNeighbors(i,j);
					sort();
					tempAry[i][j] = neighborAry[4];
					if(tempAry[i][j] < newMin)
						newMin = tempAry[i][j];
					if(tempAry[i][j] > newMax)
						newMax = tempAry[i][j];
				}
			}
		}
		
		void sort(){
			HeapSort neighbors;
			for(int i = 0; i < 9; i++)
				neighbors.buildHeap(neighborAry[i]);
			for(int i = 0; i < 9; i++)
				neighborAry[i] = neighbors.deleteHeap();
		}
	
		void computeThreshold(int thr_val){
			for(int i = 1; i < numRows+1; i++){
				for(int j = 1; j < numCols+1; j++){
					int p = tempAry[i][j];
					if(p >= thr_val)
						imgOutAry[i-1][j-1] = 1;
					else
						imgOutAry[i-1][j-1] = 0;
				}
			}
		}
		
		void prettyPrint(ofstream& Output3){
			for(int i = 0; i < numRows; i++){
				for(int j = 0; j < numCols; j++){
					int p = imgOutAry[i][j];
					 if(p > 0)
						Output3 << p << " ";
					else
						Output3 <<"  ";
				}
				Output3 << endl;
			}
		}
		
		void printHeader(ofstream& Output2){
			Output2 << numRows << " " << numCols << " " << newMin << " " << newMax << endl;
		}
	
		void printTempAry(ofstream& Output2){
			for(int i = 1; i < numRows+1; i++){
				for(int j = 1; j < numCols+1; j++){
					Output2 << tempAry[i][j] << " ";
				}
				Output2 << endl;
			}
		}
		
};

int main(int argc, char** argv){
	int rows, cols, min, max;
	int whichMethod = 0;

	ifstream Input1;
	ofstream Output1, Output2, Output3;
	Input1.open(argv[1]);
	Output1.open(argv[3]);
	Output2.open(argv[4]);
	Output3.open(argv[5]);
	
	Input1 >> rows;
	Input1 >> cols;
	Input1 >> min;
	Input1 >> max;
	int thr_val = atoi(argv[2]);
	imageProcessing image(rows, cols, min, max);
	
	while(whichMethod == 0){
		cout << "Which method? \nEnter '1' for average filtering \nEnter '2' for median filtering \n";
		cin >> whichMethod;
		if(whichMethod != 1 && whichMethod != 2){
			whichMethod = 0;
			cout << "That is not a valid method.\n";
		}
	}

	image.loadImage(Input1);
	image.computeHistogram();
	image.printHist(Output1);
	image.mirrorFramed();
	
	if(whichMethod == 1)
		image.computeAvg3x3();
	else if(whichMethod == 2)
		image.computeMedian3x3();
	else {
		cout << "Error!";
		return -1;
	}
	
	image.computeThreshold(thr_val);
	image.prettyPrint(Output3);
	image.printHeader(Output2);
	image.printTempAry(Output2);
	
	Input1.close();
	Output1.close();
	Output2.close();
	Output3.close();
	return 0;
}