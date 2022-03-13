#include <iostream>
#include <fstream> // ifstream
#include <string>	//string
#include <time.h>	//clock
#include <vector>	//vector
#include <limits> // numeric_limits<long double>::max()/min()
#include <algorithm>	//swap


/*
Ramal Seyidli
150180901
*/

using namespace std;


struct vehicle{
	long double key;		//time to get hotel: key = distance/speed
	string id;		//vehicle id
	string location; 	//vehicle location
	long double distance;	//distance to the hotel
	int speed;		//vehicle speed	
};

class Min{	//Minimum Priority Queue
	private:
		vector <vehicle> heap;	
	public:
		Min() {}	//constructor
		~Min() {}	//destructor
		//vector <vehicle> heap;
		void insertVehicle(vehicle input);	//addTaxi
		int getSize(); //for current size		//i will use heap.size() instead
		void heapify(int i);	//min_heapify	//heapify with i index
		vehicle extract(int index);	//extract_min
		void decreaseKey(int index, long double key);	//for decrease function
};

void Min::heapify(int i){
	int min = i; 
	int left = 2 * i + 1;	
	int right = 2 * i + 2;

	//left and its parent comparision
	if((long unsigned int)left < heap.size() && heap[left].key < heap[i].key){
		min = left;
	}
	//right and its parent comparision
	if((long unsigned int)right < heap.size() && heap[right].key < heap[min].key){
		min = right;
	}
	//when parent vehicle does not have the smallest time key
	if(min != i){
		swap(heap[i], heap[min]);
		heapify(min);	//recursive approach
	}
}

vehicle Min::extract(int index){
	if(heap.size() < 1){	//in case heap is empty
		cerr << "Heap underflow" << endl;
	}
	if(index != 0){		//in case lucky number is different than 0
		decreaseKey(index, numeric_limits<long double>::min());
	}
	vehicle temp = heap[0];	//this is for returning extracted vehicle for writing it to output file
	swap(heap[0], heap[heap.size()-1]);	//swapping root and leaf
	heap.pop_back();	//removing last element
	heapify(0);	//heapifying last element which has just placed at root
	return temp;	//returns the old top
}

//insert function
void Min::insertVehicle(vehicle input){	//argument is vehicle to insert to heap
	long double tempKey = input.key;	//to send argument vehicle's key to decrease funtion
	input.key = numeric_limits<long double>::max();	//assigning +infinity to input's key
	heap.push_back(input);	//inserting input vehicle to heap from the end
	decreaseKey(heap.size()-1, tempKey);	//decreasing key and placing it into right place
}
//decrease function
void Min::decreaseKey(int i,long double key){	//arguments are index and key
	heap[i].key = key;	//assigning argument key to heap[index] from +infinity
	while(i != 0 && heap[(i-1)/2].key > heap[i].key){	//comparing parent to child
		swap(heap[i], heap[(i-1)/2]);	//if parent is higher in key
		i = (i-1)/2;	//assigning parent's index to input index
	}
}


int main(int argc, char** argv){
	
	int N;	//number of entries
	
	if (argc > 1) {
		N = atoi(argv[1]); // ./a.out N 
	} else {
		N = 1000; // default value for N
	}
	
	ifstream file;	
	file.open("vehicles.txt");	//opnening input file
	
	if (!file){	//security
		cerr << "vehicles.txt cannot be opened";
		return 0;
	}

	string line;
	getline(file, line);
	string tableHeader = line; //to skip header

	Min* priority = new Min;	//Priority queue for vehicles
	
	for (int i = 0; i < 1642; i++) {

		vehicle input;

		getline(file, line, '\t'); //to read vehicle id which is a string
		input.id = line;

		getline(file, line, '\t'); //to read location which is a string 
		input.location = line;

		getline(file, line, '\t'); //to read distance which is a long double
		input.distance = stold(line);//stold for string double convertation

		getline(file, line, '\n'); //to read speed which is an integer 
		input.speed = stoi(line);	//stoi for string integer convention

		input.key = input.distance/input.speed;	//calculeting key

		priority->insertVehicle(input);	//asking insert function for vehicle from input file

		//To use insertVehicle function, which is also used in reinsertion of vehicles after usage
	}


	//Time operations

	clock_t elapsedTime; //to count runing time
	elapsedTime = clock();

	//Time operations 


	ifstream myFile;	
	myFile.open("requests.txt");		//opening second input file

	if (!file) {	//security
		cerr << "requests.txt cannot be opened";
		return 0;
	}

	string myLine;
	getline(myFile, myLine);
	string myTableHeader = myLine; //to skip header
	
	string reqLocation;	//location of launched request
	long double reqDistance; //distance of launched request
	int luckyNumber;	//lucky number of a customer

	ofstream outfile;	//opening the output file
	outfile.open("call_history.txt");

	if (!outfile) {	//security
		cerr << "Output file cannot be opened";	//cant open
		
	}
	int counter = 0; //to compare total number of insert, extract and decrease operations with N

	while (counter < N) {

		getline(myFile, line, '\t'); //to read request location which is a string
		reqLocation = line;

		getline(myFile, line, '\t'); //to read distance which is a long double
		reqDistance = stold(line);//stold for string double convertation

		getline(myFile, line, '\n'); //to read luck number which is an integer 
		luckyNumber = stoi(line);	//stoi for string integer convention

		
		if(luckyNumber != 0){		//in case luckynumber is different from 0
			counter = counter + 2;
		}else{	//lucky number = 0
			counter++;
		}
		
		vehicle exCar = priority->extract(luckyNumber);	//extracted vehicle

		outfile<< exCar.id<<endl;	//writing id to output file

		exCar.location = reqLocation;	//assigning new location and distance info to car which has extracted and will be inserted again
		exCar.distance = reqDistance;

		exCar.key = exCar.distance/exCar.speed;	//calculating key

		priority->insertVehicle(exCar);	//reinserting the vehicle

		counter++;	
	}

	outfile.close(); // close "call_history.txt"

	delete priority;	

	elapsedTime = clock() - elapsedTime;	//calculates time

	cout << "Runtime is " << ( (float) elapsedTime ) * 1000 / CLOCKS_PER_SEC << " in miliseconds. " <<endl;

	return 0;
}
