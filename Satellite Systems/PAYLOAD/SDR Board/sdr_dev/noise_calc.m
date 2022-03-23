%Radiometer Noise Calculation
%Matthew E. Nelson
%Updated 12/14/2012

%This script will ask for how many components in a cascaded system
%and then the noise factor for each component and the gain for each
%It will then calculate both the noise factor for the system and the temp

%Clear the workspace
clear all;

%Static variables for testing
n = 3;
%Gain array
G = [2 88 .5];

%Noise Figure array
F = [.1 1 .2];

FN = 0;

%T0 is assumed to be 290 K
T0 = 290;

disp('Running simulation...')

%Noise Factor calculation
%F1+(F2-1)/G1+(F3-1)/(G1*G2)+...+(Fn-1)/(G1*G2*Gn-1)

for j=1:n;
    if j == 3;
        
        FN = (F(j)-1)/(prod(G));
    end
    
    if j == 2;
        FN = FN + (F(j)-1)/(G(j)*G(j-1));
    end
    
    if j == 1;
        FN = FN + (F(j)-1)/(G(j));
    end
    FN = FN + F(1);
    
end
FN
