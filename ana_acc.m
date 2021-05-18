function V = ana_acc() % an analytical approximation function for disctete Barrier-accumulaor with delayed payoff

S=5.7;
X =4.7824;
H = 6.1425;
v = 0.3;
r = 0.02;
T = 250;
DELAY = 3;

%function V = ana_acc(T) 
SETTLE =[20, 19, 23, 18,21,21,20,22,23,21,21,21];

% the last trading day for each period  
LastTday=[20 ,39 ,62 ,80 ,101 ,122 ,142 ,164 ,187 ,208 ,229 ,250];


% the settlement day for each trading day assuming there is no knock-out
for i=1:T
   index = 0; 
   %search through lastTday
   for j=1:12
       if(LastTday(j)>=i)
          SDATE(i) = LastTday(j);
          break;
       end
   end
end



% calculate the delayed accumulator price

mu = r-0.5*v*v;
lamda = 1+ mu/v/v;
dt = 1/T;
H_m = exp(0.5826*v*sqrt(dt))*H;

V = 0;
for i=1:250 

    term1 = normcdf(x(S, X, dt*i, mu,v),0,1);
    term2 = normcdf(x1(S,X,dt*i,mu, v, H_m),0,1);
    term3 = (H_m/S)^(2*lamda)*normcdf(-y(S,X,dt*i, mu, v, H_m),0,1);
    term4 = (H_m/S)^(2*lamda)*normcdf(-y1(S, X, dt*i,mu, v, H_m),0,1);
    term5 = normcdf(x(S, X, dt*i, mu, v)-v*sqrt(dt*i),0,1);
    term6 = normcdf(x1(S, X, dt*i,mu,v, H_m)-v*sqrt(dt*i),0,1);
    term7 = (H_m/S)^(2*lamda-2)*normcdf(-y(S, X, dt*i, mu,v,H_m)+v*sqrt(i*dt),0,1);
    term8 = (H_m/S)^(2*lamda-2)*normcdf(-y1(S, X,dt*i, mu,v,H_m)+v*sqrt(i*dt),0,1);
    
    V = V + S*(2-term1-term2-term3-term4)-X*exp(-r*dt*SDATE(i))*(2-term5-term6-term7-term8);

end

    V = V*1000;

return

function z = x(S,X,t,mu,v)
   z = (log(S/X)+(mu+v*v)*t)/v/sqrt(t);
return 


function z = x1(S, X,t,mu,v,H_m)
   z = (log(S/H_m)+(mu+v*v)*t)/v/sqrt(t);
return 

function z = y(S, X, t, mu,v,H_m)
   z = (log(H_m*H_m/X/S)+(mu+v*v)*t)/v/sqrt(t);
return 

function z = y1(S, X, t, mu,v,H_m)
   z = (log(H_m/S)+(mu+v*v)*t)/v/sqrt(t);
return 









