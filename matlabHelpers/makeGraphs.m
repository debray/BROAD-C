% Subject IDs
SubjID = [0:10, 11, 13:18];

numSubj = length(SubjID);

numR = 30;
numM = 6;

% Aggregate posteriors
Prob = zeros(numR, numSubj);
RT = zeros(numSubj, numR);

for s=1:numSubj
    
    str = sprintf('Results/Subject-%d.mat',SubjID(s));
    
    load(str);
    
    %[Prob(:,s), Model(:,s)] = max(Subject.ModelPost,[],2);
    [Prob(:,s), Model(:,s)] = max(RandomSubject.ModelPost,[],2);
    
end

% for each model, get mean and stderr across rounds

numModel = zeros(numM, numR);

for m=1:numM
    
    for r=1:numR
       
        numModel(m,r)=sum(Model(r,:)==m);
        
    end
    
end

% colors
Color = ['r','m','g','c','k','y'];

X = [1:numR];
figure(1); hold on; 
for i=1:numM
plot(numModel(i,:), Color(i));
end
legend('FPT','CPT','MVS','NMVS','EV','CRRA')