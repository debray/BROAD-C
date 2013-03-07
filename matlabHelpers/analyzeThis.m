function Outcome = analyzeThis(SubjID)

[Subj, RandomSubj] = readData(SubjID);

% randomly choose a gamble
numR = 30;

T = randperm(numR); randG = T(1);

% randomly picked
fprintf('\n\nRandomly selected Lotteries: \n\n');
fprintf('Lottery 1:\n');
fprintf('Amount: $%0.2f, $%0.2f, $%0.2f\n\n', Subj.Outcome1(randG,1),...
            Subj.Outcome1(randG,2), Subj.Outcome1(randG,3));
fprintf('Probability: $%0.2f, $%0.2f, $%0.2f\n\n\n', Subj.Prob1(randG,1),...
            Subj.Prob1(randG,2), Subj.Prob1(randG,3));
        
fprintf('Lottery 2:\n');
fprintf('Amount: $%0.2f, $%0.2f, $%0.2f\n\n', Subj.Outcome2(randG,1),...
            Subj.Outcome2(randG,2), Subj.Outcome2(randG,3));
fprintf('Probability: $%0.2f, $%0.2f, $%0.2f\n\n', Subj.Prob2(randG,1),...
            Subj.Prob2(randG,2), Subj.Prob2(randG,3));       

fprintf('Your choice: Lottery %d\n\n', Subj.Y(randG));

% play out the lottery
% Random number input
Input = input('Enter Random Number (0-99): ');
RandInput = Input/100;

if Subj.Y(randG)==0
    X=Subj.Outcome1(randG,:); 
    P=Subj.Prob1(randG,:); 
else
    X=Subj.Outcome2(randG,:); 
    P=Subj.Prob2(randG,:); 
end

V = cumsum(P); [v,ind] = find(RandInput>V, 1, 'last'); 
if isempty(ind); ind=1; end

Outcome = X(ind+1);

fprintf('\n\n\nOutcome of Lottery: $%d\n\n', Outcome);

fprintf('Thank you for your participation!\n');

save(sprintf('Results/Outcome-Subject-%d.mat',SubjID));