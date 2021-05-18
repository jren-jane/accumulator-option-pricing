function value = acc_XH()

SETTLE = [20, 19, 23,18,21,21,20,22,23,21,21,21];
SDATE =[20 ,39 ,62 ,80 ,101 ,122 ,142 ,164 ,187 ,208 ,229 ,250];

S = 5.7;
X = 4.7824;
sigma = 0.3;
r = 0.02;
DT = 1.0/250;
AMOUNT = 1000;

value = 0;

for i=1:12
    
        for p = SDATE(i)-SETTLE(i)+1:SDATE(i)  % run for each period 
           %accumulate the call option value
           value = value + pcall(S,X,r,sigma,p*DT,(3+SDATE(i))*DT);
           %accumulate the put option value
           value = value-2*pput(S,X,r,sigma,p*DT,(3+SDATE(i))*DT);
        end
    
end

value = value * AMOUNT;
return




function cvalue = pcall(S,X,r,sigma, t1, t2)
D1 = d1(t1,r,sigma,X,S);
D2 = d2(t1,r,sigma,X,S);
cvalue = S*normcdf(D1,0,1) - X*exp(-r*t2)*normcdf(D2,0,1);
return

function pvalue = pput(S,X,r,sigma, t1, t2)
D1 = d1(t1,r,sigma,X,S);
D2 = d2(t1,r,sigma,X,S);
pvalue = X*exp(-r*t2)*normcdf(-D2,0,1)-S*normcdf(-D1,0,1);
return


function D1 = d1(t, r, sigma,X,S)
D1 =(log(S/X)+(r+0.5*sigma*sigma)*t)/(sigma*sqrt(t));
return

function D2 = d2(t, r, sigma,X,S)
D2 =(log(S/X)+(r-0.5*sigma*sigma)*t)/(sigma*sqrt(t));
return