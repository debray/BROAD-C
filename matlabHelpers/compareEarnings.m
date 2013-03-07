%SubjID = [0,21:29,32:41,43:49];  % UCLA1
%SubjID = [50:58, 60:64, 66:70, 72:82]; %UCLA2
SubjList = [0,21:29,32:41,43:49, 50:58, 60:64, 66:70, 72:82];

n = length(SubjList)

for s=1:n
    
    str = sprintf('Results/UCLA/Outcome-Subject-%d.mat',SubjList(s));
    
    load(str);
    
    Amount(s) = Outcome;
    
end