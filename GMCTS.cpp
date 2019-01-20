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
	for(i=0;i<site;i++)//�����õص�Ԫ
	{	
		WaitForIdle_A();
		//value_goal_oriented_max=0;
		if(MTCLA_On==false) //���ֹͣ��־
			return;
		MTCLA tree_Branch;
		tree_Branch=*pCur_tree_Branch;
		for(k=0; k<goal_time; k++)//ÿһ���õص�Ԫ��time�ε���
		{
			WaitForIdle_A();
			if(MTCLA_On==false) //���ֹͣ��־
				return;
			MTCLA *pCur = &tree_Branch;
			visited.push(pCur);//�״ζ�ջ��ĸ��
			action_step.clear();
			action_step=MTCLA_action_step_finnal;//action�ν�
			deep=pCur_tree_Branch->deep_i;//deep�ν�
			deep++;//ÿ�ζ�ջ��ȼ�1
			MTCLA_P_Once=MTCLA_P_finnal; 
			MTCLA_Color_Once=MTCLA_Color_finnal; 
			if((pCur->isLeaf()==true)&&(pCur->deep_i==pCur_tree_Branch->deep_i))//����Ҷ��Ψһһ����չ
			{
				pCur->expand();//�¸��ڵ���չ,isLeaf_i=false;
				MTCLA_Goal_Steps(goal_steps,deep);//��������״���õ�US_P_Step��Goal_P_Step
				MTCLA_P_i.resize(NBBROW_B);
				MTCLA_Color_i.resize(NBBROW_B);
				MTCLA_US_present_i.resize(NBBROW_B);
				MTCLA_Goal_present_i.resize(NBBROW_B);
				for (n=0;n<NBBROW_B;n++)//������λ��ֵ
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
			while (pCur->isLeaf()==false) //����Ǹ��ڵ�.����ѡ��ǰ��ֱ��Ҷ�ڵ�
			{
				action = pCur->selectAction(NBBROW_B);//ѡ�����Ҷ�ڵ�
				pCur = pCur->vpChildren_i[action];//Ҷ���
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
			if((pCur->isLeaf()==true)&&(pCur->nVisits_i==1))//�����Ҷ�ڵ㣬�ﵽһ�β���չ
			{
				pCur->expand();//�¸��ڵ���չ,isLeaf_i=false;
				MTCLA_Goal_Steps(goal_steps,deep);//��������״���õ�US_P_Step��Goal_P_Step
				MTCLA_P_i.resize(NBBROW_B);
				MTCLA_Color_i.resize(NBBROW_B);
				MTCLA_US_present_i.resize(NBBROW_B);
				MTCLA_Goal_present_i.resize(NBBROW_B);
				for (n=0;n<NBBROW_B;n++)//������λ��ֵ
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
				action = pCur->selectAction(NBBROW_B);//�¸��ڵ�ѡ�����Ҷ�ڵ�
				pCur = pCur->vpChildren_i[action];//Ҷ���
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
			value=MTCLA_Value_Estamat(US_present,Goal_present);//��ֵ���㣬�ؼ���
			if (value>=value_max)//��¼���ֵ���ӵ����
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
					if (deep_max<(deep+1))//��¼������
						deep_max=deep+1;
					if (deep_max_step<(i+1))//��¼������
						deep_max_step=i+1;

				}
				value_max=value;
			}
			//if (value<10)
			//	pCur->isCut_i=true;
			if (deep_max_deep1<(deep+1) && i==0)//��¼������
				deep_max_deep1=deep+1;	
			while (!visited.empty()) //���򴫲�
			{
				pCur = visited.top();// get the current node in the path
				pCur->updateStats(value,value_goal_oriented);   // update statisticsΪʲô��+�����
				visited.pop();// remove the current node from the stack
			}
			Per_t=100*(i*goal_time+k)/(site*goal_time);
			MTCLA_Esplise2 = ::GetTickCount() - MTCLA_Esplise1;//��ֵ=���ڼ�ʱ��-ԭ�м�ʱ��
			str_t.Format("Step %d/%d,  number %d/%d,  %d s ,  %.0f%s ;",i+1,goal_steps,k+1,goal_time,MTCLA_Esplise2/1000,Per_t,"%");
			Progreess_Per.SetWindowText(str_t);
		}//for k
		double best_v=0;
		bestAction = tree_Branch.bestAction(&best_v);//���㵥�����
		MTCLA_action_step_finnal.push_back(bestAction);
		MTCLA_action_best_v_finnal.push_back(best_v);
		MTCLA *pCur_B = &tree_Branch;
		pCur_B = pCur_B->vpChildren_i[bestAction];//Ҷ���
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
		MTCLA_value_Goal_oriented.push_back(value_goal_oriented_max);//��ֵ���㣬�ؼ���	//��
		if (value_Step_max<MTCLA_Value_Estamat(US_present,Goal_present))
			value_Step_max=MTCLA_Value_Estamat(US_present,Goal_present);
		MTCLA_value_Step.push_back(value_Step_max);//��ֵ���㣬�ؼ���	//��
		/////////////////////////////////////////////////////////
		*pCur_tree_Branch=tree_Branch;//ÿһ���õص�Ԫ��time�ε���������ԭ��ĸ��
		pCur_tree_Branch = pCur_tree_Branch->vpChildren_i[bestAction];//ѡ��ĸ������һ���֧
		Progress.SetPos(i);
		//ȱ�ٴ��ֹͣ����������
	}//for int i
	KillTimer(MTCLA_Timer1);//ɾ����ʱ��
	MTCLA_Esplise2 = ::GetTickCount() - MTCLA_Esplise1;//��ֵ=���ڼ�ʱ��-ԭ�м�ʱ��
	Per_t=100;
	str_t.Format("Step %d/%d,  number %d/%d,  %d s ,  %.0f%s ;",goal_steps,goal_steps,goal_time,goal_time,MTCLA_Esplise2/1000,Per_t,"%");
	Progreess_Per.SetWindowText(str_t);
	EndWaitCursor();
	WaitForIdle_A();
	if(MTCLA_On==false) //���ֹͣ��־
		return;
	MTCLA_Save_All_Picture=true;
	if (MTCLA_Save_All_Picture==true)
	{
		MTCLA_OnImageSave();//��ȡ·��
		MTCLA_Purpers_Show(MTCLA_P_Once_Cell_Oriented,MTCLA_Color_Once_Cell_Oriented);	//���ԭʼ��cellͼ��
		MTCLA_Purpers_Show_Finnal(MTCLA_P_Picture,MTCLA_Color_Picture,&MTCLA_US_Picture,&MTCLA_Goal_Picture,&value);	//������ٲ���ͼ��
		MTCLA_Purpers_Show_Text(value);	//����ı�
		CString m_s;
		m_s.Format("Degree of satisfying the Goals(0-1):%.2f", value);
		AfxMessageBox(m_s);
	}
	//MTCLA_Purpers_Show(MTCLA_P_finnal,MTCLA_Color_finnal);	//���ͼ��,�����ڶ���,��ʱ�ò���
	MTCLA_On=false;
	Button_Enable();
	OnClickedButton_Up();
	MTCLA_Play=true;
	((CButton* )GetDlgItem(IDC_BUTTON_MTCL_PLAY))->EnableWindow(true);
	((CButton* )GetDlgItem(IDC_BUTTON_MTCL_STOP))->EnableWindow(false);

}