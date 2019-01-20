#include "stdafx.h"
#include "GMCTS.h"


GMCTS::GMCTS(void)
{
}


GMCTS::~GMCTS(void)
{
}

void GMCTS::OnClickedButtonRun()//main function of GMCTS
{
	MTCLA_Init();
	MTCLA_Purpers_Get();//get the goals as heuristics 
	MTCLA_Sequence_Get();//get the priorities
	MTCLA_Fast_Sequence_Get();//get the cell-oriented searching resault

	BeginWaitCursor();
	MTCLA_On=true;
	double Per_t=0;
	CString str_t;
	DWORD MTCLA_Esplise1,MTCLA_Esplise2;
	MTCLA_Timer1 = SetTimer(1, 10000, NULL);
	MTCLA_Esplise1 = ::GetTickCount();
	vector <int> action_step;
	int action; 
	int bestAction;
	int deep;
	deep_max=0;
	deep_max_deep1=0;
	deep_max_step=0;
	double value=0;
	double value_goal_oriented=0;
	double value_max=0;
	double value_goal_oriented_max=0;
	double value_Step_max=0;
	int i,k,n;
	MTCLA tree;//main tree
	MTCLA *pCur_tree_Branch = &tree;//new subtree
	stack <MTCLA *> visited;
	for(i=0;i<site;i++)//the steps, 8 for example
	{	
		MTCLA tree_Branch;
		tree_Branch=*pCur_tree_Branch;
		for(k=0; k<goal_time; k++)//times in each steps
		{
			MTCLA *pCur = &tree_Branch;
			visited.push(pCur);
			action_step.clear();
			action_step=MTCLA_action_step_finnal;
			MTCLA_P_Once=MTCLA_P_finnal; 
			MTCLA_Color_Once=MTCLA_Color_finnal; 
			while (pCur->isLeaf()==false) //sellection
			{
				action = pCur->selectAction(NBBROW_B);
				pCur = pCur->vpChildren_i[action];
				if (pCur->isCut_i==true)
				{
					pCur->totValue_i=0;
					pCur->eveValue_i=0;
					break;
				}
				action_step.push_back(action);
				visited.push(pCur);
				deep++;
				MTCLA_P_Once.push_back(pCur->Point_i);
				MTCLA_Color_Once.push_back(pCur->Color_i);
				US_present=pCur->US_present_i;
				Goal_present=pCur->Goal_present_i;
			}
			if((pCur->isLeaf()==true)&&(pCur->nVisits_i==1))//expansion
			{
				pCur->expand();//
				MTCLA_Goal_Steps(goal_steps,deep);
				MTCLA_P_i.resize(NBBROW_B);
				MTCLA_Color_i.resize(NBBROW_B);
				MTCLA_US_present_i.resize(NBBROW_B);
				MTCLA_Goal_present_i.resize(NBBROW_B);
				for (n=0;n<NBBROW_B;n++)
				{	
					MTCLA_US_present_i[n]=US_present;
					MTCLA_Goal_present_i[n]=Goal_present;
					//expansion by the priorities of land cells and evaluate the goal-oriented values
					MTCLA_Find_Move(n,US_P_Step,Goal_P_Step,MTCLA_P_Once,&MTCLA_P_i[n],&MTCLA_Color_i[n],&MTCLA_US_present_i[n],&MTCLA_Goal_present_i[n]);
					MTCLA *pCur_C;
					pCur_C=pCur->vpChildren_i[n];
					pCur_C->deep_i=deep;
					pCur_C->Point_i=MTCLA_P_i[n];
					pCur_C->Color_i=MTCLA_Color_i[n];
					pCur_C->US_present_i=MTCLA_US_present_i[n];
					pCur_C->Goal_present_i=MTCLA_Goal_present_i[n];
				}
				action = pCur->selectAction(NBBROW_B);//sellection
				pCur = pCur->vpChildren_i[action];
				action_step.push_back(action);
				visited.push(pCur);
				deep++;
				MTCLA_P_Once.push_back(pCur->Point_i);
				MTCLA_Color_Once.push_back(pCur->Color_i);
				US_present=pCur->US_present_i;
				Goal_present=pCur->Goal_present_i;
			}
			MTCLA_Purpers_Main_Fastmove(MTCLA_P_Once,&US_present,&Goal_present);//get the rollout value
			pCur->US_present_i_All=US_present;
			pCur->Goal_present_i_All=Goal_present;
			value=MTCLA_Value_Estamat(US_present,Goal_present);//evaluation=goal+rollout
			if (value>=value_max)
			{
				MTCLA_P_Picture=MTCLA_P_Once;
				MTCLA_Color_Picture=MTCLA_Color_Once;
				MTCLA_US_Picture=pCur->US_present_i;
				MTCLA_Goal_Picture=pCur->Goal_present_i;
				value_goal_oriented=MTCLA_Value_Estamat(pCur->US_present_i,pCur->Goal_present_i);
				if (value_goal_oriented>value_goal_oriented_max)
					value_goal_oriented_max=value_goal_oriented;
			while (!visited.empty()) 
			{
				pCur = visited.top();// get the current node in the path
				pCur->updateStats(value,value_goal_oriented);   // update statistics
				visited.pop();// remove the current node from the stack
			}
			Per_t=100*(i*goal_time+k)/(site*goal_time);
			MTCLA_Esplise2 = ::GetTickCount() - MTCLA_Esplise1;/
			str_t.Format("Step %d/%d,  number %d/%d,  %d s ,  %.0f%s ;",i+1,goal_steps,k+1,goal_time,MTCLA_Esplise2/1000,Per_t,"%");
			Progreess_Per.SetWindowText(str_t);
		}//for k
		double best_v=0;
		bestAction = tree_Branch.bestAction(&best_v);//get the best step action after a step's searching
		MTCLA_action_step_finnal.push_back(bestAction);
		MTCLA_action_best_v_finnal.push_back(best_v);
		MTCLA *pCur_B = &tree_Branch;
		pCur_B = pCur_B->vpChildren_i[bestAction];
		MTCLA_P_finnal.push_back(pCur_B->Point_i);
		MTCLA_Color_finnal.push_back(pCur_B->Color_i);
		for (k=0;k<(pCur_B->Point_i.size());k++)
		{
			for (int p=0;p<5;p++)
			{
				MTCLA_Value_Small[(pCur_B->Point_i)[k].x][(pCur_B->Point_i)[k].y][p]=0;
			}
		}
		US_present=pCur_B->US_present_i;
		Goal_present=pCur_B->Goal_present_i;
		MTCLA_US_present_Picture.push_back(pCur_B->US_present_i);
		MTCLA_Goal_present_Picture.push_back(pCur_B->Goal_present_i);	
		MTCLA_value_Goal_oriented.push_back(value_goal_oriented_max);
		if (value_Step_max<MTCLA_Value_Estamat(US_present,Goal_present))
			value_Step_max=MTCLA_Value_Estamat(US_present,Goal_present);
		MTCLA_value_Step.push_back(value_Step_max);
		*pCur_tree_Branch=tree_Branch;//the subtree result added back to main tree
		pCur_tree_Branch = pCur_tree_Branch->vpChildren_i[bestAction];//select the next layer as the new subtree
		Progress.SetPos(i);
	}//for int i
	if (MTCLA_Save_All_Picture==true)//show the results
	{
		MTCLA_OnImageSave()
		MTCLA_Purpers_Show(MTCLA_P_Once_Cell_Oriented,MTCLA_Color_Once_Cell_Oriented);
		MTCLA_Purpers_Show_Finnal(MTCLA_P_Picture,MTCLA_Color_Picture,&MTCLA_US_Picture,&MTCLA_Goal_Picture,&value);
		MTCLA_Purpers_Show_Text(value);	
		CString m_s;
		m_s.Format("Degree of satisfying the Goals(0-1):%.2f", value);
		AfxMessageBox(m_s);
	}
}
	
MTCLA::MTCLA(void)//main tree
{
	isLeaf_i=true;
	isCut_i=false;
	nVisits_i=0;
	totValue_i=0;
	eveValue_i=0;
	goalValue_i=0;
	childNum=NBBROW_A;////////////////////////////////////////////////////////////
	deep_i=0;
	Value_Pre=0;
	for (int k = 0; k < childNum; ++k)
	{
		vpChildren_i[k] = 0;
	}
}

MTCLA::~MTCLA(void)
{
}
	
bool MTCLA::isLeaf() const 
{
	return isLeaf_i;
}

int MTCLA::selectAction(int childNum_B) //selection
	{
		assert(isLeaf_i==false); 
		int selected = 0;
		double bestValue;
		bestValue= -(numeric_limits<double>::max)();
		double uctValue ;
		for (int k = 0; k < childNum_B; ++k)
		{
			MTCLA *pCur = vpChildren_i[k];
			//assert(0 != pCur); 
			if (pCur->nVisits_i==0)
				uctValue=(numeric_limits<double>::max)();
			else if (pCur->nVisits_i>0)
			{
				uctValue = pCur->totValue_i / (pCur->nVisits_i + EPSILON_A) + 0.4*sqrt(log(nVisits_i) / (pCur->nVisits_i + EPSILON_A));
			}
			if (uctValue >bestValue)
			{
				selected = k;
				bestValue = uctValue;
			}
		} // for loop
		return selected; 
	} // selectAction

void MTCLA::expand() //expansion
{
	if (!isLeaf_i)
		return;
	//isLeaf_i = false;
	for (int k = 0; k < childNum; ++k)
		vpChildren_i[k] = new MTCLA();
	isLeaf_i=false;
	
} // expand


void  MTCLA::updateStats(double value,double gvalue)
{
	nVisits_i++;  // increment the number of visits
	if (nVisits_i==1)
	{
		if (value>0)
		{
			totValue_i = value; // update the total value for all visits
			goalValue_i=gvalue;
		}
	}
	else
	{
		if (value>(totValue_i/(nVisits_i-1)))
		{
			totValue_i = value*nVisits_i; // update the total value for all visits
			goalValue_i=gvalue;
		}
		else
			totValue_i=(totValue_i/(nVisits_i-1))*nVisits_i;

	}
	eveValue_i=totValue_i/nVisits_i;
}

void MTCLA::Value() const 
{
	CString m_s;
	m_s.Format("%lf,  %lf ",totValue_i  ,nVisits_i );
	AfxMessageBox(m_s);
}

int MTCLA:: bestAction(double* bv) //evaluation
{ 
	int selected = 0;
	double bestValue = -(numeric_limits<double>::max)();
	for (int k = 0; k < childNum; ++k) 
	{
		MTCLA *pCur = vpChildren_i[k]; // ptr to current child node
		assert(0 != pCur);
		double expValue = pCur->totValue_i / (pCur->nVisits_i + EPSILON_A);
		expValue += static_cast<double>(rand()) * EPSILON_A / RAND_MAX;
		if (expValue >= bestValue) 
		{
			selected = k;
			bestValue = expValue;
		}
	} // for loop
	* bv=bestValue;
	return selected;
} // bestAction
