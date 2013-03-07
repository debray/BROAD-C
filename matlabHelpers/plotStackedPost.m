% plot stacked posterior distributions by class

%SubjID = [0,21:29,32:41,43:49];  % UCLA1
SubjID = [50:58, 60:64, 66:70, 72:82]; %UCLA2
%SubjID = [0,21:29,32:41,43:49, 50:58, 60:64, 66:70, 72:82];

numSubj = length(SubjID);

numR = 30;
numM = 6;

% Take the last few rounds for Classification
lastR = 4;

for s=1:numSubj
    str = sprintf('Results/UCLA/Subject-%d.mat',SubjID(s));
    load(str);
    [Prob(:,s), Model(:,s)] = max(Subject.ModelPost,[],2);
    FinalPost(:,s) = mean(Subject.ModelPost(end-lastR:end,:));
end

% Final classification
FinalClass = mode(Model(end-lastR:end,:));

% Order of display
dispOrder = [5,1,2,3,4,6];

stackPost = [];

for i=1:numM
    
    indM = find(FinalClass==dispOrder(i));
    postM = FinalPost(:,indM);
    
    % Order by posterior on the class
    [temp, Id] = sort(postM(dispOrder(i),:), 'descend');
    
    stackPost = [stackPost, postM(dispOrder,Id)];
end

% Plot bar graph
bar(stackPost', 'stack'); ylim([0, 1]); xlim([0.5 numSubj+.5]);
colormap(hot); legend('EV', 'FPT', 'CPT', 'MVS', 'NMVS', 'CRRA')