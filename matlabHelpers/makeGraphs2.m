% Subject IDs
SubjID = [0:10, 11, 13:18];

load SubjectAggregateData.mat
load RandomSubjectAggregateData.mat

numSubj = length(SubjID);

numR = 30;
numM = 6;

% colors
Color = ['r','m','g','c','k','y'];

X = [1:numR];
figure(1); hold on; 
for i=1:numM
errorbar(X, MeanPost(i,:), StdPost(i,:), Color(i));
end

figure(2); hold on; 
for i=1:numM
errorbar(X, RandomMeanPost(i,:), RandomStdPost(i,:), Color(i));
end

figure(3); hold on; 
errorbar(X, MeanRT, StdRT, 'r'); errorbar(X, RandomMeanRT, RandomStdRT, 'k');