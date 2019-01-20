#include "stdafx.h"
#include "GMCTS.h"


GMCTS::GMCTS(void)
{
}


GMCTS::~GMCTS(void)
{
}

void GMCTS::OnClickedButtonRun()
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
	MTCLA *pCur_tree_Branch = &tree;//
	stack <MTCLA *> visited;
	for(i=0;i<site;i++)//所有用地单元
	{	
		WaitForIdle_A();
		//value_goal_oriented_max=0;
		if(MTCLA_On==false) //检查停止标志
			return;
		MTCLA tree_Branch;
		tree_Branch=*pCur_tree_Branch;
		for(k=0; k<goal_time; k++)//每一个用地单元的time次迭代
		{
			WaitForIdle_A();
			if(MTCLA_On==false) //检查停止标志
				return;
			MTCLA *pCur = &tree_Branch;
			visited.push(pCur);//首次堆栈，母树
			action_step.clear();
			action_step=MTCLA_action_step_finnal;//action衔接
			deep=pCur_tree_Branch->deep_i;//deep衔接
			deep++;//每次堆栈深度加1
			MTCLA_P_Once=MTCLA_P_finnal; 
			MTCLA_Color_Once=MTCLA_Color_finnal; 
			if((pCur->isLeaf()==true)&&(pCur->deep_i==pCur_tree_Branch->deep_i))//最早叶点唯一一次扩展
			{
				pCur->expand();//新根节点扩展,isLeaf_i=false;
				MTCLA_Goal_Steps(goal_steps,deep);//基于现在状况得到US_P_Step，Goal_P_Step
				MTCLA_P_i.resize(NBBROW_B);
				MTCLA_Color_i.resize(NBBROW_B);
				MTCLA_US_present_i.resize(NBBROW_B);
				MTCLA_Goal_present_i.resize(NBBROW_B);
				for (n=0;n<NBBROW_B;n++)//深度与点位赋值
				{	
					MTCLA_US_present_i[n]=US_present;
					MTCLA_Goal_present_i[n]=Goal_present;
					MTCLA_Find_Move(n,US_P_Step,Goal_P_Step,MTCLA_P_Once,&MTCLA_P_i[n],&MTCLA_Color_i[n],&MTCLA_US_present_i[n],&MTCLA_Goal_present_i[n]);
					MTCLA *pCur_C;
					pCur_C=pCur->vpChildren_i[n];
					pCur_C->deep_i=deep;
					pCur_C->Point_i=MTCLA_P_i[n];
					pCur_C->Color_i=MTCLA_Color_i[n];
					pCur_C->US_present_i=MTCLA_US_present_i[n];
					pCur_C->Goal_present_i=MTCLA_Goal_present_i[n];
				}
			}
			while (pCur->isLeaf()==false) //如果是根节点.向下选择前进直到叶节点
			{
				action = pCur->selectAction(NBBROW_B);//选择最大叶节点
				pCur = pCur->vpChildren_i[action];//叶变根
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
			if (pCur->isCut_i==true)
				continue;
			if((pCur->isLeaf()==true)&&(pCur->nVisits_i==1))//如果是叶节点，达到一次才扩展
			{
				pCur->expand();//新根节点扩展,isLeaf_i=false;
				MTCLA_Goal_Steps(goal_steps,deep);//基于现在状况得到US_P_Step，Goal_P_Step
				MTCLA_P_i.resize(NBBROW_B);
				MTCLA_Color_i.resize(NBBROW_B);
				MTCLA_US_present_i.resize(NBBROW_B);
				MTCLA_Goal_present_i.resize(NBBROW_B);
				for (n=0;n<NBBROW_B;n++)//深度与点位赋值
				{	
					MTCLA_US_present_i[n]=US_present;
					MTCLA_Goal_present_i[n]=Goal_present;
					MTCLA_Find_Move(n,US_P_Step,Goal_P_Step,MTCLA_P_Once,&MTCLA_P_i[n],&MTCLA_Color_i[n],&MTCLA_US_present_i[n],&MTCLA_Goal_present_i[n]);
					MTCLA *pCur_C;
					pCur_C=pCur->vpChildren_i[n];
					pCur_C->deep_i=deep;
					pCur_C->Point_i=MTCLA_P_i[n];
					pCur_C->Color_i=MTCLA_Color_i[n];
					pCur_C->US_present_i=MTCLA_US_present_i[n];
					pCur_C->Goal_present_i=MTCLA_Goal_present_i[n];
				}
				action = pCur->selectAction(NBBROW_B);//新根节点选择最大叶节点
				pCur = pCur->vpChildren_i[action];//叶变根
				action_step.push_back(action);
				visited.push(pCur);
				deep++;
				MTCLA_P_Once.push_back(pCur->Point_i);
				MTCLA_Color_Once.push_back(pCur->Color_i);
				US_present=pCur->US_present_i;
				Goal_present=pCur->Goal_present_i;
			}
			//value_goal_oriented=MTCLA_Value_Estamat(US_present,Goal_present);
			MTCLA_Purpers_Main_Fastmove(MTCLA_P_Once,&US_present,&Goal_present);
			pCur->US_present_i_All=US_present;
			pCur->Goal_present_i_All=Goal_present;
			value=MTCLA_Value_Estamat(US_present,Goal_present);//价值计算，关键！
			if (value>=value_max)//记录最大值增加的情况
			{
				MTCLA_P_Picture=MTCLA_P_Once;
				MTCLA_Color_Picture=MTCLA_Color_Once;
				MTCLA_US_Picture=pCur->US_present_i;
				MTCLA_Goal_Picture=pCur->Goal_present_i;
				//value_goal_oriented=MTCLA_Value_Estamat(MTCLA_US_Picture,MTCLA_Goal_Picture);
				value_goal_oriented=MTCLA_Value_Estamat(pCur->US_present_i,pCur->Goal_present_i);
				if (value_goal_oriented>value_goal_oriented_max)
					value_goal_oriented_max=value_goal_oriented;
				if (value>value_max)
				{
					if (deep_max<(deep+1))//记录最大深度
						deep_max=deep+1;
					if (deep_max_step<(i+1))//记录最大深度
						deep_max_step=i+1;

				}
				value_max=value;
			}
			//if (value<10)
			//	pCur->isCut_i=true;
			if (deep_max_deep1<(deep+1) && i==0)//记录最大深度
				deep_max_deep1=deep+1;	
			while (!visited.empty()) //反向传播
			{
				pCur = visited.top();// get the current node in the path
				pCur->updateStats(value,value_goal_oriented);   // update statistics为什么是+随机数
				visited.pop();// remove the current node from the stack
			}
			Per_t=100*(i*goal_time+k)/(site*goal_time);
			MTCLA_Esplise2 = ::GetTickCount() - MTCLA_Esplise1;//差值=现在计时器-原有计时器
			str_t.Format("Step %d/%d,  number %d/%d,  %d s ,  %.0f%s ;",i+1,goal_steps,k+1,goal_time,MTCLA_Esplise2/1000,Per_t,"%");
			Progreess_Per.SetWindowText(str_t);
		}//for k
		double best_v=0;
		bestAction = tree_Branch.bestAction(&best_v);//计算单步最佳
		MTCLA_action_step_finnal.push_back(bestAction);
		MTCLA_action_best_v_finnal.push_back(best_v);
		MTCLA *pCur_B = &tree_Branch;
		pCur_B = pCur_B->vpChildren_i[bestAction];//叶变根
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
		MTCLA_value_Goal_oriented.push_back(value_goal_oriented_max);//价值计算，关键！	//新
		if (value_Step_max<MTCLA_Value_Estamat(US_present,Goal_present))
			value_Step_max=MTCLA_Value_Estamat(US_present,Goal_present);
		MTCLA_value_Step.push_back(value_Step_max);//价值计算，关键！	//新
		/////////////////////////////////////////////////////////
		*pCur_tree_Branch=tree_Branch;//每一个用地单元的time次迭代——还原至母树
		pCur_tree_Branch = pCur_tree_Branch->vpChildren_i[bestAction];//选择母树的下一层分支
		Progress.SetPos(i);
		//缺少达标停止函数！！！
	}//for int i
	KillTimer(MTCLA_Timer1);//删除计时器
	MTCLA_Esplise2 = ::GetTickCount() - MTCLA_Esplise1;//差值=现在计时器-原有计时器
	Per_t=100;
	str_t.Format("Step %d/%d,  number %d/%d,  %d s ,  %.0f%s ;",goal_steps,goal_steps,goal_time,goal_time,MTCLA_Esplise2/1000,Per_t,"%");
	Progreess_Per.SetWindowText(str_t);
	EndWaitCursor();
	WaitForIdle_A();
	if(MTCLA_On==false) //检查停止标志
		return;
	MTCLA_Save_All_Picture=true;
	if (MTCLA_Save_All_Picture==true)
	{
		MTCLA_OnImageSave();//获取路径
		MTCLA_Purpers_Show(MTCLA_P_Once_Cell_Oriented,MTCLA_Color_Once_Cell_Oriented);	//输出原始，cell图像
		MTCLA_Purpers_Show_Finnal(MTCLA_P_Picture,MTCLA_Color_Picture,&MTCLA_US_Picture,&MTCLA_Goal_Picture,&value);	//输出快速布局图像
		MTCLA_Purpers_Show_Text(value);	//输出文本
		CString m_s;
		m_s.Format("Degree of satisfying the Goals(0-1):%.2f", value);
		AfxMessageBox(m_s);
	}
	//MTCLA_Purpers_Show(MTCLA_P_finnal,MTCLA_Color_finnal);	//输出图像,倒数第二张,暂时用不到
	MTCLA_On=false;
	Button_Enable();
	OnClickedButton_Up();
	MTCLA_Play=true;
	((CButton* )GetDlgItem(IDC_BUTTON_MTCL_PLAY))->EnableWindow(true);
	((CButton* )GetDlgItem(IDC_BUTTON_MTCL_STOP))->EnableWindow(false);

}