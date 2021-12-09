#include<iostream>
#include<fstream>
#include<string>
#include<iomanip>
#include<vector>
# define T template <class k>

using namespace std;

// class to keep record of patients
class PatientRecord
{
private:
	int Patient_ID;
	string Patient_Name;
	string Admission_Date;
	string Disease;
	string Status;
public:
	PatientRecord()
	{
		Patient_ID = 0;
		Patient_Name = Admission_Date = Disease = Status = { };
	}

	PatientRecord(int id, string name, string date, string disease, string status)
	{
		Patient_ID = id;
		Patient_Name = name;
		Admission_Date = date;
		Disease = disease;
		Status = status;
	}

	int GetID()
	{
		return Patient_ID;
	}

	void Input()
	{
		cout << "\nEnter Patient Information\n";
		cout << "\nEnter ID Information\n";
		cin >> Patient_ID;
		cout << "\nEnter Name Information\n";
		cin >> Patient_Name;
		cout << "\nEnter Date Information\n";
		cin >> Admission_Date;
		cout << "\nEnter Disease Information\n";
		cin >> Disease;
		cout << "\nEnter Status Information\n";
		cin >> Status;
		cout << endl;
	}

	void Ouput()
	{
		cout << "\n_____________________________________Patient Record_____________________________________" << endl << endl;
		cout << "\nName:	" << Patient_Name;
		cout << "\nID:	" << Patient_ID;
		cout << "\nAdmission Date:	" << Admission_Date;
		cout << "\nDisease:	" << Disease;
		cout << "\nStatus:	" << Status << endl;
	}

	T
	friend class HospitalData;
};

T 
class TNode// Node of tree 
{
private:
	PatientRecord Data;// composition
	TNode<k>* Left;
	TNode<k>* Right;
public:
	TNode()
	{
		Data = 0;
		Left = NULL;
		Right = NULL;
	}

	TNode(PatientRecord Patient = 0 , TNode<k>* left =NULL , TNode<k>* right = NULL)
	{

		Data = Patient;
		Left = left;
		Right = right;

	}
	T friend class HospitalData;
};
 
T
class HospitalData// class to hold nodes data
{
private:
	TNode<k>* Root;
	int Total_Patients;
public:
	HospitalData()
	{
		Root = NULL;
		Total_Patients = 0;
	}

	void Destroy(TNode<k>*& Ptr)
	{
		if (Ptr)
		{
			Destroy(Ptr->Left);// all lefts start from leaf to root
			Destroy(Ptr->Right);
			delete Ptr;
			Total_Patients--;
		}
	}

	~HospitalData()
	{
		Destroy(Root);
	}

	int height(TNode<k>* node)
	{
		if (node == nullptr)
			return -1;
		else
		{
			if (node->Left == NULL && node->Right == NULL)
				return 0;
			else
			{
				int LtreeHeight = height(node->Left);
				int RtreeHeight = height(node->Right);
				return max(LtreeHeight, RtreeHeight) + 1;
			}
		}
	}

	void RightRotate(TNode<k>*& x)
	{
		TNode<k>* y = x->Left;
		if (y)
		{
			TNode<k>* orphan = x->Left->Right;
			y->Right = x;
			x->Left = orphan;
			x = y;
		}
			
	}

	void LeftRotate(TNode<k>*& x)// rotate to left and update the node coming by reference
	{
		TNode<k>* y = x->Right;
		if (y)
		{
			TNode<k>* orphan = x->Right->Left;
			y->Left = x;
			x->Right = orphan;
			x = y;
		}
	}

	void MoveToRoot(TNode<k>*& curr, int& i)// decide the rotation depending upon the value of i
	{
		if (i == 1)
			RightRotate(curr);
		else if (i == 2)
			LeftRotate(curr);
	}

	void InsertPatient(PatientRecord Patient)// wrapper for insertion
	{
		TNode<k>* temp = Root;
		int rotation_type = 0;
		InsertPatient(Root, Patient , rotation_type);
		Total_Patients++;
	}

	void InsertPatient(TNode<k>*& curr, PatientRecord Patient , int& i)// inserrt node at the end then move the newly inserted node to root
	{
		if (curr == NULL) 
		{
			curr = new TNode<k>(Patient);
				if (Root == NULL)
					Root = curr;
		}
		else if (Patient.Patient_ID < curr->Data.Patient_ID)
			InsertPatient(curr->Left, Patient , i) , i=1;// i=1 indicate node is inserted at left side
		else
			InsertPatient(curr->Right, Patient , i) , i=2;// i=2 indicate node is inserted at right side

		MoveToRoot(curr, i);// move the new node to root
	}

	PatientRecord getPredecessor(TNode<k>* curr)
	{
		while (curr->Right != NULL)
			curr= curr->Right;
		return curr->Data;
	}

	void DeletePatient(TNode<k>*& curr)// search predecessor and delete node
	{
		TNode <k>* temp = curr;
		if (curr->Left == NULL)
		{
			curr = curr->Right;
			delete temp;
		}
		else if (curr->Right == NULL) 
		{
			curr = curr->Left;
			delete temp;
		}
		else
		{
			PatientRecord data = getPredecessor(curr->Left);
			curr->Data = data;
			Remove(curr->Left , data.GetID());
		}
	}

	void Remove(TNode<k>*& curr, int ID)
	{
		if (curr)
		{
			if (ID < curr->Data.Patient_ID)
				Remove(curr->Left, ID);
			else if (ID > curr->Data.Patient_ID)
				Remove(curr->Right, ID);
			else// if found call the delete function
				DeletePatient(curr);
		}
	}

	void Remove(int ID)// wrapper for remove
	{
		TNode<k>* temp = Root;
		Remove(Root, ID);
		Total_Patients--;
	}

	void Print()// wrapper for print
	{
		cout << "\nEnter ID to view status:	";
		int ID; 
		cin >> ID;
		Print(Root, ID);
	}

	void Print(TNode<k>*& curr, int ID)
	{
		if (curr)
		{
			if (curr->Data.Patient_ID == ID)
				curr->Data.Ouput();
			else if (curr->Data.Patient_ID > ID)
				Print(curr->Left, ID);
			else
				Print(curr->Right, ID);
		}
	}

	void Print_Admitted(TNode<k>*& curr)// Print all the admiited patients in order of their ID's
	{
		if (curr)
		{
			Print_Admitted(curr->Left);
			if (curr->Data.Status == "Admitted")
				curr->Data.Ouput();
			Print_Admitted(curr->Right);
		}
	}

	void Print_Admitted()
	{
		TNode<k>* temp = Root;
		Print_Admitted(temp);
	}

	void SearcH(TNode<k>*& curr, int ID, int Level , int& type , int& counter , int& level , PatientRecord& P)
	{
		if (curr)
		{
			TNode<k>* prev = curr;// store the curr node
			if (curr->Data.Patient_ID == ID)
			{
				P = curr->Data;// if found store data
				counter = level - Level;
			}
			else if (curr->Data.Patient_ID > ID)// move to left
			{
				type = 1;
				prev = curr;
				SearcH(curr->Left, ID, Level, type, counter, level+=1 , P);
			}
			else
			{
				type = 2;
				prev = curr;
				SearcH(curr->Right, ID, Level, type, counter, level+=1 , P);
			}

			if (level > Level)// rotation is performed only if the current level of node is higher than the desired level
			{
				if (counter > 0)
				{
					prev = curr;
					MoveToRoot(curr, type);

					if(prev->Data.Patient_ID != P.Patient_ID)// check fotr leaf node
						counter--;
				}
			}
		}
	}

	PatientRecord Search(int ID, int Level)
	{
		int Rotation = 0;
		int counter = 0; 
		int level = 0;
		PatientRecord P;
		SearcH(Root, ID, Level, Rotation, counter, level , P);
		return P;
	}

	void ArrayFill(TNode<k>*& curr, vector<PatientRecord> &arr)// helper for split
	{
		if (curr)
		{
			ArrayFill(curr->Left, arr);
			arr.push_back(curr->Data);
			ArrayFill(curr->Right, arr);
		}
	}

	HospitalData Split()
	{
		HospitalData<PatientRecord> SplitTree1;
		HospitalData<PatientRecord> SplitTree2;
		HospitalData<PatientRecord>* Tree = this;
		vector<PatientRecord> vec;
		if (Tree->Root)// find median
		{
			ArrayFill(Tree->Root,vec);// find median
			int median = -1;
			if (vec.size() % 2 == 0)
				median = (vec.size() + 1) / 2;
			else
				median = (vec.size() )/ 2;

			int i = 0;
			for ( ; i < median; i++)
				SplitTree1.InsertPatient(vec[i]);
			i++;
			for ( ;i < vec.size() ; i++)
				SplitTree2.InsertPatient(vec[i]);
		}
		return SplitTree2;
	}

	HospitalData& operator=(const HospitalData<k>& obj)// assignment operator
	{
		if (this != *obj)// avoid self assignment
		{
			if (this->Root)// if it exists already delete 
				Destroy(this);
			Duplicate(obj.Root, this->Root);
		}
	}

	void Duplicate(TNode<k>* curr_tree, TNode<k>*& dup_tree) // copy constructor
	{
		if (curr_tree)// set the value from curr_tree
		{
			dup_tree = new TNode<k>(curr_tree->Data);
			Duplicate(curr_tree->Left, dup_tree->Left);
			Duplicate(curr_tree->Right, dup_tree->Right);
		}
	}

	HospitalData (const HospitalData<k>& obj)
	{
		Duplicate(obj.Root, this->Root);
	}
};


int main()
{
	cout << "\n_____________________________________Hospital System_____________________________________" << endl << endl;// menu bnana 
	HospitalData<PatientRecord> Hospital;

	PatientRecord P1(34, "Aqeedat", "12/9/2021", "Pain", "Admitted");
	PatientRecord P2(23,"Abeera","12/12/2021","Eye Sight","Discharged");
	PatientRecord P3(61, "Rayma", "12/11/2021", "Dengue", "Admitted");
	PatientRecord P4(46, "Sameera", "12/10/2021", "Malaria", "Discharged");
	PatientRecord P5(58, "Raheem", "12/7/2021", "Headache", "Admitted");

	Hospital.InsertPatient(P1);
	Hospital.InsertPatient(P2);
	Hospital.InsertPatient(P3);
	Hospital.InsertPatient(P4);
	Hospital.InsertPatient(P5);

	PatientRecord P6;
	P6.Input();

	
	Hospital.InsertPatient(P6);

	Hospital.Print();

	cout << "\n_____________________________________Admitted Patients_____________________________________" << endl << endl;
	Hospital.Print_Admitted();

	cout << "\n_____________________________________Split Tree_____________________________________" << endl << endl;
	HospitalData<PatientRecord> SplitTree = Hospital.Split();
	SplitTree.Print_Admitted();

	cout << "\n_____________________________________After Removal_____________________________________" << endl << endl;
	Hospital.Remove(P1.GetID());
	Hospital.Print_Admitted();

	cout << "\n_____________________________________Search Patient_____________________________________" << endl << endl;
	PatientRecord P = Hospital.Search(34, 1);

	if (P.GetID() == 0)
		cout << "\nPatient record not Found.......\n\n";
	else
		P.Ouput();

	return 0;
}