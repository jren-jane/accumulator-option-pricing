/* This program prices an accumulator by a basic Monte Carlo method */

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <time.h>

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/variate_generator.hpp>
#include <fstream>
using namespace std;

//define parameters
#define SETTLE    3 //delay in settlement
#define AMOUNT    1000   //the number of basic transactions


double INITIAL;
double STRIKE;
double KOUT; //knock-out price
double SIGMA; //volatility
double RATE; //interest rate
int    NP; //number of paths for each simulation
int    NSIM; //number of simulations

int DAYS[] ={
	         20,
             19,
			 23,
			 18,
			 21,
			 21,
			 20,
             22,
			 23,
			 21,
			 21,
			 21
            }; //the number of trading days in each accumulation period


//standard-normal PRNG
boost::mt19937 gen;
boost::normal_distribution<double> dist(0, 1);
boost::variate_generator<boost::mt19937&, boost::normal_distribution<double>> die(gen, dist);


int main()
{

AGAIN:
    //read product parameters
	cout<<"\nSpot Price: ";
	cin>>INITIAL;
	cout<<"\nStrike Price: ";
	cin>>STRIKE;
	cout<<"\nBarrier: ";
	cin>>KOUT;
	cout<<"\nInterest Rate: ";
	cin>>RATE;
	cout<<"\nVolatility: ";
	cin>>SIGMA;
	cout<<"\nNumber of simulations: ";
	cin>>NSIM;
	cout<<"\nNumber of path: ";
	cin>>NP;


	int i, j, k, d, s;
	int ND = 0; //number of days in the trading year
    for(i=0; i<12; i++)ND += DAYS[i]; 
    double* VALUE; // the value of accumulator
	double DT = 1.0/ND; // time step in year
	double POS; // the ACCUMLATED amount of the stock in each period


	//define some intermediate parameters to speed up the computation
	double CONST_PART = RATE*DT-0.5*SIGMA*SIGMA*DT; // the drift-part of daily evolution
	double RAND_PART = sqrt(DT)*SIGMA; // the diffusion part of a day
	double DISCOUNT = RATE*DT; //for one day
	double DISCS =  exp(-DT*RATE*SETTLE);
	int    TIME; //the number of days from the initialization of the accumulator

	double* WALK; //to hold the random-walk
	double** PATH; //the actual stock price
	

	WALK = new double[ND];
	PATH = new double*[4];
	for(i=0; i<4; i++)PATH[i] = new double[ND+1];


	//initialize 
	VALUE = new double[NSIM];
	for(i=0; i<NSIM; i++)VALUE[i] = 0;

    cout<<"\nStart simulation...\n\n\n";
    for(s=0; s<NSIM; s++)
	{
        
	for (i=0; i<NP; i++)
	{
		if(i%10000==0)cout<<"("<<s+1<<", "<<i<<")"<<endl;
		for(j=0; j<ND; j++)WALK[j] = die();
			
		//use the same series of standard normal walk to generate 4 paths
		for(j=0; j<4; j++)PATH[j][0] = INITIAL; 
		for(j=1; j<=ND; j++)
		{
			//a simple implementation of control variate method
		    PATH[0][j] = PATH[0][j-1]*exp(CONST_PART+RAND_PART*WALK[j-1]);
			PATH[1][j] = PATH[1][j-1]*exp(CONST_PART-RAND_PART*WALK[j-1]);
			PATH[2][j] = PATH[2][j-1]*exp(CONST_PART+RAND_PART*WALK[ND-j]); //running backwards
		    PATH[3][j] = PATH[3][j-1]*exp(CONST_PART-RAND_PART*WALK[ND-j]); // running backwads
		}

	    //evalueate the payoff from each path
		for(j=0; j<4; j++) // for each path
		{
	            TIME = 0; //initialiate time     
				for(k=0; k<12; k++) //for each period
				{
					POS = 0;
					//for each day in a period.
					for(d=0; d<DAYS[k]; d++)
					{
						TIME++;
						if(PATH[j][TIME]>=KOUT) //knock-out occurs
						{
                            //stop accumulating and calculate the present value of the payoff
							//VALUE[s] += POS * (PATH[j][TIME+3]-STRIKE)*exp(-DISCOUNT*(TIME + 3)); //pay-off 3 days later
                            VALUE[s] += POS * (PATH[j][TIME]-STRIKE*DISCS)*exp(-DISCOUNT*(TIME));
							goto OUT;
						}

						else if(PATH[j][TIME]>=STRIKE) // buy AMOUNT stock at STRIKE price 
						{	
                            POS += AMOUNT;
						}

						else POS += 2*AMOUNT;
						
						if(d==DAYS[k]-1)VALUE[s] +=  POS * (PATH[j][TIME]-STRIKE*DISCS)*exp(-DISCOUNT*(TIME));
					}              
				}

                OUT:;// for k
		} //for (j)
        
	} // for (i)
	}//for (s)


	//compute the statistics

	for(i=0; i<NSIM; i++)VALUE[i] /=NP*4.0;
	
	double V = 0;
	for(i=0; i<NSIM; i++)
	{
		V += VALUE[i]/double(NSIM);
	}

    double ERR = 0;
	for(i=0; i<NSIM; i++)
	{
        ERR = (VALUE[i]-V)*(VALUE[i]-V);
	}
    ERR = sqrt(ERR/NSIM);

	cout<<"\n*************************************************************\n";
	cout<<"\n\nNumber of Simulation = "<<NSIM;
	cout<<"\n\nNumber of paths for each simulation = "<<NP<<endl;
	cout<<"\n\n[VALUE, STD] = ["<<V<<", "<<ERR<<"]"<<endl;
	

	cout<<"\n\n\nPut 1 for next, 0 to end: ";
	int C;
	cin>>C;
	if(C==1)goto AGAIN;
	system("pause");
	return EXIT_SUCCESS;
}