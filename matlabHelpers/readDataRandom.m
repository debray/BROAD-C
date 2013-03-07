function [Subject, RandomSubject] = readDataRandom(SubjID)

% read Random data

dir = 'Results';
str = sprintf('%s/Random-Subject-%d.dat', dir, SubjID);
fid = fopen(str, 'r');

Str = fgetl(fid);

numR = 30;

RandomSubject.SubjID = Str;

for i=1:numR
    
    line = fgetl(fid);
    RandomSubject.NumR(i) = str2num(line);
    
    % lottery 1
    line = fgetl(fid);
    Outcome1 = regexp(line, ',', 'split');
    for j=1:3; RandomSubject.Outcome1(i,j)=str2num(Outcome1{j}); end
    
    line = fgetl(fid);
    Prob1 = regexp(line, ',', 'split');
    for j=1:3; RandomSubject.Prob1(i,j)=str2num(Prob1{j}); end
    
    % lottery 2
    line = fgetl(fid);
    Outcome2 = regexp(line, ',', 'split');
    for j=1:3; RandomSubject.Outcome2(i,j)=str2num(Outcome2{j}); end
    
    line = fgetl(fid);
    Prob2 = regexp(line, ',', 'split');
    for j=1:3; RandomSubject.Prob2(i,j)=str2num(Prob2{j}); end
    
    % response
    line = fgetl(fid);
    RandomSubject.Y(i) = str2num(line);
    
    % model posterior
    line = fgetl(fid);
    Post = regexp(line, ',', 'split');
    for j=1:6; RandomSubject.ModelPost(i,j)=str2num(Post{j}); end
    
    % Response Time
    line = fgetl(fid);
    RandomSubject.RT(i) = str2num(line);
    
end

fclose(fid);

% save results to .mat file
save(sprintf('Results/Subject-%d.mat', SubjID), 'Subject', 'RandomSubject');