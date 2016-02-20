//============================================================================
// Name        : wall.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <set>

using namespace std;
typedef int Brick;

class Layer
{
	public:
		Layer(){;};

		Layer(const Layer& l){
			bricks = l.bricks;
		}

		int& operator [](const int i){
			return bricks[i];
		};

		friend std::ostream& operator<<(std::ostream& os, const Layer& l)
		{
			for(int i=0; i<l.bricks.size();i++)
				os << l.bricks[i] <<",";
			os<<endl;
		};

		void addBrick(Brick i){
			bricks.push_back(i);
		};

		int length(){
			return accumulate(bricks.begin(),bricks.end(),0);
		}

		int size(){
			return bricks.size();
		}


	static Layer CreateLayer(int i){
		Layer l;
		l.bricks.push_back(i);
		return l;
	};

	private:
		vector<int> bricks;
};

struct Layers : vector<Layer>
{
	friend std::ostream& operator<<(std::ostream& os, const Layers& l)
	{
		for(int i=0; i<l.size();i++)
			os << l[i];
		return os<<endl;
	};
};

typedef Layers Wall;

class WallFactory
{
	private:
		int width;
		int length;
		vector<Brick> startList;
		Layers patterns;
		vector<vector<int>> adjList;
	public:
		WallFactory(int w, int l, vector<Brick>& start){
			width=w;
			length=l;
			startList=start;
		}

		void setup(queue<Layer>& q, vector<Brick> bricks){
			for(int i=0; i<bricks.size();i++)
				q.push(Layer::CreateLayer(bricks[i]));
		}

		void createLayers(){
			queue<Layer> q;
			setup(q,startList);
			while(!q.empty()){
				Layer l = q.front();
				q.pop();
				if(l.length()> this->length) //pops the wall element
					continue;
				if(l.length()==this->length) //add to the layers
					patterns.push_back(l);
				for(int i=0; i<startList.size();i++ ){
					Layer newLayer = l;
					newLayer.addBrick(startList[i]);
					q.push(newLayer);
				}
			}
		}

		set<int> getPositions(Layer l, int w){
			int acc=0;
			set<int> pos;
			for(int i=0;i<l.size();i++){
				acc+=l[i];
				pos.insert(acc);
			}
			pos.erase(w);
			return pos;
		}

		bool isDisjoint(const set<int> &set1, const set<int> &set2)
		{
		    if(set1.empty() || set2.empty()) return true;

		    set<int>::const_iterator it1 = set1.begin(), it1End = set1.end();
		    set<int>::const_iterator it2 = set2.begin(), it2End = set2.end();

		    if(*it1 > *set2.rbegin() || *it2 > *set1.rbegin()) return true;

		    while(it1 != it1End && it2 != it2End)
		    {
		        if(*it1 == *it2) return false;
		        if(*it1 < *it2) { it1++; }
		        else { it2++; }
		    }

		    return true;
		}

		void createAdjList(){
			//based on vector of layers, you can track what can be combined
			vector<set<int>> layerPos;
			for(int i=0;i<patterns.size();i++){
				set<int> pos = getPositions(patterns[i], length);
				layerPos.push_back(pos);
			}
			vector<vector<int>> adjList_(patterns.size(),vector<int>());
			for(int i=0;i<patterns.size();i++){
				for(int j=0;j<patterns.size();j++){
					if(isDisjoint(layerPos[i],layerPos[j]))
						adjList_[i].push_back(j);
				}
			}
			adjList=adjList_;
		}

		void printPatterns(){ cout<<this->patterns<<endl;}

		void printAdjList(){
			cout<<"adjList: [";
			for(int i=0; i<adjList.size();i++){
				//cout<<"["<<;
				cout<<"[";
				for(int j=0;j<adjList[i].size();j++){
					cout<<adjList[i][j]<<",";
				}
				cout<<"]";
			}
			cout<<"]"<<endl;
		}

		int countWalls(){
			vector<int> nWalls(patterns.size(),1);
			for(int i=1;i<width;i++){ //wall height
				vector<int> tempWalls(patterns.size(),1);
				for(int j=0; j<patterns.size(); j++){
					int num=0;
					for(int k=0;k<adjList[j].size();k++){
						int index = adjList[j][k];
						num+=nWalls[index];
					}
					tempWalls[j]=num;
				}
				nWalls=tempWalls;
			}
			return accumulate(nWalls.begin(),nWalls.end(),0);
		}
};

void pause() {
    cin.clear();
    cout << endl << "Press any key to continue...";
    cin.ignore();
};

void BuildWall(int width, int height, vector<Brick> start )
{
	WallFactory w(height, width, start);
	w.createLayers();
	//w.printPatterns();
	w.createAdjList();
	//w.printAdjList();
	cout<<"Wall count for "<<width<<"x"<<height<<" ="<<w.countWalls()<<endl;
	cout.flush();
}

int main(){
	vector<Brick> start = {2,3};
	BuildWall(9,3,start);
	BuildWall(18,5,start);
	pause();
	return 0;
}

