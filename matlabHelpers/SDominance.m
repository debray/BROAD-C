function Dom = SDominance(Out1, Prob1, Out2, Prob2)
% Returns 1 is Lotto1 stochastically dominates Lotto2, else 0

% 1 if Lotto1 dominates Lotto2
Dom = 0;

% Loss outcome for 1 is at least as good as 2 
if ((Out1(1)>=Out2(1)) && (Prob1(1)<=Prob2(1)))
    
    if ((Out1(3)>=Out2(3)) && (Prob1(3)>=Prob2(3)))
        
        Dom = 1;
        
    end
end