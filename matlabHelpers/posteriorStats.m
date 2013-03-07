% Subject IDs
%SubjID = [0:10, 11, 13:18];
%SubjID = [0,21:29,32:41,43:49];  % UCLA1
SubjID = [50:58, 60:64, 66:70, 72:82]; %UCLA2
%SubjID = [0,21:29,32:41,43:49, 50:58, 60:64, 66:70, 72:82];

numSubj = length(SubjID);

numR = 30;
numM = 6;

% Aggregate posteriors
Post = zeros(numSubj, numR, numM);
RT = zeros(numSubj, numR);

for s=1:numSubj
    
    str = sprintf('Results/UCLA/Subject-%d.mat',SubjID(s));
    
    load(str);
    
    Post(s, :, :) = Subject.ModelPost;
    RT(s,:) = Subject.RT;
    %RT(s,:) = RandomSubject.RT;
    %Post(s, :, :) = RandomSubject.ModelPost;
end

% for each model, get mean and stderr across rounds

MeanPost = zeros(numM, numR);
StdPost = zeros(numM, numR);

for m=1:numM
    
    MeanPost(m,:) = mean(squeeze(Post(:,:,m)),1);
    
    StdPost(m,:) = std(squeeze(Post(:,:,m)),1) / sqrt(numSubj);
end

MeanRT = mean(RT,1);
StdRT = std(RT,1)/sqrt(numSubj);

% colors
Color = ['r','m','g','c','k','y'];

MeanPost = [1/6*ones(numM,1), MeanPost];
StdPost = [zeros(numM,1), StdPost];

X = [0:numR];
figure(1); hold on; ylim([0 0.31]); xlim([0 30.5]);
for i=1:numM
errorbar(X, MeanPost(i,:), StdPost(i,:), Color(i));
end
legend('FPT','CPT','MVS','NMVS','EV','CRRA')

X = [1:numR];
figure(2); hold on; errorbar(X, MeanRT, StdRT);

% plot individual
s = 50;
str = sprintf('Results/UCLA/Subject-%d.mat',s);
load(str);
PostProb = [1/6*ones(1,6); Subject.ModelPost];
figure(3); 
plot([0:30],PostProb); 
ylim([0 1]); xlim([0 30.5]);
legend('FPT','CPT','MVS','NMVS','EV','CRRA')
%xlabel('Rounds'); ylabel('Posterior Prob.');