% Subject IDs
%SubjID = [0:10, 11, 13:18];
SubjID = [21:29,32:41,43:49];  % UCLA
%SubjID = 0;

numSubj = length(SubjID);

numR = 30;
numM = 6;

% Aggregate posteriors
GambleSD = zeros(numSubj, numR);
Violated = zeros(numSubj, numR);

flag_Random = 1;

for s=1:numSubj
    
    str = sprintf('Results/UCLA/Subject-%d.mat',SubjID(s));
    %str = sprintf('Results/Subject-%d.mat',SubjID(s));
    
    load(str);
    
    % check SD violations for all gambles
    for r=1:numR
        
        Out1 = Subject.Outcome1(r,:);
        Out2 = Subject.Outcome2(r,:);
    
        Prob1 = Subject.Prob1(r,:);
        Prob2 = Subject.Prob2(r,:);
    
        Resp = Subject.Y(r);
        
        if (flag_Random==1)
            
            Out1 = RandomSubject.Outcome1(r,:);
            Out2 = RandomSubject.Outcome2(r,:);
    
            Prob1 = RandomSubject.Prob1(r,:);
            Prob2 = RandomSubject.Prob2(r,:);
    
            Resp = RandomSubject.Y(r);
            
        end
        
        % Lottery 1 stochastically dominates 2?
        SD1 = SDominance(Out1, Prob1, Out2, Prob2);
            
        % Lottery 2 stochastically dominates 1?
        SD2 = SDominance(Out2, Prob2, Out1, Prob1);
            
        % if none are true, then no SD gamble
        if (SD1==0 && SD2==0)
            GambleSD(s,r) = 0;
        end
        
        % if one is true, then SD gamble
        if (SD1==1 || SD2==1)
            GambleSD(s,r) = 1;
        end
        
        % if both are true, then both lotteries are equal
        if (SD1==1 && SD2==1)
            GambleSD(s,r) = 2;
        end
        
        % did violation occur?
        if GambleSD(s,r)==1
            
            if (SD1==1 && Resp==1)
                Violated(s,r)=1;
            elseif (SD2==2 && Resp==0)
                Violated(s,r)=1;
            end
        end
        
    end
    
end