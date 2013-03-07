% Subject IDs
%SubjID = [0:10, 11, 13:18];
%SubjID = [0,21:29,32:41,43:49];  % UCLA1
SubjID = [50:58, 60:64, 66:70, 72:82]; %UCLA2
%SubjID = [0,21:29,32:41,43:49, 50:58, 60:64, 66:70, 72:82];

numSubj = length(SubjID);

numR = 30;
numM = 6;

% Take the last few rounds for Classification
lastR = 4;

% Aggregate posteriors
Prob = zeros(numR, numSubj);
RT = zeros(numSubj, numR);

for s=1:numSubj
    
    str = sprintf('Results/UCLA/Subject-%d.mat',SubjID(s));
    
    load(str);
    
    [Prob(:,s), Model(:,s)] = max(Subject.ModelPost,[],2);
    FinalPost(:,s) = mean(Subject.ModelPost(end-lastR:end,:));
    %[Prob(:,s), Model(:,s)] = max(RandomSubject.ModelPost,[],2);
    
end

% Final classification
FinalClass = mode(Model(end-lastR:end,:));
%FinalClass = Model(end,:)';

for s=1:numSubj
    IsFinalClass(s,:) = Model(:,s)==FinalClass(s);
end

% for each model, get mean and stderr across rounds
numModel = zeros(numM, numR);

for m=1:numM
    
    for r=1:numR
       
        numModel(m,r)=sum(Model(r,:)==m);
        
    end
    
end

% Classification for each model
classModel = numModel(:,end);
MeanclassModel = mean(numModel(:,end-lastR:end),2);
StdclassModel = std(numModel(:,end-lastR:end),[],2)/sqrt(lastR);

MeanPostModel = zeros(numM, numR);
StdPostModel = zeros(numM, numR);

% Find classification for each model
for m=1:numM
    
    % find subjects classified as the model
    SubjInd = find(Model(end,:)==m);
    
    MeanPostModel(m,:) = mean(Prob(:,SubjInd),2)';
    StdPostModel(m,:) = std(Prob(:,SubjInd),[],2)'/sqrt(length(SubjInd));
end

% colors
Color = ['r','m','g','c','k','y'];

X = [1:numR];
figure(1); hold on; 
for i=1:numM
plot(numModel(i,:), Color(i));
end
legend('FPT','CPT','MVS','NMVS','EV','CRRA')

X = [1:numR];
figure(2); hold on; 
for i=1:numM
errorbar(MeanPostModel(i,:), StdPostModel(i,:), Color(i));
end
legend('FPT','CPT','MVS','NMVS','EV','CRRA')

figure(3); 
bar(MeanclassModel);
%bar(hist(FinalClass,[1:numM]))