#include <iostream>
#include <ctime>
#include <cmath>

using namespace std;

double rand_expon(double lambda)
{
	double uniform=rand()*1.0/RAND_MAX;
	return -1000*log(uniform)/lambda;
}

int fact(int n)
{
	if (n>1)
	{
		return n*fact(n-1);
	}
	return 1;
}

int frac(int k,double l,double n)
{
	return pow(l/n,k*1.0)/fact(k);
}

double p_zero(int num,double l,double n)
{
	int i=0;
	double s=0;
	while (i<num)
	{
		s+=frac(i,l,n);
		i++;
	}
	s+=pow(l/n,num+1)/(fact(num)*(num-l/n));
	return 1/s;
}

double prob(int k,int num,double l,double n)
{
	if (k<num)
	{
		return frac(k,l,n)*p_zero(num,l,n);
	}
	return pow(l/n,k)/(fact(num)*pow(num,k-num));
}

struct event_struct
{
	double next_event_time;

	double intensivity;
	
	bool in_work=false;

	virtual double perform_event()
	{
		return 0;
	}
};

struct people_que:event_struct
{
	int amount=0;

	people_que(double in,double cur_time)
	{
		intensivity=in;
		next_event_time=cur_time+rand_expon(intensivity);
	}

	double perform_event()
	{
		amount++;
		next_event_time+=rand_expon(intensivity);
		return next_event_time;
	}
};

struct bank_operator:event_struct
{

	people_que *source;

	bank_operator(double in,double cur_time,people_que *q)
	{
		intensivity=in;
		next_event_time=cur_time+rand_expon(intensivity);
		source=q;
	}

	double perform_event()
	{
		if (in_work)
			in_work=false;
		else
		{
			if ((source->amount)>0)
			{
				(source->amount)--;
				in_work=true;
				next_event_time+=rand_expon(intensivity);
			}
			else
			{
				next_event_time+=100;
			}
		}
		return next_event_time;
	}
};

int main()
{
	srand(time(0));
	cout.flush();
	int probs[16];
	int exps=0;
	int y=0;
	while (y<16)
	{
		probs[y]=0;
		y++;
	}
	
	int n=0;
	cin>>n;
	people_que a(5,time(0)*1000);
	event_struct **agents= new event_struct*[n+1];
	agents[0]=&a;
	int i=0;
	while (i<n)
	{
		agents[i+1]=new bank_operator(1,time(0)*1000,&a);
		i++;
		
	}
	while (true)
	{
		
		system("cls");
		int t=0;
		int am=0;
		while (t<n+1)
		{
			if (agents[t]->next_event_time<time(0)*1000)
			{
				agents[t]->perform_event();
			}
			t++;
		}
		cout<<a.amount<<" people in queue"<<endl;
		am+=a.amount;
		t=0;
		while (t<n)
		{
			if (agents[t+1]->in_work)
			{
				cout<<"Bank operator "<<t<<" is working"<<endl;
				am+=1;
			}
			else
			{
				cout<<"Bank operator "<<t<<" is idle"<<endl;
			}
			t++;
		}
		cout<<endl;
		probs[min(am,16)]++;
		exps++;
		cout<<"Currently "<<am<<" people in system"<<endl;
		cout<<"Theoretical probability is "<<prob(am,n,5,1)<<endl;
		cout<<"Experemental probability is"<<probs[am]*1.0/exps;
		cout<<endl<<"First 6 probabilities"<<endl;
		cout<<"Theoretical:  ";
		y=0;
		while (y<6)
		{
			cout<<prob(y,n,5,1)<<" ";
			y++;
		}
		cout<<endl<<"Experimental: ";
		y=0;
		while (y<6)
		{
			cout<<probs[y]*1.0/exps<<" ";
			y++;
		}
		
	}
	return 0;
}
