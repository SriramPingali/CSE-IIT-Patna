#include <iostream>
#include <stack>
#include <queue>

using namespace std;

const int liftUp = 1;
const int liftDown = -1;
int floorRate[] = {200,23,22,24,21,20,25,20,21,24};

struct InReq {
	char t = 'i';
	int empId = 0;
	int desFloor = 0;
	float reqTime = 0;
	InReq(){}
	InReq(int empId, int des, float time) {this->empId = empId; desFloor = des; reqTime = time;}
	void showInR() {
		cout << "empId: " << empId << ", desFloor:" << desFloor << ", reqTime: " << reqTime << ", t: " << t << endl;
	}
};

struct OutReq {
	char t = 'o';
	int empId = 0;
	int reqFloor = 0;
	int reqDir = 0;
	float reqTime = 0.0;
	OutReq(){}
	OutReq(int empId, int flr, float time) {this->empId = empId; reqFloor = flr; reqTime = time;}
	void showOutR() {
		cout << "empId: " << empId << ", reqFloor:" << reqFloor << ", reqTime:" << reqTime << ", t:" << t << endl;
	}
};

struct Emp {
	int id = 0;
	int office = 0;
	int curFloor = 0;
	int inLift = 0;
	Emp(){}
	Emp(int ID) {id = ID; office = ID%10; curFloor = ID%10;}
	InReq makeInReq(int des, float reqTime) { inLift = 1; return InReq(id,des,reqTime);}
	OutReq makeOutReq(int des, float reqTime) {return OutReq(id,des,reqTime);}
};

union ServiceStackElement {
	InReq inR;
	OutReq outR;
	ServiceStackElement(){}
};

struct ServeElement {
	char t;
	ServiceStackElement stackElement;
};

struct Service {
	queue <InReq> inQ;
	queue <OutReq> outQ;
	stack <ServeElement> serveStack;
};

struct Lift {
	Service serve;
	int curFloor = 0;
	int curDir = 0;
	int nextDes = 0;
	int pid[10];
	int noppl = 0;
	int notrips = 0;
	int doorOpen = 0;
	float stopwatch = 0;
	float timeout = 0;
	float traveltime = 0;
	Lift(){}
	Lift(int time) {timeout = time;}
	void updateRequest(InReq newinR) {
		serve.inQ.push(newinR);
		noppl++;
	}
	void updateRequest(OutReq newoutR) {
		serve.outQ.push(newoutR);
	}
	void serviceFunction() {
		if(serve.serveStack.empty()) {
			if(!serve.inQ.empty() && !serve.outQ.empty()) {
				if(serve.inQ.front().reqTime <= serve.outQ.front().reqTime) {
					ServeElement temp; temp.stackElement.inR = serve.inQ.front();
					temp.t = 'i'; serve.serveStack.push(temp);
					serve.inQ.pop();
				} else {
					ServeElement temp; temp.stackElement.outR = serve.outQ.front();
					temp.t = 'o'; serve.serveStack.push(temp);
					serve.outQ.pop();
				}
			} else if(!serve.inQ.empty()) {
				ServeElement temp; temp.stackElement.inR = serve.inQ.front();
				temp.t = 'i'; serve.serveStack.push(temp);
				serve.inQ.pop();
			} else {
				ServeElement temp; temp.stackElement.outR = serve.outQ.front();
				temp.t = 'o'; serve.serveStack.push(temp);
				serve.outQ.pop();
			}
		} //else {
			ServeElement temp = serve.serveStack.top();
			char t = temp.t;
			if(t == 'i') {
				nextDes = temp.stackElement.inR.desFloor;
			} else if(t == 'o') {
				nextDes = temp.stackElement.outR.reqFloor;
			}
			newDes(nextDes);
		//}
	}
	void liftMove(int dir) {
		if(dir == liftUp) {curFloor += liftUp; traveltime += 20; curDir = liftUp;}
		else if(dir == liftDown) {curFloor += liftDown; traveltime += 20; curDir = liftDown;}
	}
	void newDes(int nextDes) {
		if(nextDes > curFloor) {
			curDir = liftUp;
			while(nextDes != curFloor) { liftMove(liftUp); }
			if(nextDes == curFloor) { notrips++; }
			curDir = 0;
		} else if(nextDes < curFloor) {
			curDir = liftDown;
			while(nextDes != curFloor) { liftMove(liftDown); }
			if(nextDes == curFloor) { notrips++; }
			curDir = 0;
		}
	}
};

int main() {
	Lift lift1(60);
	Emp emp[1000];
	for(int i = 0; i < 1000; i++) { emp[i] = Emp(i);}
	//for(int i = 0; i < 10; i++) {
		lift1.updateRequest(emp[2].makeOutReq(emp[2].curFloor,lift1.traveltime));
		lift1.serve.outQ.front().showOutR();
		lift1.serviceFunction();
		lift1.serve.serveStack.top().stackElement.outR.showOutR();
		cout << lift1.traveltime << endl;
		cout << lift1.notrips << endl;
	//}
}